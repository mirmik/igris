#include <doctest/doctest.h>
#include <igris/util/printf_impl.h>
#include <string.h>

struct sprint_char_handler_data
{
    char *cursor;
};

static void sprint_printchar(void *d, int c)
{
    *(((struct sprint_char_handler_data *)d)->cursor)++ = c;
}

static int vsprintf(char *s, const char *format, va_list ap)
{
    int ret;

    struct sprint_char_handler_data data;
    data.cursor = s;

    ret = __printf(sprint_printchar, &data, format, ap);

    return ret;
}

static int sprintf(char *buf, const char *format, ...)
{
    int ret;
    va_list args;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    return ret;
}

static int snprintf(char *buf, size_t maxlen, const char *format, ...)
{
    int ret;
    va_list args;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    return ret;
}

static void prflt(char * buf, float fl) 
{
	sprintf(buf, "%lf", fl);
} 

static void prdbl(char * buf, double fl) 
{
	sprintf(buf, "%lf", fl);
} 

TEST_CASE("sprintf") 
{
	char buf[48];
    memset(buf, 0, 48);
	sprintf(buf, "%d", 42);
	CHECK(strcmp("42", buf) == 0);

	sprintf(buf, "%ld", 42);
	CHECK(strcmp("42", buf) == 0);

	prflt(buf, 42.250);
	CHECK(strcmp("42.250000", buf) == 0);

	prdbl(buf, 42.250);
	CHECK(strcmp("42.250000", buf) == 0);
}