#ifndef IGRIS_SERIES_ITERATOR_H
#define IGRIS_SERIES_ITERATOR_H

#include <igris/datastruct/dlist.h>
#include <iterator>

namespace igris
{
    class series_block;

    class series_iterator
    {
    public:
        dlist_head *block_lnk;
        int num;

    public:
        series_iterator() = default;
        series_iterator(dlist_head *block_lnk, int num);

        series_iterator operator++();
        series_iterator operator++(int);
        series_iterator operator--();
        series_iterator operator--(int);

        bool operator!=(const series_iterator &oth) const
        {
            return block_lnk != oth.block_lnk || num != oth.num;
        }

        bool operator==(const series_iterator &oth) const
        {
            return block_lnk == oth.block_lnk && num == oth.num;
        }

        void *pointer();
        const void *pointer() const;

        series_block *block();
        const series_block *block() const;

        template <class T> T &get() { return *(T *)pointer(); }
    };
}

namespace std
{
    template <> struct iterator_traits<igris::series_iterator>
    {
        using difference_type = int;
        using iterator_category = std::bidirectional_iterator_tag;
    };
}

#endif
