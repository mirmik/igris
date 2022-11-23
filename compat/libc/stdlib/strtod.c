#include <ctype.h>
#include <igris/util/numconvert.h>
#include <stddef.h>
#include <stdlib.h>

__attribute__((weak)) double strtod(const char *nptr, char **endptr)
{
    return igris_atof64(nptr, endptr);
}

double atof(const char *nptr)
{
    return igris_atof64(nptr, NULL);
}
