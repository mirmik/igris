#include <gtest/gtest.h>
#include <igris/trent/trent.h>
#include <igris/trent/json.h>

TEST(trent_types, TRENT) 
{
	igris::trent a = 0;
	igris::trent b = "hello";
	igris::trent c;
	igris::trent d;

	c["mirmik"] = "hello";
	d[3] = 78;

	EXPECT_EQ(a.get_type(), igris::trent::type::numer);
	EXPECT_EQ(b.get_type(), igris::trent::type::string);
	EXPECT_EQ(c.get_type(), igris::trent::type::dict);
	EXPECT_EQ(d.get_type(), igris::trent::type::list);
}

TEST(trent_numer, TRENT) 
{
	igris::trent a = 42.78;

	EXPECT_EQ(a.as_numer(), 42.78);
	EXPECT_EQ(a.as_integer(), 42);
}

TEST(trent_array, TRENT) 
{
	igris::trent a;

	a[3] = 67;

	EXPECT_EQ(a.as_list().size(), 4);
}

TEST(trent_dictionary, TRENT) 
{
	igris::trent a;

	a["hello"] = "m";
	a["hello2"] = "b";

	EXPECT_EQ(a.as_dict().size(), 2);
}

TEST(trent_json_parse0, TRENT) 
{
	igris::trent a = igris::json::parser_cstr(" { 'akeke' : 'hello world' , 'aa' : {'ab' : 33 , 'v' : 'vvv', 'ac' : 44 } }").parse();

	EXPECT_EQ(a["akeke"].as_string_except(), "hello world");
	EXPECT_EQ(a["aa"]["ab"].as_integer_except(), 33);
	EXPECT_EQ(a["aa"]["v"].as_string_except(), "vvv");
	EXPECT_EQ(a["aa"]["ac"].as_integer_except(), 44);
}

TEST(trent_json_parse1, TRENT) 
{
	igris::trent b = igris::json::parser_cstr("{'bkeke':'helloworld','ba':{'bb':33,'v':'vvv','bc':44}}").parse();

	EXPECT_EQ(b["bkeke"].as_string_except(), "helloworld");
	EXPECT_EQ(b["ba"]["bb"].as_integer_except(), 33);
	EXPECT_EQ(b["ba"]["v"].as_string_except(), "vvv");
	EXPECT_EQ(b["ba"]["bc"].as_integer_except(), 44);
}

TEST(trent_json_parse2, TRENT) 
{
igris::trent b = igris::json::parser_cstr(R"(
{
	'bkeke' : 'helloworld',
	'ba' : {
		'bb':33,
		'v':'vvv',
		'bc':44
	},
	'la' : {
		'a':33,
	},
	'a' : 
	[1,3]

	,

	'l' :  true,

	'f':false,
}
)").parse();

	EXPECT_EQ(b["bkeke"].as_string_except(), "helloworld");
	EXPECT_EQ(b["ba"]["bb"].as_integer_except(), 33);
	EXPECT_EQ(b["ba"]["v"].as_string_except(), "vvv");
	EXPECT_EQ(b["ba"]["bc"].as_integer_except(), 44);
	EXPECT_EQ(b["la"]["a"].as_integer_except(), 33);
	EXPECT_EQ(b["a"][0].as_integer_except(), 1);
	EXPECT_EQ(b["a"][1].as_integer_except(), 3);
	EXPECT_EQ(b["l"].as_bool_except(), true);
	EXPECT_EQ(b["f"].as_bool_except(), false);
}
