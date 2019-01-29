#ifndef GXX_EVENT_H
#define GXX_EVENT_H

#include <gxx/dlist.h>
#include <gxx/pool.h>

namespace gxx {
	namespace event {
		constexpr uint8_t SelfDestruct = 0x01;
		constexpr uint8_t Nothing = 0x01;
		constexpr uint8_t Unbind = 0x01;

		class event_processor {

		};

		template <typename ... Args>
		class node {
		public:
			bool en;
			gxx::delegate<void, node*, Args ...>
		};

		class event {
		public:
			dlist<node, &node::lnk> list;
		};

		class flag : public event {
		public:
		};

		extern gxx::pool<node> node_pool;

		node* get_event_node();
		void  put_event_node(node*);
	}
}

#endif