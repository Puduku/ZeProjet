// c-posix/alarm.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

// Purpose: Universal alarm()-equivalent service
// =======
// aim: insulate an inherent incompatibility between traditional (non multi-threaded)
// and multi-threaded (pthread-dependent) POSIX applications.

#ifndef __C_POSIX_ALARM_H_INCLUDED__
#define __C_POSIX_ALARM_H_INCLUDED__

#include <unistd.h>
#include <signal.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"


// This module defines an alarm()-equivalent service...
// However, it's just an interface definition ; there is no implementation "behind"...
// the different implementations are expected to follow this canvas:
// DECLARATION     alarm.h => s-alarm.h / t-alarm.h
// IMPLEMENTATION  n/a        s-alarm.c / t-alarm.c
// PROGRAM TYPE    n/a        standard  / pthread


// That (private) data structure depends on the implementation:
struct ALARM_SYSTEM ;
typedef struct ALARM_SYSTEM *ALARM_SYSTEM_HANDLE ;


// Schedule a timeout for the current process / thread.
// Functionally, the function is identical to alarm() system call.
// That is, the timeout is materialized by the delivery of a SIGALRM signal to the
// process / thread.
//
// Passed:
// - handle:
// - seconds: (only >= 0 values are expected)
//   + 0: cancel
//   + >0: new schedule
//
// Return:
// - 0: no previous alarm 
// - > 0: remaining number of seconds corresponding to previous alarm 
// - -1: unexpected problem; anomaly is raised
int SetAlarm (ALARM_SYSTEM_HANDLE handle, int seconds) ;



#endif // __C_POSIX_ALARM_H_INCLUDED__
