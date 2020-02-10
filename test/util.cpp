#include <gtest/gtest.h>
#include <igris/util/graycode.h>
#include <igris/util/size.h>

TEST(util, graycode) 
{
	EXPECT_EQ(igris::graycode(0), 0b000);	
	EXPECT_EQ(igris::graycode(1), 0b001);
	EXPECT_EQ(igris::graycode(2), 0b011);
	EXPECT_EQ(igris::graycode(3), 0b010);
	EXPECT_EQ(igris::graycode(4), 0b110);	
	EXPECT_EQ(igris::graycode(5), 0b111);
	EXPECT_EQ(igris::graycode(6), 0b101);
	EXPECT_EQ(igris::graycode(7), 0b100);
}


TEST(util, size) 
{
	char c[3];
	EXPECT_EQ(igris::size(c), 3);
}
