// flint/tid.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023
//
// Purpose: provide support for plain threads ids 
// =======

#ifndef __FLINT_TID_H_INCLUDED__
#define __FLINT_TID_H_INCLUDED__

// TID : 1. THE KITCHEN...
// ====================

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


// TID : 2. THE MENU...
// =================

// We assert that:
// - tid_t is defined (typedef) 
//   tid_t is similar to pid_t but identifies a thread  
// - tid_t gettid(void) function is available 
//   gettid() is similar to getpid() but retrieve the thread id in lieu of the process id.
//   NOTICE: gettid() might be a macro in lieu of a function


#endif //  __FLINT_TID_H_INCLUDED__
