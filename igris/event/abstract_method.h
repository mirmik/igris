#ifndef GXX_ABSTRACT_METHOD_H
#define GXX_ABSTRACT_METHOD_H

#include <gxx/event/delegate.h>

namespace gxx {
	template<typename R ,typename ... Args>	
	class abstract_method {
		using obj_t 		= AbstractDelegate*;			
        using mtd_t 		= R (AbstractDelegate::*)(Args ...);

        mtd_t method;

	public:
		template <typename T>
		abstract_method(R(T::*mtd)(Args ...)) {
            method = horrible_cast<mtd_t, R(T::*)(Args ...)>(mtd);
		}

        R operator()(void* obj, Args ... args) volatile {
            return (reinterpret_cast<obj_t>(obj)->*method)(std::forward<Args>(args) ...);
        }
	};
}

#endif
