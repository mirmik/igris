#include <igris/series/iterator.h>
#include <igris/series/series.h>
#include <igris/series/block.h>

void * igris::series_iterator::pointer()
{
	return block()->get(num);
}

igris::series_iterator igris::series_iterator::operator++()
{
	if (num == -1)
		return *this;

	num++;
	if (num == block()->fini)
	{
		block_lnk = block_lnk->next;
		num = 0;
	}
	return *this;
}

igris::series_iterator igris::series_iterator::operator++(int)
{
	igris::series_iterator iter = *this;
	++*this;
	return iter;
}


igris::series_iterator igris::series_iterator::operator--()
{
	if (num == -1)
		return *this;

	auto * parent = block()->parent;

	num--;
	if (num == block()->strt-1)
	{
		dprln("FFFFF");
		block_lnk = block_lnk->prev;
		if (&parent->blocks != block_lnk) 
			num = 0;
		else
			num = -1;
	}
	return *this;
}

igris::series_iterator igris::series_iterator::operator--(int)
{
	igris::series_iterator iter = *this;
	--*this;
	return iter;
}

igris::series_block * igris::series_iterator::block()
{
	assert(num != -1);
	return dlist_entry(block_lnk, series_block, lnk);
}

igris::series_iterator::series_iterator(dlist_head * block_lnk, int num) 
	: block_lnk(block_lnk), num(num)
{}
