#include "c-posix/t-alarm.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"



#include <stdio.h>
#include <string.h>

static int Bidon(int cpt) {
  m_DIGGY_BOLLARD()
  cpt++ ;
  m_DIGGY_VAR_D(cpt)
  if (cpt >= 16) {
    m_DIGGY_RETURN(0)
  } // if
  Bidon(cpt) ;
  m_DIGGY_RETURN(0);
} // Bidon

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,MULTITHREADED__TESTY_FLAG) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;

  ALARM_SYSTEM_HANDLE h_handle = (ALARM_SYSTEM_HANDLE) UNDEFINED;

  m_TRACK_IF(PthreadAlarmSystemCreateInstance(&h_handle) != RETURNED)

  ret = SetAlarm(h_handle,10);
  m_TRACK_IF(ret < 0)
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret == 0)

  //Bidon(0) ;

  ret = SetAlarm(h_handle,2);
  m_TRACK_IF(ret < 0)
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret != 0)

  m_DIGGY_INFO("sleep(5)...")
  ret = sleep(5);
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret != 0)
  //Bidon(0) ;

  ret = SetAlarm(h_handle,3);
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret == 0)

  m_DIGGY_INFO("sleep(15)...")
  ret = sleep(15);
  m_DIGGY_VAR_D(ret)
  m_ASSERT(ret != 0)

  ret = PthreadAlarmSystemDestroyInstance(h_handle);
  m_TRACK_IF(ret != RETURNED)

  m_DIGGY_RETURN(0)
} // main
