#include "doctest/doctest.h"
#include <igris/container/ring.h>
#include <igris/datastruct/ring.h>
#include <string.h>

TEST_CASE("ring")
{
    struct ring_head ring;

    ring_init(&ring, 10);
    ring_move_head(&ring, 4);

    CHECK_EQ(ring_avail(&ring), 4);
    CHECK_EQ(ring_room(&ring), 5);
}

TEST_CASE("ring_getc distinguishes every byte from empty")
{
    struct ring_head ring;
    char buffer[257];

    ring_init(&ring, sizeof(buffer));
    for (int value = 0; value <= 0xff; ++value)
        CHECK_EQ(ring_putc(&ring, buffer, (char)value), 1);

    for (int value = 0; value <= 0xff; ++value)
        CHECK_EQ(ring_getc(&ring, buffer), value);

    CHECK_EQ(ring_getc(&ring, buffer), -1);
}

TEST_CASE("ring_read preserves 0xff with signed char")
{
    struct ring_head ring;
    char buffer[5];
    const char input[] = {0x00, 0x7f, (char)0x80, (char)0xff};
    char output[sizeof(input)] = {};

    ring_init(&ring, sizeof(buffer));
    CHECK_EQ(ring_write(&ring, buffer, input, sizeof(input)), sizeof(input));
    CHECK_EQ(ring_read(&ring, buffer, output, sizeof(output)), sizeof(output));
    CHECK_EQ(memcmp(output, input, sizeof(input)), 0);
    CHECK(ring_empty(&ring));
}
