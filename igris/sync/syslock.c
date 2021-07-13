#include <igris/sync/syslock.h>

volatile unsigned char __system_lock_state = 0;

unsigned char system_lock_state() { return __system_lock_state; }