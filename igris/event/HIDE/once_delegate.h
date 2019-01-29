#ifndef GXX_EVENT_ONCE_DELEGATE_H
#define GXX_EVENT_ONCE_DELEGATE_H

#include <gxx/event/delegate.h>

namespace gxx {
	template<typename Ret, typename ... Args>
	class once_delegate : public delegate<Ret, Args ...> {
		using Parent = delegate<Ret, Args ...>;

	public:
		//template<typename ... TArgs>
		//once_delegate(TArgs&& ... args) : Parent(std::forward<TArgs>(args) ... ) {}

		once_delegate(const gxx::delegate<Ret, Args ...>& dlg) : Parent(dlg) {}

		template<typename ... TArgs>
		once_delegate(TArgs&& ... args) : Parent(std::forward<TArgs>(args) ...) {}

		template<typename ... TArgs>
		void operator=(TArgs&& ... args) { 
			Parent::operator=(std::forward<TArgs>(args) ... );
		}

		Ret operator()(Args ... args) {
			gxx::delegate<Ret, Args ...> copy = (Parent&)*this;
			Parent::clean();
			return copy(std::forward<Args>(args) ...);
		};
	};

	class once_delegate_flag {
		once_delegate<void> dlg;
		bool flag = false;

	public:
		void set() {
			flag = true;
			if (dlg.armed()) {
				dlg();
			} 
		}

		void clr() {
			flag = false;
		}

		bool armed() {
			return dlg.armed();
		}

		void event(gxx::delegate<void>& ev) {
			if (flag) {
				ev();
			}
			else {
				dlg = ev;
			}
		}
	};
}

#endif