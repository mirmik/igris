#ifndef IGRIS_SERIALIZE_ARCHIEVE_H
#define IGRIS_SERIALIZE_ARCHIEVE_H

namespace igris
{

    template <class Protocol, class Storage> class serialize_archive
    {
        Storage storage;
        Protocol protocol;
    };
}

#endif