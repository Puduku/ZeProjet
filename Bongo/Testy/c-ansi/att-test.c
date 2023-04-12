#include <stdio.h>

#include "c-ansi/stderr.h"
#include "c-ansi/att.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


static int RingRing (void *r_handle, g_ATT_ID_unsigned_int attId, unsigned long delay) {
  m_DIGGY_BOLLARD()
  m_DIGGY_VAR_D(attId)
  m_DIGGY_VAR_GEN(delay,lu)
  
  m_DIGGY_RETURN(RETURNED)
} // RingRing


int main (int argc, char **argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  gen_ATT_TIME_long attTime = UNDEFINED, ancientAttTime = UNDEFINED;
  ALARM_TIMETABLE_HANDLE handle = (ALARM_TIMETABLE_HANDLE) UNDEFINED;

  m_TRACK_IF(AlarmTimetableCreateInstance(&handle) != RETURNED)

  m_DIGGY_INFO("Registering alarms 1969:69, 1070:70,... => 1978:78...")
  int i = UNDEFINED;
  for (i = 0; i < 10; i++) {
    m_TRACK_IF((ret = AlarmTimetableProgram(handle,69+i,b_NEW_ALARM,1969+i,&ancientAttTime)) < 0)
    m_RAISE_VERBATIM_IF(ret != RESULT__NOT_FOUND)
  } // for

  m_DIGGY_INFO("Cancelling alarms 1974:74 => 1978:78..")
  for (i = 5; i < 10; i++) {
    m_TRACK_IF((ret = AlarmTimetableProgram(handle,69+i,b_REMOVE_ALARM,UNDEFINED,&ancientAttTime)) 
               < 0)
    m_RAISE_VERBATIM_IF(ret != RESULT__FOUND)
    m_RAISE_VERBATIM_IF(ancientAttTime != 1969+i)
  } // for
  m_TRACK_IF((ret = AlarmTimetableGetRefreshTime(handle,&attTime)) < 0)
  m_RAISE_VERBATIM_IF(ret != RESULT__FOUND)
  m_RAISE_VERBATIM_IF(attTime != 1969)
  
  m_TRACK_IF((ret = AlarmTimetableRefresh(handle,1967,RingRing,NULL)) < 0) 
  m_RAISE_VERBATIM_IF(ret != 0)
  m_TRACK_IF((ret = AlarmTimetableGetRefreshTime(handle,&attTime)) < 0)
  m_RAISE_VERBATIM_IF(ret != RESULT__FOUND)
  m_RAISE_VERBATIM_IF(attTime != 1969)
  
  m_TRACK_IF((ret = AlarmTimetableRefresh(handle,1971,RingRing,NULL)) < 0)
  m_RAISE_VERBATIM_IF(ret != 3)
  m_TRACK_IF((ret = AlarmTimetableGetRefreshTime(handle,&attTime)) < 0)
  m_RAISE_VERBATIM_IF(ret != RESULT__FOUND)
  m_RAISE_VERBATIM_IF(attTime != 1972)

  m_TRACK_IF((ret = AlarmTimetableRefresh(handle,1971,RingRing,NULL)) < 0) 
  m_RAISE_VERBATIM_IF(ret != 0)

  m_TRACK_IF((ret = AlarmTimetableRefresh(handle,ULTIMATE_ATT_TIME,RingRing,NULL)) < 0)
  m_RAISE_VERBATIM_IF(ret != 2)
  m_TRACK_IF((ret = AlarmTimetableGetRefreshTime(handle,&attTime)) < 0)
  m_RAISE_VERBATIM_IF(ret != RESULT__NOT_FOUND)

  m_TRACK_IF((ret = AlarmTimetableRefresh(handle,ULTIMATE_ATT_TIME,RingRing,NULL)) <0)
  m_RAISE_VERBATIM_IF(ret != 0)

  m_TRACK_IF(AlarmTimetableDestroyInstance(handle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main


