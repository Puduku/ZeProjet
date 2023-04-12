// c-posix/t-alarm.h, version 1.93 
//
// Purpose: "alarm" implementation for multi-threaded POSIX programs
// =======
// Principle: provide alarm()-like service working with threads


#ifndef __C_POSIX_T_ALARM_H_INCLUDED
#define __C_POSIX_T_ALARM_H_INCLUDED

//TBC : pthread_kill() SIGSEGV when no thread 

#include <unistd.h>
#include <pthread.h>

#include "c-ansi/types.h"

#include "c-posix/alarm.h"


// Install "alarm system" compatible with multi-threaded POSIX programs.
//
// Passed:
// - azdh_handle: (pointer to) "alarm system" handle to initialize
//   Note: ONLY one such instance can be created in a program ("thread-safe" singleton)
//   (the handle is only here to avoid singleton design issue...)
//
// Modified
// - *azdh_handle: "alarm system" handle
//
// Returned
// - RETURNED: "alarm system" installed...
// - -1 : unexpected problem ; anomaly is raised
int PthreadAlarmSystemCreateInstance (ALARM_SYSTEM_HANDLE *azdh_handle);


// Schedule a timeout for the current thread.
// The timeout would be materialized by the delivery of a SIGALRM signal to the thread.
//
// Passed:
// - p_handle: protected handle ("thread-safety")
// - seconds: (only >= 0 values are expected)
//  + 0: cancel alarm
//  + >0: new alarm 
//
// Return:
// - 0: no previous alarm 
// - > 0: remaining number of seconds corresponding to ancient alarm  
// - -1 : unexpected problem ; anomaly is raised
int SetAlarm (ALARM_SYSTEM_HANDLE p_handle, int seconds) ;


// Liquidate the alarm system...
// Note: does NOT "uninstall" the SIGALRM signal handler...
//
// Passed:
// - xhj_handle: the alarm system to liquidate...
// 
// Modified
// - xhj_handle: no more usable ; NOTICE: on platforms on which 
//   so-called "robust" pthread mutexes are NOT available, the internal 
//   mutex instancied by the alarm system is NOT destroyed ;
//   that "junk" resource will remain on the system till process death...   
//
// Returned
// - RETURNED: desinstallation done...
// - -1 : unexpected problem ; anomaly is raised
int PthreadAlarmSystemDestroyInstance (ALARM_SYSTEM_HANDLE xhj_handle);

#endif // __C_POSIX_T_ALARM_H_INCLUDED


