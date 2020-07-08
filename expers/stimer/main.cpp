#include <igris/sched/stimer.h>
#include <igris/time.h>
#include <stdio.h>

int main() 
{
	stimer_t timer;

	stimer_plan(&timer, millis(), 1000);

	while(1) 
	{
		STIMER_PERIODIC(&timer, millis()) 
		{
			printf("HelloWorld %ld\r\n", timer.start);
		}
	}
}