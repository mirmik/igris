#ifndef IGRIS_SERIALIZE_SERIALIZE_STORAGE_H_
#define IGRIS_SERIALIZE_SERIALIZE_STORAGE_H_

#include <igris/compiler.h>

#if __HAS_CONCEPT
#include <concepts>
#endif

#include <cstdint>
#include <cstdlib>
#include <string>

namespace igris
{
#if __HAS_CONCEPT
    template <typename T>
    concept appendable_storage_type = requires(T a, const char *data,
                                               size_t size)
    {
        a.append(data, size);
        a.size();
    };

    static_assert(appendable_storage_type<std::string>);
    static_assert(!appendable_storage_type<const char *>);
#endif

    class serialize_storage_base
    {
    protected:
        int readpos = 0;

    public:
        void dumps(const std::string &str) { dump(str.data(), str.size()); }

        std::string loads(size_t size)
        {
            std::string ret;
            ret.resize(size);
            load(&*ret.begin(), size);
            return ret;
        }

        size_t avail() { return size() - readpos; }

    public:
        virtual void dump(const char *data, size_t size) = 0;
        virtual void load(char *data, size_t size) = 0;
        virtual size_t size() = 0;
    };

#if __HAS_CONCEPT
    template <appendable_storage_type T>
#else
    template <class T>
#endif
    class appendable_storage // : public
                             // serialize_storage_base<appendable_storage<T>>
    {
        T storage;

    public:
        appendable_storage() = default;
        appendable_storage(T storage) : storage(storage) {}

        void dump(const char *data, size_t size) override
        {
            storage.append(data, size);
        }

        void load(char *data, size_t size) override
        {
            auto it = std::next(storage.begin(), this->readpos);
            auto eit = storage.end();

            while (size-- && it != eit)
            {
                *data++ = *it++;
            }
        }

        size_t size() override { return storage.size(); }
    };
}

#endif