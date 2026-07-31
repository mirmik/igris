#include <doctest/doctest.h>
#include <igris/datastruct/bytering.h>

TEST_CASE("bytering preserves FIFO across wrap")
{
    unsigned char storage[4] = {};
    bytering_head ring;
    bytering_init(&ring, storage, sizeof(storage));

    CHECK_EQ(bytering_push(&ring, 1), 0);
    CHECK_EQ(bytering_push(&ring, 2), 0);
    CHECK_EQ(bytering_push(&ring, 3), 0);
    CHECK_EQ(bytering_push(&ring, 4), -1);
    CHECK_EQ(bytering_pop(&ring), 1);
    CHECK_EQ(bytering_push(&ring, 4), 0);
    CHECK_EQ(bytering_pop(&ring), 2);
    CHECK_EQ(bytering_pop(&ring), 3);
    CHECK_EQ(bytering_pop(&ring), 4);
    CHECK_EQ(bytering_pop(&ring), -1);
}

TEST_CASE("bytering zero capacity is always full")
{
    unsigned char storage = 0;
    bytering_head ring;
    bytering_init(&ring, &storage, 0);
    CHECK(bytering_empty(&ring));
    CHECK(bytering_full(&ring));
    CHECK_EQ(bytering_push(&ring, 1), -1);
}

TEST_CASE("bytering one-byte storage has zero payload capacity")
{
    unsigned char storage = 0;
    bytering_head ring;
    bytering_init(&ring, &storage, 1);
    CHECK(bytering_empty(&ring));
    CHECK(bytering_full(&ring));
    CHECK_EQ(bytering_push(&ring, 1), -1);
}
