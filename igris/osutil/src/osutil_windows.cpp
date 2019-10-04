#include <fcntl.h>
#include <winsock2.h>
#include <stdio.h>

#include <gxx/osutil/fd.h>

int gxx::osutil::nonblock(int fd, bool en) {
	unsigned long mode = en;
	return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
}

namespace gxx {
	namespace osutil {
        /*void setsig(int fd, int sig) {
			fcntl(fd, F_SETOWN, getpid());
			fcntl(fd, F_SETSIG, SIGUSR1);
			fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK | O_ASYNC); 
        }*/

        /*void signal(int sig, void(*handler)(int)) {
   			::signal(sig, handler);
        }*/
	}
}
