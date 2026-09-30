#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include <string>

std::string output;

#define LT_BEGIN_TEST(a, b) TEST_CASE(#b)
#define LT_END_TEST(a)

extern "C" void debug_putchar(char c)
{
    output.push_back(c);
}

extern "C" void debug_write(const char *c, int i)
{
    while (i--)
        debug_putchar(*c++);
}

// Старые bits.hpp, container/unbounded_array.hpp и osutil.hpp не подключаем:
// этих файлов больше нет; их актуальные сценарии живут в обычных *.cpp-тестах.
#include "argvc.hpp"
#include "chunked_vector.hpp"
#include "container/array_view.hpp"
#include "dprint.hpp"
#include "event.hpp"
#include "signature.hpp"
#include "sync.hpp"
#include "util.hpp"
