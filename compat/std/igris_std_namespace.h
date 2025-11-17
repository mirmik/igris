#ifndef IGRIS_COMPAT_STD_NAMESPACE_H
#define IGRIS_COMPAT_STD_NAMESPACE_H

namespace igris_std {}

#ifdef IGRIS_COMPAT_STD_TO_STD
namespace std {
    using namespace igris_std;
}
#endif

#endif
