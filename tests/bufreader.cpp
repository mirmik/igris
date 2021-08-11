#include <doctest/doctest.h>
#include <igris/bufreader.h>

#include <string>

TEST_CASE("bufreader.readline") 
{
	const char * str = " lalalal \n casdc  \r\n ";

	struct bufreader reader;
	const char * token;
	int len;

	bufreader_init(&reader, str, strlen(str));

	len = bufreader_readline(&reader, &token);
	CHECK_EQ(len, 9);
	CHECK_EQ(std::string(token, len), std::string(" lalalal "));

	len = bufreader_readline(&reader, &token);
	CHECK_EQ(len, 8);
	CHECK_EQ(std::string(token, len), std::string(" casdc  "));
}