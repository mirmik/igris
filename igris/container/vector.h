#ifndef IGRIS_VECTOR_H
#define IGRIS_VECTOR_H

#include <cassert>
#include <algorithm>
#include <memory>
#include <utility>
#include <initializer_list>
#include <igris/util/ctrdtr.h>
#include <memory>

namespace igris 
{
	template<typename T, class Allocator = std::allocator<T>>
	class vector
	{
	public:
		using iterator = T*;
		using const_iterator = const T*;

	private:
		//resources
		T* m_data = nullptr;
		size_t m_capacity = 0;
		size_t m_size = 0;

		//generated
		Allocator m_alloc;

	public:
		T* data() { return m_data; }
		const T* data() const { return m_data; }
		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }


		vector(const Allocator& alloc = Allocator()) : m_alloc(alloc) {}

		vector(const std::initializer_list<T>& initializers) 
		{
			reserve(initializers.size());
			for (auto& a : initializers)
				push_back(a);
		}

		vector(std::initializer_list<T>&& initializers) 
		{
			reserve(initializers.size());
			for (auto& a : initializers)
				push_back(a);
		}

		vector(const vector& other) :
			m_size(other.m_size)
		{
			m_data = m_alloc.allocate(m_size);
			m_capacity = m_size;
			for (auto ip = other.m_data, op = m_data; ip != other.m_data + other.m_size; ip++, op++)
			{
				igris::constructor(op, *ip);
			}
		}

		template <class I, class O>
		vector(I first, O last) 
		{
			reserve(std::distance(first, last));
			for (;first!=last;first++) 
			{
				push_back(*first);
			}
		}

		vector(vector&& other) :
			m_data(other.m_data),
			m_capacity(other.m_capacity),
			m_size(other.m_size)
		{
			other.m_data = nullptr;
			other.m_capacity = 0;
			other.m_size = 0;
		}

		vector& operator= (const vector& other)
		{
			if (*this == other)
				return *this;

			invalidate();

			m_data = m_alloc.allocate(m_size);
			m_size = other.m_size;
			m_capacity = m_size;
			for (auto ip = other.m_data, op = m_data; ip != other.m_data + other.m_size; ip++, op++)
			{
				igris::constructor(op, *ip);
			}

			return *this;
		}


		vector& operator= (vector&& other)
		{
			if (*this == other)
				return *this;
			m_data = other.m_data;
			m_capacity = other.m_capacity;
			m_size = other.m_size;
			other.m_data = nullptr;
			other.m_capacity = 0;
			other.m_size = 0;
			return *this;
		}

		vector(size_t sz) :
			m_data(nullptr),
			m_capacity(0),
			m_size(0)
		{
			resize(sz);
		}

		vector(iterator a, const iterator b) :
			m_data(nullptr),
			m_capacity(0),
			m_size(0)
		{
			while (a != b)
			{
				push_back(*a++);
			}
		}

		~vector()
		{
			invalidate();
		}

		void invalidate()
		{
			if (m_data)
			{
				igris::array_destructor(begin(), end());
				if (m_data) m_alloc.deallocate(m_data, m_capacity);
			}
			m_data = nullptr;
			m_size = 0;
			m_capacity = 0;
		}

		unsigned char reserve(size_t sz)
		{
			if (sz > m_capacity) 
				return changeBuffer(sz);
			return 1;
		}

		void clear()
		{
			for (unsigned int i = 0; i < m_size; ++i)
			{
				igris::destructor(m_data + i);
			}
			m_size = 0;
		}

		iterator begin() { return m_data; }
		const_iterator end() { return m_data + m_size; }
		iterator rbegin() { return m_data + m_size - 1; }
		const_iterator rend() { return m_data - 1; }

		const_iterator begin() const { return m_data; }
		const_iterator end() const { return m_data + m_size; }
		const_iterator rbegin() const { return m_data + m_size - 1; }
		const_iterator rend() const { return m_data - 1; }

		template <typename ... Args>
		void emplace_back(Args&& ... args)
		{
			reserve(m_size + 1);
			igris::constructor(m_data + m_size, std::forward<Args>(args)...);
			m_size++;
		}

		void push_back(const T& ref)
		{
			reserve(m_size + 1);
			igris::constructor(m_data + m_size, ref);
			m_size++;
		}

		bool operator == (const vector & oth) const
		{
			if (size() != oth.size())
				return false;

			auto it = begin();
			auto eit = end();
			auto bit = oth.begin();

			for (; it != eit; ++it, ++bit)
			{
				if (*it != *bit)
					return false;
			}

			return true;
		}

		bool operator != (const vector & oth) const
		{
			return ! operator==(oth);
		}

		template<typename ... Args>
		iterator emplace(const_iterator pos, Args&& ... args)
		{
			//TODO insert optimization
			size_t _pos = pos - m_data;

			reserve(m_size + 1);
			m_size++;

			iterator first = m_data + _pos;
			iterator last = std::prev(end());
			std::move_backward(first, last, end());
			new (first) T(std::forward<Args>(args) ...);

			return first;
		}

		iterator insert(const_iterator pos, const T& value)
		{
			//TODO insert optimization
			size_t _pos = pos - m_data;

			reserve(m_size + 1);
			m_size++;

			iterator first = m_data + _pos;
			iterator last = std::prev(end());
			std::move_backward(first, last, end());
			*first = value;

			return first;
		}

		iterator insert(int pos, const T& value)
		{
			return insert(m_data + pos, value);
		}

		iterator insert_sorted(T const& item)
		{
			return insert(std::upper_bound(begin(), end(), item ), item);
		}

		T& operator[](size_t num)
		{
			return *(m_data + num);
		}

		const T& operator[](size_t num) const 
		{
			return *(m_data + num);
		}

		void resize(size_t n)
		{
			reserve(n);
			m_size = n;
		}

		void erase(iterator newend)
		{
			m_size = newend - m_data;
		}

	protected:
		unsigned char changeBuffer (size_t sz)
		{
			size_t oldcapacity = m_capacity;
			auto newbuf = m_alloc.allocate(sz);
			assert((uintptr_t)newbuf % sizeof(uintptr_t) == 0);
			m_capacity = sz;
			if (m_data == nullptr)
			{
				m_data = newbuf;
				return 1;
			}

			auto ie = end();

			for (auto ip = begin(), op = newbuf; ip != ie; op++, ip++)
			{
				igris::move_constructor(op, std::move(*ip));
			}

			igris::array_destructor(begin(), end());
			auto oldbuf = m_data;
			m_data = newbuf;
			m_alloc.deallocate(oldbuf, oldcapacity);
			return 1;
		}
	};
}

#endif