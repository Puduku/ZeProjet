// c-ansi/sucker-test.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003


#include <stdio.h>

#include "c-ansi/types.h"
#include "c-ansi/g-string.h"
#include "c-posix/sucker.h"
#include "c-posix/s-alarm.h"
//#include "c-posix/t-alarm.h"
#include "c-posix/rwf.h"
#include "c-posix/pathname.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"



#define DUMMY_ORIG_FILENAME "prout-orig"
#define DUMMY_COPY_FILENAME "prout-copy"


int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,SANDBOX__TESTY_FLAG | DATADIR__TESTY_FLAG) ;
  m_CREATE_FILE_PATHNAME_G_STRING(h_dummySFilePathnameStuff,p_testyDatadirPath,DUMMY_ORIG_FILENAME)
  const char *dummyCopyFilePathname = DUMMY_COPY_FILENAME;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  SUCKER_HANDLE h_handle = (SUCKER_HANDLE) UNDEFINED;
  ALARM_SYSTEM_HANDLE h_alarmSystemHandle = (ALARM_SYSTEM_HANDLE) UNDEFINED;
  BROKEN_PIPE_FIX_HANDLE h_brokenPipeFixHandle = (BROKEN_PIPE_FIX_HANDLE) UNDEFINED;

  m_TRACK_IF(StandardAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  //m_TRACK_IF(PthreadAlarmSystemCreateInstance(&h_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixCreateInstance(&h_brokenPipeFixHandle) != RETURNED)
  //m_TRACK_IF(SuckerCreateInstance(&h_handle,h_alarmSystemHandle,h_brokenPipeFixHandle, 1024,
  //  10000000) != RETURNED)
  m_TRACK_IF(SuckerCreateInstance(&h_handle, 10000000) != RETURNED)

  int suckedLength = UNDEFINED;
  int r_flopCause = UNDEFINED;
  int openErrno = UNDEFINED;

  ret = SuckerCopy(h_handle,h_alarmSystemHandle,h_brokenPipeFixHandle,
    h_dummySFilePathnameStuff->nhi_string,dummyCopyFilePathname,
    &suckedLength,&r_flopCause) ;
  m_TRACK_IF(ret < 0)
  m_RAISE_VERBATIM_IF(ret != SUCKER_STATUS__OK)

  int dummyCopyFile = UNDEFINED;
  int answer = ProtectedOpenFile(dummyCopyFilePathname, READ__BASIC_OPEN_FILE_MODE,
    BATEAU__FILE_CREATION_MODE, &dummyCopyFile, &openErrno);
  m_RAISE_VERBATIM_IF(answer != ANSWER__YES)
  //struct SUCKER_BUTT sSuckerButt;
  //m_ASSIGN_SUCKER_BUTT__FILE_STREAM(sSuckerButt,dummyCopyFile)
  STREAM_BUTT_SPOTTER_HANDLE h_streamSButtSpotterHandle = (STREAM_BUTT_SPOTTER_HANDLE)UNDEFINED;
  m_TRACK_IF(StreamButtSpotterCreateInstance(&h_streamSButtSpotterHandle,
    h_alarmSystemHandle, h_brokenPipeFixHandle,
    p_fileStreamSimpleWaitingPlan, 1024, dummyCopyFile) != RETURNED)

  G_STRING_STUFF h_gStringStuff = (G_STRING_STUFF) UNDEFINED;
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&h_gStringStuff) != RETURNED)
  //struct SUCKER_BUTT dSuckerButt;
  //m_ASSIGN_SUCKER_BUTT__G_STRING(dSuckerButt,h_gStringStuff)
  G_STRING_BUTT_SPOTTER_HANDLE h_gStringButtSpotterHandle = (G_STRING_BUTT_SPOTTER_HANDLE)UNDEFINED;
  m_TRACK_IF(GStringButtSpotterCreateInstance(&h_gStringButtSpotterHandle,h_gStringStuff) != RETURNED)

  ret = SuckerSuckOut(h_handle,&suckedLength,&r_flopCause) ;
  m_TRACK_IF(ret < 0)
  m_RAISE_VERBATIM_IF(ret != SUCKER_STATUS__OK)
  m_RAISE_VERBATIM_IF(suckedLength != h_gStringStuff->c_copiedLength) 
  m_RAISE_VERBATIM_IF(suckedLength != 1668) // data/prout-orig file size 

  m_TRACK_IF(SuckerDestroyInstance(h_handle) != RETURNED)
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_gStringStuff) != RETURNED)
  m_TRACK_IF(BrokenPipeFixDestroyInstance(h_brokenPipeFixHandle) != RETURNED)
  m_TRACK_IF(StandardAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)
  //m_TRACK_IF(PthreadAlarmSystemDestroyInstance(h_alarmSystemHandle) != RETURNED)

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_dummySFilePathnameStuff) != RETURNED)
  m_TRACK_IF(StreamButtSpotterDestroyInstance(h_streamSButtSpotterHandle) != RETURNED)
  m_TRACK_IF(GStringButtSpotterDestroyInstance(h_gStringButtSpotterHandle) != RETURNED)
  
  m_DIGGY_RETURN(SUCCESS__EXECUTIVE__EXIT_STATUS)
} // main
