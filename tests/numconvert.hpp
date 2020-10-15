#include <igris/util/numconvert.h>
#include <math.h>
#include <string.h>

LT_BEGIN_TEST(igris_test_suite, numconvert)
{
	char buf[256];
	float f;
	int i;
	double d;

	i = 42;
	i32toa(i, buf, 10);
	LT_CHECK(strcmp(buf, "42") == 0);

	i = -42;
	i32toa(i, buf, 10);
	LT_CHECK(strcmp(buf, "-42") == 0);

	strcpy(buf, "42");
	i = atoi32(buf, 10, nullptr);
	LT_CHECK_EQ(i, 42);

	strcpy(buf, "-42");
	i = atoi32(buf, 10, nullptr);
	LT_CHECK_EQ(i, -42);

	strcpy(buf, "0.1000");
	f = atof32(buf, nullptr);
	LT_CHECK(fabs(f - 0.1) < 1e-5);

	strcpy(buf, "0.7773213");
	f = atof32(buf, nullptr);
	LT_CHECK(fabs(f - 0.7773213) < 1e-5);

	strcpy(buf, "0.1000");
	f = atof64(buf, nullptr);
	LT_CHECK(fabs(f - 0.1) < 1e-5);

	strcpy(buf, "0.7773213");
	f = atof64(buf, nullptr);
	LT_CHECK(fabs(f - 0.7773213) < 1e-5);

	f = 0.1;
	f32toa(f, buf, 1);
	LT_CHECK(strcmp(buf, "0.1") == 0);

	f = 0.1;
	f32toa(f, buf, 10);
	LT_CHECK(strcmp(buf, "0.1000000000") == 0);

	f = std::nanf("");
	f32toa(f, buf, 10);
	LT_CHECK(strcmp(buf, "nan") == 0);

	d = std::nanf("");
	f64toa(d, buf, 10);
	LT_CHECK(strcmp(buf, "nan") == 0);

	const char * s = "5.63431234";
	f = atof32(s, nullptr);
	LT_CHECK(fabs(f - 5.634) < 0.01);
	
	const char * s2 = "7.12328767123287";
	f = atof32(s2, nullptr);
	LT_CHECK(fabs(f - 7.123287) < 0.0001);


//	f = std::nan("");
//	f32toa(buf, f, 10);
//	LT_CHECK(std::isnan());
}
LT_END_TEST(numconvert)