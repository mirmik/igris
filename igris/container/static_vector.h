#ifndef IGRIS_CONTAINER_STATIC_VECTOR_H
#define IGRIS_CONTAINER_STATIC_VECTOR_H

#include <new>
#include <type_traits>
#include <string.h>
#include <utility>
#include <algorithm>

namespace igris
{
    template <class T, std::size_t N> class static_vector
    {
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_iterator = const T *;

        // Переместить во внешний буффер.
        typename std::aligned_storage<sizeof(T), alignof(T)>::type _data[N];
        std::size_t m_size = 0;

    public:
        static_vector() 
        {
            memset(_data, 0, sizeof(_data));
        }

        // Create an object in aligned storage
        template <typename... Args> void emplace_back(Args &&... args)
        {
            if (m_size >= N)
                return;
            new (&_data[m_size]) T(std::forward<Args>(args)...);
            ++m_size;
        }

        T &operator[](std::size_t pos)
        {
            return *reinterpret_cast<T *>(&_data[pos]);
        }

        const T &operator[](std::size_t pos) const
        {
            return *reinterpret_cast<const T *>(&_data[pos]);
        }

        T* data() 
        {
            return *reinterpret_cast<T *>(&_data[0]);
        }

        const T* data() const 
        {
            return *reinterpret_cast<const T *>(&_data[0]);
        }

        std::size_t room() const { return N - m_size; }
        std::size_t size() const { return m_size; }

        // Delete objects from aligned storage
        ~static_vector()
        {
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                reinterpret_cast<T *>(&_data[pos])->~T();
            }
        }

        iterator begin() 
        { 
            return reinterpret_cast<T *>(&_data[0]); 
        }
        
        const_iterator end() 
        { 
            return reinterpret_cast<T *>(&_data[m_size]); 
        }

        const_iterator begin() const
        { 
            return reinterpret_cast<const T *>(&_data[0]); 
        }
        
        const_iterator end() const
        { 
            return reinterpret_cast<const T *>(&_data[m_size]); 
        }

        void resize(size_t newsize) 
        {
            if (newsize >= N)
                newsize = N;
         
            for (size_t i = m_size; i < newsize; ++i) 
            {
                new (&_data[i]) T{};
            }
        
            m_size = newsize;
        }

        template<class U>
        static_vector& operator=(const U& oth) 
        {
            resize(oth.size());
            for (int i = 0; i < oth.size(); ++i) 
                operator[](i) = oth[i];
        }
    };
}

#endif
