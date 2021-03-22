#ifndef igris_UTIL_CTR_DTR_H
#define igris_UTIL_CTR_DTR_H

//#include <igris/util/placed_new.h>

namespace igris
{
    template <typename T> void destructor(T *ptr) { ptr->~T(); }

    template <typename T, typename... Args>
    void constructor(T *ptr, Args &&... args)
    {
        new (ptr) T(std::forward<Args>(args)...);
    }

    template <typename T> void copy_constructor(T *ptr, const T &other)
    {
        new (ptr) T(other);
    }

    template <typename T> void move_constructor(T *ptr, T &&other)
    {
        new (ptr) T(std::move(other));
    }

    template <class InputIterator>
    void array_destructor(InputIterator first, InputIterator last)
    {
        while (first != last)
        {
            igris::destructor(&*first);
            ++first;
        }
    }

    template <class InputIterator, typename... Args>
    void array_constructor(InputIterator first, InputIterator last,
                           Args... args)
    {
        while (first != last)
        {
            igris::constructor(&*first, args...);
            ++first;
        }
    }

    /*	template <typename S>
        struct sint_ctrcollect
        {
            S s;

            constexpr sint_ctrcollect(int8_t o) : s(o) {}
            constexpr sint_ctrcollect(int16_t o) : s(o) {}
            constexpr sint_ctrcollect(int32_t o) : s(o) {}
            constexpr sint_ctrcollect(int64_t o) : s(o) {}

            constexpr operator S() const { return s; }
        };

        template <typename S>
        struct bool_ctrcollect
        {
            S s;

            constexpr bool_ctrcollect(const bool& o) : s(o) {}

            constexpr operator S() const { return s; }
        };

        template <typename S>
        struct numer_ctrcollect
        {
            S s;

            constexpr numer_ctrcollect(bool) = delete;
            constexpr numer_ctrcollect(bool&) = delete;
            constexpr numer_ctrcollect(const bool&) = delete;

            constexpr numer_ctrcollect(const int8_t& o) : s(o) {}
            constexpr numer_ctrcollect(const int16_t& o) : s(o) {}
            constexpr numer_ctrcollect(const int32_t& o) : s(o) {}
            constexpr numer_ctrcollect(const int64_t& o) : s(o) {}

            constexpr numer_ctrcollect(const uint8_t& o) : s(o) {}
            constexpr numer_ctrcollect(const uint16_t& o) : s(o) {}
            constexpr numer_ctrcollect(const uint32_t& o) : s(o) {}
            constexpr numer_ctrcollect(const uint64_t& o) : s(o) {}

            constexpr numer_ctrcollect(const float& o) : s(o) {}
            constexpr numer_ctrcollect(const double& o) : s(o) {}
            constexpr numer_ctrcollect(const long double& o) : s(o) {}

            constexpr operator S() const { return s; }
        };
    */
} // namespace igris

#endif
