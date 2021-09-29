#include <igris/series/block.h>
#include <igris/series/series.h>

#include <cassert>

void * igris::series_block::get(int num)
{
	return (void*)((char*)ptr + (elemsize() * (strt + num)));
}

void * igris::series_block::last()
{
	assert(fini - 1 >= 0);
	assert(fini - 1 < size);
	return (void*)((char*)ptr + (elemsize() * (fini - 1)));
}

igris::series_block::series_block(igris::series * parent, void* ptr, int size)
	: parent(parent), ptr(ptr), size(size), strt(0), fini(0)
{
	assert(parent);
	DPRINTPTR(ptr);
}

bool igris::series_block::has_place()
{
	return size != fini;
}

void * igris::series_block::emplace()
{
	int pos = fini++;
	return (void*)((char*)ptr + (elemsize() * pos));
}

int igris::series_block::elemsize() 
{
	return parent->elemsize();
}