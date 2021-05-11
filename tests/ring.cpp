#include <igris/datastruct/ring.h>
#include "doctest/doctest.h"

TEST_CASE("ring")
{
	struct ring_head ring;

	ring_init(&ring, 10);
	ring_move_head(&ring, 4);

	CHECK_EQ(ring_avail(&ring), 4);
	CHECK_EQ(ring_room(&ring), 5);
}