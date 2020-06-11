#ifndef IGRIS_CONTAINER_ARRAY_VIEW_H
#define IGRIS_CONTAINER_ARRAY_VIEW_H

#include <stdlib.h>
#include <utility>
#include <initializer_list> 

namespace igris 
{
	template <class T>
	class array_view 
	{
		T* _data;
		size_t _size;
	public:
		using iterator = T*;
		using const_iterator = const T*;
		using value_type = T;

	public:

		array_view(T* data, size_t size) : _data(data), _size(size) {}
		array_view() : _data(nullptr), _size(0) {}

		template<size_t N>
		array_view(T(&arr)[N]) : _data(arr), _size(N) {}

		// Warning: UB
/*		array_view(const std::initializer_list<T>& lst) :
			_data((T*)lst.begin()), _size(lst.size())
		{}*/

		array_view& operator=(std::nullptr_t null) 
		{ _data = nullptr; _size = 0; return *this; }

		T* const data() const { return _data; }
		T* data() { return _data; }

		size_t size() const { return _size; } 

		T* begin() { return _data; }
		T* const end() { return _data + _size; }
		const T* begin() const { return _data; }
		const T* const end() const { return _data + _size; }

		T* rbegin() { return _data + _size - 1; }
		T* const rend() { return _data - 1; }
		const T* rbegin() const { return _data + _size - 1; }
		const T* const rend() const { return _data - 1; }

		T& operator [] (int i) { return *(_data + i); }
		const T& operator [] (int i) const { return *(_data + i); }

		array_view slice(int start, int size)
		{ return array_view(_data + start, size); }
	};
}

#endif