#ifndef IGRIS_FIELD_ANNOTATOR_H
#define IGRIS_FIELD_ANNOTATOR_H

#include <igris/series/field_annotation.h>
#include <igris/util/size_incrementor.h>

namespace igris
{
    class series_field_annotator
    {
        igris::size_incrementor inc;
        std::vector<series_field_annotation> _annotations;

      public:
        series_field_annotator() : inc(0)
        {
        }
        series_field_annotator(int offset) : inc(offset)
        {
        }
        series_field_annotator(const igris::size_incrementor &oth) : inc(oth)
        {
        }

        template <class T>
        series_field_annotation &add(const std::string &name)
        {
            auto annot = igris::make_series_field_annotation<T>(
                name, inc.increment<T>());
            _annotations.push_back(annot);

            return _annotations[_annotations.size() - 1];
        }

        const auto &annotations()
        {
            return _annotations;
        }
    };
}

#endif