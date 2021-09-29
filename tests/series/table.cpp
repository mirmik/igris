#include <doctest/doctest.h>
#include <igris/series/series.h>
#include <igris/series/block.h>

struct A 
{
	int a;
	double b;
	float c;
};

TEST_CASE("table.capacity") 
{
	igris::series data = igris::make_series<A>(); 
	CHECK_EQ(data.count_of_blocks(), 0);

	data.reserve(15);
	data.reserve(7);

	CHECK_EQ(data.right_capacity(), 22);
	CHECK_EQ(data.count_of_blocks(), 2);
}


TEST_CASE("table.0") 
{
	igris::series data = igris::make_series<A>(); 
	CHECK_EQ(data.count_of_blocks(), 0);

	data.reserve(15);

	auto iter = data.get_iterator(0);
	CHECK_EQ(&data.blocks, iter.block_lnk);
	CHECK_EQ(iter, data.end());
}

TEST_CASE("table.1") 
{
	igris::series data = igris::make_series<A>(); 
	CHECK_EQ(data.count_of_blocks(), 0);

	data.reserve(15);
	A* obj = data.emplace<A>();

	obj->a = 33;
	obj->b = 33.05;
	obj->c = 33.04;

	auto iter = data.get_iterator(0);
	CHECK_EQ(data.blocks.next, iter.block_lnk);

	//auto ptr = iter.pointer();
}


TEST_CASE("table") 
{
	igris::series data = igris::make_series<A>(); 
	CHECK_EQ(data.count_of_blocks(), 0);

	data.reserve(15);
	A* obj = data.emplace<A>();

	obj->a = 33;
	obj->b = 33.05;
	obj->c = 33.04;

	auto iter = data.get_iterator(0);
	auto ptr = iter.pointer();

	CHECK_EQ(iter.block()->ptr, iter.pointer());
	CHECK_EQ(data.count_of_blocks(), 1);
	CHECK_EQ(data.blocks.next, iter.block_lnk);
	CHECK_EQ(ptr, obj);
	CHECK_EQ(((A*)ptr)->a, 33);
}

TEST_CASE("table.2") 
{
	igris::series data = igris::make_series<A>(); 
	CHECK_EQ(data.count_of_blocks(), 0);

	data.reserve(15);
	A* obj = data.emplace<A>();

	obj->a = 33;
	obj->b = 33.05;
	obj->c = 33.04;

	A& a = data.get<A>(0); 
	CHECK_EQ(a.a, 33); 
	CHECK_EQ(a.c, 33.04f);
}