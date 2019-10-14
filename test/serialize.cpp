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

TEST(serialize, SERIALIZE) 
{
	auto s = igris::serialize((int32_t)12);
	auto r = igris::deserialize<int32_t>(s);

	EXPECT_EQ(s.size(), 4);
	EXPECT_EQ(r, 12);
}

TEST(obj, SERIALIZE) 
{
	A a;
	A b;

	auto s = igris::serialize(a);
	auto r = igris::deserialize<A>(s);

	EXPECT_EQ(s.size(), sizeof(A::a) + sizeof(A::b) + sizeof(A::c));
	EXPECT_EQ(r, b);
}