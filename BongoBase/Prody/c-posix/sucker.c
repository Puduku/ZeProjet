// c-posix/sucker.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduu - 2023

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"
#include "c-ansi/magic.h"

#include "c-posix/sucker.h"
#include "c-posix/erw.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


struct STREAM_BUTT_ADEPT {
  m_DECLARE_MAGIC_FIELD(STREAM_BUTT_ADEPT_HANDLE)
  ERW_HANDLE h_erwHandle;
} ;

// Public function: see .h
int StreamButtAdeptCreateInstance(STREAM_BUTT_ADEPT_HANDLE *azh_handle,
  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle, BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle,
  struct WAITING_PLAN waitingPlan, int n_readBufferSize, int nf_openDescriptor) { 
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 
  STREAM_BUTT_ADEPT_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(STREAM_BUTT_ADEPT_HANDLE,handle)
  m_TRACK_IF(ErwCreateInstance(&handle->h_erwHandle, f_brokenPipeFixHandle,
    nf_alarmSystemHandle, n_readBufferSize, NULL, NULL, (void *)UNDEFINED, waitingPlan) != RETURNED)
  
  if (nf_openDescriptor != -1) {
    switch (ErwReset(handle->h_erwHandle, nf_openDescriptor)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT:
      m_RAISE(ANOMALY__TRAILING_DATA)
    break; default:
      m_TRACK()
    } // switch
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // StreamButtAdeptCreateInstance


// Public function: see .h
int StreamButtAdeptKick(STREAM_BUTT_ADEPT_HANDLE handle, int f_openDescriptor) {
  m_DIGGY_BOLLARD()
  switch (ErwReset(handle->h_erwHandle, f_openDescriptor)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__TRAILING_DATA)
  break; default:
    m_TRACK()
  } // switch
  
  m_DIGGY_RETURN(RETURNED)
} // StreamButtAdeptKick 


// BUTT_ADEPT_SUCKER_FUNCTION
// Public function: see .h
int StreamButtAdeptSuck(void *r_handle,  char b_kickButt, struct P_STRING *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  STREAM_BUTT_ADEPT_HANDLE handle = (STREAM_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(STREAM_BUTT_ADEPT_HANDLE,handle)

  // b_kickButt: not used

  int answer = ANSWER__YES; // A priori
  int readChunkLength = UNDEFINED;
  char *readChunk = (char *) UNDEFINED;
  switch (ErwRead(handle->h_erwHandle,NULL,&readChunk,&readChunkLength)) {
    case RW_STATUS__OK :
    break; case RW_STATUS__TERMINATING :
      *ac_chunk = m_PString2(readChunk,readChunkLength);
    break; case RW_STATUS__TRY_AGAIN :
      m_ASSERT(readChunkLength == 0)
      answer = ANSWER__NO; // flop
      if (nar_flopCause != NULL) *nar_flopCause = STREAM_FLOP_CAUSE__TIMEOUT;
    break; case RW_STATUS__CONNECTION_LOST :
      m_ASSERT(readChunkLength == 0)
      answer = ANSWER__NO; // flop
      if (nar_flopCause != NULL) *nar_flopCause = STREAM_FLOP_CAUSE__CONNECTION_LOST;
    break; default:
      m_TRACK()
  } // switch 

  m_DIGGY_RETURN(answer)
} // StreamButtAdeptSuck


// BUTT_ADEPT_FILLER_FUNCTION
// Public function: see .h
int StreamButtAdeptFill(void *r_handle, struct P_STRING chunk, int *nar_flopCause) { 
  m_DIGGY_BOLLARD()
  STREAM_BUTT_ADEPT_HANDLE handle = (STREAM_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(STREAM_BUTT_ADEPT_HANDLE,handle)

  int answer = ANSWER__YES; // A priori
  switch (ErwWrite2(handle->h_erwHandle,NULL, m_P_STRING_2_BUFFER_LENGTH(chunk), NULL)) {
  case RW_STATUS__OK :
  break; case RW_STATUS__TERMINATING :
    answer = ANSWER__NO; // flop
    if (nar_flopCause != NULL) *nar_flopCause = STREAM_FLOP_CAUSE__BROKEN_PIPE;
  break; case RW_STATUS__TRY_AGAIN :
    answer = ANSWER__NO; // flop
    if (nar_flopCause != NULL) *nar_flopCause = STREAM_FLOP_CAUSE__TIMEOUT;
  case RW_STATUS__CONNECTION_LOST :
    answer = ANSWER__NO; // flop
    if (nar_flopCause != NULL) *nar_flopCause = STREAM_FLOP_CAUSE__CONNECTION_LOST;
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(answer)
} // StreamButtAdeptFill


// Public function: see .h
int StreamButtAdeptDestroyInstance(STREAM_BUTT_ADEPT_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  m_TRACK_IF(ErwDestroyInstance(xh_handle->h_erwHandle) != RETURNED)
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // StreamButtAdeptDestroyInstance


// Public function: see .h
int l_SuckerCopy (SUCKER_HANDLE handle, ALARM_SYSTEM_HANDLE n_alarmSystemHandle,
   BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle,
   const char *p_sFilePathname, const char *p_dFilePathname,
   int *na_copiedLength, int *na_streamFlopCause) {
  m_DIGGY_BOLLARD()

  STREAM_BUTT_ADEPT_HANDLE sSreamButtAdeptHandle = (STREAM_BUTT_ADEPT_HANDLE)UNDEFINED;
  STREAM_BUTT_ADEPT_HANDLE dSreamButtAdeptHandle = (STREAM_BUTT_ADEPT_HANDLE)UNDEFINED;
  
  int h_sOpenDescriptor = UNDEFINED; // TODO: what about BINARY__NON_POSIX_OPEN_FLAG ? 
  switch (ProtectedOpenFile(p_sFilePathname, READ__BASIC_OPEN_FILE_MODE,
    BATEAU__FILE_CREATION_MODE, &h_sOpenDescriptor, NULL)) {
  case ANSWER__YES: // Succesfully open
  break; case ANSWER__NO: // Could not open
    if (na_streamFlopCause != NULL) *na_streamFlopCause = STREAM_FLOP_CAUSE__NOT_CONNECTED;
    m_DIGGY_RETURN(SUCKER_STATUS__S_FLOP) 
  break; default: 
    m_TRACK()
  } // switch

  int h_dOpenDescriptor = UNDEFINED; 
  switch (ProtectedOpenFile(p_sFilePathname, COLD_WRITE__BASIC_OPEN_FILE_MODE,
    BATEAU__FILE_CREATION_MODE, &h_dOpenDescriptor, NULL)) {
  case ANSWER__YES: // Succesfully open
  break; case ANSWER__NO: // Could not open
    if (na_streamFlopCause != NULL) *na_streamFlopCause = STREAM_FLOP_CAUSE__NOT_CONNECTED;
    m_DIGGY_RETURN(SUCKER_STATUS__D_FLOP) 
  break; default: 
    m_TRACK()
  } // switch

  m_TRACK_IF(StreamButtAdeptCreateInstance(&sSreamButtAdeptHandle,n_alarmSystemHandle,brokenPipeFixHandle,
    p_fileStreamSimpleWaitingPlan,BATEAU__FILE_BUTT_ADEPT__READ_BUFFER_SIZE,h_sOpenDescriptor) !=
    RETURNED) 

  m_TRACK_IF(StreamButtAdeptCreateInstance(&dSreamButtAdeptHandle,n_alarmSystemHandle,brokenPipeFixHandle,
    p_fileStreamSimpleWaitingPlan,-1,h_dOpenDescriptor) != RETURNED) 

  int suckerStatus = SuckerSuckOut(handle,na_copiedLength,na_streamFlopCause);
  m_TRACK_IF(suckerStatus < 0)

  m_TRACK_IF(StreamButtAdeptDestroyInstance(sSreamButtAdeptHandle) != RETURNED)
  m_TRACK_IF(StreamButtAdeptDestroyInstance(dSreamButtAdeptHandle) != RETURNED)

  m_DIGGY_RETURN(suckerStatus)
} // l_SuckerCopy

