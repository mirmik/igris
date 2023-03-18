#ifndef IGRIS_FIELD_ANNOTATOR_H
#define IGRIS_FIELD_ANNOTATOR_H

#include <igris/series/field_annotation.h>
#include <igris/util/size_incrementor.h>
#include <unordered_map>
#include <vector>

namespace igris
{
    class series_field_annotator
    {
        igris::size_incrementor inc = {};
        std::vector<series_field_annotation> _annotations = {};
        std::unordered_map<std::string, series_field_annotation *> _dict = {};

    public:
        series_field_annotator() : inc(0) {}
        series_field_annotator(int offset) : inc(offset) {}
        series_field_annotator(const igris::size_incrementor &oth) : inc(oth) {}

        const std::unordered_map<std::string, series_field_annotation *> &
        annotations_dict()
        {
            return _dict;
        }

        template <class T>
        series_field_annotation &add(const std::string &machname,
                                     const std::string &username)
        {
            auto annot = igris::make_series_field_annotation<T>(
                machname, username, inc.increment<T>());
            _annotations.push_back(annot);
            return _annotations[_annotations.size() - 1];
        }

        template <class T> series_field_annotation &add(const std::string &name)
        {
            return add<T>(name, name);
        }

        void add(const std::string &machname,
                 const std::string &username,
                 size_t size,
                 FieldDataType type)
        {
            _annotations.emplace_back(
                machname, username, inc.increment(size), size, type);
            _dict[machname] = &_annotations.back();
        }

        const auto &annotations()
        {
            return _annotations;
        }

        series_field_annotation *find_annotation(const std::string &name)
        {
            for (auto &obj : _annotations)
            {
                if (name == obj.machname)
                    return &obj;
            }

            return nullptr;
        }

        series_field_annotation *find(const std::string &name)
        {
            for (auto &a : _annotations)
            {
                if (a.machname == name)
                    return &a;
            }
            return nullptr;
        }
    };
}

#endif
