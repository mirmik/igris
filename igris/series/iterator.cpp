#include <igris/series/iterator.h>
#include <igris/series/series.h>
#include <igris/series/block.h>

void * igris::series_iterator::pointer()
{
	return block()->get(num);
}

igris::series_iterator igris::series_iterator::operator++()
{
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

igris::series_block * igris::series_iterator::block()
{
	return dlist_entry(block_lnk, series_block, lnk);
}

igris::series_iterator::series_iterator(dlist_head * block_lnk, int num) 
	: block_lnk(block_lnk), num(num)
{}