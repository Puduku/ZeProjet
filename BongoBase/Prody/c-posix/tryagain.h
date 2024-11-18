// c-posix/tryagain.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

// Purpose: temporization support for POSIX system calls
// =======
// Provide generic wrapper suitable to **any** system call that can be interrupted (and thus
// controlled) by a signal.
// By the way, you "protect" your system calls against awkward signal interruptions...

#ifndef __C_POSIX_TRYAGAIN_H_INCLUDED__
#define __C_POSIX_TRYAGAIN_H_INCLUDED__

////////////////////////////////
#include "c-posix/tryagain.topo"
////////////////////////////////
#include <unistd.h>
#include <signal.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"

#include "c-posix/alarm.h"


// (Resource access) "waiting plans"
// ---------------------------------

enum {
  NON_BLOCKING__WAITING_PLAN, // #REF enum-WAITING_PLAN <resource>
  // Don't wait if "the <resource> is not immediately available" (in other words, try accessing the
  // <resource> WITHOUT active temporization)
  // Note: this mode does NOT mean the underlying <primitive> (alias "system call") may not suspend
  // the process / thread beforehand (see c-posix/tryagain.topo)...
  //
  // Blocking modes :
  // if the <resource> is not directly available, block actively the process / thread till the
  // <resource> is finally available or ...
  LIGHTLY_BLOCKING__WAITING_PLAN, //
  // ...the underlying system call is "interrupted" (for any reason...)
  DEEPLY_BLOCKING__WAITING_PLAN //
  // ...a STRICT deadline has expired
} ;


// Wrapping system calls
// ---------------------

// "waiting plan" parameters defined for your wrapping <function>
struct WAITING_PLAN { 
  int waitingPlan ; // #REF struct-WAITING_PLAN <resource>
  // If <resource> is currently not available, <function>...
  // - NON_BLOCKING__WAITING_PLAN: ...returns "immediately" with "TRY AGAIN!" status.
  // - LIGHTLY_BLOCKING__WAITING_PLAN: ...blocks till either <resource> is available or
  //   the system call waiting for <resource> is "interrupted" (*)
  // - DEEPLY_BLOCKING__WAITING_PLAN: ...blocks till either <resource> is available or
  //   after a STRICT deadline expiration.
  int c_deadline ; // Only significant for blocking waiting plan ;
  // delay, in seconds (**) of timeout (0 for infinite, that is "no deadline")

  // (*) the system call can be "interrupted" : 
  // - by a signal (in particular the "timeout" signal raised according to c_deadline) 
  // - by own timeout : see setsockopt(SO_xxxTIMEO) option (in the case of socket descriptor)
  // (**) : because of the potential "system latency" of the system calls on which we have no
  // control, there is NO POINT to express such deadline more precisely...
} ;

// Establish (regular) waitin plan.
static inline struct WAITING_PLAN m_WaitingPlan(int waitingPlan, int c_deadline) {
  struct WAITING_PLAN _waitingPlan = { .waitingPlan = waitingPlan, .c_deadline = c_deadline,};
  return _waitingPlan;
} // m_WaitingPlan



// PD (pretty descriptor) : offers temporization support for file descriptors...

// private structure...
struct _sps2_PD ;
typedef struct _sps2_PD *PD_HANDLE ;


// Create and initialize a pretty descriptor (instance) 
//
// Passed:
// - azh_handle: address of PD handle to initialize...
// - nf_alarmSystemHandle : alarm system used for temporizations 
//   + NULL special pointer (not provided) ; limited functionality ; no temporization is possible ;
//     all blocking access requests are de facto indefinite...
//   + non NULL pointer: actual alarm system (see c-posix/alarm.h) ; 100 % functionality with
//     temporizations... 
//
// Modified:
// - *azh_handle: handle initialized
//
// Ret:
// - RETURNED: OK
// - -1: unexpected problem ; anomaly is raised
int PdCreateInstance (PD_HANDLE *azh_handle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle) ;


// This "util" function adapts the O_NONBLOCK flag of the descriptor according to the waiting plan.
// That adaptation is needed when wrapping most of the "descriptor-based" <primitives> (such as
// read(), write() and accept()).
// However, you DON'T NEED to adapt the O_NONBLOCK flag for some specific system calls, such as
// fcntl(F_SETFL / F_SETFLW))
//
// Returned:
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int SynchronizeONonblock (int descriptor, struct WAITING_PLAN waitingPlan) ;


// Prepare wrapping <function> according to "waiting plan".
// => when wrapping most of the "descriptor-based" <primitives> (such as
// read(), write() and accept()) : see SynchronizeONonblock( above
//
// Passed:
// - handle: see PdCreateInstance()
// - waitingPlan: waiting plan to the resource (reachable with the system call) ;
//
// Return:
// - RETURNED:
// - -1: unexpected problem ; anomaly is raised
int PdSetDeadline (PD_HANDLE handle, struct WAITING_PLAN waitingPlan) ;



// To be called IN FRONT of any call to the wrapped <primitive>...
// In "blocking waiting plan", schedule (signal-based) alarm ;
//
// Passed:
// - handle: see PdCreateInstance()
//
// Returned:
// - RETURNED: OK
// - -1: unexpected problem ; anomaly is raised
int PdSetAlarm (PD_HANDLE handle) ;


// Check whether there is a "timeout" (i.e the "deadline" has expired).
// To be call in case of "untimely" return of the wrapped <primitive> (typically when errno is
// EINTR, EAGAIN or EWOULDBLOCK)
//
// Passed:
// - handle: see PdCreateInstance()
//
// Returned: timeout ?
// - ANSWER__NO: (only possible in deeply blocking waiting plan) no timeout ; resume the system call...
// - ANSWER__YES: timeout (i.e in deeply blocking waiting plan, deadline has expired) => "try again!"
// - -1: unexpected problem ; anomaly is raised
int PdCheckTimeout (PD_HANDLE handle) ;


// In "blocking waiting plan", cancel "timeout" signal delivery. 
// To be called before returning from the function wrapping the system call... 
//
// Passed:
// - handle: see PdCreateInstance()
//
// Returned:
// - RETURNED: OK
// - -1: unexpected problem ; anomaly is raised
int PdClearAlarm (PD_HANDLE handle) ;


// Release memory taken by PD instance
//
// Passed:
// - xh_handle: handle to liquidate
//
// Modified:
// - xh_handle: this handle is no more utilizable.
//
// Ret:
// - RETURNED: OK
// - -1: unexpected problem ; anomaly is raised
int PdDestroyInstance (PD_HANDLE xh_handle) ;


// Preserve the CPU!!
// ------------------

// => That macro is meant when you continuously recall a <primitive> (or its wrapping <function>...)
// within a loop till it "succeeds"...
// The purpose is simply to avoid your program hogging the cpu...
// A good use case of that macro is within the "wrapping loop" of <primitives> ! (see above)
//
// Passed:
// - errno: corresponding to <primitive> failure.
//
// Returned:
// - true: need to preserve CPU ! that is, add a small delay (like one tenth of second...) before
//   calling the primitive again ; use for instance SLEEP_JUST_TO_AVOID_CPU_HOGGING() below... 
// - false: the <primitive> actually "blocked" before failing... hence you need NOT to observe
//   delay before calling the <primitive> again...
#define b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno) \
  ((errno) != EINTR)


// => If you don't know how long your process should sleep when you have to preserve CPU, then use
// that macro...
#define SLEEP_JUST_TO_AVOID_CPU_HOGGING() usleep(100000)


#endif // __C_POSIX_TRYAGAIN_H_INCLUDED__
