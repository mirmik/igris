#ifndef IGRIS_TRENT_JSON_H
#define IGRIS_TRENT_JSON_H

#include <igris/cread.h>

namespace igris
{
	namespace json
	{
		//void print_to(const trent& tr, std::ostream& os);
		//void pretty_print_to(const trent& tr, std::ostream& os, int tab = 0);

		/*result<trent> parse(std::istream& is);
		trent parse_numer(std::istream& is);
		result<trent> parse_mnemonic(std::istream& is);
		result<trent> parse_string(std::istream& is);
		result<trent> parse_list(std::istream& is);
		result<trent> parse_dict(std::istream& is);*/

		//template <template <class Allocator> TAlloc>
		//trent_basic<TAlloc> parse();


		class parser
		{
			char onebuf = 0;

		public:
			virtual ~parser() {}
			virtual char readnext() = 0;

			char getnext()
			{
				char c;
				char next;

			__try__:
				c = readnext();

				if (c == '/')
				{
					c = readnext();

					switch (next)
					{
						case '*':
							while (true)
							{
								c = readnext();

								if (c == '*')
									if ((c = readnext()) == '/')
									{
										goto __try__;
									}
							}

						case '/':
							//is.ignore(INT_MAX, '\n');
							while (c != '\n')
								c = readnext();
							goto __try__;

						default:
							//is.unget();
							break;
					}
				}

				//is.unget();
				return c;

			}

			void parse(const char* cur, Trent& tr)
			{
				cur = igris::cread::skipws(cur);

				if (*cur == '/')
					cur = igris::cread::skip_cstyle_comments(cur);

				char c = *cur;

				if (isdigit(c) || c == '-') { parse_numer(cur, tr); return; }
				if (c == '"') { parse_string(cur, tr); return; }
				if (c == '\'') { parse_string(cur, tr); return; }
				if (c == '[') { parse_list(cur, tr); return; }
				if (c == '{') { parse_dict(cur, tr); return; }
				if (isalpha(c)) { parse_mnemonic(cur, tr); return; }

				throw std::exception();
			}



			template <typename Trent>
			void parse_mnemonic(const char ** cur, Trent& tr)
			{
				igris::buffer mnem;
				const char * strt = *cur;
				const char * fini = *cur;

				for (isalpha(*fini)) 
					++fini;				

				mnem = igris::buffer(strt, fini);
				*cur = fini;

				if (mnem == "true") { tr = true; return; }
				if (mnem == "false") { tr = true; return; }

				throw std::exception();
			}

			template <typename Trent>
			void parse_numer(const char ** cur, Trent& tr)
			{
				numer_type num;

				igris::buffer mnem;
				const char * strt = *cur;
				const char * fini = *cur;

				// digit or minus
				fini++;

				for (isdigit(*fini) || ) 
					++fini;				

				mnem = igris::buffer(strt, fini);
				*cur = fini;

				num = atof64(strt);

				tr = num;
				throw std::exception();
			}
/*
			result<trent> json::parse_string(std::istream& is)
			{
				trent::string_type str;

				char c = detail::getnext(is);
				is.ignore();

				if (c == '"') std::getline(is, str, '"');

				if (c == '\'') std::getline(is, str, '\'');

				trent ret(str);

				return ret;
			}

			result<trent> json::parse_list(std::istream& is)
			{
				char c;
				trent js(trent::type::list);

				int counter = 0;

				while (true)
				{
					c = detail::getnext(is);

					if (c == ']')
					{
						is.ignore();
						return js;
					}

					if (c == ',' || c == '[')
					{
						is.ignore();

						c = detail::getnext(is);

						if (c == ']')
						{
							is.ignore();
							return js;
						}

						auto&& r = parse(is);

						if (r.is_error())
							return std::move(r.error());

						js.as_list().push_back(r.value());
					}
					else return error("wrong list syntax");;

					counter++;
				}
			}

			result<trent> json::parse_dict(std::istream& is)
			{
				char c;
				trent js(trent::type::dict);

				while (true)
				{
					c = detail::getnext(is);

					if (c == '}')
					{
						is.ignore();
						return js;
					}

					if (c == ',' || c == '{')
					{
						is.ignore();

						c = detail::getnext(is);

						if (c == '}')
						{
							is.ignore();
							return js;
						}

						if ( c != '"' && c != '\'' )
							return error("wrong dictionary syntax: not find \"");

						is.ignore();

						std::string key;

						if (c == '"') std::getline(is, key, '"');

						if (c == '\'') std::getline(is, key, '\'');

						c = detail::getnext(is);

						if ( c != ':' )
							return error("wrong dicionary syntax: not find colomn");

						is.ignore();

						c = detail::getnext(is);

						auto&& r = parse(is);

						if (r.is_error())
							return std::move(r.error());

						js.as_dict().insert(std::make_pair(key, r.value()));
					}

					else
						return error("trent::internal:dict_parse");
				}
			}












		};
	}
}


#endif