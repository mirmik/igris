#ifndef IGRIS_CSECTION_H
#define IGRIS_CSECTION_H

#include <sys/cdefs.h>

__BEGIN_DECLS

void csection_enter(csection_t* sect);
void csection_leave(csection_t* sect);

__END_DECLS

#endif