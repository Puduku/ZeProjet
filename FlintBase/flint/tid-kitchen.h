// flint/tid-kitchen.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: elementary thread id definitions 
// =======

#ifndef __FLINT_TID_KITCHEN_H_INCLUDED__
#define __FLINT_TID_KITCHEN_H_INCLUDED__


#include <sys/syscall.h>
#include <pthread.h>


// "unspoken precept" assimilates tid_t as pid_t
typedef pid_t tid_t ;



#ifdef __APPLE__
#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_12
static inline tid_t gettid(void) {
  uint64_t tid64;
  pthread_threadid_np(NULL, &tid64);
  return (tid_t)tid64;
}
#else
#define gettid() syscall(SYS_thread_selfid)
#endif
#else
#define gettid() syscall(__NR_gettid)
#endif





#endif //  __FLINT_TID_KITCHEN_H_INCLUDED__
