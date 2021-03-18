#ifndef GENOS_DATASTRUCT_ARGV_H
#define GENOS_DATASTRUCT_ARGV_H

// Вычислить длину первого слова строки.
static inline int argvc_length_of_first(const char *str)
{
    const char *strt = str;

    while (*str != ' ' && *str != '\0')
    {
        ++str;
    }

    return str - strt;
}

// Выполняет разбиение строки data, терминированной нулём
// Разбиение выполняется путём записи нулей в тело строки,
// вектор результата возвращается в массив argv,
// Максимальное количество элементов вектора argcmax
static inline int argvc_internal_split(char *data, char **argv, int argcmax)
{
    int argc = 0;

newarg_search:
    while (*data == ' ')
        ++data;
    if (*data == '\0' || argc >= argcmax)
        return argc;

    argv[argc++] = data;
    while (*data != ' ' && *data != '\0')
        ++data;
    if (*data == ' ')
    {
        *data++ = '\0';
        goto newarg_search;
    };

    return argc;
}

// Безопасный вариант функции argvc_internal_split, дополнительно
// проверяющий длину строки (может не быть терминирована).
static inline int argvc_internal_split_n(char *data, int maxlen, char **argv,
                                         int argcmax)
{
    int argc = 0;
    char *eptr = data + maxlen;

newarg_search:
    while (*data == ' ')
        ++data;
    if (*data == '\0' || argc >= argcmax || data == eptr)
        return argc;

    argv[argc++] = data;
    while (*data != ' ' && *data != '\0')
        ++data;
    if (*data == ' ')
    {
        *data++ = '\0';
        goto newarg_search;
    };

    return argc;
}

#ifdef __cplusplus

namespace igris
{
    // аналог с возможностью задания разделителя delim.
    static inline int internal_split(char *data, int maxlen, char **argv,
                                     int argcmax, char delim = ' ')
    {
        int argc = 0;
        char *eptr = data + maxlen;

    newarg_search:
        while (*data == delim)
            ++data;
        if (*data == '\0' || argc >= argcmax || data == eptr)
            return argc;

        argv[argc++] = data;
        while (*data != delim && *data != '\0')
            ++data;
        if (*data == delim)
        {
            *data++ = '\0';
            goto newarg_search;
        };

        return argc;
    }
} // namespace igris

#endif

#endif
