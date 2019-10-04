#ifndef GXX_SERIALIZE_DATABLOCK_H
#define GXX_SERIALIZE_DATABLOCK_H

#include <gxx/print.h>

namespace gxx {
	template<typename OutputIterator>
	void send_objblock(gxx::io::ostream& o, OutputIterator it, OutputIterator eit) {
		size_t len = std::distance(it, eit) * sizeof(*it);

		o.bwrite((uint64_t)len);
		for (;it != eit; ++it) {
			o.bwrite(*it);
		}
	}

	//template<typename InputIterator>
	//void recv_objblock(gxx::io::istream& i, InputIterator it, InputIterator eit) {
	//	size_t maxlen = std::distance(it,eit);


	//}
}

#endif