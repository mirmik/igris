#include <doctest/doctest.h>
#include <igris/sclonner.h>

TEST_CASE("SCLONNER")
{
    igris::subprocess proc;

    proc.exec("/bin/echo ECHO: subprocess test 1");
    proc.wait();

    proc.exec("/bin/cat tests/text.txt");
    proc.wait();
}