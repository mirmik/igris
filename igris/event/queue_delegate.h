#ifndef GXX_EVENT_QUEUE_DELEGATE_H
#define GXX_EVENT_QUEUE_DELEGATE_H

#include <gxx/ring.h>
#include <gxx/event/delegate.h>

namespace gxx {
	template<typename ... Args>
	class queue_delegate {
		gxx::ring<gxx::delegate<void, Args ...>> ring;

	public:
		void operator+=(const gxx::delegate<void, Args ...>& dlg) {
			ring.push(dlg);
		}
	};
}

#endif