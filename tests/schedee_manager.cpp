#include <igris/osinter/schedee_manager.h>
#include <doctest/doctest.h>
#include <csetjmp>

std::jmp_buf jmpbuf;
int i = 0;

class test_schedee : public igris::schedee 
{
	void execute() override 
	{
		i++;
		if (i < 10)
			displace();
	}

	void finalize() override {}
	void displace() override 
	{
		longjmp(jmpbuf, 1);
	}
};

test_schedee sched;
igris::schedee_manager manager;

void schedule() 
{
	setjmp(jmpbuf);
	manager.start(sched);
	manager.exec();
}

TEST_CASE("scheduler") 
{
	schedule();
	CHECK_EQ(i, 10);
}