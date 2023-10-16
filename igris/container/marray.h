#ifndef IGRIS_MARRAY_H
#define IGRIS_MARRAY_H

#include <igris/compiler.h>
#include <igris/container/unbounded_array.h>
#include <vector>

namespace igris
{
    template <class T> class marray
    {
        igris::unbounded_array<T> _storage;
        std::vector<size_t> _shape = {};
        std::vector<size_t> _strides = {};

    public:
        marray() = default;

        T &operator()(std::vector<size_t> idxs)
        {
            size_t idx = 0;
            for (size_t i = 0; i < idxs.size(); ++i)
            {
                idx += idxs[i] * _strides[i];
            }

            return _storage[idx];
        }

        void reshape(std::vector<size_t> shape)
        {
            _shape = shape;
            _strides.resize(shape.size());

            size_t stride = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                _strides[i] = stride;
                stride *= shape[i];
            }

            _storage.resize(stride);
        }

        marray &operator=(const marray &other) = default;
        marray &operator=(marray &&other) = default;
        marray(const marray &other) = default;
        marray(marray &&other) = default;
    };
}

#endif