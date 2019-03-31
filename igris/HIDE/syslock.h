#ifndef GXX_CSECTION_H
#define GXX_CSECTION_H

#include <sys/cdefs.h>

__BEGIN_DECLS

void system_lock(); 
void system_unlock(); 

__END_DECLS

#ifdef __cplusplus
namespace gxx {
	struct syslock {
		void lock() { system_lock(); };
		void unlock() { system_unlock(); };
	};

	using ::system_lock;
	using ::system_unlock;	
}
#endif

#endif