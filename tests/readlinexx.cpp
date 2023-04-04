#include <doctest/doctest.h>
#include <igris/shell/readlinexx.h>

char buffer[48];
char hbuffer[48 * 5];

TEST_CASE("readlinexx")
{
    igris::readline rl;

    rl.init(buffer, 48);
    rl.history_init(hbuffer, 5);

    CHECK(rl.line().len == 0);

    rl.newdata('a');
    CHECK(rl.line().len == 1);
    CHECK(rl.line().buf[0] == 'a');

    rl.newdata('b');
    CHECK(rl.line().len == 2);
    CHECK(rl.line().buf[0] == 'a');
    CHECK(rl.line().buf[1] == 'b');
}