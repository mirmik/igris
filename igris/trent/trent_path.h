#ifndef TRENT_PATH_H
#define TRENT_PATH_H

/**
    @file
*/

#include <igris/util/string.h>
#include <iostream>
#include <vector>

namespace igris
{
    class trent_path_node
    {
    public:
        bool is_string;
        union
        {
            std::string str;
            int32_t i32;
        };

        trent_path_node() = delete;

        trent_path_node(const std::string &str_)
        {
            if (isdigit(str_[0]))
            {
                is_string = false;
                igris::constructor(&this->i32, std::stoi(str_));
            }
            else
            {
                is_string = true;
                igris::constructor(&this->str, str_);
            }
        }

        trent_path_node(const trent_path_node &oth)
        {
            if (oth.is_string == true)
            {
                is_string = true;
                igris::constructor(&str, oth.str);
            }
            else
            {
                is_string = false;
                igris::constructor(&i32, oth.i32);
            }
        }

        ~trent_path_node()
        {
            if (is_string)
                igris::destructor(&str);
        }

        igris::trent_path_node &operator=(const igris::trent_path_node &oth)
        {
            this->is_string = oth.is_string;
            if (this->is_string)
                this->str = oth.str;
            else
                this->i32 = oth.i32;

            return *this;
        }
    };

    class trent_path : public std::vector<trent_path_node>
    {
    public:
        trent_path() {}
        trent_path(const std::string &path) : trent_path(path.c_str()) {}

        trent_path(const char *path)
        {
            auto svec = igris::split(path, '/');

            for (const auto &s : svec)
            {
                std::vector<trent_path_node>::emplace_back(s);
            }
        }

        std::string to_string() const
        {
            std::vector<std::string> svec;
            svec.resize(size());

            for (unsigned int i = 0; i < size(); ++i)
            {
                auto &cur = operator[](i);
                if (cur.is_string)
                {
                    svec[i] = cur.str;
                }
                else
                {
                    svec[i] = std::to_string(cur.i32);
                }
            }

            return igris::join(svec, '/');
        }
    };
}

#endif
