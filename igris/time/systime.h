/**@file*/

#ifndef IGRIS_SYSTIME_H
#define IGRIS_SYSTIME_H

#include <igris/compiler.h>
#include <stdint.h>

#if __has_include(<asm/systime.h>)
#include <asm/systime.h>
#else
typedef int64_t systime_t;
typedef int64_t systime_difference_t;
#endif

__BEGIN_DECLS

systime_t system_time();
static inline systime_t systime() { return system_time(); }
float systime_frequency();

uint64_t systime_millis();

static inline systime_difference_t ms2systime(float ms)
{
    return ms * (systime_frequency() * 0.001f);
}

static inline systime_difference_t us2systime(float us)
{
    return us * (systime_frequency() * 0.000001f);
}

static inline systime_difference_t ns2systime(double ns)
{
    return ns * (systime_frequency() * 0.000000001);
}

systime_t millis();
systime_t micros();
systime_t nanos();

void delay(systime_t ms);

__END_DECLS

#ifdef __cplusplus
namespace igris
{
    // double time();
    void start_local_time();
    double local_time();
    systime_t nanos();
}
#endif

#endif
