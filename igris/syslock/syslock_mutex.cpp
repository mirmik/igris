#include <gxx/syslock.h>
#include <mutex>

std::recursive_mutex mtx;

__BEGIN_DECLS

void system_lock() {
	mtx.lock();
}

void system_unlock() {
	mtx.unlock();
}

__END_DECLS