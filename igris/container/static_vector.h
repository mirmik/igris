#ifndef IGRIS_CONTAINER_STATIC_VECTOR_H
#define IGRIS_CONTAINER_STATIC_VECTOR_H

#include <new>
#include <type_traits>
#include <utility>

namespace igris
{
    template <class T, std::size_t N> class static_vector
    {
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_iterator = const T *;

        // Переместить во внешний буффер.
        typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
        std::size_t m_size = 0;

    public:
        // Create an object in aligned storage
        template <typename... Args> void emplace_back(Args &&... args)
        {
            if (m_size >= N)
                return;

            // construct value in memory of aligned storage
            // using inplace operator new
            new (&data[m_size]) T(std::forward<Args>(args)...);
            ++m_size;
        }

        T &operator[](std::size_t pos)
        {
            return *reinterpret_cast<T *>(&data[pos]);
        }

        const T &operator[](std::size_t pos) const
        {
            return *reinterpret_cast<const T *>(&data[pos]);
        }

        std::size_t room() { return N - m_size; }
        std::size_t size() { return m_size; }

        // Delete objects from aligned storage
        ~static_vector()
        {
            for (std::size_t pos = 0; pos < m_size; ++pos)
            {
                // note: needs std::launder as of C++17
                reinterpret_cast<T *>(&data[pos])->~T();
            }
        }

        iterator begin() { return reinterpret_cast<T *>(&data[0]); }
        const_iterator end() { return reinterpret_cast<T *>(&data[m_size]); }
    };
}

#endif
