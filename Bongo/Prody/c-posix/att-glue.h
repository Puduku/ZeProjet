// c-posix/att-glue.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose:
// --------
// ensure safe conversion between c-ansi/att.h formal types and (platform dependent) POSIX types:
//   + ATT_ID (g_ATT_ID_unsigned_int) vs. tid_t
//   + ATT_TIME (gen_ATT_TIME_long) vs. time_t

#ifndef __C_POSIX_ATT_GLUE_H_INCLUDED__
#define __C_POSIX_ATT_GLUE_H_INCLUDED__

#include <time.h>
#include <limits.h>

#include "c-ansi/att.h"
#include "c-posix/att-glue-kitchen.h"



// Transforms tid into att id
//
// Passed:
// - p_handle: (protected instance)
// - tid: thread id
//
// Ret:
// Corresponding att id
#define /*g_ATT_ID_unsigned_int*/ TID_2_ATT_ID(/*ATT_GLUE_HANDLE*/ p_handle, /*tid_t*/ tid) \
  K_TID_2_ATT_ID(p_handle,tid)

  
// Transforms att id into tid
//
// Passed:
// - p_handle: (protected instance)
// - attId : 
// 
// Ret: 
// Corresponding "thread id" (tid_t)
#define /*tid_t*/ ATT_ID_2_TID(/*ATT_GLUE_HANDLE*/ p_handle, /*g_ATT_ID_unsigned_int*/ id) \
  K_ATT_ID_2_TID(p_handle,id)

  
// Transforms alarm time into "epoch time" (time_t)
//
// Passed:
// - p_handle: (protected instance)
// - attTime:
//
// Returned:
// Corresponding "epoch time" (time_t)
#define /*time_t*/ ATT_2_EPOCH_TIME(/*ATT_GLUE_HANDLE*/ p_handle, /*gen_ATT_TIME_long*/ attTime) \
  K_ATT_2_EPOCH_TIME(p_handle,attTime) 


// Transforms "epoch time" (time-t) into alarm time
//
// Passed:
// - p_handle: (protected instance)
// - epochTime:
//
// Returned:
// Corresponding alarm time 
#define /*gen_ATT_TIME_long*/ EPOCH_2_ATT_TIME(/*ATT_GLUE_HANDLE*/ p_handle, /*time_t*/ epochTime) \
  K_EPOCH_2_ATT_TIME(p_handle,epochTime) 


struct ATT_GLUE ; // Private !
typedef struct ATT_GLUE *ATT_GLUE_HANDLE ;


// Create "att glue" instance.
// Verify @ run time that the ATT_..._2_... macros are working on the target...
// (i.e verify the glue is "good"...)
// 
// Passed: azhd_handle:
//
// Changed: *azh_handle: intitialized instance (thread safe) 
//
// Return:
// - RETURNED: ok (the handle asserts the runtime tests have passed...)
// - -1: ANOMALY is raised
int AttGlueCreateInstance (ATT_GLUE_HANDLE *azh_handle) ;


// Return:
// - RETURNED:
// - -1: ANOMALY is raised
int AttGlueDestroyInstance(ATT_GLUE_HANDLE xh_handle) ;


#endif // __C_POSIX_ATT_GLUE_H_INCLUDED__
