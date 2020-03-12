#include <igris/serialize/serialize.h>

struct A 
{
	int a = 34;
	uint8_t b = 83;
	int16_t c = 17;

	template <class R>
	void reflect(R& r) 
	{
		r & a;
		r & b;
		r & c;
	}

	bool operator == (const A& oth) const 
	{
		return a == oth.a && b == oth.b && c == oth.c;
	}
};

LT_BEGIN_TEST(igris_test_suite, serialize_type_int32_t)
{
	int32_t a = 89;
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	LT_CHECK_EQ(r, a);
	LT_CHECK_EQ(s.size(), sizeof(a));
}
LT_END_TEST(serialize_type_int32_t)

LT_BEGIN_TEST(igris_test_suite, serialize_type_float)
{
	float a = 0.15;
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	LT_CHECK_EQ(r, a);
	LT_CHECK_EQ(s.size(), sizeof(a));
}
LT_END_TEST(serialize_type_float)

LT_BEGIN_TEST(igris_test_suite, serialize_object)
{
	A a;
	A b;

	auto s = igris::serialize(a);
	auto r = igris::deserialize<A>(s);

	LT_CHECK_EQ(s.size(), sizeof(A::a) + sizeof(A::b) + sizeof(A::c));
	LT_CHECK(r == b);
}
LT_END_TEST(serialize_object)

LT_BEGIN_TEST(igris_test_suite, serialize_vector)
{
	std::vector<int32_t> a = { 33, 44, 55 };
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<std::vector<int32_t>>(s);

	LT_CHECK(r == a);
	LT_CHECK_EQ(s.size(), sizeof(typename decltype(a)::value_type) * a.size() + sizeof(uint16_t));
}
LT_END_TEST(serialize_vector)

LT_BEGIN_TEST(igris_test_suite, serialize_map)
{
	std::map<std::string, int32_t> a = { {"A", 33}, {"B", 44}, {"C",55} };
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	LT_CHECK(r == a);
}
LT_END_TEST(serialize_map)

LT_BEGIN_TEST(igris_test_suite, serialize_string)
{
	std::string a = "hello world";
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<std::string>(s);

	LT_CHECK(r == a);
}
LT_END_TEST(serialize_string)