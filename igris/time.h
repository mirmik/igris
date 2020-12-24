#ifndef IGRIS_TIME_H
#define IGRIS_TIME_H

#include <stdint.h>

#ifdef __cplusplus
namespace igris 
{
	int64_t millis();
	int64_t micros();

	int msleep(int64_t m);
}
#endif

__BEGIN_DECLS

unsigned long millis();
unsigned long micros();

__END_DECLS

#endif
