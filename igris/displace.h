#ifndef IGRIS_DISPLACE_H
#define IGRIS_DISPLACE_H

#include <sys/cdefs.h>

__BEGIN_DECLS

// Вызывается потоком, желающим отдать управление супервизору.
int __displace__();

__END_DECLS

#endif
