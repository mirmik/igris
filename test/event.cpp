#include <gtest/gtest.h>
#include <igris/event/delegate.h>

int sum(int a, int b) 
{
	return a + b;
} 

int sub(int a, int b) 
{
	return a - b;
} 

TEST(delegate, EVENT) 
{
	int a = 5;
	int b = 3;

	igris::delegate<int,int,int> func;

	func = sum;
	EXPECT_EQ(func(a,b), a+b);	

	func = sub;
	EXPECT_EQ(func(a,b), a-b);	
}
