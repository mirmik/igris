#include <igris/util/graycode.h>
#include <igris/util/size.h>

LT_BEGIN_TEST(igris_test_suite, util_graycode)
{
	LT_CHECK_EQ(igris::graycode(0), 0b000);	
	LT_CHECK_EQ(igris::graycode(1), 0b001);
	LT_CHECK_EQ(igris::graycode(2), 0b011);
	LT_CHECK_EQ(igris::graycode(3), 0b010);
	LT_CHECK_EQ(igris::graycode(4), 0b110);	
	LT_CHECK_EQ(igris::graycode(5), 0b111);
	LT_CHECK_EQ(igris::graycode(6), 0b101);
	LT_CHECK_EQ(igris::graycode(7), 0b100);
}
LT_END_TEST(util_graycode)


LT_BEGIN_TEST(igris_test_suite, util_size)
{
	char c[3];
	LT_CHECK_EQ(igris::size(c), 3);
}
LT_END_TEST(util_size)
