#ifndef IGRIS_SYNC_SYSLOCK_H
#define IGRIS_SYNC_SYSLOCK_H

#include <igris/compiler.h>

struct syslock_save_pair 
{
	int count;
	int state;
};

__BEGIN_DECLS

void system_lock(); 
void system_unlock(); 
void syslock_reset(); 

int syslock_counter(); 
void syslock_counter_set(int count); 

struct syslock_save_pair system_lock_save();
void system_lock_restore(struct syslock_save_pair save);

__END_DECLS

#ifdef __cplusplus
namespace igris {
	struct syslock {
		void lock() { system_lock(); };
		void unlock() { system_unlock(); };
	};

	using ::system_lock;
	using ::system_unlock;	

	class syslock_guard 
	{
	public:
		syslock_guard() { system_lock(); }
		~syslock_guard() { system_unlock(); }
	};
}
#endif

#endif
