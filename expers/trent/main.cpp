#include <igris/trent/trent.h>
#include <nos/print.h>

int main() 
{
	igris::trent m;

	m = 8;

	nos::println(m, m.typestr());

	m = 0.8;
	nos::println(m, m.typestr());

	m = true;
	nos::println(m, m.typestr());
}