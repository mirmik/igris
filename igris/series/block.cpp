#include <igris/series/block.h>
#include <igris/series/series.h>

#include <cassert>

void * igris::series_block::get(int num)
{
	DPRINT(strt);
	DPRINT(num);
	DPRINT(elemsize());
	return (void*)((char*)ptr + (elemsize() * (strt + num)));
}

igris::series_block::series_block(igris::series * parent, void* ptr, int size)
	: parent(parent), ptr(ptr), size(size)
{
	assert(parent);
}

bool igris::series_block::has_place()
{
	return size != fini;
}

void * igris::series_block::emplace()
{
	return (void*)((char*)ptr + (elemsize() * fini));
}

int igris::series_block::elemsize() 
{
	DPRINTPTR(parent);
	return parent->elemsize();
}