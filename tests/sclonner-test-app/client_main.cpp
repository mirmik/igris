#include <nos/print.h>
#include <nos/io/stdfile.h>
#include <nos/input.h>

#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main() 
{
	nos::println("stdout:HelloWorld");
	nos::println_to(nos::cerr, "stderr:HelloWorld");

	std::this_thread::sleep_for(10ms);
}
