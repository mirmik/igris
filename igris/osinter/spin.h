#ifndef IGRIS_OSINTER_SPIN_H
#define IGRIS_OSINTER_SPIN_H

namespace igris 
{
	void spin(void(*func)(void*), void* arg);
}

#endif