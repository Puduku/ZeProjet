// c-posix/s-alarm.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: "alarm" implementation  for "standard" (i.e mono-pthread) POSIX programs
// =======

#ifndef __C_POSIX_S_ALARM_H_INCLUDED
#define __C_POSIX_S_ALARM_H_INCLUDED

#include "c-ansi/types.h"
#include "c-posix/alarm.h"


// => Notice: this alarm()-based implementation would not work satisfactorily for multi-threaded
// applications because the of alarm clock that is unique for the process...


// Install "alarm system" compatible with "standard" (i.e NO pthread) POSIX programs.
// "Standard" alarm installation simply consists in :
// - catching timeout (alias SIGALRM) signal
// - implementing SetlAlarm() as a simple alarm() wrapper...
// Note: you need not (and cannot!) install more than one "alarm system" instance per application /
// process... (singleton) 
//
// Passed:
// - azdh_handle:
//
// Modified: 
// - *azdh_handle: standard "alarm system" handle initialized
//
// Returned
// - RETURNED: standard "alarm system" installed...
// - -1 : unexpected problem ; anomaly is raised
int StandardAlarmSystemCreateInstance (ALARM_SYSTEM_HANDLE *azdh_handle) ;



// Schedule a timeout for the current process.
// The timeout would be materialized by the delivery of a SIGALRM signal to the process.
// In other words, the function simply calls alarm() system call...
//
// Passed:
// - handle:
// - seconds: (only >= 0 values are expected)
//  + 0: cancel
//  + >0: new schedule
//
// Return:
// 0: no previous scheduling
// > 0: remaining number of seconds corresponding to previous scheduling
// -1: unexpected problem ; anomaly is raised
int SetAlarm (ALARM_SYSTEM_HANDLE handle, int seconds) ;



// Returned
// - RETURNED: desinstallation done...
// - -1 : unexpected problem
int StandardAlarmSystemDestroyInstance (ALARM_SYSTEM_HANDLE xh_handle);



#endif // __C_POSIX_S_ALARM_H_INCLUDED
