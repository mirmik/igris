
#include <igris/dprint.h>
#include <cmath>

LT_BEGIN_TEST(dprint_suite, integer) {
	dpr(88);
	LT_CHECK_EQ(output, "88");
}
LT_END_TEST(integer)

LT_BEGIN_TEST(dprint_suite, float_print) {
	dpr(0.1);
	LT_CHECK_EQ(output, "0.10000000");
}
LT_END_TEST(float_print)

LT_BEGIN_TEST(dprint_suite, dprf_float) {
	dprf("%f", 0.1);
	LT_CHECK_EQ(output, "0.100000");
}
LT_END_TEST(dprf_float)


LT_BEGIN_TEST(dprint_suite, dpr_float_nanf) {
	float f = std::nanf("");
	dpr(f);
	LT_CHECK_EQ(output, "nan");
}
LT_END_TEST(dpr_float_nanf)

LT_BEGIN_TEST(dprint_suite, dpr_float_infp) {
	float f = 10e200;
	dpr(f);
	LT_CHECK_EQ(output, "+inf");
}
LT_END_TEST(dpr_float_infp)
