#include <igris/time.h>

#include <thread>
#include <chrono>

int64_t igris::millis()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto millis =
	    std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return (uint16_t)millis;
}

int64_t igris::micros()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto mcross =
	    std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	return (uint16_t)mcross;
}

int igris::msleep(int64_t m)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(m));
	return 0;
}