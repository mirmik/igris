#include <igris/compiler.h>
#include <igris/dprint.h>

__WEAK
void emergency_stop() { debug_print("std_emergency_stop"); }
