// c-posix/sucker.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"

#include "c-posix/sucker.h"
#include "c-posix/erw.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// Big butt
// --------

struct BIG_BUTT {
  ERW_HANDLE nh_erwHandle;
  char b_suckerButt;
  struct SUCKER_BUTT c_suckerButt;
  int suckedOutlength ;
} ; 


// Passed:
// - m_bigButt:
#define m_SPAWN_BIG_BUTT(/*struct BIG_BUTT*/ m_bigButt) {\
  (m_bigButt).nh_erwHandle = NULL;\
  (m_bigButt).b_suckerButt = b_FALSE0;\
  (m_bigButt).suckedOutlength = 0;\
}


// Passed:
// - *a_bigButt:
// - *af_suckerButt:
// - nf_alarmSystemHandle:
// - f_brokenPipeFixHandle:
// - n_streamReadBufferSize:
//   + -1 special value: big butt used as "destination"
//   + >0 : big butt used as "source" ; read buffer size when used as "stream" butt 
//
// Changed
// - *a_bigButt
//
// Ret:
// - RETURNED: done 
// - -1: anomaly is raised
static int PlugBigButt (struct BIG_BUTT *a_bigButt, const struct SUCKER_BUTT *af_suckerButt,
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle,
  int n_streamReadBufferSize) {
  m_DIGGY_BOLLARD_S()
  a_bigButt->b_suckerButt = b_TRUE; 
  a_bigButt->c_suckerButt = *af_suckerButt;
  switch (af_suckerButt->suckerButt) {
  break; case STREAM__SUCKER_BUTT:
    if (a_bigButt->nh_erwHandle == NULL) {
      m_TRACK_IF(ErwCreateInstance(&a_bigButt->nh_erwHandle,  f_brokenPipeFixHandle,
        nf_alarmSystemHandle, n_streamReadBufferSize,  NULL, NULL,  (void*)UNDEFINED) != RETURNED)
    } // if
    switch (ErwReset(a_bigButt->nh_erwHandle,af_suckerButt->select.c_stream.descriptor,
      &af_suckerButt->select.c_stream.waitingPlan)) {
    case COMPLETED__BUT:
      // TODO ?: m_RAISE_VERBATIM_IF(!a_bigButt->b_flop)
    case COMPLETED__OK:
    break; default :
      m_TRACK()
    } // switch 
  break; case G_STRING__SUCKER_BUTT:
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,af_suckerButt->suckerButt) 
  } // switch
  a_bigButt->suckedOutlength = 0;

  m_DIGGY_RETURN(RETURNED)
} // PlugBigButt


// Passed:
// - m_bigButt:
#define m_WIPE_BIG_BUTT(m_bigButt) {\
  if (m_bigButt.nh_erwHandle != NULL) {\
    m_TRACK_IF(ErwDestroyInstance(m_bigButt.nh_erwHandle) != RETURNED)\
  }\
}


// Passed:
// - *a_bigButt:
//
// Ret:
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem - anomaly is raised... 
static int FillBigButt(struct BIG_BUTT *a_bigButt, struct STRING_PORTION chunk, 
  int *na_flopCauseRwStatus) {
  m_DIGGY_BOLLARD_S()
  int answer = ANSWER__YES; // a priori
  int rwStatus = UNDEFINED; 
  if (b_EMPTY_STRING_PORTION(chunk)) {
    m_DIGGY_RETURN(answer)
  } // if  
  m_ASSERT(a_bigButt->b_suckerButt)
  switch (a_bigButt->c_suckerButt.suckerButt) {
  case STREAM__SUCKER_BUTT:
    m_ASSERT(a_bigButt->nh_erwHandle != NULL)
    switch (rwStatus = ErwWrite2(a_bigButt->nh_erwHandle,NULL,
      m_STRING_PORTION_2_BUFFER_LENGTH(chunk), NULL)) {
    case RW_STATUS__OK :
    break; case RW_STATUS__TERMINATING :
    case RW_STATUS__TRY_AGAIN :
    case RW_STATUS__CONNECTION_LOST :
      answer = ANSWER__NO; // flop
      if (na_flopCauseRwStatus != NULL) *na_flopCauseRwStatus = rwStatus;
    break; default:
      m_TRACK()
    }
  break; case G_STRING__SUCKER_BUTT:
    m_TRACK_IF(GStringCopy(a_bigButt->c_suckerButt.select.c_gStringStuff, -1, &chunk) < 0) 
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,a_bigButt->c_suckerButt.suckerButt)
  } // switch

  m_DIGGY_RETURN(answer)
} // FillBigButt 


// Passed:
// - *a_bigButt:
// 
// Changed:
// - *ac_chunk: only significant is suck OK (ANSWER__YES)
//
// Ret:
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem - anomaly is raised... 
static int SuckBigButt (struct BIG_BUTT *a_bigButt,  struct STRING_PORTION *ac_chunk,
  int *na_flopCauseRwStatus) {
  m_DIGGY_BOLLARD_S()
  int rwStatus = UNDEFINED; 
  int answer = ANSWER__YES; // a priori
  int readChunkLength = UNDEFINED;
  char *readChunk = (char *) UNDEFINED;
  m_ASSERT(a_bigButt->b_suckerButt)
  switch (a_bigButt->c_suckerButt.suckerButt) {
  case STREAM__SUCKER_BUTT:
    m_ASSERT(a_bigButt->nh_erwHandle != NULL)
    switch (rwStatus = ErwRead(a_bigButt->nh_erwHandle,NULL,&readChunk,&readChunkLength)) {
    case RW_STATUS__OK :
      m_ASSERT(readChunkLength > 0)
      m_ASSIGN_STRING_PORTION(*ac_chunk,readChunk,readChunkLength)
    break; case RW_STATUS__TERMINATING :
      m_ASSERT(readChunkLength == 0)
      m_ASSIGN_EMPTY_STRING_PORTION(*ac_chunk)
    break; case RW_STATUS__TRY_AGAIN :
      m_ASSERT(readChunkLength == 0)
    case RW_STATUS__CONNECTION_LOST :
      answer = ANSWER__NO; // flop
      if (na_flopCauseRwStatus != NULL) *na_flopCauseRwStatus = rwStatus;
    break; default:
      m_TRACK()
    } // switch 
  break; case G_STRING__SUCKER_BUTT:
     m_ASSIGN_STRING_PORTION__G_STRING(*ac_chunk,a_bigButt->c_suckerButt.select.c_gStringStuff)
     if (a_bigButt->suckedOutlength != 0) {
       m_ASSERT(a_bigButt->suckedOutlength == m_StringPortionLength(ac_chunk))
       m_ASSIGN_EMPTY_STRING_PORTION(*ac_chunk)
     } // if
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,a_bigButt->c_suckerButt.suckerButt)
  } // switch

  if (answer == ANSWER__YES) a_bigButt->suckedOutlength += m_StringPortionLength(ac_chunk) ; 

  m_DIGGY_RETURN(answer) 
} // SuckBigButt


// Sucker
// ------

struct SUCKER {
  struct BIG_BUTT sBigButt; 
  struct BIG_BUTT dBigButt; 
  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle;
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle;
  int sStreamReadBufferSize;
  int suckingLimit ;
} ;


// Public function: see .h
int SuckerCreateInstance (SUCKER_HANDLE *azh_handle,  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle,
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle,  int sStreamReadBufferSize,  int suckingLimit) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 

  m_SPAWN_BIG_BUTT((*azh_handle)->sBigButt)
  m_SPAWN_BIG_BUTT((*azh_handle)->dBigButt)

  (*azh_handle)->nf_alarmSystemHandle = nf_alarmSystemHandle;
  (*azh_handle)->f_brokenPipeFixHandle = f_brokenPipeFixHandle;
  (*azh_handle)->sStreamReadBufferSize = sStreamReadBufferSize;
  (*azh_handle)->suckingLimit = suckingLimit ;

  m_DIGGY_RETURN(RETURNED)
} // SuckerCreateInstance


// Public function: see .h
int SuckerGetSuckingLimit (SUCKER_HANDLE handle) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(handle->suckingLimit);
} // SuckerGetSuckingLimit 


const struct WAITING_PLAN p_suckerFileStreamWaitingPlan = {
  .waitingPlan = DEEPLY_BLOCKING__WAITING_PLAN ,
  .c_deadline = 30 ,
} ;

 
// Public function ; see description in .h
int SuckerPlugDButt(SUCKER_HANDLE handle,  const struct SUCKER_BUTT *af_dSuckerButt) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PlugBigButt(&handle->dBigButt,af_dSuckerButt, handle->f_brokenPipeFixHandle,
    handle->nf_alarmSystemHandle, -1) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // SuckerPlugDButt

// Public function ; see description in .h
int SuckerPlugSButt(SUCKER_HANDLE handle, const struct SUCKER_BUTT *af_sSuckerButt) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PlugBigButt(&handle->sBigButt,af_sSuckerButt, handle->f_brokenPipeFixHandle,
    handle->nf_alarmSystemHandle,handle->sStreamReadBufferSize) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // SuckerPlugSButt


// Public function ; see description in .h
int SuckerFillDButt(SUCKER_HANDLE handle, struct STRING_PORTION chunk, int *na_flopCauseRwStatus) {
  m_DIGGY_BOLLARD()

  int answer = FillBigButt(&handle->dBigButt,chunk,na_flopCauseRwStatus) ;
  m_TRACK_IF(answer < 0) 
   
  m_DIGGY_RETURN(answer)
} // SuckerFillDButt 


// Public function ; see description in .h
int SuckerSuckSButt(SUCKER_HANDLE handle,  struct STRING_PORTION *ac_chunk,
  int *na_flopCauseRwStatus) {
  m_DIGGY_BOLLARD()

  int answer = SuckBigButt(&handle->sBigButt, ac_chunk,na_flopCauseRwStatus) ; 
  m_TRACK_IF(answer < 0) 

  m_DIGGY_RETURN(answer) 
} // SuckerSuckSButt


// Public function ; see description in .h
int SuckerCopy(SUCKER_HANDLE handle,  const char *p_sFilePathname,  const char *p_dFilePathname,
  int nonPosixOpenFlags,  int *na_copiedLength,  int *na_flopCauseRwStatus, int *na_openErrno) {
  m_DIGGY_BOLLARD()
  int suckerStatus = SUCKER_STATUS__OK; // so far, so good... 
  int nh_sFileStreamDescriptor = -1, nh_dFileStreamDescriptor = -1;
  if (na_flopCauseRwStatus != NULL) *na_flopCauseRwStatus = RW_STATUS__OK; // a priori

  switch (ProtectedOpenFile(p_sFilePathname,  READ__BASIC_OPEN_FILE_MODE | nonPosixOpenFlags, 0,
    &nh_sFileStreamDescriptor,na_openErrno)) {
  case ANSWER__YES:
    switch (ProtectedOpenFile(p_dFilePathname, COLD_WRITE__BASIC_OPEN_FILE_MODE | nonPosixOpenFlags,
      BATEAU__FILE_CREATION_MODE, &nh_dFileStreamDescriptor,na_openErrno)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      nh_dFileStreamDescriptor = -1;
      suckerStatus = SUCKER_STATUS__D_FLOP;
    break; default:
     m_TRACK()
    }  // switch
  break; case ANSWER__NO:
    nh_sFileStreamDescriptor = -1;
    suckerStatus = SUCKER_STATUS__S_FLOP;
  break; default:
    m_TRACK()
  } // switch

  if (nh_sFileStreamDescriptor != -1 && nh_dFileStreamDescriptor != -1) { 
    struct SUCKER_BUTT sSuckerButt ;
    struct SUCKER_BUTT dSuckerButt ;

    m_ASSIGN_SUCKER_BUTT__FILE_STREAM(sSuckerButt, nh_sFileStreamDescriptor)
    m_ASSIGN_SUCKER_BUTT__FILE_STREAM(dSuckerButt, nh_dFileStreamDescriptor)

    m_TRACK_IF((suckerStatus = SuckerSuckOut(handle,&sSuckerButt,&dSuckerButt,na_copiedLength,
      na_flopCauseRwStatus)) < 0)
  } // if

  if (nh_sFileStreamDescriptor > 0) { 
    m_PROTECTED_CLOSE(nh_sFileStreamDescriptor, 
     if (suckerStatus == SUCKER_STATUS__OK) suckerStatus = SUCKER_STATUS__S_FLOP; )
  } // if

  if (nh_dFileStreamDescriptor > 0) { 
    m_PROTECTED_CLOSE(nh_dFileStreamDescriptor,
      if (suckerStatus == SUCKER_STATUS__OK) suckerStatus = SUCKER_STATUS__D_FLOP; )
  } // if

  m_DIGGY_RETURN(suckerStatus)
} // SuckerCopy


// Public function ; see description in .h
int SuckerDestroyInstance (SUCKER_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  m_WIPE_BIG_BUTT(xh_handle->sBigButt)
  m_WIPE_BIG_BUTT(xh_handle->dBigButt)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // SuckerDestroyInstance
