#include <doctest/doctest.h>
#include <igris/container/ring.h>

TEST_CASE("ring") 
{
	int buf[12];

	igris::ring<int> ring(buf, 12);

	CHECK_EQ(ring.avail(), 0);
	CHECK_EQ(ring.room(), 11);
}