#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <queue>
#include <igris/sync/semaphore.h>

namespace igris {

    template <class T>
    class safe_queue 
    {
        std::queue<T> queue = {};
        igris::semaphore sem = {};


        public:
        safe_queue() {}
        safe_queue(const std::initializer_list<T>& lst) : queue(lst) {}

        safe_queue(const safe_queue&) = delete;
        safe_queue(safe_queue&&) = delete;

        void push(const T& val) 
        {
            sem.wait();
            queue.push(val);
            sem.post();
        }

        T pop() 
        {
            sem.wait();
            T val = queue.front();
            queue.pop();
            sem.post();
            return val;
        }
    };

}

#endif