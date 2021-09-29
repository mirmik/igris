#ifndef IGRIS_SERIES_ITERATOR_H
#define IGRIS_SERIES_ITERATOR_H

#include <igris/datastruct/dlist.h>

namespace igris
{
    class series_block;

    class series_iterator
    {
        dlist_head *block_lnk;
        int num;

      public:
        series_iterator(dlist_head *block_lnk, int num);

        series_iterator operator++();
        series_iterator operator++(int);

        bool operator!=(const series_iterator &oth)
        {
            return block_lnk != oth.block_lnk || num != oth.num;
        }

        void *pointer();
        series_block *block();
    };
}

#endif