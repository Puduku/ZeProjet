#include "c-posix/s-alarm.h"
#include "c-posix/tryagain.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


#include <stdio.h>

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  ALARM_SYSTEM_HANDLE h_alarmSystemHandle = (ALARM_SYSTEM_HANDLE) UNDEFINED;
  PD_HANDLE h_handle = (PD_HANDLE) UNDEFINED;
  struct WAITING_PLAN waitingPlan = { .waitingPlan = DEEPLY_BLOCKING__WAITING_PLAN , .c_deadline = 1 } ;

  m_TRACK_IF(StandardAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)

  m_TRACK_IF(PdCreateInstance(&h_handle,h_alarmSystemHandle) != RETURNED)

  m_DIGGY_VAR_D(waitingPlan.waitingPlan)
  m_TRACK_IF(PdSetDeadline(h_handle,&waitingPlan) != RETURNED)

  m_TRACK_IF(PdSetAlarm(h_handle) != RETURNED)
  ret = PdCheckTimeout(h_handle) ;
  m_TRACK_IF(ret < 0)
  m_RAISE_VERBATIM_IF(ret != ANSWER__NO)

  m_TRACK_IF(PdSetAlarm(h_handle) != RETURNED)
  ret = PdCheckTimeout(h_handle) ;
  m_TRACK_IF(ret < 0)
  m_RAISE_VERBATIM_IF(ret != ANSWER__NO)

  m_DIGGY_INFO("sleep(1)...")
  sleep(1);
  m_TRACK_IF(PdSetAlarm(h_handle) != RETURNED)
  ret = PdCheckTimeout(h_handle) ;
  m_TRACK_IF(ret < 0)
  m_RAISE_VERBATIM_IF(ret != ANSWER__YES)

  m_TRACK_IF(PdClearAlarm(h_handle) != RETURNED)
  m_TRACK_IF(PdDestroyInstance(h_handle) != RETURNED)

  m_TRACK_IF(StandardAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
