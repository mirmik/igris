#include <gtest/gtest.h>
#include <nos/trace.h>

static void bar() 
{
	TRACE();
}

static void foo() 
{
	TRACE();
	bar();
}

static void bar_a(int a) 
{
	TRACE_ARGS(a);
}

static void foo_a(int a) 
{
	TRACE_ARGS(a);
	bar_a(a);
}

TEST(trace, TRACE) {
	testing::internal::CaptureStdout();
	foo();
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, 
		"TRACE: 1: -> void foo()\r\n"
		"TRACE: 2: -> void bar()\r\n"
		"TRACE: 2: <- void bar()\r\n"
		"TRACE: 1: <- void foo()\r\n");
}

TEST(trace, TRACE_ARGS) {
	testing::internal::CaptureStdout();
	foo_a(42);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, 
		"TRACE: 1: -> void foo_a(int) args: a:42 \r\n"
		"TRACE: 2: -> void bar_a(int) args: a:42 \r\n"
		"TRACE: 2: <- void bar_a(int)\r\n"
		"TRACE: 1: <- void foo_a(int)\r\n");
}