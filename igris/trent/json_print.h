#include <igris/trent/json.h>
#include <iostream>
#include <sstream>
#include <string>

namespace igris {
    namespace json {
        void pretty_print_to(const trent &tr, std::ostream &os,
                             int tab = 0)
        {
            bool sep = false;
            bool havedict;

            switch (tr.get_type())
            {
            case trent::type::numer:
                os << tr.unsafe_numer_const();
                break;

            case trent::type::boolean:
                os << (tr.unsafe_bool_const() ? "true" : "false");
                break;

            case trent::type::string:
                os.put('"');
                os << tr.unsafe_string_const();
                os.put('"');
                break;

            case trent::type::list:
                havedict = false;

                for (const auto &m : tr.unsafe_list_const())
                {
                    if (m.get_type() == trent::type::dict)
                    {
                        havedict = true;
                        break;
                    }
                }

                os.put('[');

                if (havedict)
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                            os << ", ";

                        json::pretty_print_to(v, os, tab + 1);
                        sep = true;
                    }
                else
                {
                    for (auto &v : tr.unsafe_list_const())
                    {
                        if (sep)
                            os.put(',');

                        os << "\r\n";

                        for (int i = 0; i < tab + 1; i++)
                            os.put('\t');

                        json::pretty_print_to(v, os, tab + 1);
                        sep = true;
                    }

                    os << "\r\n";

                    for (int i = 0; i < tab; i++)
                        os.put('\t');
                }

                os.put(']');
                break;

            case trent::type::dict:
                os.put('{');

                for (auto &p : tr.unsafe_dict_const())
                {
                    if (sep)
                        os.put(',');

                    os.put('\n');

                    for (int i = 0; i < tab + 1; i++)
                        os.put('\t');

                    os.put('"');
                    os << p.first;
                    os.put('"');
                    os.write(": ", 2);

                    json::pretty_print_to(p.second, os, tab + 1);
                    sep = true;
                }

                os.put('\n');

                for (int i = 0; i < tab; i++)
                    os.put('\t');

                os.put('}');
                break;

            case trent::type::nil:
                os.write("nil", 3);
                break;
            }

            if (tab == 0)
                os << "\r\n";
        }
 
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
            pretty_print_to(tr, static_cast<std::ostream&>(ss));
            return ss.str();
        }
    }
}