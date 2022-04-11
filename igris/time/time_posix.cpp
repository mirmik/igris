#include <igris/time/systime.h>

#include <chrono>
#include <thread>

systime_t millis()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

systime_t micros()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto mcross =
        std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return mcross;
}

systime_t nanos()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto ncross =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return ncross;
}

systime_t igris::millis()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto millis =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return millis;
}

systime_t igris::micros()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto mcross =
        std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return mcross;
}

systime_t igris::nanos()
{
    auto duration = std::chrono::system_clock::now().time_since_epoch();
    auto ncross =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    return ncross;
}

int msleep(int64_t m)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(m));
    return 0;
}



static int64_t __local_time = igris::nanos();

void igris::start_local_time() { __local_time = igris::nanos(); }

double igris::local_time()
{
    return (double)((igris::nanos() - __local_time) * 1e-9);
}
