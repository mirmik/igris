#include <igris/event/delegate.h>

int sum(int a, int b) 
{
	return a + b;
} 

int sub(int a, int b) 
{
	return a - b;
} 

class B
{
public:
	int b = 3;
	int inc(int a, int c) { return a + b; }
};

LT_BEGIN_TEST(igris_test_suite, delegate_test)
{
	int a = 5;
	int b = 3;

	igris::delegate<int,int,int> func;

	func = sum;
	LT_CHECK_EQ(func(a,b), a+b);	

	func = sub;
	LT_CHECK_EQ(func(a,b), a-b);	

	B a_obj;
	a_obj.b = 4;
	func = igris::make_delegate(&B::inc, &a_obj);
	LT_CHECK_EQ(func(7, 100500), 11);	
}
LT_END_TEST(delegate_test)
