#include <igris/trent/json.h>

LT_BEGIN_TEST(igris_test_suite, json_test)
{
	igris::trent tr;

	tr = igris::json::parse("13");
	LT_CHECK_EQ(tr.as_numer(), 13);

	tr = igris::json::parse("0.5");
	LT_CHECK_EQ(tr.as_numer(), 0.5);

	tr = igris::json::parse("-0.5");
	LT_CHECK_EQ(tr.as_numer(), -0.5);

	tr = igris::json::parse("/*13*/ 42");
	LT_CHECK_EQ(tr.as_numer(), 42);

	tr = igris::json::parse("{'a':42, /*aaa*/ 'b' : 13}");
	LT_CHECK(tr.is_dict());


	tr = igris::json::parse(R"(
		[42, //aaa 
		13]
	)");
	LT_CHECK(tr.is_list());

	tr = igris::json::parse(R"(

		[42,

	/*aaa*/ 

	13]

	)");
	LT_CHECK(tr.is_list());
}
LT_END_TEST(json_test)

LT_BEGIN_TEST(igris_test_suite, trent_basic_test)
{
	igris::trent tr;
	
	tr["a"]["b"][28] = 42;
	int a = tr["a"]["b"][28].as_numer();
	LT_CHECK_EQ(a, 42);
}
LT_END_TEST(trent_basic_test)

LT_BEGIN_TEST(igris_test_suite, trent_path)
{
	igris::trent tr;
	int a;

	tr["a"]["b"][28] = 42;
	a = tr[igris::trent_path("a/b/28")].as_numer();
	LT_CHECK_EQ(a, 42);

	tr[28]["a"]["b"] = 42;
	a = tr[igris::trent_path("28/a/b")].as_numer();
	LT_CHECK_EQ(a, 42);

	tr[7] = 42;
	a = tr[igris::trent_path("7")].as_numer();
	LT_CHECK_EQ(a, 42);
}
LT_END_TEST(trent_path)

LT_BEGIN_TEST(igris_test_suite, get_test)
{
	igris::trent tr;
	int a;

	tr["a"]["b"] = 3;
	LT_CHECK_EQ(tr.get("a/b"), &tr["a"]["b"]);

	tr[7]["a"]["b"] = 3;
	LT_CHECK_EQ(tr.get("7/a/b"), &tr[7]["a"]["b"]);

	tr["a"]["b"] = 3;
	LT_CHECK_EQ(tr.get_as_numer_ex("a/b"), 3);

	tr["A"][7] = 8;
	LT_CHECK_EQ(tr.get_as_numer_ex("A/7"), 8);
	
	tr[7] = 8;
	LT_CHECK_EQ(tr.get_as_numer_ex("7"), 8);
	
	tr[7]["A"] = 8;
	LT_CHECK_EQ(tr.get_as_numer_ex("7/A"), 8);
	
	tr["a"]["b"][28] = 42;
	LT_CHECK_EQ(tr.get_as_numer_ex("a/b/28"), 42);

	int except = 0;
	LT_CHECK_THROW(tr.get_as_numer_ex("a/c/28"));

	try 
	{
		tr.get_as_numer_ex("a/c/28");
	}	
	catch(std::exception& ex) 
	{
		std::string what = ex.what();
		LT_CHECK_EQ(what, std::string("trent:wrong_path: a/c/28"));
	}

	tr["a"]["b"][28] = "hello";
	try 
	{
		tr.get_as_numer_ex("a/b/28");
	}	
	catch(std::exception& ex) 
	{
		std::string what = ex.what();
		LT_CHECK_EQ(what, std::string("trent:wrong_type: path:a/b/28 request:num realtype:str"));
	}
}
LT_END_TEST(get_test)
