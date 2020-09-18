#include <igris/container/unbounded_array.h>

LT_BEGIN_TEST(igris_test_suite, unbounded_array)
{
	igris::unbounded_array<float> arr(35);

	arr[28] = 33;

	LT_CHECK_EQ(arr[28], 33);
}
LT_END_TEST(unbounded_array)