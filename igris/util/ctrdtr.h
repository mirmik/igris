#ifndef igris_UTIL_CTR_DTR_H
#define igris_UTIL_CTR_DTR_H

//#include <igris/util/placed_new.h>

namespace igris
{
	template<typename T>
	void destructor(T* ptr)
	{
		ptr->~T();
	}

	template<typename T, typename ... Args>
	void constructor(T* ptr, Args&& ... args)
	{
		new(ptr) T(std::forward<Args>(args)...);
	}

	template<typename T>
	void copy_constructor(T* ptr, const T& other)
	{
		new(ptr) T(other);
	}

	template<typename T>
	void move_constructor(T* ptr, T&& other)
	{
		new(ptr) T(std::forward<T>(other));
	}

	template<class InputIterator>
	void
	array_destructor(InputIterator first, InputIterator last)
	{
		while (first != last)
		{
			igris::destructor(&*first);
			++first;
		}
	}

	template<class InputIterator, typename ... Args>
	void
	array_constructor(InputIterator first, InputIterator last, Args ... args)
	{
		while (first != last)
		{
			igris::constructor(&*first, args ...);
			++first;
		}
	}

}

#endif