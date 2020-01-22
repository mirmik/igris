#include <gtest/gtest.h>
#include <igris/dprint.h>

TEST(dprint, integer) {
	testing::internal::CaptureStdout();
	dpr(88);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "88");
}

TEST(dprint, float_print) {
	testing::internal::CaptureStdout();
	dpr(0.1);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "0.10000000");
}
