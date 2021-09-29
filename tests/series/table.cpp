#include <doctest/doctest.h>
#include <igris/series/series.h>

struct A 
{
	int a;
	double b;
	float c;
};

TEST_CASE("table") 
{
	igris::series data = igris::make_series<A>(); 

	data.reserve(15);
	data.reserve(7);

	CHECK_EQ(data.right_capacity(), 22);

	A* obj = data.emplace<A>();
}