#ifndef IGRIS_SERIALIZE_PROTOCOL_H
#define IGRIS_SERIALIZE_PROTOCOL_H

#include <cstdint>
#include <cstdlib>
#include <vector>

#include <igris/serialize/serialize_tags.h>

namespace igris
{
    class binary_protocol
    {
        template <class Storage, class Iterator, class EndIterator>
        void dump(Storage &storage,
                  igris::serialize_list_tag<Iterator, EndIterator> listtag)
        {
            storage.dump((uint16_t)listtag.distance());

            auto it = listtag.begin();
            auto eit = listtag.end();
            while (it != eit)
                storage.dump(*it++);
        }
    };
}

#endif
