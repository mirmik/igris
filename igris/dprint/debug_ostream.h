#ifndef GXX_DEBUG_DEBUG_OSTREAM_H
#define GXX_DEBUG_DEBUG_OSTREAM_H

#include <gxx/io/ostream.h>
#include <gxx/util/setget.h>

namespace gxx {
	class debug_ostream : public gxx::io::ostream {
		bool _hexmode = false;
	public:
		ACCESSOR(hexmode, _hexmode);
		
	protected:
		ssize_t writeData(const char *data, size_t maxSize) override {
			if (_hexmode) {
				while(maxSize--) {
					debug_printhex_uint8(*data++);
				}
			} else {
				debug_write(data, maxSize);
				return maxSize; 
			}
			return maxSize;
		}
	};
}

#endif