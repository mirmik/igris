#ifndef IGRIS_TENSOR_STORAGE_H
#define IGRIS_TENSOR_STORAGE_H

#include <cstddef>
#include <cstdint>
#include <igris/container/array_view.h>
#include <memory>

namespace igris
{
    template <typename T, typename A = std::allocator<T>> class tensor_storage
    {
    public:
        using allocator_type = A;
        using value_type = T;
        using reference = T &;
        using const_reference = const T &;

        A _alloc = {};
        T *_data = nullptr;
        size_t _size = 0;
        bool _view = false;

        bool is_view() const
        {
            return _view;
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

        tensor_storage() : _data(nullptr), _size(0), _view(false) {}

        tensor_storage(size_t sz)
            : _alloc{}, _data(_alloc._allocate(sz)), _size(sz), _view(false)
        {
        }

        tensor_storage(const igris::array_view<T> &buf)
            : _data(_alloc._allocate(buf.size())), _size(buf.size()),
              _view(false)
        {

            std::copy(buf.begin(), buf.end(), _data);
        }

        tensor_storage(const std::initializer_list<T> &buf)
            : _data(_alloc._allocate(buf.size())), _size(buf.size()),
              _view(false)
        {
            std::copy(buf.begin(), buf.end(), _data);
        }

        tensor_storage(const tensor_storage &oth)
            : _data(_alloc._allocate(oth.size())), _size(oth.size()),
              _view(false)
        {
            auto ptr = _data;
            for (const auto &ref : oth)
            {
                new (ptr++) T(ref);
            }
        }

        tensor_storage &operator=(const tensor_storage &oth)
        {
            _data = _alloc.allocate(oth.size());
            _size = oth.size();

            auto ptr = _data;
            for (const auto &ref : oth)
            {
                new (ptr++) T(ref);
            }

            return *this;
        }

        tensor_storage(tensor_storage &&arr)
            : _alloc(arr._alloc), _data(arr._data), _size(arr._size),
              _view(false)
        {
            arr._size = 0;
            arr._data = nullptr;
        }

        void resize(size_t size)
        {
            invalidate();
            create_buffer(size);
        }

        void create_buffer(size_t size)
        {
            _data = _alloc.allocate(size);
            _size = size;
        }

        void invalidate()
        {
            if (!is_view())
                _alloc.deallocate(_data, _size);
        }

        ~tensor_storage()
        {
            if (!is_view())
                _alloc.deallocate(_data, _size);
        }

        T &operator[](size_t i)
        {
            return *(_data + i);
        }

        const T &operator[](size_t i) const
        {
            return *(_data + i);
        }

        using iterator = T *;
        using const_iterator = const T *;

        iterator begin()
        {
            return _data;
        }
        iterator end()
        {
            return _data + _size;
        }

        const_iterator begin() const
        {
            return (const_iterator)_data;
        }
        const_iterator end() const
        {
            return (const_iterator)_data + _size;
        }

        tensor_storage subarray(size_t offset, size_t size)
        {
            tensor_storage res;
            res._view = true;
            res._data = _data + offset;
            res._size = size;
            return res;
        }
    };
}

#endif
