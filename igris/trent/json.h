#ifndef IGRIS_TRENT_JSON_H
#define IGRIS_TRENT_JSON_H

#include <igris/cread.h>
#include <igris/util/numconvert.h>
#include <igris/trent/trent.h>

#include <nos/print.h>
#include <nos/trace.h>

#include <exception>

using namespace std::literals::string_literals;

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
			int symbno = 0;
			int lineno = 1;
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
			virtual char readnext_impl() = 0;

			char readnext() 
			{
				symbno++;
				char c = readnext_impl();
				if (c == '\n') {lineno++; symbno = 0;}
				return c;
			}

			std::string errloc() 
			{
				return " lineno:" + std::to_string(lineno) + " symbno:" + std::to_string(symbno);
			}

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

				if (isalpha(onebuf))
					return parse_mnemonic();

				throw std::runtime_error("unexpected_symbol "s + onebuf + errloc());
			}



			template <template <class Allocator> class TAlloc = std::allocator>
			trent_basic<TAlloc> parse_mnemonic()
			{
				char buf[32];
				char * ptr = &buf[1];

				buf[0] = onebuf;

				while (isalpha(onebuf = readnext()))
				{
					*ptr++ = onebuf;
				}

				*ptr = 0;

				if (strcmp("true",buf)==0) { return true; }
				if (strcmp("false",buf)==0) { return false; }

				throw std::runtime_error("unexpected_mnemonic "s + errloc());
			}

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

				if(isspace(onebuf))
					onebuf = 0;

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

				onebuf = 0;
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

					if (onebuf == 0)
						onebuf = readnext_skipping();

					if (onebuf == ']')
					{
						onebuf = 0;
						return js;
					}

					if (onebuf != ',')
					{
						throw std::runtime_error("json::parse_list: wait_, expect_"s + onebuf + errloc());
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
						throw std::runtime_error("json:parse_dict wait_: expect_"s + onebuf+ " lineno:" + std::to_string(lineno)  + " symbno:" + std::to_string(symbno));

					// skip ':' symbol

					onebuf = 0;

					trent_basic<TAlloc> value = parse();

					js.as_dict().emplace(std::move(key.as_string()), std::move(value));

					if (onebuf == 0)
						onebuf = readnext_skipping();

					if (onebuf == '}')
					{
						onebuf = 0;
						return js;
					}

					if (onebuf != ',')
					{
						throw std::runtime_error("json:parse_dict wait_, expect_"s + onebuf+ " lineno:" + std::to_string(lineno) + " symbno:" + std::to_string(symbno));
					}

					counter++;
					onebuf = readnext_skipping();

					if (onebuf == '}')
					{
						onebuf = 0;
						return js;
					}
				}
			}
		};

		class parser_cstr : public parser
		{
			const char * ptr;

		public:
			parser_cstr(const std::string& str) : ptr(str.data()) {}

			char readnext_impl()
			{
				return *ptr++;
			}
		};

		class parser_input_stream : public parser
		{
			std::istream& is;

		public:
			parser_input_stream(std::istream& is) : is(is) {}

			char readnext_impl()
			{
				return is.get();
			}
		};

		/*template <template <class Allocator> class TAlloc = std::allocator>
		void print_to(const trent_basic<TAlloc>& tr, std::ostream& os)
		{
			bool sep = false;

			switch (tr.get_type())
			{

				//case trent::type::integer:
				//	os << tr.unsafe_integer_const();
				//	return;

				case trent::type::numer:
					os << tr.unsafe_numer_const();
					return;

				case trent::type::boolean:
					os << (tr.unsafe_bool_const() ? "true" : "false");
					break;

				case trent::type::string:
					os << '"';
					os << tr.unsafe_string_const();
					os << '"';
					return;

				case trent::type::list:
					os << '[';

					for (auto& v : tr.unsafe_list_const())
					{
						if (sep) os << ',';

						json::print_to(v, os);
						sep = true;
					}

					os << ']';
					return;

				case trent::type::dict:
					os << '{';

					for (auto& p : tr.unsafe_dict_const())
					{
						if (sep) os << ',';

						os << '"';
						os << p.first;
						os << '"';
						os << ':';
						json::print_to(p.second, os);
						sep = true;
					}

					os << '}';
					return;

				case trent::type::nil:
					os << "nil";
					return;
			}
		}

		template <template <class Allocator> class TAlloc = std::allocator>
		void pretty_print_to(const trent& tr, std::ostream& os, int tab)
		{
			bool sep = false;
			bool havedict;

			switch (tr.get_type())
			{

				case trent::type::numer:
					os << std::fixed << tr.unsafe_numer_const();
					break;

				case trent::type::boolean:
					os << (tr.unsafe_bool_const() ? "true" : "false");
					break;

				//case trent::type::integer:
				//	os << tr.unsafe_integer_const();
				//	break;

				case trent::type::string:
					os << '"' << tr.unsafe_string_const() << '"';
					break;

				case trent::type::list:
					havedict = false;

					for (const auto& m : tr.unsafe_list_const())
					{
						if (m.get_type() == trent_basic<TAlloc>::type::dict)
						{	havedict = true; break; }
					}

					os << '[';

					if (havedict) for (auto& v : tr.unsafe_list_const())
						{
							if (sep) os << ", ";

							json::pretty_print_to(v, os, tab + 1);
							sep = true;
						}
					else
					{
						for (auto& v : tr.unsafe_list_const())
						{
							if (sep) os.put(',');

							os << std::endl;

							for (int i = 0; i < tab + 1; i++) os.put('\t');

							json::pretty_print_to(v, os, tab + 1);
							sep = true;
						}

						os << std::endl;

						for (int i = 0; i < tab; i++) os.put('\t');
					}

					os.put(']');
					break;

				case trent::type::dict:
					os.put('{');

					for (auto& p : tr.unsafe_dict_const())
					{
						if (sep) os << ',';

						os.put('\n');

						for (int i = 0; i < tab + 1; i++) os.put('\t');

						os << '"' << p.first << '"';
						os.write(": ", 2);
						json::pretty_print_to(p.second, os, tab + 1);
						sep = true;
					}

					os.put('\n');

					for (int i = 0; i < tab; i++) os.put('\t');

					os.put('}');
					break;

				case trent::type::nil:
					os.write("nil", 3);
					break;
			}

			if (tab == 0) os << std::endl;
		}*/





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