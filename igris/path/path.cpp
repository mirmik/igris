#include <igris/path/path.h>
#include <igris/creader.h>

std::string igris::path::dirname(const std::string& path)
{
	const char* strt = path.c_str();
	const char* ptr = strt;
	const char* last = strt;

	igris::chars_set_checker pattern("/\\");

	while (*ptr)
	{
		if (pattern(*ptr)) last = ptr;
		++ptr;
	}

	return std::string(strt, last - strt);
}
