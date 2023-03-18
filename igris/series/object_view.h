#ifndef IGRIS_SERIES_OBJECT_VIEW_H
#define IGRIS_SERIES_OBJECT_VIEW_H

#include <igris/series/field_view.h>

namespace igris
{
    class series_object_view
    {
        void *ptr;
        const std::vector<series_field_annotation> &annotations;
        const std::unordered_map<std::string, series_field_annotation *>
            &annotation_dict;

    public:
        series_object_view(
            void *ptr,
            const std::vector<series_field_annotation> &annotations,
            const std::unordered_map<std::string, series_field_annotation *>
                &annotation_dict)
            : ptr(ptr), annotations(annotations),
              annotation_dict(annotation_dict)
        {
            assert(annotations.size() == annotation_dict.size());
        }

        series_field_view operator[](int i)
        {
            auto &annotation = annotations[i];
            return series_field_view{(void *)((char *)ptr + annotation.offset),
                                     annotation};
        }

        series_field_view operator[](const std::string &name)
        {
            auto &annotation = *annotation_dict.at(name);
            return series_field_view{(void *)((char *)ptr + annotation.offset),
                                     annotation};
        }

        size_t size()
        {
            return annotations.size();
        }
    };
}

#endif
