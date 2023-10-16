#ifndef IGRIS_MARRAY_H
#define IGRIS_MARRAY_H

#include <igris/compiler.h>
#include <igris/container/tensor_storage.h>
#include <vector>

namespace igris
{
    template <class T> class tensor;

    template <class T> class indices_iterator
    {
        tensor<T> *_tensor;
        std::vector<size_t> _idxs;

    public:
        indices_iterator(tensor<T> *tensor, std::vector<size_t> idxs)
            : _tensor(tensor), _idxs(idxs)
        {
        }

        indices_iterator(const indices_iterator &other) = default;
        indices_iterator(indices_iterator &&other) = default;
        indices_iterator &operator=(const indices_iterator &other) = default;
        indices_iterator &operator=(indices_iterator &&other) = default;

        indices_iterator operator++(int)
        {
            auto cur = indices_iterator(_tensor, _idxs);
            std::vector<size_t> idxs = _idxs;
            for (size_t i = 0; i < _idxs.size(); ++i)
            {
                size_t j = _idxs.size() - i - 1;

                if (idxs[j] + 1 < _tensor->shape()[j])
                {
                    idxs[j]++;
                    _idxs = idxs;
                    return cur;
                }
                else
                {
                    idxs[j] = 0;
                }
            }
            _idxs = {};
            return cur;
        }

        indices_iterator &operator++()
        {
            (*this)++;
            return *this;
        }

        std::vector<size_t> operator*()
        {
            return _idxs;
        }

        bool operator==(const indices_iterator &other)
        {
            return _idxs == other._idxs && _tensor == other._tensor;
        }

        bool operator!=(const indices_iterator &other)
        {
            return _idxs != other._idxs || _tensor != other._tensor;
        }
    };

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
            _storage.fill(T());
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

        tensor reshape(std::vector<size_t> shape)
        {
            if (_storage.is_view())
            {
                auto t = contiguous();
                return t.reshape(shape);
            }

            tensor res;
            res._storage = _storage.view();
            res._shape = shape;
            res._stride.resize(shape.size());
            size_t stride = 1;
            for (size_t i = 0; i < shape.size(); ++i)
            {
                res._stride[shape.size() - i - 1] = stride;
                stride *= shape[shape.size() - i - 1];
            }
            return res;
        }

        indices_iterator<T> indices_iterator_begin()
        {
            return indices_iterator<T>(this,
                                       std::vector<size_t>(_shape.size(), 0));
        }

        indices_iterator<T> indices_iterator_end()
        {
            return indices_iterator<T>(this, std::vector<size_t>());
        }

        tensor resize(std::vector<size_t> newshape)
        {
            tensor res(newshape);

            auto it = indices_iterator_begin();
            auto end = indices_iterator_end();
            for (; it != end; ++it)
            {
                auto indices = *it;
                if (res.has_indices(indices))
                    res(indices) = (*this)(indices);
            }
            return res;
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

        tensor unsqueeze(size_t dim)
        {
            std::vector<size_t> shape = _shape;
            shape.insert(shape.begin() + dim, 1);

            std::vector<size_t> stride = _stride;
            stride.insert(stride.begin() + dim, 1);

            tensor res;
            res._storage = _storage.view();
            res._shape = shape;
            res._stride = stride;
            return res;
        }

        bool has_indices(std::vector<size_t> idxs)
        {
            if (idxs.size() != _shape.size())
                return false;

            for (size_t i = 0; i < idxs.size(); ++i)
            {
                if (idxs[i] >= _shape[i])
                    return false;
            }
            return true;
        }
    };
}

#endif