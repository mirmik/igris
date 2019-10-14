#include <gtest/gtest.h>
#include <igris/serialize/serialize.h>
#include <nos/print.h>

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

TEST(serialize, type_int32_t) 
{
	int32_t a = 89;
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	EXPECT_EQ(r, a);
	EXPECT_EQ(s.size(), sizeof(a));
}

TEST(serialize, type_float) 
{
	float a = 0.15;
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	EXPECT_EQ(r, a);
	EXPECT_EQ(s.size(), sizeof(a));
}

TEST(serialize, object) 
{
	A a;
	A b;

	auto s = igris::serialize(a);
	auto r = igris::deserialize<A>(s);

	EXPECT_EQ(s.size(), sizeof(A::a) + sizeof(A::b) + sizeof(A::c));
	EXPECT_EQ(r, b);
}

TEST(serialize, vector) 
{
	std::vector<int32_t> a = { 33, 44, 55 };
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<std::vector<int32_t>>(s);

	EXPECT_EQ(r, a);
	EXPECT_EQ(s.size(), sizeof(typename decltype(a)::value_type) * a.size() + sizeof(uint16_t));
}

TEST(serialize, map) 
{
	std::map<std::string, int32_t> a = { {"A", 33}, {"B", 44}, {"C",55} };
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<decltype(a)>(s);

	EXPECT_EQ(r, a);
}

TEST(serialize, string) 
{
	std::string a = "hello world";
	
	auto s = igris::serialize(a);
	auto r = igris::deserialize<std::string>(s);

	EXPECT_EQ(r, a);
}