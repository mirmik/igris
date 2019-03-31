#ifndef IGRIS_UTIL_CTRDTR_H
#define IGRIS_UTIL_CTRDTR_H

namespace igris {
	template<typename T>
	void destructor(T* ptr) {
		ptr->~T();
	}
	
	template<class InputIterator>  
	void
	array_destructor(InputIterator first, InputIterator last)
	{
		while(first != last){
			igris::destructor(&*first);
			++first;
		}
	}
	
	template<typename T, typename ... Args>
	void constructor(T* ptr, Args&& ... args) {
		new(ptr) T(std::forward<Args>(args)...);
	}
	
	template<typename T>
	void copyConstructor(T* ptr, const T& other) {
		new(ptr) T(other);
	}
	
	template<typename T>
	void moveConstructor(T* ptr, T&& other) {
		new(ptr) T(std::forward<T>(other));
	}
	
	template<class InputIterator, typename ... Args>  
	void
	array_constructor(InputIterator first, InputIterator last, Args ... args) {
		while(first != last){
			igris::constructor(&*first, args ...);
			++first;
		}
	}

}

#endif