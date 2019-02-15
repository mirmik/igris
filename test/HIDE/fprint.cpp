#include <gtest/gtest.h>
#include <nos/print.h>

using namespace nos::argument_literal;

TEST(fprint, fprint) {
	testing::internal::CaptureStdout();
	nos::fprint("fprint {} {} {}", "aaa", "bbb", "ccc");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "fprint aaa bbb ccc");
}

TEST(fprint, fprint_integer) {
	testing::internal::CaptureStdout();
	nos::fprint("integer({})", 42);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "integer(42)");
}

TEST(fprint, fprint_named) {
	testing::internal::CaptureStdout();
	nos::fprint("fprint {a} {b} {c}", "a"_a="aaa", "b"_a="bbb", "c"_a="ccc");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "fprint aaa bbb ccc");
}

TEST(fprint, fprint_named_and_numers) {
	testing::internal::CaptureStdout();
	nos::fprint("fprint {b} {2} {0}", "a"_a="aaa", "b"_a="bbb", "c"_a="ccc");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "fprint bbb ccc aaa");
}

TEST(fprint, fprint_zero_size_arglist) {
	testing::internal::CaptureStdout();
	nos::fprint("zerosize");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "zerosize");
}

TEST(fprint, fprint_longstring) {
	testing::internal::CaptureStdout();
	nos::fprint("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
}

TEST(fprint, format) {
	std::string out = nos::format("format {} {} {}", "aaa", "bbb", "ccc");
	EXPECT_EQ(out, "format aaa bbb ccc");
}

