#include <sys/select.h>
#include <igris/util/bug.h>

int select(int nfds, fd_set *readfds, fd_set *writefds,
			fd_set *exceptfds, struct timeval *timeout) 
{
	BUG();
}