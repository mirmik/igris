#ifndef IGRIS_RING_H
#define IGRIS_RING_H

#include <igris/datastruct/ring.h>
#include <new>
#include <utility>

namespace igris
{
    template <typename T> class ring
    {
      public:
        ring_head r;
        T *buffer;

      public:
        ring(T *buffer, int size) : buffer(buffer) { ring_init(&r, size); }

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

        void pop()
        {
            int idx = r.tail;
            buffer[idx]->~T();
            ring_move_tail_one(&r);
        }

        void move_tail_one() { ring_move_tail_one(&r); }

        void move_head_one() { ring_move_tail_one(&r); }

        unsigned int avail() { return ring_avail(&r); }

        unsigned int room() { return ring_room(&r); }

        T &get(int index) { return buffer[index]; }

        T &last() { return buffer[r.tail]; }
    };
}

#endif
