#ifndef STD_SPAN_H
#define STD_SPAN_H

#include <array>
#include <cstddef>

namespace igris
{
    template <class T> class span
    {
        T *_data;
        size_t _size;

    public:
        span() : _data(nullptr), _size(0) {}
        span(T *data, size_t size) : _data(data), _size(size) {}
        template <size_t N> span(T (&arr)[N]) : _data(arr), _size(N) {}
        template <size_t N>
        span(std::array<T, N> &arr) : _data(arr.data()), _size(arr.size())
        {
        }

        T *data()
        {
            return _data;
        }
        const T *data() const
        {
            return _data;
        }

        size_t size() const
        {
            return _size;
        }
        size_t size_bytes() const
        {
            return _size * sizeof(T);
        }

        T &operator[](size_t index)
        {
            return _data[index];
        }
        const T &operator[](size_t index) const
        {
            return _data[index];
        }

        T &front()
        {
            return _data[0];
        }

        T &back()
        {
            return _data[_size - 1];
        }

        span<T> subspan(size_t offset, size_t count)
        {
            return span<T>(_data + offset, count);
        }

        span<T> subspan(size_t offset)
        {
            return span<T>(_data + offset, _size - offset);
        }

        span<T> first(size_t count)
        {
            return span<T>(_data, count);
        }

        span<T> last(size_t count)
        {
            return span<T>(_data + _size - count, count);
        }

        bool empty() const
        {
            return _size == 0;
        }

        void remove_prefix(size_t count)
        {
            _data += count;
            _size -= count;
        }

        void remove_suffix(size_t count)
        {
            _size -= count;
        }
    };
}

#endif