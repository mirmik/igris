#ifndef GXX_MULTIPLE_DELEGATE_H
#define GXX_MULTIPLE_DELEGATE_H

#include <gxx/event/delegate.h>
#include <vector> 

namespace gxx {
	template<typename ... Args>
	class multiple_delegate {
	public:
		std::vector<delegate<void,Args...>> vect;
	
		void operator()(Args ... args) {
			for(auto dlg : vect) {
				dlg(args ...);
			}
		}
	
		template<typename ... TArgs>
		void operator += (TArgs ... args) {
			add(std::forward<TArgs>(args) ...);
		}

		template<typename ... TArgs>
		void operator -= (TArgs ... args) {
			erase(std::forward<TArgs>(args) ...);
		}

		template<typename ... TArgs>
		void add(const TArgs& ... args) {
			vect.emplace_back(args ...);
		}

	//	template <typename T>
	//	void add(member<T> mbr, T* obj) {
	//		vect.emplace_back(mbr, obj);
	//	}
	
	//	void add(fnc_t func) {
	//		vect.emplace_back(func);
	//	}
	
		/*template <typename T>
		void priorityConnect(member<T> mbr, T* obj) {
			vect.emplace(vect.begin(),mbr, obj);
		}
	
		void priorityConnect(fnc_t func) {
			vect.emplace(vect.begin(),func);
		}*/
	
		bool erase(const delegate<void,Args...>& dlg) {
			for(auto it = vect.begin(); it != vect.end(); it++) {
				if (*it == dlg) { 
					vect.erase(it);
					return true;
				}
			}
			return false;
		}
	};
}

#endif