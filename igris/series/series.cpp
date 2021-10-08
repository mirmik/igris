#include <igris/series/series.h>
#include <igris/series/block.h>
#include <igris/series/iterator.h>
#include <igris/util/string.h>

#include <igris/math.h>
#include <igris/util/numconvert.h>

#include <igris/util/bug.h>

igris::series::series(int elemsize)
	: _elemsize(elemsize)
{}

void igris::series::reserve(int size)
{
	add_block(size);
}

void igris::series::add_block(int size)
{
	void * ptr = allocator.allocate(size * _elemsize);
	auto * block = new series_block(this, ptr, size);

	DPRINTPTR(block);
	dlist_add(&block->lnk, &blocks);
}

int igris::series::right_capacity()
{
	int accum = 0;

	igris::series_block * block;
	dlist_for_each_entry(block, &blocks, lnk)
	{
		accum += block->size - block->fini;
	}

	return accum;
}

int igris::series::size()
{
	int accum = 0;

	igris::series_block * block;
	dlist_for_each_entry(block, &blocks, lnk)
	{
		accum += block->fini - block->strt;
	}

	return accum;
}

void * igris::series::emplace()
{
	igris::series_block * block;
	dlist_for_each_entry(block, &blocks, lnk)
	{
		if (block->has_place())
		{
			return block->emplace();
		}
	}

	add_block(block_size_hint);
	return last_block()->emplace();
}

igris::series_field_annotator & igris::series::annotator()
{
	return _annotator;
}

void igris::series::push_csv_string_parse(const std::string & str)
{
	auto lst = igris::split(str, ',');
	void * ptr = emplace();
	auto view = object_view(ptr);

	for (unsigned int i = 0; i < MIN(lst.size(), view.size()); ++i)
	{
		double data = atof64(lst[i].c_str(), nullptr);
		view[i].assign(data);
	}
}

igris::series_iterator igris::series::get_iterator(int num)
{
	int64_t accum = 0;
	int64_t saccum;

	igris::series_block * block;
	dlist_for_each_entry(block, &blocks, lnk)
	{
		saccum = accum;
		accum += block->fini - block->strt;

		if (accum > num)
		{
			return igris::series_iterator(&block->lnk, block->strt + (num - saccum));
		}
	}

	BUG();
	return end();
}

igris::series_iterator igris::series::begin()
{
	return { blocks.next, dlist_first_entry(&blocks, series_block, lnk)->strt };
}

igris::series_iterator igris::series::end()
{
	return { &blocks, -1 };
}


igris::series_block * igris::series::last_block()
{
	return dlist_entry(blocks.prev, series_block, lnk);
}


igris::series_field_annotation * igris::series::find_annotation(const std::string & name)
{
	return _annotator.find_annotation(name);
}