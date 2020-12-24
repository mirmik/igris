#include <igris/time.h>

#include <thread>
#include <chrono>

unsigned long millis()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto millis =
	    std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return millis;
}

unsigned long micros()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto mcross =
	    std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	return mcross;
}

int msleep(long m)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(m));
	return 0;
}

int64_t igris::millis()
{
	return ::millis();
}

int64_t igris::micros()
{
	return ::micros();
}

int igris::msleep(int64_t m)
{
	return ::msleep(m);
}
