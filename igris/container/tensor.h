#ifndef IGRIS_MARRAY_H
#define IGRIS_MARRAY_H

#include <igris/compiler.h>
#include <igris/container/array_view.h>
#include <igris/container/unbounded_array.h>
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
        std::shared_ptr<igris::unbounded_array<T>> _storage = {};
        igris::array_view<T> _storview = {};
        std::vector<size_t> _shape = {};
        std::vector<size_t> _stride = {};

    public:
        tensor() = default;
        tensor(std::vector<size_t> shape)
        {
            _shape = shape;
            size_t storsize = shape_product();
            _storage = std::make_shared<igris::unbounded_array<T>>(storsize);
            _storview = {_storage->data(), _storage->size()};
            _storage->fill(T());

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

        igris::unbounded_array<T> storage()
        {
            return _storage;
        }

        igris::array_view<T> storage_view()
        {
            return _storview;
        }

        size_t shape_product()
        {
            size_t res = 1;
            for (size_t i = 0; i < _shape.size(); ++i)
            {
                res *= _shape[i];
            }
            return res;
        }

        T &operator()(std::vector<size_t> idxs)
        {
            size_t idx = 0;
            for (size_t i = 0; i < idxs.size(); ++i)
            {
                idx += idxs[i] * _stride[i];
            }

            return _storview[idx];
        }

        tensor reshape(std::vector<size_t> shape)
        {
            tensor res;
            res._storage = _storage;
            res._storview = _storview;
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

        bool is_contiguous()
        {
            // tensor is contiguous if it stride array is ordered
            for (size_t i = 0; i < _stride.size() - 1; ++i)
            {
                if (_stride[i] < _stride[i + 1])
                    return false;
            }
            return true;
        }

        tensor operator[](size_t idx)
        {
            tensor res;
            res._shape = _shape;
            res._stride = _stride;
            res._storage = _storage;
            res._storview = _storview.slice(idx * _stride[0], _stride[0]);
            res._shape.erase(res._shape.begin());
            res._stride.erase(res._stride.begin());
            return res;
        }

        tensor operator[](std::vector<size_t> indexes)
        {
            if (!is_contiguous())
            {
                auto t = contiguous();
                return t[indexes];
            }

            // storage size
            size_t areasize = _storview.size();
            for (size_t i = 0; i < indexes.size(); ++i)
            {
                areasize /= _shape[i];
            }

            // first element index
            size_t firstidx = 0;
            for (size_t i = 0; i < indexes.size(); ++i)
            {
                firstidx += indexes[i] * _stride[i];
            }

            tensor res;
            res._shape = _shape;
            res._stride = _stride;
            res._storage = _storage;
            res._storview = _storview.slice(firstidx, areasize);
            res._shape.erase(res._shape.begin(),
                             res._shape.begin() + indexes.size());
            res._stride.erase(res._stride.begin(),
                              res._stride.begin() + indexes.size());
            return res;
        }

        tensor transpose()
        {
            tensor res;
            res._storage = _storage;
            res._storview = _storview;
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
            for (size_t i = 0; i < _storview.size(); ++i)
            {
                res._storview[i] = _storview[ordinal_to_storage_index(i)];
            }
            return res;
        }

        tensor unsqueeze(size_t dim)
        {
            std::vector<size_t> newshape = _shape;
            newshape.insert(newshape.begin() + dim, 1);

            std::vector<size_t> newstride = _stride;
            newstride.insert(newstride.begin() + dim, 1);

            tensor res;
            res._storage = _storage;
            res._storview = _storview;
            res._shape = newshape;
            res._stride = newstride;
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