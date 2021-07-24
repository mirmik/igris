#include <doctest/doctest.h>
#include <igris/datastruct/sparse_array.h>

struct A 
{
	int i;
	int j;
};

TEST_CASE("sparse_array") 
{
	A a, b;

	a.i = 1;
	a.j = 2;
	b.i = 3;
	b.j = 4;

	SPARSE_ARRAY(array0, &a.i, sizeof(A));
	SPARSE_ARRAY(array1, &a.j, sizeof(A));

	CHECK_EQ(sparse_array_ref(&array0, 0, int), 1);
	CHECK_EQ(sparse_array_ref(&array0, 1, int), 3);
	CHECK_EQ(sparse_array_ref(&array1, 0, int), 2);
	CHECK_EQ(sparse_array_ref(&array1, 1, int), 4);

	
	int * it = &a.i;
	CHECK_EQ(sparse_array_next(it, array0.stride), &b.i);

	int acc = 0;
	sparse_array_for_each(it, &array0, 2) 
	{
		acc += *it;
	}
	CHECK_EQ(acc, 1+3);

	acc = 0;
	sparse_array_for_each(it, &array1, 2) 
	{
		acc += *it;
	}
	CHECK_EQ(acc, 2+4);
}