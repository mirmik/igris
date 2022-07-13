#include <igris/dprint.h>
#include <igris/util/hexascii.h>
#include <igris/util/numconvert.h>
#include <igris/util/ctype.h>
#include <math.h>
#include <string.h>

char *i64toa(int64_t num, char *buf, uint8_t base)
{
    char *p = buf;
    char *p1, *p2;
    uint64_t ud = 0;

    *buf = '\0'; /* initialize buffer. In the case of an error, this will
                    already be in the buffer, indicating that the result is
                    invalid (NULL). */
    p1 = buf;    /* start of buffer */

    // check base
    if (base < 2 || base > 36)
    {
        return buf;
    }

    /* If num < 0, put `-' in the head.  */
    if (num < 0)
    {
        *(p++) = '-';
        p1++;
        ud = -num;
    }
    else
    {
        ud = num;
    }

    /* Divide ud by base until ud == 0.  */
    int16_t remainder = 0;
    do
    {
        remainder = ud % base;
        *(p++) = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while (ud /= base);

    /* Terminate buf.  */
    *p = '\0';

    /* Reverse buffer.  */
    p2 = p - 1; /* end of buffer */
    char tmp;
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return p;
}

char *i32toa(int32_t num, char *buf, uint8_t base)
{
    return i64toa(num, buf, base);
}

char *i16toa(int16_t num, char *buf, uint8_t base)
{
    return i64toa(num, buf, base);
}

char *i8toa(int8_t num, char *buf, uint8_t base)
{
    return i64toa(num, buf, base);
}

char *u64toa(uint64_t num, char *buf, uint8_t base)
{
    char *p = buf;
    char *p1, *p2;
    uint64_t ud = 0;

    *buf = '\0'; /* initialize buffer. In the case of an error, this will
                    already be in the buffer, indicating that the result is
                    invalid (NULL). */
    p1 = buf;    /* start of buffer */

    // check base
    if (base < 2 || base > 36)
    {
        return buf;
    }

    ud = num;

    /* Divide ud by base until ud == 0.  */
    int16_t remainder = 0;
    do
    {
        remainder = ud % base;
        *(p++) = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
    } while (ud /= base);

    /* Terminate buf.  */
    *p = '\0';

    /* Reverse buffer.  */
    p2 = p - 1; /* end of buffer */
    char tmp;
    while (p1 < p2)
    {
        tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return p;
}

char *u32toa(uint64_t num, char *buf, uint8_t base)
{
    return u64toa(num, buf, base);
}

char *u16toa(uint64_t num, char *buf, uint8_t base)
{
    return u64toa(num, buf, base);
}

char *u8toa(uint64_t num, char *buf, uint8_t base)
{
    return u64toa(num, buf, base);
}

uint32_t igris_atou32(const char *buf, uint8_t base, char **end)
{
    char c = *buf;
    uint32_t res = 0;

    while (igris_isxdigit(c))
    {
        c = *buf++;
        res = res * base + hex2half(c);
    }

    if (end)
        *end = (char *)buf - 1;

    return res;
}

uint64_t igris_atou64(const char *buf, uint8_t base, char **end)
{
    char c = *buf;
    uint64_t res = 0;

    while (igris_isxdigit(c))
    {
        c = *buf++;
        res = res * base + hex2half(c);
    }

    if (end)
        *end = (char *)buf - 1;

    return res;
}

int32_t igris_atoi32(const char *buf, uint8_t base, char **end)
{
    uint8_t minus;
    int32_t u;

    minus = *buf == '-';
    if (minus)
        ++buf;

    u = igris_atou32(buf, base, end);
    return minus ? -u : u;
}

int64_t igris_atoi64(const char *buf, uint8_t base, char **end)
{
    uint8_t minus;
    int64_t u;

    minus = *buf == '-';
    if (minus)
        ++buf;

    u = igris_atou64(buf, base, end);
    return minus ? -u : u;
}

#define MAX_PRECISION (10)
static const double rounders[MAX_PRECISION + 1] = {
    0.5,          // 0
    0.05,         // 1
    0.005,        // 2
    0.0005,       // 3
    0.00005,      // 4
    0.000005,     // 5
    0.0000005,    // 6
    0.00000005,   // 7
    0.000000005,  // 8
    0.0000000005, // 9
    0.00000000005 // 10
};

char *f32toa(float32_t f, char *buf, int8_t precision)
{
    char *ptr = buf;
    char *p = ptr;
    char *p1;
    char c;
    int32_t intPart;

    if (isinf(f))
    {
        *buf++ = f > 0 ? '+' : '-';
        return strcpy(buf, "inf");
        ;
    }

    if (isnan(f))
    {
        return strcpy(buf, "nan");
        ;
    }

    // check precision bounds
    if (precision > MAX_PRECISION)
        precision = MAX_PRECISION;

    // sign stuff
    if (f < 0)
    {
        f = -f;
        *ptr++ = '-';
    }

    if (precision < 0) // negative precision == automatic precision guess
    {
        if (f < 1.0)
            precision = 6;
        else if (f < 10.0)
            precision = 5;
        else if (f < 100.0)
            precision = 4;
        else if (f < 1000.0)
            precision = 3;
        else if (f < 10000.0)
            precision = 2;
        else if (f < 100000.0)
            precision = 1;
        else
            precision = 0;
    }

    // round value according the precision
    if (precision)
        f += rounders[precision];

    // integer part...
    intPart = f;
    f -= intPart;

    if (!intPart)
        *ptr++ = '0';
    else
    {
        // save start pointer
        p = ptr;

        // convert (reverse order)
        while (intPart)
        {
            *p++ = '0' + intPart % 10;
            intPart /= 10;
        }

        // save end pos
        p1 = p;

        // reverse result
        while (p > ptr)
        {
            c = *--p;
            *p = *ptr;
            *ptr++ = c;
        }

        // restore end pos
        ptr = p1;
    }

    // decimal part
    if (precision)
    {
        // place decimal point
        *ptr++ = '.';

        // convert
        while (precision--)
        {
            f *= 10.0;
            c = f;
            *ptr++ = '0' + c;
            f -= c;
        }
    }

    // terminating zero
    *ptr = 0;

    return buf;
}

static inline double local_pow(int b, int n)
{
    int64_t res = 1;
    while (n--)
    {
        res *= b;
    }
    return res;
}

float32_t igris_atof32(const char *str, char **pend)
{
    if (!igris_isdigit(*str) && *str != '-')
    {
        return 0;
    }

    uint8_t minus = *str == '-' ? 1 : 0;
    if (minus)
        str++;

    char *end;
    unsigned int u = igris_atou32(str, 10, &end);

    str = end;
    if (*str == '.')
    {
        int64_t d = igris_atou64(++str, 10, &end);
        if (pend)
            *pend = end;

        float ret = (float)u + ((double)d) / ((double)local_pow(10, end - str));
        return minus ? -ret : ret;
    }

    else
    {
        if (pend)
            *pend = end;
        return minus ? -(float)u : (float)u;
    }
}

#ifndef WITHOUT_FLOAT64
char *f64toa(float64_t f, char *buf, int8_t precision)
{
    return f32toa(f, buf, precision);
}

float64_t igris_atof64(const char *str, char **pend)
{
    if (!igris_isdigit(*str) && *str != '-')
    {
        return 0;
    }

    uint8_t minus = *str == '-' ? 1 : 0;
    if (minus)
        str++;

    char *end;
    unsigned int u = igris_atou32(str, 10, &end);

    str = end;
    if (*str == '.')
    {
        int64_t d = igris_atou64(++str, 10, &end);
        if (pend)
            *pend = end;
        double ret = (double)u + ((double)d) / (local_pow(10, end - str));

        return minus ? -ret : ret;
    }

    else
    {
        if (pend)
            *pend = end;
        return minus ? -(double)u : (double)u;
    }
}

#endif
