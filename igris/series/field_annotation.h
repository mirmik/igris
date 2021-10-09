#ifndef IGRIS_series_field_annotation_H
#define IGRIS_series_field_annotation_H

#include <string>

namespace igris
{
    enum class FieldDataType
    {
        Float,
        Double,

        I8,
        I16,
        I32,
        I64,

        U8,
        U16,
        U32,
        U64,

        Bool,
        Undefined
    };

    class series_field_annotation
    {
      public:
        std::string machname;   // имя поля
        std::string username;   // имя поля
        int offset;             // смещение в битах
        int size;               // размер
        FieldDataType datatype; // тип

        union
        {
            uint8_t flags;
            struct
            {
                uint8_t scatter : 1; // подсказка, что данные следует отображать
                // поточечно
            } f;
        };

        series_field_annotation(const std::string &machname,
                                const std::string &username, int offset,
                                int size, FieldDataType datatype)
            : machname(machname), username(username), offset(offset),
              size(size), datatype(datatype), flags(0)
        {
        }

        series_field_annotation() = default;

        series_field_annotation(const series_field_annotation &oth) =
            default; /*
: machname(oth.machname), username(oth.username), offset(oth.offset),
size(oth.size), datatype(oth.datatype), flags(oth.flags)
{
}*/

        series_field_annotation &
        operator=(const series_field_annotation &oth) = default;
        /*{
            this->machname = oth.machname;
            this->username = oth.username;
            this->offset = oth.offset;
            this->size = oth.size;
            this->datatype = oth.datatype;
            this->flags = oth.flags;
            return *this;
        }*/

        series_field_annotation &scatter(bool en)
        {
            f.scatter = en;
            return *this;
        }

        double expand_numeric(void *record_pointer)
        {
            char *ptr = ((char *)record_pointer) + offset;

            switch (datatype)
            {
            case FieldDataType::Float:
                return *(float *)ptr;
            case FieldDataType::Double:
                return *(double *)ptr;
            case FieldDataType::I8:
                return *(int8_t *)ptr;
            case FieldDataType::I16:
                return *(int16_t *)ptr;
            case FieldDataType::I32:
                return *(int32_t *)ptr;
            case FieldDataType::I64:
                return *(int64_t *)ptr;
            case FieldDataType::U8:
                return *(uint8_t *)ptr;
            case FieldDataType::U16:
                return *(uint16_t *)ptr;
            case FieldDataType::U32:
                return *(uint32_t *)ptr;
            case FieldDataType::U64:
                return *(uint64_t *)ptr;
            case FieldDataType::Bool:
                return *(bool *)ptr;
            default:
                return 0;
            }

            return 0;
        }
    };

    template <class T, FieldDataType Field = FieldDataType::Undefined>
    struct series_field_annotation_helper
    {
        constexpr static int size = sizeof(T);
        constexpr static FieldDataType type = Field;
    };

    template <>
    struct series_field_annotation_helper<bool>
        : series_field_annotation_helper<bool, FieldDataType::Bool>
    {
    };

    template <>
    struct series_field_annotation_helper<int8_t>
        : series_field_annotation_helper<int8_t, FieldDataType::I8>
    {
    };
    template <>
    struct series_field_annotation_helper<int16_t>
        : series_field_annotation_helper<int16_t, FieldDataType::I16>
    {
    };
    template <>
    struct series_field_annotation_helper<int32_t>
        : series_field_annotation_helper<int32_t, FieldDataType::I32>
    {
    };
    template <>
    struct series_field_annotation_helper<int64_t>
        : series_field_annotation_helper<int64_t, FieldDataType::I64>
    {
    };

    template <>
    struct series_field_annotation_helper<uint8_t>
        : series_field_annotation_helper<uint8_t, FieldDataType::U8>
    {
    };
    template <>
    struct series_field_annotation_helper<uint16_t>
        : series_field_annotation_helper<uint16_t, FieldDataType::U16>
    {
    };
    template <>
    struct series_field_annotation_helper<uint32_t>
        : series_field_annotation_helper<uint32_t, FieldDataType::U32>
    {
    };
    template <>
    struct series_field_annotation_helper<uint64_t>
        : series_field_annotation_helper<uint64_t, FieldDataType::U64>
    {
    };

    template <>
    struct series_field_annotation_helper<float>
        : series_field_annotation_helper<float, FieldDataType::Float>
    {
    };
    template <>
    struct series_field_annotation_helper<double>
        : series_field_annotation_helper<double, FieldDataType::Double>
    {
    };

    template <class T>
    series_field_annotation
    make_series_field_annotation(const std::string &machname,
                                 const std::string &username, int offset)
    {
        return series_field_annotation(machname, username, offset,
                                       series_field_annotation_helper<T>::size,
                                       series_field_annotation_helper<T>::type);
    }
}

#endif