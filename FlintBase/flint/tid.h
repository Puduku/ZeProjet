// flint/tid.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: provide support for plain threads ids 
// =======

#ifndef __FLINT_TID_H_INCLUDED__
#define __FLINT_TID_H_INCLUDED__


#include "flint/tid-kitchen.h" 
// That #include ensures that: 
//
// - tid_t is defined (typedef) 
//   tid_t is similar to pid_t but identifies a thread  
// - tid_t gettid(void) function is available 
//   gettid() is similar to getpid() but retrieve the thread id in lieu of the process id.
//   NOTICE: gettid() might be a macro in lieu of a function



#endif //  __FLINT_TID_H_INCLUDED__
