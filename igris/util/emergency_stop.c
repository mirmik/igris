#include <igris/dprint.h>
#include <igris/compiler.h>

__WEAK
void emergency_stop() 
{
	debug_print("std_emergency_stop");
}
