// c-ansi/sucker-test.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include <stdio.h>

#include "c-ansi/types.h"
#include "c-ansi/g-string.h"
#include "c-posix/sucker.h"
#include "c-posix/s-alarm.h"
//#include "c-posix/t-alarm.h"
#include "c-posix/rwf.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"



#define DUMMY_ORIG_FILENAME "prout-orig"
#define DUMMY_COPY_FILENAME "prout-copy"


int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,SANDBOX__TESTY_FLAG | DATADIR__TESTY_FLAG) ;
  G_STRING_STUFF h_dummySFilePathnameStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&h_dummySFilePathnameStuff) != RETURNED)
  m_TRACK_IF(GStringPrintf(h_dummySFilePathnameStuff,0,"%s/%s", p_testyDatadirPath,
    DUMMY_ORIG_FILENAME) < 0)
  const char *dummyCopyFilePathname = DUMMY_COPY_FILENAME;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  SUCKER_HANDLE h_handle = (SUCKER_HANDLE) UNDEFINED;
  ALARM_SYSTEM_HANDLE h_alarmSystemHandle = (ALARM_SYSTEM_HANDLE) UNDEFINED;
  BROKEN_PIPE_FIX_HANDLE h_brokenPipeFixHandle = (BROKEN_PIPE_FIX_HANDLE) UNDEFINED;

  m_TRACK_IF(StandardAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  //m_TRACK_IF(PthreadAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixCreateInstance(&h_brokenPipeFixHandle) != RETURNED)
  m_TRACK_IF(SuckerCreateInstance(&h_handle, 10000000) != RETURNED)

  int suckedLength = UNDEFINED;
  int r_flopCause = UNDEFINED;
  int openErrno = UNDEFINED;

  ret = SuckerCopy(h_handle,h_alarmSystemHandle,h_brokenPipeFixHandle,
    h_dummySFilePathnameStuff->nhi_string,dummyCopyFilePathname,
    &suckedLength,&r_flopCause) ;
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == SUCKER_STATUS__OK)

  int dummyCopyFile = UNDEFINED;
  int answer = ProtectedOpenFile(dummyCopyFilePathname, READ__BASIC_OPEN_FILE_MODE,
    BATEAU__FILE_CREATION_MODE, &dummyCopyFile, &openErrno);
  m_ASSERT(answer == ANSWER__YES)
  STREAM_BUTT_ADEPT_HANDLE h_streamSButtAdeptHandle = (STREAM_BUTT_ADEPT_HANDLE)UNDEFINED;
  m_TRACK_IF(StreamButtAdeptCreateInstance(&h_streamSButtAdeptHandle,
    h_alarmSystemHandle, h_brokenPipeFixHandle,
    p_fileStreamSimpleWaitingPlan, 1024, dummyCopyFile) != RETURNED)

  G_STRING_STUFF h_gStringStuff = (G_STRING_STUFF) UNDEFINED;
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&h_gStringStuff) != RETURNED)
  G_STRING_BUTT_ADEPT_HANDLE h_gStringButtAdeptHandle = (G_STRING_BUTT_ADEPT_HANDLE)UNDEFINED;
  m_TRACK_IF(GStringButtAdeptCreateInstance(&h_gStringButtAdeptHandle,h_gStringStuff) != RETURNED)

  ret = SuckerSuckOut(h_handle,&suckedLength,&r_flopCause) ;
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == SUCKER_STATUS__OK)
  m_ASSERT(suckedLength == h_gStringStuff->c_copiedLength) 
  m_ASSERT(suckedLength == 1668) // data/prout-orig file size 

  m_TRACK_IF(SuckerDestroyInstance(h_handle) != RETURNED)
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_gStringStuff) != RETURNED)
  m_TRACK_IF(BrokenPipeFixDestroyInstance(h_brokenPipeFixHandle) != RETURNED)
  m_TRACK_IF(StandardAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)
  //m_TRACK_IF(PthreadAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_dummySFilePathnameStuff) != RETURNED)
  m_TRACK_IF(StreamButtAdeptDestroyInstance(h_streamSButtAdeptHandle) != RETURNED)
  m_TRACK_IF(GStringButtAdeptDestroyInstance(h_gStringButtAdeptHandle) != RETURNED)
  
  m_DIGGY_RETURN(SUCCESS__EXECUTIVE__EXIT_STATUS)
} // main
