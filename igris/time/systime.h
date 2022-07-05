/**@file*/

#ifndef IGRIS_SYSTIME_H
#define IGRIS_SYSTIME_H

#include <cstdint>

namespace igris 
{
    int64_t system_time();
    static inline int64_t systime() { return system_time(); }
    uint32_t systime_frequency();
    uint64_t systime_millis();

    static inline int64_t ms2systime(float ms)
    {
        return ms * (systime_frequency() * 0.001f);
    }

    static inline int64_t us2systime(float us)
    {
        return us * (systime_frequency() * 0.000001f);
    }

    static inline int64_t ns2systime(double ns)
    {
        return ns * (systime_frequency() * 0.000000001);
    }

    int64_t millis();
    int64_t micros();
    int64_t nanos();

    void delay(int64_t ms);
}

#endif
