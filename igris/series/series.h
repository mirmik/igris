#ifndef IGRIS_SERIES_H
#define IGRIS_SERIES_H

#include <memory>
#include <vector>

#include <igris/datastruct/dlist.h>
#include <igris/series/field_annotator.h>
#include <igris/series/object_view.h>

#include <igris/series/iterator.h>

namespace igris
{
    class series_iterator;
    class series_block;

    class series
    {
        dlist_head blocks = DLIST_HEAD_INIT(blocks);
        int _elemsize;
        int block_size_hint = 100;
        igris::series_field_annotator _annotator;

        std::allocator<char> allocator;

      public:
        series(int elemsize);
        series(const series &) = delete;
        series(series &&oth)
        {
            dlist_insert_instead(&blocks, &oth.blocks);
            _elemsize = oth._elemsize;
            block_size_hint = oth.block_size_hint;
            _annotator = std::move(oth._annotator);
            allocator = std::move(allocator);
        }

        int elemsize()
        {
            dprln("ELEMSIZE");
            return _elemsize;
        }
        void reserve(int size);
        void add_block(int size);
        void pop_block();

        int size();
        int right_capacity();

        void pop_back();
        void pop_front();

        igris::series_field_annotator &annotator();
        auto &annotations()
        {
            return _annotator.annotations();
        }

        void set_block_size_hint(int sz)
        {
            block_size_hint = sz;
        }

        series_iterator begin();
        series_iterator end();

        series_iterator get_iterator(int num);

        template <class T>
        T *emplace()
        {
            return (T *)emplace();
        }

        void *emplace();

        template <class T>
        T &get(int i);

        void push_csv_string_parse(const std::string &str);

        series_object_view object_view(void *ptr)
        {
            return series_object_view(ptr, _annotator.annotations());
        }
    };

    template <class T, class... Args>
    series make_series(Args &&... args)
    {
        return igris::series(sizeof(T), args...);
    }
}

template <class T>
T &igris::series::get(int i)
{
    return *(T *)(get_iterator(i).pointer());
}

#endif