#include <gtest/gtest.h>
#include <owl/datastruct/argvc.h>

TEST(argvc, ARGVC) {
	char* argv[10];
	char* str = strdup("one two three 314");
	int argc = argvc_internal_split(str, argv, 10);

	EXPECT_EQ(std::string(argv[0]), std::string("one"));
	EXPECT_EQ(std::string(argv[1]), std::string("two"));
	EXPECT_EQ(std::string(argv[2]), std::string("three"));
	EXPECT_EQ(std::string(argv[3]), std::string("314"));
}
