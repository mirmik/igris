#ifndef IGRIS_CONTAINER_ARRAY_VIEW_H
#define IGRIS_CONTAINER_ARRAY_VIEW_H

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

	public:

		array_view(T* data, size_t size) : _data(data), _size(size) {}
		array_view(){}

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

		template<size_t N>
		array_view(T(&arr)[N]) : _data(arr), _size(N) {}
	};
}

#endif