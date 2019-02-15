#include <gtest/gtest.h>
#include <nos/print.h>

struct A { int i = 42; ssize_t print_to(nos::ostream& os) const { os << i; }; };
struct B { int i = 42; };
struct C { int i = 42; };
struct D { int i = 42; };

ssize_t nos_print(nos::ostream& os, const B& b) { os << b.i; };
std::ostream& operator << (std::ostream& os, const C& c) { os << c.i; };
namespace nos { template <> struct print_implementation<D> { static ssize_t print_to(nos::ostream& out, const D& d) { return out.print(d.i); } }; }

TEST(print, print) {
	testing::internal::CaptureStdout();
	nos::print("print");
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "print");
}

TEST(print, integer) {
	testing::internal::CaptureStdout();
	nos::print(88);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "88");
}

TEST(print, number) {
	testing::internal::CaptureStdout();
	nos::print(0.88);
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "0.88");
}

TEST(print, method) {
	testing::internal::CaptureStdout();
	nos::print(A());
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "42");
}

TEST(print, nos_print) {
	testing::internal::CaptureStdout();
	nos::print(B());
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "42");
}

TEST(print, std_ostream) {
	testing::internal::CaptureStdout();
	nos::print(C());
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "42");
}

/*TEST(print, print_implementation) {
	testing::internal::CaptureStdout();
	nos::print(D());
	std::string output = testing::internal::GetCapturedStdout();
	EXPECT_EQ(output, "42");
}*/