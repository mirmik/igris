#ifndef IGRIS_CPU_DELAY_H
#define IGRIS_CPU_DELAY_H

#include <igris/compiler.h>

__BEGIN_DECLS

static inline void cpu_delay(unsigned long long ticks)
{
    volatile unsigned long long t = ticks;
    while (--t)
        ;
}

__END_DECLS

#endif
