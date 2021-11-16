#include <fstream>
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

igris::trent igris::json::parse_file(const std::string &str)
{
    std::ifstream t("file.txt");
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);

    return parse(buffer);
}
