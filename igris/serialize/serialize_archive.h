#ifndef IGRIS_SERIALIZE_ARCHIEVE_H
#define IGRIS_SERIALIZE_ARCHIEVE_H

#include <igris/serialize/serializer.h>

#include <igris/serialize/serialize_protocol.h>
#include <igris/serialize/serialize_scheme.h>
#include <igris/serialize/serialize_storage.h>
#include <igris/serialize/serialize_tags.h>

static_assert(igris::has_serialize_realization<std::vector<int>>);

class Test
{
    int a, b, c;

public:
    template <class Archive> void serialize_reflect(Archive &arch)
    {
        arch &a;
        arch &b;
        arch &c;
    }
};

static_assert(igris::has_serialize_reflect_method<Test>);

#endif