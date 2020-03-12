#include <igris/datastruct/argvc.h>

LT_BEGIN_TEST(igris_test_suite, ARGVC_test)
{
	char* argv[10];
	char* str = strdup("one two three 314");
	int argc = argvc_internal_split(str, argv, 10);

	LT_CHECK_EQ(std::string(argv[0]), std::string("one"));
	LT_CHECK_EQ(std::string(argv[1]), std::string("two"));
	LT_CHECK_EQ(std::string(argv[2]), std::string("three"));
	LT_CHECK_EQ(std::string(argv[3]), std::string("314"));
}
LT_END_TEST(ARGVC_test)

LT_BEGIN_TEST(igris_test_suite, spaces_test)
{
	char* argv[10];
	char* str = strdup("    one two three 314    ");
	int argc = argvc_internal_split(str, argv, 10);

	LT_CHECK_EQ(std::string(argv[0]), std::string("one"));
	LT_CHECK_EQ(std::string(argv[1]), std::string("two"));
	LT_CHECK_EQ(std::string(argv[2]), std::string("three"));
	LT_CHECK_EQ(std::string(argv[3]), std::string("314"));
}
LT_END_TEST(spaces_test)