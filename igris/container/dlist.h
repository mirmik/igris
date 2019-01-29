#ifndef IGRIS_DLIST_H
#define IGRIS_DLIST_H

#include <igris/datastruct/dlist.h>
#include <igris/util/member.h>
#include <igris/util/memberxx.h>

namespace igris {

	template<typename type, dlist_head type::* member>
	class dlist {
	public:
		//FIELDS:
		dlist_head list;

		//SUBCLASSES: 
		class iterator {
		public:
			//using iterator_category = std::bidirectional_iterator_tag;
			using value_type = type;
			using difference_type = ptrdiff_t;
			using pointer = type*;
			using reference = type&;

		public:
			dlist_head* current;
		public:
			iterator() : current(nullptr) {};	
			iterator(dlist_head* head) : current(head) {};	
			iterator(const iterator& other) : current(other.current) {};		
					
			iterator operator++(int) { iterator i = *this; current=current->next; return i; }
			iterator operator++() { current=current->next; return *this; }
			iterator operator--(int) { iterator i = *this; current=current->prev; return i; }
			iterator operator--() { current=current->prev; return *this; }
			bool operator!= (const iterator& b) {return current != b.current;}
			bool operator== (const iterator& b) {return current == b.current;}
					
			type& operator*() {return *member_container(current,member);}
			type* operator->() {return member_container(current,member);}
		};
	
		class reverse_iterator {
		private:
			dlist_head* current;
		public:
			reverse_iterator(dlist_head* head) : current(head) {}	
					
			reverse_iterator operator++(int) { reverse_iterator i = *this; current=current->prev; return i; }
			reverse_iterator operator++() { current=current->prev; return *this; }
			reverse_iterator operator--(int) { reverse_iterator i = *this; current=current->next; return i; }
			reverse_iterator operator--() { current=current->next; return *this; }
			bool operator!= (const reverse_iterator& b) {return current != b.current;}
			bool operator== (const reverse_iterator& b) {return current == b.current;}
					
			type& operator*() {return *member_container(current,member);}
			type* operator->() {return member_container(current,member);}
		};
	
		//METHODS:
		dlist()	{
			dlist_init(&list);
		}
	
		~dlist() {
			dlist_del(&list);
		}
	
		bool empty() {
			return dlist_empty(&list);
		};

		type* first() {
			return member_container(list.next, member);
		}
		
		//Прилинковать первым в списке.
		void move_front(type& obj) {
			dlist_move_next(&(obj.*member), &list);
		};
		
		//Прилинковать последним в списке.
		void move_back(type& obj) {
			dlist_move_prev(&(obj.*member), &list);
		};

		void add_first(type& obj) {
			dlist_add(&(obj.*member), &list);			
		}

		void add_last(type& obj) {
			dlist_add_tail(&(obj.*member), &list);			
		}

		//Поставить объект перед объектом.		
		void move_next(type& obj, type& head) {
			dlist_move_next(&(obj.*member), &(head.*member));
		};

		//Поставить объект перед итерируемым объектом.		
		void move_next(type& obj, iterator head) {
			dlist_move_next(&(obj.*member), head.current);
		};

		//Поставить объект после объекта.		
		void move_prev(type& obj, type& head) {
			dlist_move_prev(&(obj.*member), &(head.*member));
		};

		//Поставить объект после итерируемого объектом.
		void move_prev(type& obj, iterator head) {
			dlist_move_prev(&(obj.*member), head.current);
		};


//		iterator insert(iterator it, type & obj) {
//			dlist_move_prev(&(obj.*member), it.current);			
//		}

		void pop(type& obj) {
			//assert(is_linked(obj));
			dlist_del(&(obj.*member));
		};

		static void unbind(type& obj) {
			dlist_del(&(obj.*member));
		};

		void del_init(type& obj) {
			dlist_del_init(&(obj.*member));
		};
	
		void pop_if_linked(type& obj) {
			if (!is_linked(obj)) return;
			dlist_del(&(obj.*member));
		};
	
		void pop_front() {
			dlist_del(&((*begin()).*member));
		};
	
		void pop_back() {
			dlist_del(&((*rbegin()).*member));
		};
	
		void round_left() {
			move_back(*begin());
		};

		int size() const {
			int i = 0;
			for(auto& v : *this) {
				i++;
			}
			return i;
		};

		
	
		iterator begin() {return iterator(list.next);}
		iterator end() {return iterator(&list);}
		
		iterator begin() const {return iterator((dlist_head*)list.next);}
		iterator end() const {return iterator((dlist_head*)&list);}

		reverse_iterator rbegin() {return reverse_iterator(list.prev);}
		reverse_iterator rend() {return reverse_iterator(&list);}
	
		//iterator insert_sorted(type & item) {
		//    return insert(std::upper_bound(begin(), end(), item ), item);
		//}

	/*	gxx::string to_info() const
		{
			gxx::string str;
			str.reserve(128);
			int i = 0;
			dlist_head* ptr;
			dlist_for_each(ptr, &list)
			{
				i++;
			};
			str << "count: " << i;
			return str;
		};
	*/
	/*	gxx::string to_str() const
		{
			gxx::string str;
			str.reserve(128);
			str << "[";
			for (auto r : *this)
			{
				str << to_str(r) << gxx::string(",");
			};
			str << "]";
			return str;
		};
	*/
	};
}

/*namespace std {
	template<typename T, dlist_head T::* L>
	class iterator_traits<typename gxx::dlist<T,L>::iterator> {
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
	};
}*/

#endif