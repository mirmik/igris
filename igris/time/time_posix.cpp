#include <igris/systime.h>

#include <chrono>
#include <thread>

int64_t millis()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

int64_t micros()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto mcross =
        std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return mcross;
}

int msleep(int64_t m)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(m));
    return 0;
}


/*double igris::time()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return (double)ns / 1.e9;
}*/

int64_t igris::nanos()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return ns;
}

static int64_t __local_time = igris::nanos();

void igris::start_local_time() 
{
    __local_time = igris::nanos();
}

double igris::local_time() 
{
    return (double)((igris::nanos() - __local_time)*1e-9);
}