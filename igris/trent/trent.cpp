#include <igris/trent/trent.h>

const char *igris::typestr(igris::trent_type m_type)
{
    switch (m_type)
    {
    case igris::trent_type::string:
        return "str";

    case igris::trent_type::list:
        return "list";

    case igris::trent_type::dict:
        return "dict";

    case igris::trent_type::numer:
        return "num";

    case igris::trent_type::boolean:
        return "bool";

    case igris::trent_type::nil:
        return "nil";

    default:
        BUG();
    }

    return "";
}
