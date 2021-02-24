#include "doctest/doctest.h"
#include <igris/binreader.h>

TEST_CASE("reader")
{
	const char * teststr_0 = "HelloWorld";
	const char * teststr_1 = "     World";
	const char * ptr;

	char buffer[64];

	SUBCASE("reader_memcpy") 
	{
		ptr = teststr_0;
		reader_memcpy(&ptr, buffer, 5);
		CHECK_EQ(std::string("Hello", 5), std::string(buffer, 5)); 

		reader_memcpy(&ptr, buffer, 5);
		CHECK_EQ(std::string("World"), std::string(buffer, 5)); 
	}

	SUBCASE("reader_skip") 
	{
		ptr = teststr_0;
		reader_skip(&ptr, 5);
		reader_memcpy(&ptr, buffer, 5);
		CHECK_EQ(std::string("World"), std::string(buffer, 5)); 
	}

	SUBCASE("reader_skip_while") 
	{
		ptr = teststr_1;
		reader_skip_while(&ptr, ' ', teststr_1+strlen(teststr_1));
		reader_memcpy(&ptr, buffer, 5);
		CHECK_EQ(std::string("World"), std::string(buffer, 5)); 
	}
}