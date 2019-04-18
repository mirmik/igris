#include <gtest/gtest.h>
#include <igris/util/bits.h>

TEST(bits, BITS) 
{
	uint32_t a = 0b101101101101;
	bits_masked_assign_multimap(a, 0b0101, 0b011, 3);

	EXPECT_EQ(bits_multimap(0b1011, 0b101, 3), 0b101000101101);
	EXPECT_EQ(a, 0b101011101011);
}
