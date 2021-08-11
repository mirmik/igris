#include <doctest/doctest.h>
#include <igris/bufreader.h>

#include <string>

TEST_CASE("bufreader.readline") 
{
	const char * str = " lalalal \n casdc  \r\n ";

	struct bufreader reader;
	struct bufreader_token token;

	bufreader_init(&reader, str, strlen(str));

	token = bufreader_readline(&reader);
	CHECK_EQ(token.size, 9);
	CHECK_EQ(std::string(token.data, token.size), std::string(" lalalal "));

	token = bufreader_readline(&reader);
	CHECK_EQ(token.size, 8);
	CHECK_EQ(std::string(token.data, token.size), std::string(" casdc  "));
}