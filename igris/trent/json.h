#ifndef IGRIS_TRENT_JSON_H
#define IGRIS_TRENT_JSON_H

#include <igris/cread.h>
#include <igris/util/numconvert.h>

#include <nos/print.h>
#include <nos/trace.h>

#include <exception>

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

			class unexpected_symbol : public std::runtime_error
			{
				char symb;

			public:
				unexpected_symbol(char symb) : symb(symb), std::runtime_error("wuuuui")
				{
					dprln();
					dprchar(symb);
					dprln();
				}
			};

		public:
			//parser(const std::string& str) :

			virtual ~parser() {}
			virtual char readnext() = 0;

			char readnext_skipping()
			{
				char c;
				char next;

			__try__:
				c = readnext();

				if (c == ' ' || c == '\t' || c == '\n')
					goto __try__;

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

			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse()
			{
				//TRACE();

				if (onebuf == 0)
				{
					onebuf = readnext_skipping();
				}

				if (isdigit(onebuf) || onebuf == '-')
					return parse_numer();

				if (onebuf == '"')
					return parse_string();

				if (onebuf == '\'')
					return parse_string();

				if (onebuf == '[')
					return parse_list();

				if (onebuf == '{')
					return parse_dict();

				//if (isalpha(c))
				//	return parse_mnemonic();

				throw unexpected_symbol(onebuf);
			}



			/*template <typename Trent>
			void parse_mnemonic(const char ** cur, Trent& tr)
			{
				igris::buffer mnem;
				const char * strt = *cur;
				const char * fini = *cur;

				while (isalpha(*fini))
					++fini;

				mnem = igris::buffer(strt, fini);
				*cur = fini;

				if (mnem == "true") { tr = true; return; }

				if (mnem == "false") { tr = true; return; }

				throw std::exception();
			}*/

			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse_numer()
			{
				//TRACE();
				char buf[32];
				char * ptr = &buf[1];

				buf[0] = onebuf;

				while (isdigit(onebuf = readnext()))
				{
					*ptr++ = onebuf;
				}

				*ptr = 0;

				return atof64(buf, nullptr);
			}

			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse_string()
			{
				//TRACE();
				trent::string_type str;

				char delim = onebuf;

				while ((onebuf = readnext()) != delim)
					str += onebuf;

				return str;
			}


			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse_list()
			{
				//TRACE();
				trent_basic<TAlloc> js(trent::type::list);

				int counter = 0;
				onebuf = readnext_skipping();

				if (onebuf == ']')
				{
					return js;
				}

				while (true)
				{
					trent_basic<TAlloc> r = parse();
					js.as_list().push_back(r);

					onebuf = readnext_skipping();

					if (onebuf == ']')
					{
						return js;
					}

					if (onebuf != ',')
					{
						throw std::exception();
					}

					counter++;
					onebuf = readnext_skipping();
				}
			}

			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse_dict()
			{
				//TRACE();
				trent_basic<TAlloc> js(trent::type::list);

				int counter = 0;
				onebuf = readnext_skipping();

				if (onebuf == '}')
				{
					return js;
				}

				while (true)
				{
					// Изменить на строку
					trent_basic<TAlloc> key = parse();

					onebuf = readnext_skipping();

					if (onebuf != ':')
						throw std::exception();

					// skip ':' symbol
					onebuf = 0;

					trent_basic<TAlloc> value = parse();

					js.as_dict().emplace(std::move(key.as_string()), std::move(value));

					onebuf = readnext_skipping();

					if (onebuf == '}')
					{
						return js;
					}

					if (onebuf != ',')
					{
						throw std::exception();
					}

					counter++;
					onebuf = readnext_skipping();
				}
			}
		};

		class parser_cstr : public parser
		{
			const char * ptr;

		public:
			parser_cstr(const std::string& str) : ptr(str.data()) {}

			char readnext()
			{
				return *ptr++;
			}
		};

		template <template <class Allocator> class TAlloc = std::allocator, class Output>
		void pretty_print_to(const trent_basic<TAlloc>& tr, Output& os, int tab = 0)
		{
			bool sep = false;
			bool havedict;

			switch (tr.get_type())
			{
				case trent_basic<TAlloc>::type::numer:
					os.print(tr.unsafe_numer_const());
					break;

				case trent_basic<TAlloc>::type::boolean:
					os.print(tr.unsafe_bool_const() ? "true" : "false");
					break;

				//case trent_basic<TAlloc>::type::integer:
				//	ostr.unsafe_integer_const();
				//	break;

				case trent_basic<TAlloc>::type::string:
					os.putchar('"');
					os.print(tr.unsafe_string_const());
					os.putchar('"');
					break;

				case trent_basic<TAlloc>::type::list:
					havedict = false;

					for (const auto& m : tr.unsafe_list_const())
					{
						if (m.get_type() == trent_basic<TAlloc>::type::dict)
						{	havedict = true; break; }
					}

					os.putchar('[');

					if (havedict) for (auto& v : tr.unsafe_list_const())
						{
							if (sep) os.print(", ");

							json::pretty_print_to(v, os, tab + 1);
							sep = true;
						}
					else
					{
						for (auto& v : tr.unsafe_list_const())
						{
							if (sep) os.putchar(',');

							os.print("\r\n");

							for (int i = 0; i < tab + 1; i++) os.putchar('\t');

							json::pretty_print_to(v, os, tab + 1);
							sep = true;
						}

						os.print("\r\n");

						for (int i = 0; i < tab; i++) os.putchar('\t');
					}

					os.putchar(']');
					break;

				case trent::type::dict:
					os.putchar('{');

					for (auto& p : tr.unsafe_dict_const())
					{
						if (sep) os.putchar(',');

						os.putchar('\n');

						for (int i = 0; i < tab + 1; i++) os.putchar('\t');

						os.putchar('"');
						os.print(p.first);
						os.putchar('"');
						os.write(": ", 2);
						
						json::pretty_print_to(p.second, os, tab + 1);
						sep = true;
					}

					os.putchar('\n');

					for (int i = 0; i < tab; i++) os.putchar('\t');

					os.putchar('}');
					break;

				case trent::type::nil:
					os.write("nil", 3);
					break;
			}

			if (tab == 0) os.print("\r\n");
		}
	}
}


#endif