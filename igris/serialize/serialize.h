#ifndef IGRIS_SERIALIZE_SERIALIZE_H
#define IGRIS_SERIALIZE_SERIALIZE_H

#include <igris/buffer.h>
#include <igris/math/defs.h>
#include <igris/result.h>

#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace igris
{
    template <typename M, typename T, typename U = int>
    struct is_have_serialize : std::false_type
    {
    };

    template <typename M, typename T>
    struct is_have_serialize<M, T, decltype((void)&T::template serialize<M>, 0)>
        : std::true_type
    {
    };

    template <typename M, typename T, bool HaveSerialize = true>
    struct serialize_helper_basic
    {
        static void serialize(M &keeper, const T &obj)
        {
            obj.serialize(keeper);
        }

        static void deserialize(M &keeper, T &obj) { obj.deserialize(keeper); }
    };

    template <typename M, typename T> struct serialize_helper_basic<M, T, false>
    {
        static void serialize(M &keeper, const T &obj) { keeper.dump(obj); }

        static void deserialize(M &keeper, T &obj) { keeper.load(obj); }

        static void deserialize(M &keeper, T &&obj) { keeper.load(obj); }
    };

    template <typename M, typename T>
    struct serialize_helper
        : public serialize_helper_basic<M, T, is_have_serialize<M, T>::value>
    {
    };

    template <typename M, typename T>
    inline void serialize(M &keeper, const T &obj)
    {
        serialize_helper<M, T>::serialize(keeper, obj);
    }

    template <typename M, typename T> inline void deserialize(M &keeper, T &obj)
    {
        serialize_helper<M, std::remove_cv_t<T>>::deserialize(keeper, obj);
    }

    template <typename M, typename T>
    inline void deserialize(M &keeper, T &&obj)
    {
        serialize_helper<M, std::remove_cv_t<T>>::deserialize(keeper,
                                                              std::move(obj));
    }

    namespace archive
    {
        template <typename T> struct data
        {
            T *ptr;
            size_t sz;
            data(T *ptr, size_t sz) : ptr(ptr), sz(sz) {}
            data(const T *ptr, size_t sz) : ptr((T *)ptr), sz(sz) {}
            template <typename R> void reflect(R &r)
            {
                r.do_data((char *)ptr, sz * sizeof(T));
            }
        };

        class binary_serializer_basic
        {
          public:
            template <typename T> void operator&(const T &obj)
            {
                igris::serialize(*this, obj);
            }

            virtual void dump_data(const char *dat, uint16_t sz) = 0;
            void do_data(const char *dat, uint16_t sz) { dump_data(dat, sz); }

            void dump(const char *dat, uint16_t sz)
            {
                dump(sz);
                dump_data(dat, sz);
            }

            void dump(char i) { dump_data((char *)&i, sizeof(i)); }
            void dump(signed char i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned char i) { dump_data((char *)&i, sizeof(i)); }

            void dump(short i) { dump_data((char *)&i, sizeof(i)); }
            void dump(int i) { dump_data((char *)&i, sizeof(i)); }
            void dump(long i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned short i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned int i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned long i) { dump_data((char *)&i, sizeof(i)); }
            void dump(unsigned long long i)
            {
                dump_data((char *)&i, sizeof(i));
            }
            void dump(float i) { dump_data((char *)&i, sizeof(i)); }
            void dump(double i) { dump_data((char *)&i, sizeof(i)); }
            void dump(long double i) { dump_data((char *)&i, sizeof(i)); }
            void dump(igris::buffer buf)
            {
                dump((uint16_t)buf.size());
                dump_data(buf.data(), buf.size());
            }

            template <typename T> void dump(const T &ref)
            {
                ((std::remove_cv_t<std::remove_reference_t<T>> &)(ref))
                    .reflect(*this);
            }
        };

        class binary_string_writer : public binary_serializer_basic
        {
          public:
            std::string &sstr;

            void dump_data(const char *dat, uint16_t size) override
            {
                sstr.append(dat, size);
            }

            binary_string_writer(std::string &str) : sstr(str) {}
        };

        class binary_buffer_writer : public binary_serializer_basic
        {
          public:
            char *ptr;
            char *_end;

            void dump_data(const char *dat, uint16_t size) override
            {
                memcpy(ptr, dat, size);
                ptr += size;
            }

            binary_buffer_writer(char *str, size_t size)
                : ptr(str), _end(str + size)
            {
            }
        };

        class writable_buffer : public igris::buffer
        {
        };

        class settable_buffer
        {
          public:
            igris::buffer &ref;

          public:
            settable_buffer(igris::buffer &buf) : ref(buf) {}
        };

        class binary_deserializer_basic
        {
          public:
            template <typename T> void operator&(T &&obj)
            {
                igris::deserialize(*this, obj);
            }

            virtual void load_data(char *dat, uint16_t sz) = 0;
            void do_data(char *dat, uint16_t sz) { load_data(dat, sz); }

            void load(char *dat, uint16_t maxsz)
            {
                uint16_t sz;
                load(sz);
                assert(sz <= maxsz);
                load_data(dat, sz);
            }

            void load(int8_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int16_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int32_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(int64_t &i) { load_data((char *)&i, sizeof(i)); }

            void load(uint8_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint16_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint32_t &i) { load_data((char *)&i, sizeof(i)); }
            void load(uint64_t &i) { load_data((char *)&i, sizeof(i)); }

            /*void load(char& i) { load_data((char*)&i, sizeof(i)); }
            void load(short& i) { load_data((char*)&i, sizeof(i)); }
            void load(int& i) { load_data((char*)&i, sizeof(i)); }
            void load(long& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned char& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned short& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned int& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned long& i) { load_data((char*)&i, sizeof(i)); }
            void load(unsigned long long& i) { load_data((char*)&i, sizeof(i));
            }*/

            void load(float &i) { load_data((char *)&i, sizeof(i)); }
            void load(double &i) { load_data((char *)&i, sizeof(i)); }
            void load(long double &i) { load_data((char *)&i, sizeof(i)); }

            void load(igris::archive::settable_buffer &buf)
            {
                uint16_t len;
                load(len);

                buf.ref.data((char *)pointer());
                buf.ref.size(len);

                skip(len);
            }

            void load(igris::archive::writable_buffer &buf)
            {
                uint16_t len;
                load(len);

                int readsize = __MIN__(buf.size(), len);
                load_data(buf.data(), readsize);

                buf.size(readsize);
            }

            void load_set_buffer(igris::buffer &buf)
            {
                igris::archive::settable_buffer arch(buf);
                load(arch);
            }

            template <typename T> void load(T &&ref)
            {
                ((std::remove_cv_t<std::remove_reference_t<T>> &)(ref))
                    .reflect(*this);
            }

            virtual void skip(int ptr) = 0;
            virtual void *pointer() = 0;
            virtual const void *end() = 0;
        };

        /*class binary_string_reader : public binary_deserializer_basic
        {
        public:
            std::istringstream stream;

            void load_data(char* dat, uint16_t size) override
            {
                stream.read(dat, size);
            }

            binary_string_reader(const std::string& str) : stream(str) {}
        };*/

        class binary_buffer_reader : public binary_deserializer_basic
        {
          public:
            const char *ptr;
            const char *_end;

            void load_data(char *dat, uint16_t size) override
            {
                memcpy(dat, ptr, size);
                ptr += size;
            }

            void skip(int size) override { ptr += size; }

            void *pointer() override { return (void *)ptr; }
            const void *end() override { return _end; }

            binary_buffer_reader(const char *str, size_t size)
                : ptr(str), _end(str + size)
            {
            }
            binary_buffer_reader(igris::buffer buf)
                : ptr(buf.data()), _end(buf.data() + buf.size())
            {
            }
        };

        using binreader = binary_buffer_reader;
        using binwriter = binary_buffer_writer;
    } // namespace archive

    template <typename Archive, typename... Args>
    struct serialize_helper<Archive, std::tuple<Args...>>
    {
        using Tuple = std::tuple<Args...>;

        template <typename std::size_t... I>
        static void tuple_serialize_helper(Archive &keeper, const Tuple &tpl,
                                           std::index_sequence<I...>)
        {
            int ___[] = {(igris::serialize(keeper, std::get<I>(tpl)), 0)...};
        }

        static void serialize(Archive &keeper, const Tuple &tpl)
        {
            tuple_serialize_helper(keeper, tpl,
                                   std::index_sequence_for<Args...>{});
        }

        template <typename std::size_t... I>
        static void tuple_deserialize_helper(Archive &keeper, Tuple &tpl,
                                             std::index_sequence<I...>)
        {
            int ___[] = {(igris::deserialize(keeper, std::get<I>(tpl)), 0)...};
        }

        static void deserialize(Archive &keeper, Tuple &tpl)
        {
            tuple_deserialize_helper(keeper, tpl,
                                     std::index_sequence_for<Args...>{});
        }
    };

    template <typename Archive> struct serialize_helper<Archive, std::string>
    {
        static void serialize(Archive &keeper, const std::string &str)
        {
            igris::serialize(keeper, igris::buffer(str.data(), str.size()));
        }

        static void deserialize(Archive &keeper, std::string &str)
        {
            uint16_t size;
            igris::deserialize(keeper, size);
            str.resize(size);
            keeper.load_data((char *)str.data(), str.size());
        }
    };

    template <typename Archive, class F, class S>
    struct serialize_helper<Archive, std::pair<F, S>>
    {
        static void serialize(Archive &keeper, const std::pair<F, S> &pair)
        {
            igris::serialize(keeper, pair.first);
            igris::serialize(keeper, pair.second);
        }

        static void deserialize(Archive &keeper, std::pair<F, S> &pair)
        {
            igris::deserialize(keeper, pair.first);
            igris::deserialize(keeper, pair.second);
        }
    };

    template <typename Archive, typename T>
    struct serialize_helper<Archive, std::vector<T>>
    {
        static void serialize(Archive &keeper, const std::vector<T> &vec)
        {
            igris::serialize(keeper, (uint16_t)vec.size());
            igris::serialize(keeper,
                             igris::archive::data<T>{vec.data(), vec.size()});
        }

        static void deserialize(Archive &keeper, std::vector<T> &vec)
        {
            uint16_t size;
            igris::deserialize(keeper, size);

            for (int i = 0; i < size; i++)
            {
                T value;
                igris::deserialize(keeper, value);
                vec.push_back(value);
            }
        }
    };

    template <typename Archive, class K, class T>
    struct serialize_helper<Archive, std::map<K, T>>
    {
        static void serialize(Archive &keeper, const std::map<K, T> &map)
        {
            igris::serialize(keeper, (uint16_t)map.size());
            // igris::serialize(keeper, igris::archive::data<T> {vec.data(),
            // vec.size()});
            for (auto pair : map)
            {
                igris::serialize(keeper, pair);
            }
        }

        static void deserialize(Archive &keeper, std::map<K, T> &map)
        {
            uint16_t size;
            igris::deserialize(keeper, size);

            for (int i = 0; i < size; i++)
            {
                // typename std::map<K,T>::value_type pair;
                K first;
                T second;
                igris::deserialize(keeper, first);
                igris::deserialize(keeper, second);
                map.insert(std::make_pair(first, second));
            }
        }
    };

    template <class T> std::string serialize(const T &obj)
    {
        std::string ret;
        igris::archive::binary_string_writer writer(ret);

        igris::serialize(writer, obj);

        return ret;
    }

    template <class T> T deserialize(const igris::buffer &in)
    {
        T ret;

        igris::archive::binary_buffer_reader reader(in.data(), in.size());
        igris::deserialize(reader, ret);

        return ret;
    }
} // namespace igris

#endif
