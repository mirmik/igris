#ifndef CALLABLE_COLLECTION_H
#define CALLABLE_COLLECTION_H

#include <functional>
#include <string>
#include <vector>

namespace igris
{
    template <class F> class callable_collection
    {
        struct record
        {
            std::string key;
            std::string help;
            std::function<F> func;
        };

        std::vector<record> records = {};

    public:
        void add(const std::string &key, const std::string &help, F func)
        {
            records.push_back({key, help, func});
        }

        void add(const std::string &key, F func)
        {
            records.push_back({key, "", func});
        }

        void add(const char *key, const char *help, F func)
        {
            records.push_back({key, help, func});
        }

        void add(const char *key, F func)
        {
            records.push_back({key, "", func});
        }

        void add(record &&rec)
        {
            records.push_back(std::move(rec));
        }

        void add(record &rec)
        {
            records.push_back(rec);
        }

        void add(std::vector<record> &&recs)
        {
            records.insert(records.end(), recs.begin(), recs.end());
        }

        void add(std::vector<record> &recs)
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