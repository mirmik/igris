#include <igris/container/dualmap.h>
#include <igris/dprint.h>

#include <string>
#include <nos/print.h>

int main() 
{
	igris::dualmap<int, std::string, float> m;

	m.insert(42, "hello", 42);

	dprln(m.at(std::pair<int, std::string>(42, "hello")));
	dprln(m.remove(std::pair<int, std::string>(42, "hello")));

	dprln(m.contains(std::pair<int, std::string>(42, "hello")));
}