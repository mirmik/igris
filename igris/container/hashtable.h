#ifndef GXX_HASHTABLE_H
#define GXX_HASHTABLE_H

#include <gxx/datastruct/hlist_head.h>
#include <gxx/container/unbounded_array.h>

#include <gxx/util/hash.h>
#include <gxx/util/field.h>

namespace gxx {
	template<
		typename K, 
		typename T, 
		hlist_node T::* Hook, 
		K&(*GetKey)(T&) = gxx::util::field<K,T,&T::key>,
		typename C = gxx::unbounded_array<hlist_head>
	>
	class hashtable {

	};
}

#endif