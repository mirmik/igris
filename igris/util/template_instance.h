#ifndef IGRIS_TEMPLATE_INSTANCE_H
#define IGRIS_TEMPLATE_INSTANCE_H

#include <igris/util/macro.h>

#define TEMPLATE_INSTANCE(func, ...)                                           \
    constexpr const auto CONCAT2(privptr, __LINE__) = &func<__VA_ARGS__>;

#endif
