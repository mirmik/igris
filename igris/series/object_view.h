#ifndef IGRIS_SERIES_OBJECT_VIEW_H
#define IGRIS_SERIES_OBJECT_VIEW_H

#include <igris/series/field_view.h>

namespace igris
{
    class series_object_view
    {
        void *ptr;
        const std::vector<series_field_annotation> &annotations;

      public:
        series_object_view(
            void *ptr, const std::vector<series_field_annotation> &annotations)
            : ptr(ptr), annotations(annotations)
        {
        }

        series_field_view operator[](int i)
        {
            return series_field_view{
                (void *)((char *)ptr + annotations[i].offset), annotations[i]};
        }

        size_t size()
        {
            return annotations.size();
        }
    };
}

#endif