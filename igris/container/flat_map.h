#ifndef IGRIS_CONTAINER_FLAT_MAP_H
#define IGRIS_CONTAINER_FLAT_MAP_H

#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace igris
{
    template <class Key,
              class T,
              class Compare = std::less<Key>,
              class Alloc = std::allocator<std::pair<Key, T>>>
    class flat_map
    {
    public:
        using value_type = std::pair<Key, T>;
        using iterator = typename std::vector<value_type>::iterator;
        using const_iterator = typename std::vector<value_type>::const_iterator;
        using reverse_iterator =
            typename std::vector<value_type>::reverse_iterator;
        using const_reverse_iterator =
            typename std::vector<value_type>::const_reverse_iterator;
        using size_type = typename std::vector<value_type>::size_type;
        using difference_type =
            typename std::vector<value_type>::difference_type;
        using mapped_type = T;
        using reference = value_type &;
        using const_reference = const value_type &;
        using key_type = Key;

    private:
        std::vector<value_type, Alloc> storage = {};
        Compare comp = {};

        bool equivalent(const Key &a, const Key &b) const
        {
            return !comp(a, b) && !comp(b, a);
        }

        iterator lower_bound(const Key &key)
        {
            return std::lower_bound(storage.begin(), storage.end(), key,
                                    [this](const value_type &value,
                                           const Key &candidate) {
                                        return comp(value.first, candidate);
                                    });
        }

        const_iterator lower_bound(const Key &key) const
        {
            return std::lower_bound(storage.begin(), storage.end(), key,
                                    [this](const value_type &value,
                                           const Key &candidate) {
                                        return comp(value.first, candidate);
                                    });
        }

    public:
        flat_map() = default;
        flat_map(const flat_map &) = default;
        flat_map(flat_map &&) = default;
        flat_map &operator=(const flat_map &) = default;
        flat_map &operator=(flat_map &&) = default;

        flat_map(const std::initializer_list<value_type> &init) : storage(init)
        {
            std::sort(storage.begin(), storage.end(),
                      [this](const value_type &a, const value_type &b) {
                          return comp(a.first, b.first);
                      });
            storage.erase(
                std::unique(storage.begin(), storage.end(),
                            [this](const value_type &a, const value_type &b) {
                                return equivalent(a.first, b.first);
                            }),
                storage.end());
        }

        bool operator==(const flat_map &other) const
        {
            return storage == other.storage;
        }

        bool operator!=(const flat_map &other) const
        {
            return storage != other.storage;
        }

        iterator begin()
        {
            return storage.begin();
        }
        const_iterator begin() const
        {
            return storage.begin();
        }
        iterator end()
        {
            return storage.end();
        }
        const_iterator end() const
        {
            return storage.end();
        }
        reverse_iterator rbegin()
        {
            return storage.rbegin();
        }
        const_reverse_iterator rbegin() const
        {
            return storage.rbegin();
        }
        reverse_iterator rend()
        {
            return storage.rend();
        }
        const_reverse_iterator rend() const
        {
            return storage.rend();
        }
        const_iterator cbegin() const
        {
            return storage.cbegin();
        }
        const_iterator cend() const
        {
            return storage.cend();
        }
        const_reverse_iterator crbegin() const
        {
            return storage.crbegin();
        }
        const_reverse_iterator crend() const
        {
            return storage.crend();
        }
        size_type size() const
        {
            return storage.size();
        }
        bool empty() const
        {
            return storage.empty();
        }
        size_type max_size() const
        {
            return storage.max_size();
        }
        void reserve(size_type n)
        {
            storage.reserve(n);
        }
        size_type capacity() const
        {
            return storage.capacity();
        }
        void shrink_to_fit()
        {
            storage.shrink_to_fit();
        }
        void clear()
        {
            storage.clear();
        }
        void swap(flat_map &other)
        {
            storage.swap(other.storage);
            std::swap(comp, other.comp);
        }

        T &operator[](const Key &key)
        {
            auto it = lower_bound(key);

            if (it == storage.end() || !equivalent(it->first, key))
                it = storage.insert(it, value_type(key, T()));

            return it->second;
        }

        const T &operator[](const Key &key) const
        {
            return at(key);
        }

        T &at(const Key &key)
        {
            auto it = find(key);

            if (it == storage.end())
            {
                throw std::out_of_range("flat_map::at");
            }

            return it->second;
        }

        const T &at(const Key &key) const
        {
            auto it = find(key);

            if (it == storage.end())
            {
                throw std::out_of_range("flat_map::at");
            }

            return it->second;
        }

        iterator find(const Key &key)
        {
            auto it = lower_bound(key);
            return it != storage.end() && equivalent(it->first, key)
                       ? it
                       : storage.end();
        }

        const_iterator find(const Key &key) const
        {
            auto it = lower_bound(key);
            return it != storage.end() && equivalent(it->first, key)
                       ? it
                       : storage.end();
        }

        size_type count(const Key &key) const
        {
            return find(key) == storage.end() ? 0 : 1;
        }

        template <class... Args>
        std::pair<iterator, bool> emplace(Key key, Args &&... args)
        {
            auto it = lower_bound(key);
            if (it != storage.end() && equivalent(it->first, key))
            {
                return std::make_pair(it, false);
            }
            it = storage.insert(
                it, std::pair(key, T(std::forward<Args>(args)...)));
            return std::make_pair(it, true);
        }

        iterator insert(const value_type &value)
        {
            auto it = lower_bound(value.first);
            if (it != storage.end() && equivalent(it->first, value.first))
                return it;
            return storage.insert(it, value);
        }
    };
}

#endif
