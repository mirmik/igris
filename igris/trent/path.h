/*	struct trent_path_node
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
				gxx::constructor(&this->i32, std::stoi(str_));
			}
			else
			{
				is_string = true;
				gxx::constructor(&this->str, str_);
			}
		}

		trent_path_node(const trent_path_node& oth)
		{
			if (oth.is_string == true)
			{
				is_string = true;
				gxx::constructor(&str, oth.str);
			}
			else
			{
				is_string = false;
				gxx::constructor(&i32, oth.i32);
			}
		}

		~trent_path_node()
		{
			if (is_string) gxx::destructor(&str);
		}

		size_t printTo(gxx::io::ostream& o) const
		{
			if (is_string) return gxx::print_to(o, str);
			return gxx::print_to(o, i32);
		}

		gxx::trent_path_node& operator= (const gxx::trent_path_node& oth) 
		{
			this->is_string = oth.is_string;
			if (this->is_string) 
				this->str = oth.str;
			else 
				this->i32 = oth.i32;

			return *this;
		}
	};

	struct trent_path : public std::vector<trent_path_node>, public gxx::array_printable<trent_path>
	{
		trent_path(){}
		trent_path(const std::string& path) : trent_path(path.c_str()) {}

		trent_path(const char* path)
		{
			gxx::strvec svec = gxx::split(path, '/');
			for (const auto& s : svec)
			{
				emplace_back(s);
			}
		}
	};
*/
