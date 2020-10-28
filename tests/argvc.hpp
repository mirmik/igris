#include <igris/datastruct/argvc.h>

TEST_CASE("ARGVC_test")
{
	char* argv[10];
	char* str = strdup("one two three 314");
	int argc = argvc_internal_split(str, argv, 10);

	CHECK_EQ(std::string(argv[0]), std::string("one"));
	CHECK_EQ(std::string(argv[1]), std::string("two"));
	CHECK_EQ(std::string(argv[2]), std::string("three"));
	CHECK_EQ(std::string(argv[3]), std::string("314"));
}

TEST_CASE("spaces_test")
{
	char* argv[10];
	char* str = strdup("    one two three 314    ");
	int argc = argvc_internal_split(str, argv, 10);

	CHECK_EQ(std::string(argv[0]), std::string("one"));
	CHECK_EQ(std::string(argv[1]), std::string("two"));
	CHECK_EQ(std::string(argv[2]), std::string("three"));
	CHECK_EQ(std::string(argv[3]), std::string("314"));
}
