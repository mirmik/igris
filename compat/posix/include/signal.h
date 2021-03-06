#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <errno.h>
#include <sys/types.h>
#include <sys/cdefs.h>

#define SIGHUP      1       /* (POSIX)    Hangup.  */
#define SIGINT      2       /* (ANSI)     Interrupt.  */
#define SIGQUIT     3       /* (POSIX)    Quit.  */
#define SIGILL      4       /* (ANSI)     Illegal instruction.  */
#define SIGTRAP     5       /* (POSIX)    Trace trap.  */
#define SIGABRT     6       /* (ANSI)     Abort.  */
#define SIGIOT      6       /* (4.2 BSD)  IOT trap.  */
#define SIGBUS      7       /* (4.2 BSD)  BUS error.  */
#define SIGFPE      8       /* (ANSI)     Floating-point exception.  */
#define SIGKILL     9       /* (POSIX)    Kill, unblockable.  */
#define SIGUSR1     10      /* (POSIX)    User-defined signal 1.  */
#define SIGSEGV     11      /* (ANSI)     Segmentation violation.  */
#define SIGUSR2     12      /* (POSIX)    User-defined signal 2.  */
#define SIGPIPE     13      /* (POSIX)    Broken pipe.  */
#define SIGALRM     14      /* (POSIX)    Alarm clock.  */
#define SIGTERM     15      /* (ANSI)     Termination.  */
#define SIGSTKFLT   16      /*            Stack fault.  */
#define SIGCLD      SIGCHLD /* (System V) Same as SIGCHLD.  */
#define SIGCHLD     17      /* (POSIX)    Child status has changed.  */
#define SIGCONT     18      /* (POSIX)    Continue.  */
#define SIGSTOP     19      /* (POSIX)    Stop, unblockable.  */
#define SIGTSTP     20      /* (POSIX)    Keyboard stop.  */
#define SIGTTIN     21      /* (POSIX)    Background read from tty.  */
#define SIGTTOU     22      /* (POSIX)    Background write to tty.  */
#define SIGURG      23      /* (4.2 BSD)  Urgent condition on socket.  */
#define SIGXCPU     24      /* (4.2 BSD)  CPU limit exceeded.  */
#define SIGXFSZ     25      /* (4.2 BSD)  File size limit exceeded.  */
#define SIGVTALRM   26      /* (4.2 BSD)  Virtual alarm clock.  */
#define SIGPROF     27      /* (4.2 BSD)  Profiling alarm clock.  */
#define SIGWINCH    28      /* (4.3 BSD, Sun) Window size change.  */
#define SIGPOLL     SIGIO   /* (System V) Pollable event occurred.  */
#define SIGIO       29      /* (4.2 BSD)  I/O now possible.  */
#define SIGPWR      30      /* (System V) Power failure restart.  */
#define SIGSYS      31      /*            Bad system call.  */
#define SIGUNUSED   31

#define SIGRTMIN    32
#define SIGRTMAX    63

#define _SIG_TOTAL  64

#define SA_NOCLDSTOP  (0x1ul << 0)
#define SA_NOCLDWAIT  (0x1ul << 1)
#define SA_SIGINFO    (0x1ul << 2)
#define SA_ONSTACK    (0x1ul << 3)
#define SA_RESTART    (0x1ul << 4)
#define SA_NODEFER    (0x1ul << 5)
#define SA_RESETHAND  (0x1ul << 6)

#endif /* SIGNAL_H_ */
