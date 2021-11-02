
#include <igris/trent/json.h>

igris::trent igris::json::parse(const char *str)
{
    parser_cstr parser(str);
    return parser.parse();
}

igris::trent igris::json::parse(const std::string &str)
{
    parser_str parser(str);
    return parser.parse();
}