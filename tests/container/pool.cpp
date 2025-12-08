#include <doctest/doctest.h>
#include <igris/container/pool.h>
#include <cstring>

TEST_CASE("pool.basic")
{
    static uint8_t buffer[64 * 4];
    igris::pool pool(buffer, sizeof(buffer), 64);

    CHECK_EQ(pool.size(), 4);
    CHECK_EQ(pool.avail(), 4);
    CHECK_EQ(pool.room(), 4);
    CHECK_EQ(pool.element_size(), 64);
}

TEST_CASE("pool.allocate_all")
{
    static uint8_t buffer[64 * 4];
    igris::pool pool(buffer, sizeof(buffer), 64);

    void *p1 = pool.get();
    void *p2 = pool.get();
    void *p3 = pool.get();
    void *p4 = pool.get();

    CHECK(p1 != nullptr);
    CHECK(p2 != nullptr);
    CHECK(p3 != nullptr);
    CHECK(p4 != nullptr);

    // All different
    CHECK(p1 != p2);
    CHECK(p2 != p3);
    CHECK(p3 != p4);

    CHECK_EQ(pool.avail(), 0);

    // Pool exhausted
    void *p5 = pool.get();
    CHECK(p5 == nullptr);
}

TEST_CASE("pool.free_and_reuse")
{
    static uint8_t buffer[64 * 2];
    igris::pool pool(buffer, sizeof(buffer), 64);

    void *p1 = pool.get();
    void *p2 = pool.get();
    CHECK_EQ(pool.avail(), 0);

    pool.put(p1);
    CHECK_EQ(pool.avail(), 1);

    void *p3 = pool.get();
    CHECK(p3 != nullptr);
    CHECK_EQ(pool.avail(), 0);

    // p3 should reuse p1's memory
    CHECK_EQ(p3, p1);

    pool.put(p2);
    pool.put(p3);
    CHECK_EQ(pool.avail(), 2);
}

TEST_CASE("pool.element_addressing")
{
    static uint8_t buffer[128 * 3];
    igris::pool pool(buffer, sizeof(buffer), 128);

    void *p1 = pool.get();
    void *p2 = pool.get();
    void *p3 = pool.get();

    // Elements must be within buffer
    CHECK((uint8_t *)p1 >= buffer);
    CHECK((uint8_t *)p1 < buffer + sizeof(buffer));
    CHECK((uint8_t *)p2 >= buffer);
    CHECK((uint8_t *)p2 < buffer + sizeof(buffer));
    CHECK((uint8_t *)p3 >= buffer);
    CHECK((uint8_t *)p3 < buffer + sizeof(buffer));

    // Addresses aligned to element_size
    CHECK(((uintptr_t)p1 - (uintptr_t)buffer) % 128 == 0);
    CHECK(((uintptr_t)p2 - (uintptr_t)buffer) % 128 == 0);
    CHECK(((uintptr_t)p3 - (uintptr_t)buffer) % 128 == 0);

    pool.put(p1);
    pool.put(p2);
    pool.put(p3);
}

TEST_CASE("pool.write_data")
{
    static uint8_t buffer[256 * 2];
    igris::pool pool(buffer, sizeof(buffer), 256);

    uint8_t *p1 = (uint8_t *)pool.get();
    uint8_t *p2 = (uint8_t *)pool.get();

    // Write pattern to first element
    memset(p1, 0xAA, 256);
    // Write different pattern to second element
    memset(p2, 0xBB, 256);

    // Verify data integrity
    for (int i = 0; i < 256; i++)
    {
        CHECK_EQ(p1[i], 0xAA);
        CHECK_EQ(p2[i], 0xBB);
    }

    pool.put(p1);
    pool.put(p2);
}

TEST_CASE("pool.cell_is_allocated")
{
    static uint8_t buffer[64 * 4];
    igris::pool pool(buffer, sizeof(buffer), 64);

    // Initially all cells are free
    CHECK_FALSE(pool.cell_is_allocated(0));
    CHECK_FALSE(pool.cell_is_allocated(1));
    CHECK_FALSE(pool.cell_is_allocated(2));
    CHECK_FALSE(pool.cell_is_allocated(3));

    void *p0 = pool.get();
    void *p1 = pool.get();

    // Find which cells were allocated
    int allocated_count = 0;
    for (int i = 0; i < 4; i++)
    {
        if (pool.cell_is_allocated(i))
            allocated_count++;
    }
    CHECK_EQ(allocated_count, 2);

    pool.put(p0);
    pool.put(p1);
}

TEST_CASE("pool.iterator")
{
    static uint8_t buffer[64 * 4];
    igris::pool pool(buffer, sizeof(buffer), 64);

    // Allocate some elements
    void *p0 = pool.get();
    void *p1 = pool.get();
    (void)p0;
    (void)p1;

    // Count allocated elements via iterator
    int count = 0;
    for (auto it = pool.begin(); it != pool.end(); ++it)
    {
        count++;
    }
    CHECK_EQ(count, 2);

    pool.put(p0);

    // Recount
    count = 0;
    for (auto it = pool.begin(); it != pool.end(); ++it)
    {
        count++;
    }
    CHECK_EQ(count, 1);

    pool.put(p1);
}

TEST_CASE("pool.reinit")
{
    static uint8_t buffer1[64 * 2];
    static uint8_t buffer2[128 * 4];

    igris::pool pool;

    pool.init(buffer1, sizeof(buffer1), 64);
    CHECK_EQ(pool.size(), 2);
    CHECK_EQ(pool.element_size(), 64);

    void *p = pool.get();
    CHECK(p != nullptr);
    pool.put(p);

    // Reinitialize with different buffer
    pool.init(buffer2, sizeof(buffer2), 128);
    CHECK_EQ(pool.size(), 4);
    CHECK_EQ(pool.element_size(), 128);
}
