#ifndef IGRIS_SERIALIZE_SERIALIZE_TAGS_H__
#define IGRIS_SERIALIZE_SERIALIZE_TAGS_H__

namespace igris
{
    template <class I, class E> class serialize_list_tag
    {
        I it;
        E eit;

    public:
        I begin() { return it; }
        E end() { return eit; }
    };

    template <class I, class E> class serialize_dict_tag
    {
        I it;
        E eit;

    public:
        I begin() { return it; }
        E end() { return eit; }
    };
}

#endif