/// @file

#ifndef IGRIS_RING_H
#define IGRIS_RING_H

#include <igris/datastruct/ring.h>
#include <new>
#include <utility>

namespace igris
{
    /**
     * Обёртка над api ring_head, предназначенная для управления буфером
     * указанного типа.
     * С целью оптимизации разрешает доступ к внутренним переменным и прямое
     * управление буффером.
     * */
    template <typename T, int N = 0> class ring : public ring<T, 0>
    {
        T data[N];

    public:
        ring() : ring<T, 0>(data, N) {}
    };

    template <typename T> class ring<T, 0>
    {
    public:
        ring_head r;
        T *buffer;

    public:
        ring(T *buffer, int bufsize) : buffer(buffer)
        {
            ring_init(&r, bufsize);
        }

        bool empty() { return ring_empty(&r); }

        void push(const T &obj)
        {
            new (buffer + r.head) T(obj);
            ring_move_head_one(&r);
        }

        template <typename... Args> void emplace(Args &&... args)
        {
            new (buffer + r.head) T(std::forward<Args>(args)...);
            ring_move_head_one(&r);
        }

        void clear()
        {
            while (!empty())
            {
                pop();
            }
        }

        void pop()
        {
            int idx = r.tail;
            buffer[idx].~T();
            ring_move_tail_one(&r);
        }

        void move_tail_one() { ring_move_tail_one(&r); }

        void move_head_one() { ring_move_head_one(&r); }

        unsigned int avail() { return ring_avail(&r); }

        unsigned int room() { return ring_room(&r); }

        unsigned int size() { return r.size; }

        T &get(int index) { return buffer[index]; }

        T &tail() { return buffer[r.tail]; }

        int index_of(T *element) { return element - buffer; }

        int tail_index() { return r.tail; }

        int head_index() { return r.head; }

        int fixup_index(int index) { return ring_fixup_index(&r, index); }

        T &head_place() { return buffer[r.head]; }

        int distance(int a, int b) { return (a - b + r.size) % r.size; }
    };
}

#endif
