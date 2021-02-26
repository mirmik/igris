#include <igris/osutil/realtime.h>
#include <pthread.h>

int this_thread_set_realtime_priority()
{
#ifndef __WIN32__
	int ret;

	// We'll operate on the currently running thread.
	pthread_t this_thread = pthread_self();

	// struct sched_param is used to store the scheduling priority
	struct sched_param params;

	// We'll set the priority to the maximum.
	params.sched_priority = 30; // sched_get_priority_max(SCHED_FIFO);
	
	// Attempt to set thread real-time priority to the SCHED_FIFO policy
	if ((ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params)))
		return ret;

	// Now verify the change in thread priority
	int policy = 0;
	if ((ret = pthread_getschedparam(this_thread, &policy, &params)))
		return ret; 

	// Check the correct policy was applied
	if (policy != SCHED_FIFO)
	{
		return -1;
	}
	
	return 0;
#else
	return -1;
#endif
}
