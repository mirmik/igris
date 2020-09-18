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

	LT_CHECK_EQ(igris::graycode(8),  0b1100);	
	LT_CHECK_EQ(igris::graycode(9),  0b1101);
	LT_CHECK_EQ(igris::graycode(10), 0b1111);
	LT_CHECK_EQ(igris::graycode(11), 0b1110);
	LT_CHECK_EQ(igris::graycode(12), 0b1010);	
	LT_CHECK_EQ(igris::graycode(13), 0b1011);
	LT_CHECK_EQ(igris::graycode(14), 0b1001);
	LT_CHECK_EQ(igris::graycode(15), 0b1000);

	LT_CHECK_EQ(igris::graycode(16), 0b11000);	
	LT_CHECK_EQ(igris::graycode(17), 0b11001);
	LT_CHECK_EQ(igris::graycode(18), 0b11011);
	LT_CHECK_EQ(igris::graycode(19), 0b11010);
	LT_CHECK_EQ(igris::graycode(20), 0b11110);	
	LT_CHECK_EQ(igris::graycode(21), 0b11111);
	LT_CHECK_EQ(igris::graycode(22), 0b11101);
	LT_CHECK_EQ(igris::graycode(23), 0b11100);

	LT_CHECK_EQ(igris::graycode(24), 0b10100);	
	LT_CHECK_EQ(igris::graycode(25), 0b10101);
	LT_CHECK_EQ(igris::graycode(26), 0b10111);
	LT_CHECK_EQ(igris::graycode(27), 0b10110);
	LT_CHECK_EQ(igris::graycode(28), 0b10010);	
	LT_CHECK_EQ(igris::graycode(29), 0b10011);
	LT_CHECK_EQ(igris::graycode(30), 0b10001);
	LT_CHECK_EQ(igris::graycode(31), 0b10000);
}
LT_END_TEST(util_graycode)


LT_BEGIN_TEST(igris_test_suite, util_size)
{
	char c[3];
	LT_CHECK_EQ(igris::size(c), 3);
}
LT_END_TEST(util_size)
