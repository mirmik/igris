#ifndef IGRIS_STATUS_TREE_H
#define IGRIS_STATUS_TREE_H

namespace igris 
{
	template <class Status, class StatusCollector>
	class status_tree_node 
	{
		std::vector<status_tree_node*> supervisors; 
		std::vector<status_tree_node*> depends;

		Status status;
		StatusCollector collected;
		igris::delegate<void, const Status&> update_status_handle;

	public:
		void _update_status() 
		{

		}

	public:
		status_tree_node() : status() {}
		status_tree_node(Status&& sts) : status(std::move(sts)) {}
		status_tree_node(const Status& sts) : status(sts) {}

		void update_status(Status sts) 
		{
			if (sts != status) 
			{
				status = std::move(sts);
				_update_status();
			}
		}
	};
}

#endif