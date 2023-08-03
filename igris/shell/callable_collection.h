#ifndef CALLABLE_COLLECTION_H
#define CALLABLE_COLLECTION_H

#include <functional>
#include <initializer_list>
#include <string>
#include <vector>

namespace igris
{
    template <class F> struct callable_collection_record
    {
        std::string key;
        std::string help;
        std::function<F> func;
    };

    template <class F,
              class Container = std::vector<callable_collection_record<F>>>
    class callable_collection
    {

        Container records = {};

    public:
        callable_collection() = default;
        callable_collection(const callable_collection &) = default;
        callable_collection(callable_collection &&) = default;
        callable_collection &operator=(const callable_collection &) = default;
        callable_collection &operator=(callable_collection &&) = default;

        callable_collection(Container &&recs)
        {
            records = std::move(recs);
        }

        callable_collection(const Container &recs)
        {
            records = recs;
        }

        callable_collection(
            const std::initializer_list<callable_collection_record<F>> &recs)
        {
            records = recs;
        }

        template <class U>
        void add(const std::string &key, const std::string &help, U func)
        {
            records.push_back({key, help, std::function<F>(func)});
        }

        template <class U> void add(const std::string &key, U func)
        {
            records.push_back({key, "", std::function<F>(func)});
        }

        template <class U> void add(const char *key, const char *help, U func)
        {
            records.push_back({key, help, std::function<F>(func)});
        }

        template <class U> void add(const char *key, U func)
        {
            records.push_back({key, "", std::function<F>(func)});
        }

        void add(callable_collection_record<F> &&rec)
        {
            records.push_back(std::move(rec));
        }

        void add(callable_collection_record<F> &rec)
        {
            records.push_back(rec);
        }

        void add(Container &&recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(Container &recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(callable_collection &&recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(callable_collection &recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        auto begin()
        {
            return records.begin();
        }
        auto end()
        {
            return records.end();
        }
        auto begin() const
        {
            return records.begin();
        }
        auto end() const
        {
            return records.end();
        }

        std::function<F> *find(const std::string &key)
        {
            for (auto &rec : records)
            {
                if (rec.key == key)
                    return &rec.func;
            }
            return nullptr;
        }
    };
}

#endif