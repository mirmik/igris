#include <doctest/doctest.h>
#include <igris/time/timer_manager.h>

static int aaa = 0;

static void inc_a(int a) 
{
	aaa += a;
}

TEST_CASE("TimerManager") 
{
	auto current_time = millis(); 

	igris::timer_manager<igris::timer_spec<systime_t>> manager(&millis);
	igris::timer<int> tim0(igris::make_delegate(inc_a), 12);

	manager.plan(tim0, current_time, 1000);

	/*manager.exec(current_time + 1001);
	CHECK_EQ(aaa, 12);

	manager.exec(current_time + 2001);
	CHECK_EQ(aaa, 24);*/
}