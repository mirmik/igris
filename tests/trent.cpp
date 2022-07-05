#include <doctest/doctest.h>
#include <igris/trent/json.h>
#include <igris/trent/json_print.h>
#include <cmath>

TEST_CASE("json_test.to_string")
{
	igris::trent tr;
	tr = 13;
	igris::json::to_string(tr);
}

TEST_CASE("json_test")
{
	igris::trent tr;

	tr = igris::json::parse("13");
	CHECK_EQ(tr.as_numer(), 13);

	tr = igris::json::parse("0.5");
	CHECK_EQ(tr.as_numer(), 0.5);

	tr = igris::json::parse("-0.5");
	CHECK_EQ(tr.as_numer(), -0.5);

	tr = igris::json::parse("/*13*/ 42");
	CHECK_EQ(tr.as_numer(), 42);

	tr = igris::json::parse("{'a':42, /*aaa*/ 'b' : 13}");
	CHECK(tr.is_dict());

	tr = igris::json::parse("{'a':42, /*aaa*/ 'b' : 13 }");
	CHECK(tr.is_dict());

	tr = igris::json::parse(R"(
		[42, //aaa 
		13]
	)");
	CHECK(tr.is_list());

	tr = igris::json::parse(R"(

		[42,

	/*aaa*/ 

	13]

	)");
	CHECK(tr.is_list());
}

TEST_CASE("trent_basic_test")
{
	igris::trent tr;
	
	tr["a"]["b"][28] = 42;
	int a = tr["a"]["b"][28].as_numer();
	CHECK_EQ(a, 42);
}

TEST_CASE("trent_basic_test")
{
	igris::trent tr;
	
	tr = igris::json::parse(" 20e-1 ");
	CHECK_EQ(tr.as_numer(), 2.0);
}

TEST_CASE("trent_path")
{
	igris::trent tr;
	int a;

	tr["a"]["b"][28] = 42;
	a = tr[igris::trent_path("a/b/28")].as_numer();
	CHECK_EQ(a, 42);

	tr[28]["a"]["b"] = 42;
	a = tr[igris::trent_path("28/a/b")].as_numer();
	CHECK_EQ(a, 42);

	tr[7] = 42;
	a = tr[igris::trent_path("7")].as_numer();
	CHECK_EQ(a, 42);
}

TEST_CASE("trent_bool")
{
	igris::trent tr;

	tr["a"][28] = true;
	bool a = tr[igris::trent_path("a/28")].as_bool();
	CHECK_EQ(a, true);

	tr["a"][29] = false;
	a = tr[igris::trent_path("a/29")].as_bool();
	CHECK_EQ(a, false);

	igris::trent tr2;
	//tr2 = igris::json::parse(" false ");
	//CHECK(tr2.is_bool());
	//CHECK_EQ(tr2.as_bool(), false);

	tr2 = igris::json::parse(" {'a': false} ");
	CHECK(tr2["a"].is_bool());
	CHECK_EQ(tr2["a"].as_bool(), false);

	tr2 = igris::json::parse(" {'a': false } ");
	CHECK(tr2["a"].is_bool());
	CHECK_EQ(tr2["a"].as_bool(), false);
}

TEST_CASE("get_test")
{
	igris::trent tr;

	tr["a"]["b"] = 3;
	CHECK_EQ(tr.get("a/b"), &tr["a"]["b"]);

	tr[7]["a"]["b"] = 3;
	CHECK_EQ(tr.get("7/a/b"), &tr[7]["a"]["b"]);

	tr["a"]["b"] = 3;
	CHECK_EQ(tr.get_as_numer_ex("a/b"), 3);

	tr["A"][7] = 8;
	CHECK_EQ(tr.get_as_numer_ex("A/7"), 8);
	
	tr[7] = 8;
	CHECK_EQ(tr.get_as_numer_ex("7"), 8);
	
	tr[7]["A"] = 8;
	CHECK_EQ(tr.get_as_numer_ex("7/A"), 8);
	
	tr["a"]["b"][28] = 42;
	CHECK_EQ(tr.get_as_numer_ex("a/b/28"), 42);

	tr["a"]["b"][28] = -123.513;
	CHECK(std::fabs(tr.get_as_numer_ex("a/b/28") + 123.513) < 1e-5);

	CHECK_THROWS(tr.get_as_numer_ex("a/c/28"));

	tr = igris::json::parse(R"(
		{
			"name": "Az",
			"type": "simple",
			"motionType": "round",
			"servo": "srv_axis1",
			"unitRatio": 2056,
			"defaultSpeed_rpm": 202,
			"defaultAccel_ms": 10000,
			"maxSpeed_rpm": 1500,
			"minAccel_ms": 2000,
			"backwardLimit": -100,
			"forwardLimit": 100,
			"reverse": 0
		}
	)");
	CHECK(tr["backwardLimit"].is_numer());	
	CHECK_EQ(tr["backwardLimit"].as_numer(), -100);	

	try 
	{
		tr.get_as_numer_ex("a/c/28");
	}	
	catch(std::exception& ex) 
	{
		std::string what = ex.what();
		CHECK_EQ(what, std::string("trent:wrong_path: a/c/28"));
	}

	tr["a"]["b"][28] = "hello";
	try 
	{
		tr.get_as_numer_ex("a/b/28");
	}	
	catch(std::exception& ex) 
	{
		std::string what = ex.what();
		CHECK_EQ(what, std::string("trent:wrong_type: path:a/b/28 request:num realtype:str"));
	}
}
