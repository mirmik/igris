#ifndef IGRIS_SCHED_SCHED_H
#define IGRIS_SCHED_SCHED_H

#include "schedee.h"
#include <igris/compliler.h>

__BEGIN_DECLS

void schedee_run(schedee_t * sch);
void schedee_stop(schedee_t * sch);
void schedee_wait(schedee_t * sch, struct dlist_head * wait);

void scheduler_step();

void __schedule__();
int __displace__();

__END_DECLS

#endif