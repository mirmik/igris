#ifndef IGRIS_SYNC_SYSLOCK_H
#define IGRIS_SYNC_SYSLOCK_H

#include <sys/cdefs.h>

__BEGIN_DECLS

void system_lock(); 
void system_unlock(); 
void syslock_reset(); 

__END_DECLS

#ifdef __cplusplus
namespace igris {
	struct syslock {
		void lock() { system_lock(); };
		void unlock() { system_unlock(); };
	};

	using ::system_lock;
	using ::system_unlock;	
}
#endif

#endif