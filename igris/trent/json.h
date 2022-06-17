#ifndef IGRIS_TRENT_JSON_H
#define IGRIS_TRENT_JSON_H

/**
    @file
*/

#include <igris/trent/trent.h>
#include <igris/util/numconvert.h>

#include <exception>
#include <sstream>

using namespace std::literals::string_literals;

namespace igris
{
    namespace json
    {
        class parser
        {
            int symbno = 0;
            int lineno = 1;
            char onebuf = 0;

            class unexpected_symbol : public std::runtime_error
            {
            public:
                unexpected_symbol(char symb)
                    : std::runtime_error(std::string("unexpected:") + symb)
                {
                }
            };

        public:
            virtual ~parser() {}
            virtual char readnext_impl() = 0;

            char readnext()
            {
                symbno++;
                char c = readnext_impl();
                if (c == '\n')
                {
                    lineno++;
                    symbno = 0;
                }
                return c;
            }

            std::string errloc()
            {
                return " lineno:" + std::to_string(lineno) +
                       " symbno:" + std::to_string(symbno);
            }

            char readnext_skipping()
            {
                char c;

            __try__:
                c = readnext();

                if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                    goto __try__;

                if (c == '/')
                {
                    c = readnext();

                    switch (c)
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

                        // is.ignore(INT_MAX, '\n');
                        while (c != '\n')
                            c = readnext();

                        goto __try__;

                    default:
                        // is.unget();
                        break;
                    }
                }

                // is.unget();
                return c;
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse()
            {
                // TRACE();

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

                throw std::runtime_error("unexpected_symbol "s + onebuf +
                                         errloc());
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_mnemonic()
            {
                char buf[32];
                char *ptr = &buf[1];

                buf[0] = onebuf;

                while (isalpha(onebuf = readnext_skipping()))
                {
                    *ptr++ = onebuf;
                }

                *ptr = 0;

                if (strcmp("true", buf) == 0)
                {
                    return true;
                }
                if (strcmp("false", buf) == 0)
                {
                    return false;
                }

                throw std::runtime_error("unexpected_mnemonic "s + errloc());
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_numer()
            {
                char buf[32];
                char *ptr = &buf[1];

                buf[0] = onebuf;

                while (isdigit(onebuf = readnext()) || onebuf == '-' ||
                       onebuf == 'e' || onebuf == '.')
                {
                    *ptr++ = onebuf;
                }

                *ptr = 0;

                if (isspace(onebuf))
                    onebuf = 0;

                return strtod(buf, nullptr);
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_string()
            {
                // TRACE();
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
                // TRACE();
                trent_basic<TAlloc> js(trent::type::list);

                int counter = 0;
                onebuf = readnext_skipping();

                if (onebuf == ']')
                {
                    onebuf = 0;
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
                        throw std::runtime_error(
                            "json::parse_list: wait_, expect_"s + onebuf +
                            errloc());
                    }

                    counter++;
                    onebuf = readnext_skipping();
                }
            }

            template <template <class Allocator> class TAlloc = std::allocator>
            trent_basic<TAlloc> parse_dict()
            {
                // TRACE();
                trent_basic<TAlloc> js(trent::type::dict);

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
                        throw std::runtime_error(
                            "json:parse_dict_0 wait_: expect_"s + onebuf +
                            " lineno:" + std::to_string(lineno) +
                            " symbno:" + std::to_string(symbno));

                    onebuf = 0;

                    trent_basic<TAlloc> value = parse();

                    js.as_dict().emplace(std::move(key.as_string()),
                                         std::move(value));

                    if (onebuf == 0)
                        onebuf = readnext_skipping();

                    if (onebuf == '}')
                    {
                        onebuf = 0;
                        return js;
                    }

                    if (onebuf != ',')
                    {
                        throw std::runtime_error(
                            "json:parse_dict_1 wait_, expect_"s + onebuf +
                            " lineno:" + std::to_string(lineno) +
                            " symbno:" + std::to_string(symbno));
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
            const char *ptr=nullptr;

        public:
            parser_cstr(const char *str) : ptr(str) {}
            parser_cstr(const parser_cstr &) =default;
            parser_cstr& operator=(const parser_cstr &) =default;

            char readnext_impl() { return *ptr++; }
        };

        class parser_str : public parser
        {
            const char *ptr=nullptr;

        public:
            parser_str(const std::string &str) : ptr(str.data()) {}
            parser_str(const parser_str &) =default;
            parser_str& operator=(const parser_str &) =default;

            char readnext_impl() { return *ptr++; }
        };

        class parser_input_stream : public parser
        {
            std::istream &is;

        public:
            parser_input_stream(std::istream &is) : is(is) {}

            char readnext_impl() { return is.get(); }
        };

        igris::trent parse(const char *str);
        igris::trent parse(const std::string &str);
        igris::trent parse_file(const std::string &str);

        template <template <class Allocator> class TAlloc = std::allocator,
                  class Output>
        void pretty_print_to(const trent_basic<TAlloc> &tr, Output &os,
                             int tab = 0)
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

            case trent_basic<TAlloc>::type::string:
                os.putbyte('"');
                os.print(tr.unsafe_string_const());
                os.putbyte('"');
                break;

            case trent_basic<TAlloc>::type::list:
                havedict = false;

                for (const auto &m : tr.unsafe_list_const())
                {
                    if (m.get_type() == trent_basic<TAlloc>::type::dict)
                    {
                        havedict = true;
                        break;
                    }
                }

                os.putbyte('[');

                if (havedict)
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                            os.print(", ");

                        json::pretty_print_to(v, os, tab + 1);
                        sep = true;
                    }
                else
                {
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                            os.putbyte(',');

                        os.print("\r\n");

                        for (int i = 0; i < tab + 1; i++)
                            os.putbyte('\t');

                        json::pretty_print_to(v, os, tab + 1);
                        sep = true;
                    }

                    os.print("\r\n");

                    for (int i = 0; i < tab; i++)
                        os.putbyte('\t');
                }

                os.putbyte(']');
                break;

            case trent::type::dict:
                os.putbyte('{');

                for (auto &p : tr.unsafe_dict_const())
                {
                    if (sep)
                        os.putbyte(',');

                    os.putbyte('\n');

                    for (int i = 0; i < tab + 1; i++)
                        os.putbyte('\t');

                    os.putbyte('"');
                    os.print(p.first);
                    os.putbyte('"');
                    os.write(": ", 2);

                    json::pretty_print_to(p.second, os, tab + 1);
                    sep = true;
                }

                os.putbyte('\n');

                for (int i = 0; i < tab; i++)
                    os.putbyte('\t');

                os.putbyte('}');
                break;

            case trent::type::nil:
                os.write("nil", 3);
                break;
            }

            if (tab == 0)
                os.print("\r\n");
        }

        template <template <class Allocator> class TAlloc = std::allocator>
        std::string to_string(const igris::trent_basic<TAlloc> &tr)
        {
            std::stringstream ss;
            pretty_print_to(tr, ss);
            return ss.str();
        }
    }
}

#endif
