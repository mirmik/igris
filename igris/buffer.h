#include <string_view>
#warning "igris::buffer is deprecated use string_view instead"

namespace igris
{
    using buffer = std::string_view;
}