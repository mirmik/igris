#ifndef IGRIS_SERIES_H
#define IGRIS_SERIES_H

#include <cassert>
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
    public:
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
            allocator = std::move(oth.allocator);
        }

        int elemsize() { return _elemsize; }
        void reserve(int size);
        void add_block(int size);
        void pop_block();

        int size();
        int right_capacity();

        void pop_back();
        void pop_front();

        igris::series_field_annotator &annotator();
        igris::series_field_annotation *
        find_annotation(const std::string &name);
        auto &annotations() { return _annotator.annotations(); }

        void set_block_size_hint(int sz) { block_size_hint = sz; }

        series_iterator begin();
        series_iterator end();

        series_iterator get_iterator(int num);

        template <class T> T *emplace() { return (T *)emplace(); }

        void *emplace();
        series_block *last_block();

        template <class T> T &get(int i);
        template <class T> T &last();

        void push_csv_string_parse(const std::string &str);

        series_object_view object_view(void *ptr)
        {
            return series_object_view(ptr, _annotator.annotations());
        }

        int count_of_blocks() { return dlist_size(&blocks); }
    };

    template <class T, class... Args> series make_series(Args &&...args)
    {
        return igris::series(sizeof(T), args...);
    }
}

#include <igris/series/block.h>

template <class T> T &igris::series::get(int i)
{
    assert(last_block()->fini - last_block()->strt > 0);
    return *(T *)(get_iterator(i).pointer());
}

template <class T> T &igris::series::last()
{
    assert(last_block()->fini - last_block()->strt > 0);
    return *(T *)(last_block()->last());
}

#endif
