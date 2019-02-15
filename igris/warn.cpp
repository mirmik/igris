#include <igris/wran.h>

	void igris::warn(const char* info, const char* err) 
	{
		std::cout << info << ": " << err << std::endl;
	}
