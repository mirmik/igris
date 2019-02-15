#include <gtest/gtest.h>
#include <nos/dtrace.h>

void bar() 
{
	DTRACE();
}

void foo() 
{
	DTRACE();
	bar();
}

void bar_a(int a) 
{
	DTRACE_ARGS(a);
}

void foo_a(int a) 
{
	DTRACE_ARGS(a);
	bar_a(a);
}

TEST(dtrace, DTRACE) {
	testing::internal::CaptureStdout();
	foo();
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, 
		"DTRACE: void foo()\r\n"
		"DTRACE: void bar()\r\n");
}

TEST(dtrace, DTRACE_ARGS) {
	testing::internal::CaptureStdout();
	foo_a(42);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, 
		"DTRACE: void foo_a(int) args: a:42 \r\n"
		"DTRACE: void bar_a(int) args: a:42 \r\n");
}