#include <owl/wran.h>

	void owl::warn(const char* info, const char* err) 
	{
		std::cout << info << ": " << err << std::endl;
	}
