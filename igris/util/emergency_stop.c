#include <igris/dprint.h>

__attribute__((weak))
void emergency_stop() 
{
	dprln("std_emergency_stop");
}
