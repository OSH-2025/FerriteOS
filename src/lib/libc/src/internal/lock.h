#ifndef LOCK_H
#define LOCK_H
#ifdef __LITEOS__
#include "pthread.h"
#include "limits.h"
#endif

hidden void __lock(volatile int *);
hidden void __unlock(volatile int *);
#ifdef __LITEOS__
#define LOCK(x) pthread_mutex_lock((pthread_mutex_t *)(&(x)))
#define UNLOCK(x) pthread_mutex_unlock((pthread_mutex_t *)(&(x)))
extern INT32 LibcLock(UINT32 *lock);
extern INT32 LibcUnlock(const UINT32 *lock);
#define LIBC_LOCK_INITIALIZER UINT_MAX
#define LIBC_LOCK_INIT(x) UINT32 x = LIBC_LOCK_INITIALIZER
#define LIBC_LOCK(x) LibcLock(&(x))
#define LIBC_UNLOCK(x) LibcUnlock(&(x))
#else
#define LOCK(x) __lock(x)
#define UNLOCK(x) __unlock(x)
#endif

#endif
