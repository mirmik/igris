#include <igris/util/bits.h>

#ifndef _MSC_VER
LT_BEGIN_TEST(igris_test_suite, bits)
{
	uint32_t a = 0b101101101101;
	bits_masked_assign_multimap(a, 0b0101, 0b011, 3);
	LT_CHECK_EQ(bits_multimap(0b1011, 0b101, 3), 0b101000101101);
	LT_CHECK_EQ(a, 0b101011101011);
}
LT_END_TEST(bits)
#endif



