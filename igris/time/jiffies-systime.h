#ifndef IGRIS_JIFFIES_SYSTIME_H
#define IGRIS_JIFFIES_SYSTIME_H

#include <stdint.h>
#include <igris/compiler.h>

typedef int64_t jiffies_t;

__BEGIN_DECLS

void jiffies_increment();
void systime_set_frequency(uint32_t freq);
jiffies_t jiffies();
//uint32_t systime_lomax();
//uint32_t systime_lopart();
jiffies_t ms2jiffies(uint32_t ms);

__END_DECLS

#endif