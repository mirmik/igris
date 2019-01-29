#include <igris/datastruct/dlist.h>
#include <igris/container/dlist.h>

#include <nos/print.h>

struct A 
{
	dlist_head lnk;
};

igris::dlist<A, &A::lnk> list;

int main() 
{
	A a,b;

	list.add_last(a);
	list.add_last(b);

	PRINT(list.size());
}