#include <igris/tests/test_allocator.h>

LT_BEGIN_TEST(igris_test_suite, test_allocator_test)
{
	igris::test_allocator<int> all;

	int * ptr = all.allocate(4);
	all.deallocate(ptr, 4);

	LT_CHECK(all.allocations == 1);
	LT_CHECK(all.deallocations == 1);
	LT_CHECK(all.current == 0);
}
LT_END_TEST(test_allocator_test)