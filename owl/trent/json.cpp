#include <nos/trent/json.h>

#include <istream>
#include <limits.h>

using namespace nos::result_type;

namespace nos
{
	namespace detail
	{
		char getnext(std::istream& is)
		{
			char c;
			char next;
		__try__:

			is >> c;

			if (c == '/')
			{
				next = (char)is.get();

				switch (next)
				{
					case '*':
						while (true)
						{
							is >> c;

							if (c == '*')
								if (is.peek() == '/')
								{
									is.ignore();
									goto __try__;
								}
						}

					case '/':
						is.ignore(INT_MAX, '\n');
						goto __try__;

					default:
						is.unget();
						break;
				}
			}

			is.unget();
			return c;
		}
	}

	result<trent> json::parse(std::istream& is)
	{
		is >> std::skipws;

		char c = detail::getnext(is);

		if (isdigit(c) || c == '-') return parse_numer(is);

		if (c == '"') return parse_string(is);

		if (c == '\'') return parse_string(is);

		if (c == '[') return parse_list(is);

		if (c == '{') return parse_dict(is);

		if (isalpha(c)) return parse_mnemonic(is);

		return error("undefined trent");
	}

	result<trent> json::parse_mnemonic(std::istream& is)
	{
		char c;
		std::string str;

		while (1)
		{
			is >> c;

			if (isalpha(c))
			{
				str += c;
				continue;
			}
			else
			{
				is.unget();
				break;
			}
		}

		if (str == "true") return trent(true);

		if (str == "false") return trent(false);

		return error("undefined mnemonic");
	}

	trent json::parse_numer(std::istream& is)
	{
		trent::numer_type num;
		is >> num;

		if (num - (trent::integer_type)num == 0)
			return trent((trent::integer_type)num);

		return trent(num);
	}

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

	void json::print_to(const trent& tr, std::ostream& os)
	{
		bool sep = false;

		switch (tr.get_type())
		{

			case trent::type::integer:
				os << tr.unsafe_integer_const();
				return;

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

	void json::pretty_print_to(const trent& tr, std::ostream& os, int tab)
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

			case trent::type::integer:
				os << tr.unsafe_integer_const();
				break;

			case trent::type::string:
				os << '"' << tr.unsafe_string_const() << '"';
				break;

			case trent::type::list:
				havedict = false;

				for (const auto& m : tr.unsafe_list_const())
				{
					if (m.get_type() == trent::trent::type::dict)
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
	}
}
