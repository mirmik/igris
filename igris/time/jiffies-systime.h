#include <stdint.h>

#include <igris/compiler.h>

typedef int64_t jiffies_t;

__BEGIN_DECLS

void system_tick();
jiffies_t jiffies();
uint32_t systime_lomax();
uint32_t systime_lopart();

__END_DECLS
