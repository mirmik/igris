#include <igris/trent/json.h>

LT_BEGIN_TEST(igris_test_suite, json_test)
{
	igris::trent tr;

	tr = igris::json::parse("13");
	LT_CHECK_EQ(tr.as_numer(), 13);

	tr = igris::json::parse("/*13*/ 42");
	LT_CHECK_EQ(tr.as_numer(), 42);

	tr = igris::json::parse(R"([42,

	/*aaa*/ 

	13])");
	LT_CHECK(tr.is_list());
}
LT_END_TEST(json_test)
