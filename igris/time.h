#ifndef IGRIS_TIME_H
#define IGRIS_TIME_H

#include <stdint.h>

namespace igris 
{
	int64_t millis();
	int64_t micros();

	int msleep(int64_t m);
}

#endif