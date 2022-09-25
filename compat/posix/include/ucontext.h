#ifndef IGRIS_POSIX_UCONTEXT_H
#define IGRIS_POSIX_UCONTEXT_H

#include <signal.h>

typedef struct ucontext_t
{
    unsigned long uc_flags;
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    sigcontext uc_mcontext;
    sigset_t uc_sigmask;
} ucontext_t;
