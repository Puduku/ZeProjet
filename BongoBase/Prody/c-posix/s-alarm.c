// c-posix/s-alarm.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: "alarm" implementation  for "standard" (i.e non "pthread") POSIX programs
// =======

#include "c-posix/s-alarm.h"

#include <unistd.h>
#include <limits.h>

#include "c-ansi/diggy.h"

struct ALARM_SYSTEM {
  int cpt ;
} ;

static struct ALARM_SYSTEM uniqueAlarmSystem = { .cpt = 0 };




static void  SigalrmHandler (int signum) {
  uniqueAlarmSystem.cpt++ ;
} //SigalrmHandler

static ALARM_SYSTEM_HANDLE n_uniqueAlarmSystemHandle = NULL;


// Public function: see description in .h
int StandardAlarmSystemCreateInstance (ALARM_SYSTEM_HANDLE *azdh_handle) {
  m_DIGGY_BOLLARD()

  const struct sigaction sigAction = {
    .sa_handler = SigalrmHandler  ,
    //.sa_mask = 0 ,
    .sa_flags = SA_RESTART ,
  } ;

  m_ASSERT(n_uniqueAlarmSystemHandle == NULL)

  m_PERROR_VERBATIM_IF(sigaction(SIGALRM,&sigAction,NULL) < 0)

  *azdh_handle = n_uniqueAlarmSystemHandle = &uniqueAlarmSystem ;

  m_DIGGY_RETURN(RETURNED)
} // StandardAlarmSystemCreateInstance


// Public function ; see description in .h
int SetAlarm (ALARM_SYSTEM_HANDLE handle, int seconds) {
  m_DIGGY_BOLLARD()
  unsigned int ret =  alarm((unsigned int)seconds);

  if ( ((int)ret) < 0) {
    return INT_MAX;
  } // if

  m_DIGGY_RETURN((int)ret)
} // SetAlarm



// Public function ; see description in .h
int StandardAlarmSystemDestroyInstance (ALARM_SYSTEM_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(n_uniqueAlarmSystemHandle == xh_handle)
  n_uniqueAlarmSystemHandle = NULL;

  m_DIGGY_RETURN(RETURNED)
} // StandardAlarmSystemDestroyInstance


