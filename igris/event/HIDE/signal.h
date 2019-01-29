#ifndef GXX_EVENTS_SIGNAL_H
#define GXX_EVENTS_SIGNAL_H

#include <gxx/dlist.h>
#include <gxx/events/slot.h>
#include <gxx/algorithm.h>

namespace gxx {
	//
	template <typename ... Args>
	class signal {
	protected:
		using Slot = basic_slot<Args ...>;
		dlist<Slot, &Slot::lnk> slots;

	public:
		virtual void connect(Slot& slt) {
			slots.move_back(slt);
		}

		void emit(Args ... args) {
			gxx::for_each_safe(slots.begin(), slots.end(), [&](Slot& slt) {
				slt.invoke(args ...);
			});
		} 

		void emit_one(Args ... args) {
			slots.begin()->invoke(args ...);
		} 
	};

	//Специальный тип сигнала, позволяющий сохранять информацию о событии
	//до момента подключения следующего слота
	class sigflag : public signal<> {
		using Parent = signal<>;
		bool flag = false;

	private:
		void emit_loop() {
			while(flag == true && !slots.empty()) {
				emit_one();
			}
		}
	public:
		void connect(Slot& slt) {
			Parent::connect(slt);
			if (flag) emit_loop();
		}

		void sigon() {
			flag = true;
			emit_loop();
		}	

		void sigoff() {
			flag = false;
		}
	};
}

#endif