#include <doctest/doctest.h>
#include <igris/container/ring.h>

class A 
{

};

class B 
{
//	int i;

public:
	B(){}
//	B(int j) : i(j) {}
};

TEST_CASE("ring") 
{
	A abuf[12];
	igris::ring<A> aring(abuf, 12);
	igris::ring<B,3> bring;
	
	CHECK_EQ(aring.avail(), 0);
	CHECK_EQ(aring.room(), 11);

	CHECK_EQ(bring.avail(), 0);
	CHECK_EQ(bring.room(), 2);

	aring.push(A());
	CHECK_EQ(aring.room(), 10);
	aring.pop();
	CHECK_EQ(aring.room(), 11);
}
