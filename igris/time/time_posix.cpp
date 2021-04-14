#include <igris/time.h>

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

