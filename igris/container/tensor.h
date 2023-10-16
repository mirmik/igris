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
        tensor(std::vector<size_t> shape)
        {
            size_t storage_size = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                storage_size *= shape[i];
            }
            _storage = igris::tensor_storage<T>(storage_size);
            _shape = shape;

            _stride.resize(shape.size());
            size_t stride = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                _stride[shape.size() - i - 1] = stride;
                stride *= shape[shape.size() - i - 1];
            }
        }

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

        void reshape(std::vector<size_t> shape) {}

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

        tensor transpose()
        {
            tensor res;
            res._storage = _storage.view();
            res._shape = reverse_vector(_shape);
            res._stride = reverse_vector(_stride);
            return res;
        }

        static std::vector<size_t> reverse_vector(std::vector<size_t> vec)
        {
            std::vector<size_t> res;
            for (size_t i = 0; i < vec.size(); ++i)
            {
                res.push_back(vec[vec.size() - i - 1]);
            }
            return res;
        }

        tensor permute(std::vector<size_t> idxs)
        {
            tensor res;
            res._storage = _storage.view();
            res._shape.resize(idxs.size());
            res._stride.resize(idxs.size());

            for (size_t i = 0; i < idxs.size(); ++i)
            {
                res._shape[i] = _shape[idxs[i]];
                res._stride[i] = _stride[idxs[i]];
            }

            return res;
        }

        T &at_ordinal(size_t ord)
        {
            return _storage[ordinal_to_storage_index(ord)];
        }

        size_t ordinal_to_storage_index(size_t ord)
        {
            size_t idx = 0;
            for (size_t i = 0; i < _shape.size(); ++i)
            {
                size_t j = _shape.size() - i - 1;
                idx += _stride[j] * (ord % _shape[j]);
                ord /= _shape[j];
            }
            return idx;
        }

        tensor contiguous()
        {
            tensor res(shape());
            for (size_t i = 0; i < storage_size(); ++i)
            {
                res._storage[i] = _storage[ordinal_to_storage_index(i)];
            }
            return res;
        }
    };
}

#endif