#include <igris/container/chunked_vector.h>
#include <igris/tests/test_allocator.h>

LT_BEGIN_TEST(igris_test_suite, chunked_vector_allocator)
{
	igris::chunked_vector<int, 
		igris::test_allocator<int*>,
		igris::test_allocator<int>> vec(3);

	LT_CHECK(vec.alloc2.current == 0);
	vec.push_back(1);
	LT_CHECK(vec.size() == 1);
	LT_CHECK(vec.alloc2.current == 1);
	vec.push_back(2);
	LT_CHECK(vec.size() == 2);
	LT_CHECK(vec.alloc2.current == 1);
	vec.push_back(3);
	LT_CHECK(vec.size() == 3);
	LT_CHECK(vec.alloc2.current == 1);
	vec.push_back(4);
	LT_CHECK(vec.size() == 4);
	LT_CHECK(vec.alloc2.current == 2);
	vec.push_back(5);
	LT_CHECK(vec.size() == 5);
	LT_CHECK(vec.alloc2.current == 2);
	vec.push_back(6);
	LT_CHECK(vec.size() == 6);
	LT_CHECK(vec.alloc2.current == 2);
	vec.push_back(7);
	LT_CHECK(vec.size() == 7);
	LT_CHECK(vec.alloc2.current == 3);

	LT_CHECK(vec[0] == 1);
	LT_CHECK(vec[1] == 2);
	LT_CHECK(vec[2] == 3);
	LT_CHECK(vec[3] == 4);
	LT_CHECK(vec[4] == 5);

	LT_CHECK(vec.size() == 7);

	auto plane_vec = vec.to_vector();
	LT_CHECK(plane_vec.size() == 7);	
}
LT_END_TEST(chunked_vector_allocator)