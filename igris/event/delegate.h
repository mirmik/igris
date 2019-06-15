
//by Mirmik 2015

//Непереносимо. Только для G++

//Реализация делегатов.
//Работа с указателями на метод реализуется с помощью horrible_cast.

//В данной реализации делегат является шаблонным классом, что ограничивает
//его возможности одной сигнатурой. Однако, позволяет возвращать результат.

#ifndef IGRIS_DELEGATE_H
#define IGRIS_DELEGATE_H
	
#include "igris/util/horrible_cast.h"
//#include "igris/util/stub.h"
#include <utility>

#include "inttypes.h"
	
namespace igris {
	
	class AbstractDelegate {};
	
	template<typename T, typename B> struct change_basic{};
	template<typename T, typename B, typename R, typename ... V>
	struct change_basic<T, R(B::*)(V...)>
	{ 
		using type =  R(T::*)(V...); 
	};
	
	template<typename R ,typename ... Args>	class delegate;
	template<typename R ,typename ... Args>	class fastdelegate;
	template<typename R ,typename ... Args>	class callback;
		
	//Делегат. Шаблонный класс. 
	//Параметры шаблона задают сигнатуру делегируемой функции. 
	//@1 тип возвращаемый тип.
	//@2 - ... Типы параметров. Доступно неограниченное количество параметров.
	template<typename R ,typename ... Args>	
	class delegate {	
	protected:	
		enum { METHOD, FUNCTION };
				
		using obj_t 		= AbstractDelegate*;			
		using mtd_t 		= R (AbstractDelegate::*)(Args ...);
		using fnc_t 		= R (*)(Args ...);
		using extfnc_t	 	= R (*)(void* ,Args ...);
		using absmemb_t		= std::pair<mtd_t, obj_t>;
	
	
		//Соответствует истине и будет работать только в G++
		union method_union
		{
			mtd_t method;
			struct {
				fnc_t function;	
				fnc_t attributes;
			} part;
		};

	protected:
		obj_t object;
		method_union method;
	
	public:			
		void clean() {
			object = 0;
			method.part.function = nullptr;
			method.part.attributes = 0;
		}

		bool armed() {
			return method.part.function != nullptr;			
		}

		delegate() { clean(); }		
	
		delegate(const delegate& d) {
			object = d.object;
			method.method = d.method.method;
		};
		
		delegate(delegate&& d) {
			object = d.object;
			method.method = d.method.method;
		};
	
		delegate(const fnc_t func) {
			object = 0;
			method.part.function = func;
			method.part.attributes = 0;
		};	

		template <typename T>
		delegate(R(T::*mtd)(Args ...), T* ptr_obj) {
			object = reinterpret_cast <obj_t> (ptr_obj);
			method.method = horrible_cast<mtd_t, R(T::*)(Args ...)>(mtd);
		}

		template <typename F>
		explicit delegate(const F& functor) {
			object = reinterpret_cast <obj_t> ((F*) &functor);
			method.method = horrible_cast<mtd_t, decltype(&F::operator())>(&F::operator());
		}
		
		delegate& operator=(const delegate& d) {
			object = d.object;
			method.method = d.method.method;
			return *this;
		};
				
		delegate& operator=(delegate&& d) {
			object = d.object;
			method = d.method;
			return *this;
		};
	
		R operator()(Args ... arg) {
			uint8_t type = object ? METHOD : FUNCTION;
			if (type == METHOD) 
				return (object->*method.method)(arg ...);
			else
				return method.part.function(arg ...);
		};
	
		bool operator==(delegate<R ,Args ... > b) {
			return method.method==b.method.method && object==b.object;
		};

		R emit_and_reset(Args ... args) {
			delegate<R, Args ...> copy = *this;
			clean();
			return copy(std::forward<Args>(args) ...);
		};

	};
			
	template<typename R ,typename ... Args>	
	class fastdelegate
	{
		using obj_t 		= AbstractDelegate*;			
		using mtd_t 		= R (AbstractDelegate::*)(Args ...);
		using fnc_t 		= R (*)(Args ...);
		using extfnc_t	 	= R (*)(void* ,Args ...);
		using absmemb_t		= std::pair<mtd_t, obj_t>;
	
		//Соответствует истине и будет работать только в G++
		union method_union{
			mtd_t method;
			struct{
				fnc_t function;	
				fnc_t attributes;
			} part;
		};
	
	public:
		obj_t object;
		extfnc_t extfunction;
	
	public:
		void clean() 
		{
			object = 0;
			extfunction = nullptr;
		}

		fastdelegate() { clean(); }
	
		fastdelegate(const fastdelegate& d)	: object(d.object), extfunction(d.extfunction) {};
	
		void operator= (const fastdelegate& d) volatile {
			object = d.object;
			extfunction = d.extfunction;
		}
	
		fastdelegate(absmemb_t&& pr) {
			object = pr.second;
			extfunction = reinterpret_cast<extfnc_t>(horrible_cast<method_union,mtd_t>(pr.first).function);
		};
	
		fastdelegate(extfnc_t func, void* obj) {
			object = (obj_t) obj;
			extfunction = func;
		};
	
		template <typename T>
		fastdelegate(R(T::*mtd)(Args ...), T* ptr_obj) {
			object = reinterpret_cast <obj_t> (ptr_obj);
			extfunction = reinterpret_cast<extfnc_t>(horrible_cast<method_union,R(T::*)(Args ...)>(mtd).part.function);
		}
		
		R operator()(Args ... arg) volatile { 
			return extfunction(object, arg ...);
		};
	};
	
	template<typename T, typename Ret, typename ... Args> 
	delegate<Ret, Args ...> make_delegate(Ret(T::* mtd)(Args...), T* ptr) {
		return delegate<Ret, Args...>(mtd, ptr);
	}

	template<typename Ret, typename ... Args> 
	delegate<Ret, Args ...> make_delegate(Ret(* fnc)(Args...)) {
		return delegate<Ret, Args...>(fnc);
	}

	template<typename T, typename Ret, typename ... Args> 
	fastdelegate<Ret, Args ...> make_fastdelegate(Ret(T::* mtd)(Args...), T* ptr) {
		return fastdelegate<Ret, Args...>(mtd, ptr);
	}

	template<typename Ret, typename ... Args> 
	fastdelegate<Ret, Args ...> make_fastdelegate(Ret(* fnc)(void*, Args...), void* obj) {
		return fastdelegate<Ret, Args...>(fnc, obj);
	}

	using action = delegate<void>;
	using fastaction = fastdelegate<void>;

	static inline fastaction make_fastaction(void(* fnc)()) {
		return make_fastdelegate(reinterpret_cast<void(*)(void*)>(fnc), nullptr);
	}
}

#endif
