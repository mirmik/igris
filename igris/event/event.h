#ifndef IGRIS_EVENT_EVENT_H
#define IGRIS_EVENT_EVENT_H

#include <igris/event/delegate.h>

namespace igris 
{
	class event_args {};
	using event = igris::delegate<void, void*>;
}

#endif