#include "littletest.hpp"

LT_BEGIN_SUITE(igris_test_suite)
	void set_up()
	{}

	void tear_down()
	{}
LT_END_SUITE(igris_test_suite)

#include "bits.hpp"
#include "util.hpp"
#include "sync.hpp"
#include "serialize.hpp"
#include "event.hpp"
#include "array_view.hpp"
#include "argvc.hpp"

LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()