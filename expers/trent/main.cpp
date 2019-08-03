#include <igris/trent/trent.h>
#include <igris/trent/json.h>
#include <nos/print.h>

int main() 
{
	//std::string str = "{'a': 'hello'}";
	//std::string str = "{'a': 'hello', 'b': 'world'}";
	std::string str = "{'a': 'hello', 'b': 'world', 'c': ['hello', 'world']}";
	//std::string str = "456";
	//std::string str = "'hello'";
	//std::string str = "['hello', 'world']";
	
	igris::json::parser_cstr parser(str);

	auto tr = parser.parse();
	nos::println(tr);
}