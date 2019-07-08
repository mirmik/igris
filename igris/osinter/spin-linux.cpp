#include <igris/osinter/spin.h>

#include <thread>
#include <chrono>

using namespace std::chrono::literals;

void igris::spin(void(*func)(void*), void* arg) 
{
	std::thread([=]()
	{
		while(1) 
		{
			func(arg);
			std::sleep_for(1ms);
		}
	}
	).detach();
}