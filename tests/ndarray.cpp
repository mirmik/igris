#include <doctest/doctest.h>

#if __cplusplus > 201709L && __has_include(<concepts>)
#include <igris/container/ndarray.h>

TEST_CASE("2darray") 
{
	igris::ndarray<double> arr = 
	{
		{ 10., 30., 50. }, 
		{ 10., 20., 30. }, 
	};

	CHECK_EQ(arr.dimension(), 2);
	CHECK_EQ(arr.shape()[0], 2);
	CHECK_EQ(arr.shape()[1], 3);
	CHECK_EQ(arr.shape().size(), 2);

	CHECK_EQ(arr.planed_index({0,0}), 0);
	CHECK_EQ(arr({0,0}), 10);
	CHECK_EQ(arr({0,1}), 30);
	CHECK_EQ(arr({0,2}), 50);
	CHECK_EQ(arr({1,0}), 10);
	CHECK_EQ(arr({1,1}), 20);
	CHECK_EQ(arr({1,2}), 30);

	arr.reshape({6});	
	CHECK_EQ(arr.shape()[0], 6);
	CHECK_EQ(arr.shape().size(), 1);
	CHECK_EQ(arr.dimension(), 1);
	CHECK_EQ(arr({0}), 10);
	CHECK_EQ(arr({1}), 30);
	CHECK_EQ(arr({2}), 50);
	CHECK_EQ(arr({3}), 10);
	CHECK_EQ(arr({4}), 20);
	CHECK_EQ(arr({5}), 30);
}

TEST_CASE("3darray") 
{
	igris::ndarray<double> arr = 
	{
		{ { 10., 20. }, { 30., 60. }, { 50., 100.} }, 
		{ { 10., 20. }, { 20., 40. }, { 30., 60. } }, 
	};

	CHECK_EQ(arr.dimension(), 3);
	CHECK_EQ(arr.shape()[0], 2);
	CHECK_EQ(arr.shape()[1], 3);
	CHECK_EQ(arr.shape()[2], 2);
	CHECK_EQ(arr.shape().size(), 3);

	CHECK_EQ(arr.planed_index({0,0,0}), 0);
	CHECK_EQ(arr.planed_index({0,0,1}), 1);
	CHECK_EQ(arr.planed_index({1,0,1}), 7);
	CHECK_EQ(arr({0,0,0}), 10);
	CHECK_EQ(arr({0,1,0}), 30);
	CHECK_EQ(arr({0,2,0}), 50);
	CHECK_EQ(arr({1,0,0}), 10);
	CHECK_EQ(arr({1,1,0}), 20);
	CHECK_EQ(arr({1,2,0}), 30);
	CHECK_EQ(arr({0,0,1}), 20);
	CHECK_EQ(arr({0,1,1}), 60);
	CHECK_EQ(arr({0,2,1}), 100);
	CHECK_EQ(arr({1,0,1}), 20);
	CHECK_EQ(arr({1,1,1}), 40);
	CHECK_EQ(arr({1,2,1}), 60);
}
#endif