// c-posix/tid-glue.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose:
// --------
// - ensure conversion between  tid_t (thread id) and "opaque" pthread_t data types...

#ifndef __C_POSIX_TID_GLUE_H_INCLUDED__
#define __C_POSIX_TID_GLUE_H_INCLUDED__

#include <pthread.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

// Ensures availability of tid_t definition:
#include "flint/tid.h"

#include "c-ansi/types.h"
#include "c-ansi/stderr.h"

struct TID_GLUE ; // Private !
typedef struct TID_GLUE *TID_GLUE_HANDLE ;

#define b_PORTABLE b_TRUE
#define b_DIRECT b_FALSE0


// Create "tid glue" instance.
// Allows (in real time) to map univocally a pthread_t "blob" with a tid_t
// Because of platforms pecularities nightmare, we propose two implementations:
// - "generic" implementation that is based on a trick:
//   in lieu of really converting pthread_t content into tid_t (or vice-versa),
//   The "Tid glue" simply registers passed pthread_t, supposedly beeing the CURRENT
//   thread's pthread_t, performs an internal gettid()-equivalent call, and registers
//   that correspondance in an index.
//   To make the opposite conversion (tid_t into pthread_t) the "tid glue" simply consults
//   the index... Of course, it would not work for pthead_t that were not previously
//   "submitted" in the "pthread_t to tid_t" pseudo conversion...
// - "direct", implementation performs "direct" conversion between pthread_t and tid_t.
//   This implementation is of course more efficient, but is not possible on all platforms
//   (i.e platforms on which pthread_t is really a "blob"...). 
//
// Passed:
// - b_portable :
//   + True (b_PORTABLE) : generic implementation ; not very efficient, but available on all platforms...
//   + False (b_DIRECT) : most efficient implementation, but not possible everywhere...
// 
// Changed:
// - *azh_handle: new instance's handle 
//   + "portable" implementation: NOT "thread-safe" 
//   + "direct" implementation: thread-safe
//
// Return:
// - RETURNED:
// - -1: ANOMALY is raised
int TidGlueCreateInstance (TID_GLUE_HANDLE *azh_handle, int b_portable) ;


// Functionnally, the function simply converts "self" pthread_t into "sane" tid_t.
// - "generic" implementation: registers self pthread and maps it with current thread 's tid.
// - "direct" implementation: simply converts pthread_t content into tid_t. 
// 
// Passed:
// - cp_handle: ONLY "direct" implementation  protects instance (i.e thread-safe)  
//
// Changed:
// -  *a_tid
//
// Return:
// - RETURNED: ok, (pthread registered against tid on "generic" implementation)
// - -1: ANOMALY is raised
int TidGlueSelfPthread2Tid (TID_GLUE_HANDLE cp_handle, tid_t *a_tid) ;


// Retrieve pthread corresponding to passed tid (as it was "registered" with function
// TidGluePthread2Tid() above on "generic" implementation...)
// 
// Passed:
// - cp_handle: ONLY "direct" implementation  protects instance (i.e thread-safe)  
//
// Return:
// - RESULT__FOUND: Ok,
// - RESULT__NOT_FOUND: that thread is NOT "registered" ; that case CANNOT happen with "direct"
//   implementation (consider raising ANOMALY__PTHREAD_NOT_FOUND)
// - -1: ANOMALY is raised
int TidGlueTid2Pthread (TID_GLUE_HANDLE cp_handle, tid_t tid, pthread_t *ac_pthread) ;

// You can raise that anomaly if the tid glue has not retrieved a pthread...
#define ANOMALY__PTHREAD_NOT_FOUND "pthread not found ???"


// Return:
// - RETURNED:
// - -1: ANOMALY is raised
int TidGlueDestroyInstance(TID_GLUE_HANDLE xh_handle) ;


#endif // __C_POSIX_TID_GLUE_H_INCLUDED__
