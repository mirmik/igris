
#include <signal.h>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
    return 0;
}

void raise(int sig) {}

int sigaltstack(const stack_t *stack, stack_t *oldstack)
{
    return 0;
}