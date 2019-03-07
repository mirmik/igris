#ifndef GXX_RING_H
#define GXX_RING_H

#include <memory>
#include <gxx/panic.h>

//Implementation of abstract ring buffer

namespace gxx {
	template <typename T, typename Alloc = std::allocator<T>>
	class ring {
		int head;
		int tail;
		bool isfull;

		T* buffer;
		size_t reserved;

		Alloc alloc;

	public:
		ring() : head(0), tail(0), isfull(false), buffer(nullptr), reserved(0) {}
		ring(int size) : ring() { reserve(size); }

		void init() {
			head = 0;
			tail = 0;
			isfull = false;
		}

		bool empty() {
			return head == tail && !isfull;
		}

		void reserve(size_t sz) {
			T* newbuffer = alloc.allocate(sz);
			
			if (buffer == nullptr) {
				buffer = newbuffer;
				reserved = sz;
				head = 0;
				tail = 0;
				isfull = false;
			}
			else {
				gxx::panic("NeedToImplement");
			}
		}
		
		T& front() {
			return *(buffer + tail);
		}

		T& back() {
			return *(buffer + (head - 1) % reserved);
		}

		void push(const T& obj) {
			if (isfull) __pop();
			alloc.construct(buffer + head++, obj);
			if (head == reserved) head = 0;
			if (head == tail) isfull = true; 
		}

		template<typename ... Args>
		void emplace(Args&& ... args) {
			if (isfull) return;
			alloc.construct(buffer + head++, std::forward<Args>(args) ... );
			if (head == reserved) head = 0;
			if (head == tail) isfull = true; 
		}

		void __pop() {
			alloc.destroy(buffer + tail);
			if (++tail == reserved) tail = 0;	
		}

		void pop() {
			if (!empty()) {
				__pop();
				isfull = false;
			}
		}

		size_t size() {
			if (isfull) return reserved;
 			int res = head - tail;
			return res < 0 ? res + reserved : res;
		}

		size_t capacity() {
			return reserved;
		}

		T& operator[](int index) {
			size_t sz = size();
			int cell;
			cell = index >= 0 ? (tail + index) % reserved : (head + index + reserved) % reserved;
			return *(buffer + cell);
		}
	};
}

#endif