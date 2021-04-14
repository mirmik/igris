#ifndef IGRIS_SYSTIME_H
#define IGRIS_SYSTIME_H

#include <stdint.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

int64_t millis();
void delay(int64_t ms);

int msleep(int64_t m);

__END_DECLS

#endif