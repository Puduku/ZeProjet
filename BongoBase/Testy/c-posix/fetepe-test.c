// sucker-test.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/types.h"
#include "c-posix/fetepe.h"
//#include "c-posix/s-alarm.h"
#include "c-posix/t-alarm.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


#include <stdio.h>

// FETEPE_LOG_COMMAND_STATUS_FUNCTION
static int FetepeTestLogCommandStatus ( void *r_handle, struct STRING_PORTION commandStatusLine,
  char b_lastLine) {
  m_DIGGY_BOLLARD()
  m_DIGGY_VAR_GEN(commandStatusLine.string, .10s)
  m_DIGGY_VAR_D(b_lastLine)
  m_DIGGY_RETURN(RETURNED)
} // FetepeTestLogCommandStatus


int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  FETEPE_HANDLE h_handle = (FETEPE_HANDLE) UNDEFINED;
  ALARM_SYSTEM_HANDLE h_alarmSystemHandle = (ALARM_SYSTEM_HANDLE) UNDEFINED;
  BROKEN_PIPE_FIX_HANDLE h_brokenPipeFixHandle = (BROKEN_PIPE_FIX_HANDLE) UNDEFINED;
  int connectErrno = UNDEFINED;

  //m_TRACK_IF(StandardAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(PthreadAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixCreateInstance(&h_brokenPipeFixHandle) != RETURNED)

  m_TRACK_IF(FetepeCreateInstance(&h_handle,h_alarmSystemHandle,h_brokenPipeFixHandle,
    10000000,FetepeTestLogCommandStatus,NULL) != RETURNED)

  ret = FetepeConnect(h_handle,"172.19.105.164","pbo","julie69",&connectErrno);
  m_TRACK_IF(ret < 0)
  m_DIGGY_VAR_D(ret)
  const char *rep = FetepeGetClue(h_handle);
  m_DIGGY_VAR_STRING(rep)

  ret = FetepePwd(h_handle) ;
  m_TRACK_IF(ret < 0)

//  ret = FetepePassive(h_handle) ;
//  m_TRACK_IF(ret < 0)

  ret = FetepeBinary(h_handle);
  m_TRACK_IF(ret < 0)

  int getLength = UNDEFINED;
  ret = FetepeGet(h_handle,"a.out","a3.out",&getLength) ;
  m_TRACK_IF(ret == -1)
  m_DIGGY_VAR_D(ret)
  m_DIGGY_VAR_D(getLength)
  rep = FetepeGetClue(h_handle);
  m_DIGGY_VAR_STRING(rep)

  G_STRINGS_HANDLE h_fileNames = (G_STRINGS_HANDLE) UNDEFINED;
  m_TRACK_IF(G_STRINGS_CREATE_INSTANCE(&h_fileNames,100) != RETURNED)

  ret = FetepeNlist(h_handle,NULL,0,h_fileNames) ;
  m_TRACK_IF(ret < 0)
  m_DIGGY_VAR_D(ret)

  G_STRING_STUFF fileName = (G_STRING_STUFF) UNDEFINED;
  int count = GStringsGetCount(h_fileNames,&fileName) ;
  m_TRACK_IF(count < 0) ;
  m_DIGGY_VAR_D(count)
  int i = 0;
  for (; i < count ; i++) {
     m_TRACK_IF(GStringsFetch(h_fileNames,i,&fileName) < 0)
  } // for

  m_TRACK_IF(GStringsDestroyInstance(h_fileNames) != RETURNED)

  m_TRACK_IF(FetepeDisconnect(h_handle) != RETURNED)

  m_TRACK_IF(FetepeDestroyInstance(h_handle) != RETURNED)

  m_TRACK_IF(BrokenPipeFixDestroyInstance(h_brokenPipeFixHandle) != RETURNED)
  //m_TRACK_IF(StandardAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(PthreadAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
