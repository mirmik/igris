#ifndef GXX_TRENT_CHECKER_H
#define GXX_TRENT_CHECKER_H

#include <set>
#include <gxx/util/string.h>
#include <gxx/trent/trent.h>
#include <gxx/result.h>

using namespace gxx::result_type;

namespace gxx
{
	class schema
	{
	public:
		enum checker_type
		{
			any_checker_type,
			dict_checker_type,
			list_checker_type,
			string_checker_type,
			numer_checker_type,
			bool_checker_type,
			numer_or_string_checker_type,
		};

		class schema_node
		{
			checker_type type;

		public:
			schema_node& merge(const schema_node& oth)
			{
				if (type != dict_checker_type || oth.type != dict_checker_type) gxx::panic("SCHEMA: no dictionary merge");
				nodes.insert(oth.nodes.begin(), oth.nodes.end());
				return *this;
			}

			schema_node& merge(const schema& oth)
			{
				merge(oth.root);
				return *this;
			}

			schema_node(checker_type type) : type(type) {}

			schema_node(const schema_node& oth) :
				type(oth.type),
				nodes(oth.nodes),
				len(oth.len),
				_inset(oth._inset),
				_content(oth._content ? new schema_node(*oth._content) : nullptr),
				_allopts(oth._allopts),
				check_dict(oth.check_dict),
				_ifexist(oth._ifexist),
				_optional(oth._optional)
			{}

			schema_node(schema_node&& oth) :
				type(oth.type),
				nodes(std::move(oth.nodes)),
				len(oth.len),
				_inset(oth._inset),
				_content(oth._content),
				_allopts(oth._allopts),
				check_dict(oth.check_dict),
				_ifexist(oth._ifexist),
				_optional(oth._optional)
			{
				oth._content = nullptr;
			}

			result<void> check(const trent& tr, gxx::strvec& strvec) const
			{
				switch (type)
				{
					case any_checker_type:
						break;
					case dict_checker_type:
						if (!tr.is_dict()) return error("should be dictionary");
						break;
					case list_checker_type:
						if (!tr.is_list()) return error(gxx::format("trent {} should be list", strvec[strvec.size() - 1]));
						if (len != -1 && tr.as_list().size() != (unsigned int)len) return error(gxx::format("array size should be {}", len));
						break;
					case string_checker_type:
						if (!tr.is_string()) return error("should be string");
						break;
					case bool_checker_type:
						if (!tr.is_bool()) return error("should be boolean");
						break;
					case numer_checker_type:
						if (!tr.is_numer()) return error("should be numer");
						break;

					case numer_or_string_checker_type:
						if (!tr.is_numer() && !tr.is_string()) return error("should be numer or string");
						break;
					default:
						PANIC_TRACED();
				}

				if (type == any_checker_type)
				{

				}

				if (type == dict_checker_type)
				{
					//Проверка на принадлежность имён определённому множеству.
					if (_inset)
					{
						for (auto& tp : tr.unsafe_dict_const())
						{
							bool flag = false;
							for (auto& str : *_inset)
							{
								if (str == tp.first) { flag = true; break; }
							}
							if (!flag) return error(gxx::format("subopt {} not in validation set", tp.first));
						}
					}

					//Проверка на соответствие всех нодов шаблону.
					if (_content)
					{
						for (auto& tp : tr.unsafe_dict_const())
						{
							strvec.push_back(tp.first);

							auto&& r = _content->check(tp.second, strvec);
							if (r.is_error())
								return std::move(r.error());

							strvec.pop_back();
						}
					}
				}

				//Индивидуальная проверка словаря.
				if (type == dict_checker_type && check_dict)
				{
					assert(nodes.size() != 0);


					for (const auto& n : nodes)
					{
						strvec.push_back(n.first);
						if (!tr.have(n.first))
						{
							//gxx::println("check_dict doesn't have", strvec);
							if (n.second._optional == false)
							{
								if (!_ifexist) return error(gxx::format("subopt {} isn't exist", n.first));
							}
							else
							{
								//gxx::fprintln("don't have optional node {}", strvec);
							}
						}
						else
						{
							auto&& r = n.second.check(tr[n.first], strvec);
							if (r.is_error())
								return std::move(r.error());
						}
						strvec.pop_back();
					}

					if (_allopts)
					{
						for (auto& tp : tr.unsafe_dict_const())
						{
							if (nodes.count(tp.first) == 0)  return error(gxx::format("unresolved options {}", tp.first));
						}
					}
				}

				if (type == string_checker_type && _inset)
				{
					bool flag = false;
					for (auto& str : *_inset)
					{
						if (str == tr.unsafe_string_const()) { flag = true; break; }
					}
					if (!flag) return error(gxx::format("{} not in validation set", tr.unsafe_string_const()));
				}

				if (type == list_checker_type && _content)
				{
					for (unsigned int i = 0; i < tr.as_list().size(); ++i)
					{
						strvec.push_back(std::to_string(i));

						auto&& r = _content->check(tr.unsafe_list_const()[i], strvec);
						if (r.is_error())
							return std::move(r.error());

						strvec.pop_back();
					}
				}

				return result<void>();
			}

			schema_node& length(int n)
			{
				len = n;
				return *this;
			}

			schema_node& inset(const std::set<std::string>& s)
			{
				_inset = &s;
				return *this;
			}

			schema_node& content(const schema_node& s)
			{
				_content = new schema_node(s);
				return *this;
			}

			schema_node& allopts(bool en)
			{
				_allopts = en;
				return *this;
			}


			schema_node& ifexist(bool en)
			{
				_ifexist = en;
				return *this;
			}

			~schema_node() { if (_content) delete(_content); }

			std::map<std::string, schema_node> nodes;
			int len = -1;
			const std::set<std::string>* _inset = nullptr;
			const schema_node* _content = nullptr;
			bool _allopts = true;
			bool check_dict = true;
			bool _ifexist = false;
			bool _optional = false;
			schema_node& operator[](std::string str) { return nodes.at(str); }


			FLOW_ACCESSOR(optional, _optional);
		};

		struct schema_dict_pair
		{
			std::string str;
			schema_node node;
		};

		struct dict : public schema_node
		{
			dict(const std::initializer_list<schema_dict_pair>& lst) : schema_node(dict_checker_type)
			{
				for (const auto& l : lst)
				{
					nodes.insert(std::make_pair(l.str, l.node));
				}
				check_dict = true;
			}

			dict() : schema_node(dict_checker_type)
			{
				check_dict = false;
			}
		};

		struct any : public schema_node
		{
			any() : schema_node(any_checker_type) {}
		};

		struct list : public schema_node
		{
			list() : schema_node(list_checker_type) {}
		};

		struct string : public schema_node
		{
			string() : schema_node(string_checker_type) {}
		};

		struct numer : public schema_node
		{
			numer() : schema_node(numer_checker_type) {}
		};

		struct boolean : public schema_node
		{
			boolean() : schema_node(bool_checker_type) {}
		};

		struct integer : public schema_node
		{
			integer() : schema_node(numer_checker_type) {}
		};

		struct numer_or_string : public schema_node
		{
			numer_or_string() : schema_node(numer_or_string_checker_type) {}
		};

		void asserted_check(const trent& tr, std::string rootname)
		{
			std::vector<std::string> strvec { rootname };
			auto ret = root.check(tr, strvec);
			if (ret.is_error())
			{
				std::string path = gxx::join(strvec, '/');
				std::string errtxt = gxx::format("SCHEMA: trent {} {}", path, ret.error().what());
				gxx::panic(errtxt.c_str());
			}
		}

		result<void> check(const trent& tr, std::string rootname)
		{
			std::vector<std::string> strvec { rootname };
			return root.check(tr, strvec);
		}

		schema_node root;
		schema(const schema_node& root) : root(root) {}

		void merge(const schema& oth)
		{
			root.merge(oth.root);
		}
	};
}

#endif
