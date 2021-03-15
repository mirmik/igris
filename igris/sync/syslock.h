#ifndef IGRIS_SYNC_SYSLOCK_H
#define IGRIS_SYNC_SYSLOCK_H

#include <igris/compiler.h>
#include <igris/util/location.h>

#define IGRIS_SYSLOCK_DEBUG 0

struct syslock_save_pair
{
	int count;
	int state;
};

__BEGIN_DECLS

#if IGRIS_SYSLOCK_DEBUG
void system_lock_impl(struct location loc);
#define system_lock() {CURRENT_LOCATION(loc);system_lock_impl(loc);};
#else
void system_lock();
#endif

#if IGRIS_SYSLOCK_DEBUG
void system_unlock_impl(struct location loc);
#define system_unlock() {CURRENT_LOCATION(loc);system_unlock_impl(loc);};
#else
void system_unlock();
#endif

void syslock_reset();

int syslock_counter();
void syslock_counter_set(int count);

struct syslock_save_pair system_lock_save();
void system_lock_restore(struct syslock_save_pair save);

__END_DECLS

#ifdef __cplusplus
namespace igris {
	class syslock_guard
	{
	public:
		syslock_guard() { system_lock(); }
		~syslock_guard() { system_unlock(); }
	};
}
#endif

#endif
