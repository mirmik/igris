#include <gtest/gtest.h>
#include <igris/container/array_view.h>

TEST(array_view, initializer_list) 
{
	igris::array_view<float> arr { 0.1, 0.2 };
	EXPECT_EQ(arr[0], 0.1f);
	EXPECT_EQ(arr[1], 0.2f);
}
