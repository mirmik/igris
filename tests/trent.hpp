#include <igris/trent/json.h>

LT_BEGIN_TEST(igris_test_suite, json_test)
{
	igris::trent tr;

	tr = igris::json::parse("13");
	LT_CHECK_EQ(tr.as_numer(), 13);

	tr = igris::json::parse("/*13*/ 42");
	LT_CHECK_EQ(tr.as_numer(), 42);

	tr = igris::json::parse(R"(

		[42,

	/*aaa*/ 

	13]

	)");
	LT_CHECK(tr.is_list());
}
LT_END_TEST(json_test)


LT_BEGIN_TEST(igris_test_suite, get_test)
{
	igris::trent tr;
	tr["a"]["b"] = 3;

	int a;

	a = tr.get_as_numer_except("a/b");
	LT_CHECK_EQ(a, 3);

	tr["a"]["b"][28] = 42;
	a = tr.get_as_numer_except("a/b/28");
	LT_CHECK_EQ(a, 42);	

	int except = 0;
	try 
	{
		tr.get_as_numer_except("a/c/28");	
	}
	catch (const std::exception& ex) 
	{
		except = 1;
		LT_CHECK_EQ(ex.what(), std::string(""));
	}

	LT_CHECK_EQ(except, 1);
}
LT_END_TEST(get_test)
