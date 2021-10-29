#ifndef IGRIS_SERIALIZE_SERIALIZER_H
#define IGRIS_SERIALIZE_SERIALIZER_H

#include <igris/serialize/serialize_checks.h>
#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_scheme.h>
#include <igris/serialize/serialize_storage.h>
#include <vector>

namespace igris
{
    template <class Protocol, class Storage> class serializer
    {
        Storage storage;
        Protocol protocol;

        template <igris::has_serialize_realization T>
        void serialize(const T &obj)
        {
            igris::serialize_scheme<T>::reflect(*this, obj);
        }

        template <igris::has_serialize_reflect_method T>
        void serialize(const T &obj)
        {
            obj.serialize_reflect(*this);
        }

        template <class T> void serialize(const T &obj)
        {
            protocol.dump(*this, obj);
        }

        template <class T> void operator&(const T &obj) { serialize(obj); }
    };
}

#endif