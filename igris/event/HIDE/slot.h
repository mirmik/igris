#ifndef GXX_EVENTS_SLOT_H
#define GXX_EVENTS_SLOT_H

#include <gxx/delegate.h>

namespace gxx {
	template <typename ... Args>
	class basic_slot {
	protected:
		struct {
			uint8_t flag_unconnect_on_invoke : 1;
		};

	public:
		basic_slot() : flag_unconnect_on_invoke(false) {}

		virtual void invoke(Args ... args) = 0;
	
		void unconnect_on_invoke(bool en) {
			flag_unconnect_on_invoke = en;
		}

		//manage
		dlist_head lnk;
	};

	template <typename ... Args>
	class slot : public basic_slot<Args ...> {
		using Parent = basic_slot<Args ...>;
		delegate<void, Args ...> dlg;

	public:
		slot(gxx::delegate<void, Args ...> dlg) : dlg(dlg) {}
		slot(void(*func)(Args ...)) : dlg(func) {}

		void invoke(Args ... args) {
			if (this->flag_unconnect_on_invoke) dlist_del_init(&this->lnk);
			dlg(args ...);
		}
	};
}

#endif