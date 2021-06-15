#ifndef IGRIS_SYSTIME_H
#define IGRIS_SYSTIME_H

#include <igris/compiler.h>
#include <stdint.h>

__BEGIN_DECLS

int64_t millis();
void delay(int64_t ms);
int msleep(int64_t m);

__END_DECLS

namespace igris
{
    // double time();
    void start_local_time();
    double local_time();
    int64_t nanos();
}

#endif