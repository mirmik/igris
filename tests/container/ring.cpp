#include <doctest/doctest.h>
#include <igris/container/ring.h>

class A 
{

};

class B 
{
	int i;

public:
	B(){}
	B(int j) : i(j) {}
};

TEST_CASE("ring") 
{
	A abuf[12];
	B bbuf[3];

	igris::ring<A> aring(abuf, 12);
	igris::ring<B> bring(bbuf, 3);
	
	CHECK_EQ(aring.avail(), 0);
	CHECK_EQ(aring.room(), 11);

	CHECK_EQ(bring.avail(), 0);
	CHECK_EQ(bring.room(), 2);

	aring.push(A());
}