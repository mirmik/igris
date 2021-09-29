#ifndef IGRIS_SERIES_FIELD_VIEW_H
#define IGRIS_SERIES_FIELD_VIEW_H

#include <igris/series/field_annotation.h>

namespace igris
{
    class series_field_view
    {
        void *ptr;
        const series_field_annotation &annotation;

      public:
        series_field_view(void *ptr, const series_field_annotation &annotation)
            : ptr(ptr), annotation(annotation)
        {
        }

        void assign(double data)
        {
            switch (annotation.datatype)
            {
            case FieldDataType::Float:
                *(float *)ptr = data;
                break;
            case FieldDataType::Double:
                *(double *)ptr = data;
                break;
            case FieldDataType::I8:
                *(int8_t *)ptr = data;
                break;
            case FieldDataType::I16:
                *(int16_t *)ptr = data;
                break;
            case FieldDataType::I32:
                *(int32_t *)ptr = data;
                break;
            case FieldDataType::I64:
                *(int64_t *)ptr = data;
                break;
            case FieldDataType::U8:
                *(uint8_t *)ptr = data;
                break;
            case FieldDataType::U16:
                *(uint16_t *)ptr = data;
                break;
            case FieldDataType::U32:
                *(uint32_t *)ptr = data;
                break;
            case FieldDataType::U64:
                *(uint64_t *)ptr = data;
                break;
            case FieldDataType::Bool:
                *(bool *)ptr = data;
                break;
            }
        }
    };
}

#endif