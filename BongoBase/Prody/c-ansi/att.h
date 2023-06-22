// c-ansi/att.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023
//
// Purpose:
// --------
// - Alarm TimeTable (provide the "logic" to schedule system alarms...)

#ifndef __C_ANSI_ATT_H_INCLUDED__
#define __C_ANSI_ATT_H_INCLUDED__

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include <limits.h>

typedef long gen_ATT_TIME_long ;
typedef unsigned int g_ATT_ID_unsigned_int ;

// ATT_TIME type: associated constants:
// - PRIMEVAL_ATT_TIME => min alarm time value
// - ULTIMATE_ATT_TIME => max alarm time value
#define PRIMEVAL_ATT_TIME LONG_MIN
#define ULTIMATE_ATT_TIME LONG_MAX


struct ALARM_TIMETABLE ; // Private!
typedef struct ALARM_TIMETABLE *ALARM_TIMETABLE_HANDLE ;


// Create (and initialize) alarms timetable.
//
// Passed:
// *azh_handle: UNINITIALIZED handle
//
// Modified:
// *azh_handle: new alarm timetable initialized; its handle
//
// Returned:
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int AlarmTimetableCreateInstance (ALARM_TIMETABLE_HANDLE *azh_handle) ;



#define b_NEW_ALARM  b_TRUE
#define b_REMOVE_ALARM b_FALSE0

// Add / update, or cancel alarm in the timetable.
//
// Passed:
// - handle: see AlarmTimetableCreateInstance()
// - attId: alarm id
// - b_newAlarm : boolean value:
//   + true: program new alarm (=> Add or update)
//   + false: do not program new alarm (=> Delete)
// - c_newAttTime: only significant if a new alarm is programmed ;
//   alarm time
// - nac_ancientAttTime: NULL address if not used 
// 
// Modified:
// -*nac_ancientAttTime: (if used) only significant in case of "update" (i.e "FOUND")
//  ancient alarm time correspoonding to the id
//
// Ret: Has alarm been updated ?
// - RESULT__FOUND: "updated" => *nac_ancientAttTime indicates ancient alarm time
// - RESULT__NOT_FOUND: "no update" (new alarm) => *nac_ancientAttTime NOT significant
// - -1: unexpected problem ; anomaly is raised
int AlarmTimetableProgram (ALARM_TIMETABLE_HANDLE handle, g_ATT_ID_unsigned_int attId,
  char b_newAlarm, gen_ATT_TIME_long c_newAttTime, gen_ATT_TIME_long *nac_ancientAttTime) ;
 

// Search "next" alarm in the timetable,
// 
// Passed:
// - handle: see AlarmTimetableCreateInstance()
//
// Modified:
// - *ac_refreshAttTime: only significant is the time table is NOT empty ("FOUND" result) ;
//   ideal time to undertake a "refresh" of the timetable - see AlarmTimetableRefresh()
//
// Ret: Is there at least one alarm in the timetable ? 
// - RESULT__FOUND: timetable is not empty
// - RESULT__NOT_FOUND: timetable is empty
// - -1: unexpected problem ; anomaly is raised
int AlarmTimetableGetRefreshTime (ALARM_TIMETABLE_HANDLE handle,
  gen_ATT_TIME_long *ac_refreshAttTime) ;


// #REF RING_ALARM_FUNCTION - Custom function: called for each "expired" alarm...
// 
// Passed:
// - r_handle: private handle
// - attId:
// - delay: >=0 ; difference between "trigger" and "scheduled" times
//
// Returned:
// - RETURNED:
// - -1: unexpected problem ; anomaly is raised
typedef int (RING_ALARM_FUNCTION) (void *r_handle, g_ATT_ID_unsigned_int attId,
  unsigned long delay) ;


// For each expired alarm:
// - ring alarm
// - remove alarm from timetable
//
// Passed:
// - handle: see AlarmTimetableCreateInstance()
// - refreshAlarmTime: indicates "present" time ; all alarms scheduled "before" that time have
//   expired and will "ring".
//   NOTE: specify ULTIMATE_ATT_TIME to ring all alarms and "clear" the timetable.
// - ringAlarmFunction: called when an alarm expires... 
// - r_ringAlarmFunctionHandle: passed to ringAlarmFunction
//
// Returned:
// - >= 0: number of "rung" alarms... 
// - -1: unexpected problem ; anomaly is raised
int AlarmTimetableRefresh (ALARM_TIMETABLE_HANDLE handle, gen_ATT_TIME_long refreshAttTime,
  RING_ALARM_FUNCTION ringAlarmFunction, void *r_ringAlarmFunctionHandle) ;


// Liquidate the timetable and release the memory.
// NOTE: pending alarms are NOT rung...
//
// Passed:
// - xh_handle: the instance to liquidate...
// 
// Modified:
// - xh_handle: this handle is no more usable
//
// Returned:
// - RETURNED:
// - -1: unexpected problem ; anomaly is raised
int AlarmTimetableDestroyInstance (ALARM_TIMETABLE_HANDLE xh_handle) ;

#endif
