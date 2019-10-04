#ifndef BINCALL_TABLE_H
#define BINCALL_TABLE_H

#include <gxx/rpc/bincall.h>

namespace gxx {
	namespace rpc {
		template <typename Key, size_t Cap>
		class bincalltable {
			using node = std::pair<Key, gxx::rpc::abstract_bincall>;

			char _buf[sizeof(node) * Cap];
			node* data = (node*)_buf;

			size_t size = 0;
		public:

			template<typename Ret, typename ... Args>
			bool add(const Key& key, gxx::delegate<Ret, Args ...> dlg) {
				if (size == Cap) gxx::panic("bincalltable");
				node* ptr = data + size++;

				new (reinterpret_cast<bincall<gxx::delegate<Ret, Args ...>>*>(&ptr->second)) 
					bincall<gxx::delegate<Ret, Args ...>>(dlg);
				new (&ptr->first) Key(key);

				return true;
			}

			bincaller_basic* find(Key key) {
				node* eit = data + size;
				for(node* it = data; it != eit; ++it) {
					dprln("it:", it->first);
					if (key == it->first) return &it->second;
				}
				return nullptr;
			}
		};
	}
}

#endif