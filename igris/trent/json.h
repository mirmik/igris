#ifndef IGRIS_TRENT_JSON_H
#define IGRIS_TRENT_JSON_H

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
			virtual ~parser(){}
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
							while(c != '\n')
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

				if (isdigit(c) || c == '-') return parse_numer(cur, tr);
				if (c == '"') return parse_string(cur, tr);
				if (c == '\'') return parse_string(cur, tr);
				if (c == '[') return parse_list(cur, tr);
				if (c == '{') return parse_dict(cur, tr);
				if (isalpha(c)) return parse_mnemonic(cur, tr);

				throw std::exception();
			}
		};
	}
}


#endif