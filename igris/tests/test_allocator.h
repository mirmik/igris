#ifndef IGRIS_TEST_ALLOCATOR_H
#define IGRIS_TEST_ALLOCATOR_H

#include <igris/dprint.h>
#include <memory>

namespace igris
{
    template <class T> class test_allocator : public std::allocator<T>
    {
      public:
        int allocations = 0;
        int deallocations = 0;
        int current = 0;

        bool debug_print = false;
        const char *allocator_name = "undefined";

        T *allocate(size_t size)
        {
            allocations++;
            current++;

            T *ptr = std::allocator<T>::allocate(size);

            if (debug_print)
            {
                dprln("alloc:", allocator_name, ":", size, ":");
                dprhexln((size_t)ptr);
            }

            return ptr;
        }

        void deallocate(T *ptr, size_t sz)
        {
            deallocations++;
            current--;

            std::allocator<T>::deallocate(ptr, sz);

            if (debug_print)
            {
                dpr("dealloc:", allocator_name, ":", sz, ":");
                dprhexln((size_t)ptr);
            }
        }
    };
} // namespace igris

#endif
