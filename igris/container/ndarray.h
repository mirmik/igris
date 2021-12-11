#if __has_include(<concepts>)

#ifndef IGRIS_NDARRAY_H
#define IGRIS_NDARRAY_H

#include <array>
#include <concepts>
#include <igris/container/array_view.h>
#include <vector>

namespace igris
{
    template <class T>
    concept ArrayType = requires(const T &a)
    {
        std::size(a);
        std::begin(a);
        std::end(a);
    };

    template <class C> size_t array_dimension(const C &container)
    {
        if constexpr (ArrayType<C>)
            return array_dimension(*std::begin(container)) + 1;
        else
            return 0;
    }

    template <class C, class S>
    void assign_array_shape(const C &container, S &shape, int idx)
    {
        if constexpr (ArrayType<C>)
        {
            shape[idx] = container.size();
            assign_array_shape(*std::begin(container), shape, idx + 1);
        }
    }

    template <typename Value> class ndarray
    {
        size_t _dim;
        std::vector<size_t> _shape;
        std::vector<Value> _values;

    public:
        ndarray() {}

        template <class C> void init(const C &container)
        {
            _dim = igris::array_dimension(container);

            _shape.resize(_dim);
            igris::assign_array_shape(container, _shape, 0);

            _values.reserve(plane_size());
            set_values(container);
        }

        template <class C> ndarray(const std::initializer_list<C> &container)
        {
            init(container);
        }

        template <class C>
        ndarray(
            const std::initializer_list<std::initializer_list<C>> &container)
        {
            init(container);
        }

        template <class C>
        ndarray(const std::initializer_list<
                std::initializer_list<std::initializer_list<C>>> &container)
        {
            init(container);
        }

        template <class C> ndarray(const C &container) { init(container); }

        size_t plane_size()
        {
            size_t acc = 1;
            for (size_t i = 0; i < _dim; ++i)
                acc *= _shape[i];
            return acc;
        }

        size_t planed_index(const size_t *indexes, size_t)
        {
            int acc = 0;
            int step = 1;
            for (int i = _dim - 1; i >= 0; --i)
            {
                acc += indexes[i] * step;
                step *= _shape[i];
            }
            return acc;
        }

        size_t planed_index(const igris::array_view<size_t> &indexes)
        {
            return planed_index(indexes.data(), indexes.size());
        }

        Value &operator()(const size_t *indexes, size_t)
        {
            return _values[planed_index(indexes)];
        }

        Value &operator()(const igris::array_view<size_t> &indexes)
        {
            return _values[planed_index(indexes)];
        }

        size_t dimension() const { return _dim; }

        const std::vector<size_t> &shape() const { return _shape; }

        void reshape(const igris::array_view<size_t> &indexes)
        {
            _dim = indexes.size();
            _shape = {indexes.data(), indexes.data() + indexes.size()};
        }

    private:
        template <class C> void set_values(const C &val)
        {
            if constexpr (ArrayType<C>)
                for (auto &c : val)
                    set_values(c);
            else
                _values.push_back(val);
        }
    };
}

#endif
#endif