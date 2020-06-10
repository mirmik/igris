#include <igris/datastruct/dlist.h>
#include <nos/print.h>
#include <nos/fprint.h>

DLIST_HEAD(list);

struct A 
{
	DLIST_HEAD(lnk);
	int key;
	std::string name;

	ssize_t print_to(nos::ostream& os) const
	{
		return nos::fprint_to(os, "{}: {}", key, name);
	}
};

bool comparator(A* a, A* b) 
{
	return a->key < b->key;
}

int main() 
{
	A a, b, c, d, e;

	nos::println("hello world");

	a.name = "a";
	b.name = "b";
	c.name = "c";
	d.name = "d";
	e.name = "e";

	a.key = 2;
	b.key = 4;
	c.key = 3;
	d.key = 1;
	e.key = 5;

	A * pos;
	dlist_move_sorted(&a, &list, lnk, comparator);
	dlist_move_sorted(&b, &list, lnk, comparator);
	dlist_move_sorted(&c, &list, lnk, comparator);
	dlist_move_sorted(&d, &list, lnk, comparator);
	dlist_move_sorted(&e, &list, lnk, comparator);

	dlist_for_each_entry(pos, &list, lnk) 
	{
		nos::println(*pos);
	}
}