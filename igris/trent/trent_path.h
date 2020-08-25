#ifndef TRENT_PATH_H
#define TRENT_PATH_H

#include <igris/util/string.h>
#include <vector>

namespace igris {
	struct trent_path_node
	{
		bool is_string;
		union
		{
			std::string str;
			int32_t i32;
		};

		trent_path_node() = delete;

		trent_path_node(const std::string& str_)
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

		trent_path_node(const trent_path_node& oth)
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
			if (is_string) igris::destructor(&str);
		}

		/*size_t print_to(nos::ostream& o) const
		{
			if (is_string) return nos::print_to(o, str);
			return nos::print_to(o, i32);
		}*/

		igris::trent_path_node& operator= (const igris::trent_path_node& oth) 
		{
			this->is_string = oth.is_string;
			if (this->is_string) 
				this->str = oth.str;
			else 
				this->i32 = oth.i32;

			return *this;
		}
	};

	struct trent_path : public std::vector<trent_path_node>
	{
		trent_path(){}
		trent_path(const std::string& path) : trent_path(path.c_str()) {}

		trent_path(const char* path)
		{
			igris::strvec svec = igris::split(path, '/');
			for (const auto& s : svec)
			{
				emplace_back(s);
			}
		}
	};
}

#endif