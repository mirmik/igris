#include <igris/util/string.h>

#include <cstring>

std::string igris::replace(const std::string &input, const std::string &sub,
                           const std::string &rep)
{
    std::string output;

    const char *strit = input.data();
    const char *streit = input.data() + input.size();

    if (sub.length() == 0)
    {
        output.append(input);
        return output;
    }

    char *finded;
    while ((finded = (char *)igris_memmem(strit, streit - strit, sub.data(),
                                          sub.size())) != NULL)
    {
        int step = finded - strit;

        output.append(strit, step);
        strit += step;

        output.append(rep);
        strit += sub.size();
    };

    output.append(strit, streit - strit);
    return output;
}
