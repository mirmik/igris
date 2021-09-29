#ifndef IGRIS_TABLE_SERIES_BLOCK_H
#define IGRIS_TABLE_SERIES_BLOCK_H

#include <igris/datastruct/dlist.h>

namespace igris
{
    class series;

    class series_block
    {
      public:
        dlist_head lnk = DLIST_HEAD_INIT(lnk);

      public:
        igris::series *parent;
        void *ptr;
        int size;
        int strt = 0;
        int fini = 0;

      public:
        int elemsize();
        void *get(int num);

        series_block(igris::series *parent, void *ptr, int size);

        bool has_place();
        void *emplace();
    };
}

#endif