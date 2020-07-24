#ifndef IGRIS_SCHED_SCHEDEE_H
#define IGRIS_SCHED_SCHEDEE_H

#include <igris/datastruct/dlist.h>
#include <igris/compiler.h>

#define SCHEDEE_STATE_RUN 			0
#define SCHEDEE_STATE_WAIT 			2
#define SCHEDEE_STATE_WAIT_SCHEDEE 	6
#define SCHEDEE_STATE_STOP 			3
#define SCHEDEE_STATE_FINAL 		4
#define SCHEDEE_STATE_ZOMBIE 		5

#ifndef SCHEDEE_MAXPRIO
#define SCHEDEE_MAXPRIO 3
#endif

struct schedee_operations 
{
	void (*execute)       ();
	int  (*displace)      ();
	void (*finalize)      ();
	void (*signal_handler)(int sig);
};

struct schedee_s
{
	union
	{
		struct ctrobj                ctr;
		struct ctrobj_schedee_waiter waiter;
		struct ctrobj_schedee_timer  timer;
	};

	struct schedee_s * parent;

	uint8_t prio;
	uint8_t state;

	int local_errno;
	
	const struct schedee_operations * ops;

#if SCHEDEE_DEBUG_STRUCT
	const char * mnemo = nullptr;
	struct dlist_head debug_lnk;
	uint16_t dispcounter;
	uint16_t execcounter;
#endif
};
typedef struct schedee_s schedee_t;

__BEGIN_DECLS

void schedee_init(schedee_t * sch, 
	const struct schedee_operations * ops,
	schedee_t * parent);

__END_DECLS

#endif