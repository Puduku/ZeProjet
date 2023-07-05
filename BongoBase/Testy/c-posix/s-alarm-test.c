#include "c-posix/s-alarm.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


#include <stdio.h>

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  ALARM_SYSTEM_HANDLE h_handle = (ALARM_SYSTEM_HANDLE) UNDEFINED;

  ret = StandardAlarmSystemCreateInstance(&h_handle) ;
  m_TRACK_IF(ret != RETURNED)

  ret = SetAlarm(h_handle,5);
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == 0)

  ret = SetAlarm(h_handle,1);
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret != 0)

  m_DIGGY_INFO("sleep(10)...")
  ret = sleep(10);
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret >= 8)

  ret = StandardAlarmSystemDestroyInstance(h_handle);
  m_TRACK_IF(ret != RETURNED)


  m_DIGGY_RETURN(0)
} // main
