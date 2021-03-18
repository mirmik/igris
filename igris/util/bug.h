#ifndef NOS_BUGON_H
#define NOS_BUGON_H

#include <igris/compiler.h>
#include <igris/util/location.h>

#ifdef __cplusplus
#include <cstdio>
#include <cstring>
#endif

__BEGIN_DECLS

__NORETURN void bug_impl();

__END_DECLS

#define BUG()                                                                  \
    {                                                                          \
        CURRENT_LOCATION(loc);                                                 \
        debug_print_location(loc);                                             \
        bug_impl();                                                            \
    }

#define BUG_ON(eq)                                                             \
    {                                                                          \
        if (eq)                                                                \
        {                                                                      \
            CURRENT_LOCATION(loc);                                             \
            debug_print_location(loc);                                         \
            dprln(#eq);                                                        \
            bug_impl();                                                        \
        }                                                                      \
    }

#endif
