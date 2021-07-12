#include <doctest/doctest.h>
#include <igris/util/pathops.h>

TEST_CASE("pathops.next") 
{
	unsigned int len;
	const char * b = "/dev/null";

	b = path_iterate(b);
	CHECK(strcmp(b, "dev/null") == 0);

	b = path_iterate(b);
	CHECK(strcmp(b, "null") == 0);

	b = path_iterate(b);
	CHECK(strcmp(b, "") == 0);

	b = path_iterate(b);
	CHECK_EQ(b, (const char *)NULL);
	
	b = path_iterate(b);
	CHECK_EQ(b, (const char *)NULL);
}