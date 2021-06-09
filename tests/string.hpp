#include <igris/util/string.h>

TEST_CASE("string.replace0")
{
	const char * input = "Hello abc worabcldabc";
	const char * output = "Hello  world";
	char buffer[64];

	replace_substrings(buffer, 64, input, strlen(input), "abc", 3, "", 0);

	CHECK_EQ(std::string(buffer), std::string(output));
}


TEST_CASE("string.replace1")
{
	const char * input = "Hello abc worabcldabc";
	char buffer[64];

	replace_substrings(buffer, 64, input, strlen(input), "", 0, "a", 1);

	CHECK_EQ(std::string(buffer), std::string(input));
}

TEST_CASE("string.replace2")
{
	std::string input = "Hello abc worabcldabc";
	
	auto output = igris::replace(input, "", "a");

	CHECK_EQ(input, output);
}

TEST_CASE("string.replace2")
{
	std::string input = "Hello abc worabcldabc";
	
	auto output = igris::replace(input, "abc", "");

	CHECK_EQ(output, "Hello  world");
}
