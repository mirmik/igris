#include <doctest/doctest.h>
#include <igris/clonner.h>

#include <string>

extern std::string output;

TEST_CASE("clonner") 
{
	igris::clonner clonner;

	clonner.init();
	clonner.start_subprocess("touch xxx");

	clonner.terminate_starter();

	output.clear();
}
