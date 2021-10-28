#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <string>

std::string output;

#define LT_BEGIN_TEST(a,b) TEST_CASE(#b)
#define LT_END_TEST(a)

extern "C"
void debug_putchar(char c)
{
	output.push_back(c);
}

extern "C"
void debug_write(const char *c, int i)
{
    while (i--)
        debug_putchar(*c++);
}

#include "numconvert.hpp"
#include "trent.hpp"
#include "osutil.hpp"
#include "bits.hpp"
#include "util.hpp"
#include "sync.hpp"
#include "dprint.hpp"
#include "serialize.hpp"
#include "event.hpp"
#include "argvc.hpp"
#include "signature.hpp"
#include "test_allocator.hpp"
#include "chunked_vector.hpp"

#include "container/unbounded_array.hpp"
#include "container/array_view.hpp"
