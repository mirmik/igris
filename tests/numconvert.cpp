#include <doctest/doctest.h>
#include <igris/util/numconvert.h>
#include <math.h>
#include <string.h>

TEST_CASE("numconvert")
{
	char buf[256];
	float f;
	int i;
	double d;

	i = 42;
	i32toa(i, buf, 10);
	CHECK(strcmp(buf, "42") == 0);

	i = -42;
	i32toa(i, buf, 10);
	CHECK(strcmp(buf, "-42") == 0);

	f = 0.1;
	f32toa(f, buf, 1);
	CHECK(strcmp(buf, "0.1") == 0);

	f = 0.1;
	f32toa(f, buf, 10);
	CHECK(strcmp(buf, "0.1000000000") == 0);

	f = nanf("");
	f32toa(f, buf, 10);
	CHECK(strcmp(buf, "nan") == 0);

	d = nanf("");
	f64toa(d, buf, 10);
	CHECK(strcmp(buf, "nan") == 0);

	CHECK_EQ(10042, igris_atou32("10042", 10, nullptr));
	CHECK_EQ(10042, igris_atoi32("10042", 10, nullptr));
}
