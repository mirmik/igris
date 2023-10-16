#ifndef IGRIS_MARRAY_H
#define IGRIS_MARRAY_H

#include <igris/compiler.h>
#include <igris/container/tensor_storage.h>
#include <vector>

namespace igris
{
    template <class T> class tensor
    {
        igris::tensor_storage<T> _storage = {};
        std::vector<size_t> _shape = {};
        std::vector<size_t> _stride = {};

    public:
        tensor() = default;
        tensor &operator=(const tensor &other) = default;
        tensor &operator=(tensor &&other) = default;
        tensor(const tensor &other) = default;
        tensor(tensor &&other) = default;

        T &operator()(std::vector<size_t> idxs)
        {
            size_t idx = 0;
            for (size_t i = 0; i < idxs.size(); ++i)
            {
                idx += idxs[i] * _stride[i];
            }

            return _storage[idx];
        }

        void reshape(std::vector<size_t> shape)
        {
            _shape = shape;
            _stride.resize(shape.size());

            size_t stride = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                _stride[shape.size() - i - 1] = stride;
                stride *= shape[shape.size() - i - 1];
            }

            _storage.resize(stride);
        }

        std::vector<size_t> shape()
        {
            return _shape;
        }

        std::vector<size_t> stride()
        {
            return _stride;
        }

        size_t storage_size()
        {
            return _storage.size();
        }

        tensor operator[](size_t idx)
        {
            tensor res;
            res._shape = _shape;
            res._stride = _stride;
            res._storage = _storage.subarray(idx * _stride[0], _stride[0]);
            res._shape.erase(res._shape.begin());
            res._stride.erase(res._stride.begin());
            return res;
        }
    };
}

#endif