#ifndef igris_ARRAY_H
#define igris_ARRAY_H

#include <memory>
//#include <igris/objbuf.h>

namespace igris
{
	template<typename T, typename A = std::allocator<T>>
	class unbounded_array
	{
	public:
		using allocator_type = A;
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;

		T* 		m_data;
		size_t 	m_size;
		A 		alloc;

		CONSTREF_GETTER(data, m_data);
		CONSTREF_GETTER(size, m_size);

		unbounded_array()
			: m_data(nullptr), m_size(0) {}

		unbounded_array(size_t sz)
			: m_data(alloc.allocate(sz)), m_size(sz) {}

		unbounded_array(const igris::array_view<T>& buf)
			: m_data(alloc.allocate(buf.size())), m_size(buf.size())
		{
			std::copy(buf.begin(), buf.end(), m_data);
		}

		unbounded_array(const std::initializer_list<T>& buf)
			: m_data(alloc.allocate(buf.size())), m_size(buf.size())
		{
			std::copy(buf.begin(), buf.end(), m_data);
		}

		unbounded_array(const unbounded_array& oth)
			: m_data(alloc.allocate(oth.size())), m_size(oth.size())
		{
			auto ptr = m_data;
			for (const auto& ref : oth)
			{
				new (ptr++) T(ref);
			}
		}

		unbounded_array(unbounded_array&& arr)
			: m_data(arr.m_data), m_size(arr.m_size), alloc(arr.alloc)
		{
			arr.m_size = 0;
			arr.m_data = nullptr;
		}

		~unbounded_array()
		{
			alloc.deallocate(m_data, m_size);
		}

		T& operator[](size_t i)
		{
			return *(m_data + i);
		}

		const T& operator[](size_t i) const
		{
			return *(m_data + i);
		}

		using iterator = T*;
		using const_iterator = const T*;

		iterator begin() { return m_data; }
		const iterator end() { return m_data + m_size; }

		const_iterator begin() const { return (const_iterator) m_data; }
		const const_iterator end() const  { return (const_iterator) m_data + m_size; }
	};
}

#endif