#include <gtest/gtest.h>
#include <igris/datastruct/tree.h>

struct node 
{
	struct tree_head link;
	const char * name;
};

TEST(tree, TREE) 
{
	struct tree_head root = { { DLIST_HEAD_INIT(root.lnk), DLIST_HEAD_INIT(root.childs) } };
	struct tree_head child_0 = { TREE_HEAD_INIT(child_0), "child_0" };
	struct tree_head child_1 = { TREE_HEAD_INIT(child_1), "child_1" };

}
