// c-posix/erw.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-posix/erw.h"

#include "c-ansi/diggy.h"


// RDH: ReaD Helper
// ================


struct RDH {
  CHECK_READ_FUNCTION checkReadFunction ;
  void *nr_checkReadHandle ;

  int bufferCurrentSize ;
  char *nih_resizableBuffer ;
  int c_resizableBufferSizeIncrement ;
  char *cv_buffer ;

  int readLength ;
  int messageLength; // > 0  <=> complete message read
  int checkedReadLength ;
  int checkReadAttemptsCount ;
} ; 

typedef struct RDH *RDH_HANDLE; 


//
#define m_RDH_RESET(handle) \
  handle->readLength = 0 ; \
  handle->messageLength = 0;\
  handle->checkReadAttemptsCount = 0;\
  handle->checkedReadLength = 0 ;



// CHECK_READ_FUNCTION
static int TrivialCheckRead (void *nr_handle, const char *p_readBuffer, int readLength,
  int attemptsCount, int *ac_messageLength) {
  m_DIGGY_BOLLARD()
  m_ASSERT(nr_handle == NULL)
  m_ASSERT(readLength > 0)

  *ac_messageLength = readLength;
  m_DIGGY_RETURN(ANSWER__YES)

} // TrivialCheckRead


// Modified:
// - *azh_handle:
//
// Ret:
// - RETURNED:
// - -1: unexpected problem
static int RdhCreateInstance (RDH_HANDLE *azh_handle, int readBufferSize,
  char *nf_traditionalReadBuffer, CHECK_READ_FUNCTION n_checkReadFunction,
  void *cnfr_checkReadHandle) {
  m_DIGGY_BOLLARD_S()
  struct RDH *ha_rdh = (struct RDH *) UNDEFINED;
  m_MALLOC_INSTANCE(ha_rdh)

  if (n_checkReadFunction != NULL) {
    ha_rdh->checkReadFunction = n_checkReadFunction ;
    ha_rdh->nr_checkReadHandle = cnfr_checkReadHandle;
  } else {
    ha_rdh->checkReadFunction = TrivialCheckRead ;
    ha_rdh->nr_checkReadHandle = NULL ;
  } // if

  ha_rdh->bufferCurrentSize = readBufferSize ;

  if (nf_traditionalReadBuffer == NULL) {
    m_MALLOC(ha_rdh->cv_buffer = ha_rdh->nih_resizableBuffer, ha_rdh->bufferCurrentSize)
    //ha_rdh->cv_buffer = ha_rdh->nih_resizableBuffer;


    ha_rdh->c_resizableBufferSizeIncrement = readBufferSize ;
  } else {
    ha_rdh->nih_resizableBuffer = NULL;
    ha_rdh->cv_buffer = nf_traditionalReadBuffer ;
  } // if

  m_RDH_RESET(ha_rdh)

  *azh_handle = ha_rdh ;

  m_DIGGY_RETURN(RETURNED)
} // RdhCreateInstance


// Passed:
// - handle:
//
// Modified:
// - *anf_newReadBuffer: 
//   + NULL special value: extra data already present; DO NOT read new data 
//   + non NULL adress: buffer address to read new data
// - *ac_newReadBufferSize: only significant if "new read" buffer is provided
//
// Ret:
// - RETURNED:
// - -1: unexpected problem
static int RdhPrepareRead (RDH_HANDLE handle, char **anf_newReadBuffer, int *ac_newReadBufferSize) {
  m_DIGGY_BOLLARD_S()
  if (handle->messageLength > 0) { // previous message read succesfully
    handle->readLength -= handle->messageLength;
    if (handle->readLength > 0) {
      memmove(handle->cv_buffer,handle->cv_buffer + handle->messageLength,
              handle->readLength);
    } // if
    handle->messageLength = 0;
    handle->checkReadAttemptsCount = 0 ;
    handle->checkedReadLength = 0;
  } // if

  if (handle->checkedReadLength == handle->readLength) { // Need to read new data
    // Ensure there is free room in buffer...
    if (handle->readLength == handle->bufferCurrentSize) {
      m_ASSERT(handle->nih_resizableBuffer != NULL)
      m_REALLOC(handle->nih_resizableBuffer, handle->bufferCurrentSize += handle->c_resizableBufferSizeIncrement)
      handle->cv_buffer = handle->nih_resizableBuffer;
    } // if

    *anf_newReadBuffer = handle->cv_buffer + handle->readLength ;
    *ac_newReadBufferSize = handle->bufferCurrentSize - handle->readLength;

  } else {
    *anf_newReadBuffer = NULL;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // RdhPrepareRead


// Passed:
// - handle:
// - newReadLength:
//
// Ret:
// - RETURNED:
// - -1: unexpected problem
static int RdhUpdateRead (RDH_HANDLE handle, int newReadLength) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT((handle->readLength += newReadLength) <= handle->bufferCurrentSize)
  
  m_DIGGY_RETURN(RETURNED)
} // RdhUpdateRead 
 
 
// Passed:
// - handle:
// - nac_messageBuffer:
//   + NULL special value: no need to get the buffer address
//   + non NULL value: 
// - ac_messageLength:
// 
// Modified : 
// - *nac_messageBuffer (is asked) : only significant if message is complete (ANSWER__YES) :  
//   start address of message buffer 
//   *ac_messageLength: only significant is message is complete (ANSWER__YES)
//   
// Ret:
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int RdhCheckRead (RDH_HANDLE handle, char **nac_messageBuffer, int *ac_messageLength) {
  m_DIGGY_BOLLARD_S()
  int rsAnswer ;
  switch (rsAnswer = handle->checkReadFunction(handle->nr_checkReadHandle, handle->cv_buffer,
    handle->checkedReadLength = handle->readLength, ++(handle->checkReadAttemptsCount),
    &handle->messageLength)) {
  case ANSWER__YES:
    if (nac_messageBuffer != NULL) {
      *nac_messageBuffer = handle->cv_buffer;
    } // if
    *ac_messageLength = handle->messageLength;
  break; case ANSWER__NO:
    handle->messageLength = 0;
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(rsAnswer)
} // RdhCheckRead 


// Passed:
// - handle:
// - na_messageBuffer:
//   + NULL special value: no need to get the buffer address
//   + non NULL value: 
// - a_messageLength:
// 
// Modified : 
// - *na_messageBuffer (is asked) : start address of message buffer 
//   *a_messageLength: 
//
// Ret:
// - RETURNED:
// - -1: unexpected problem
static int RdhFlush (RDH_HANDLE handle, char **na_messageBuffer, int *a_messageLength) {
  m_DIGGY_BOLLARD_S()
  if (na_messageBuffer != NULL) {
    *na_messageBuffer = handle->cv_buffer ;
  } // if
  *a_messageLength = handle->messageLength = handle->readLength ;

  m_DIGGY_RETURN(RETURNED)
} // RdhFlush


//
#define b_RDH_RESIDUE(handle) \
  ((handle)->readLength > (handle)->messageLength)


// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT:
// - -1: unexpected problem
static int RdhReset (RDH_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK ; // a priori
  if (b_RDH_RESIDUE(handle)) {
    completed = COMPLETED__BUT;
  } // if

  m_RDH_RESET(handle)

  m_DIGGY_RETURN(completed)
} // RdhReset


// Ret:
// - RETURNED:
static int RdhDestroyInstance (RDH_HANDLE xh_handle) {
  m_DIGGY_BOLLARD_S()

  if (xh_handle->nih_resizableBuffer != NULL) {
    free(xh_handle->nih_resizableBuffer);
  } // if 

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // RdhDestroyInstance


// WRH: WRite Helper
// ================

#define MAX_MESSAGE_PARTS_NUMBER 10

struct WRH {
  int messagePartWrittenLength ;
  int writtenMessagePartsCount ;

  // message data:
  int messagePartsNumber ;
  const char *sp_messagePartsBuffer[MAX_MESSAGE_PARTS_NUMBER];
  int s_messagePartsLength[MAX_MESSAGE_PARTS_NUMBER];
} ;

typedef struct WRH *WRH_HANDLE; 


#define m_WRH_RESET(handle) \
  (handle)->messagePartWrittenLength = 0 ;\
  (handle)->writtenMessagePartsCount = 0 ;\
  (handle)->messagePartsNumber = 0 ;

// Ret:
// - RETURNED:
// - -1: unexpected problem
static int WrhCreateInstance (WRH_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD_S()
  struct WRH *ha_wrh = (struct WRH*) UNDEFINED;
  m_MALLOC_INSTANCE(ha_wrh)

  m_WRH_RESET(ha_wrh)

  *azh_handle = ha_wrh;

  m_DIGGY_RETURN(RETURNED)
} // WrhCreateInstance


// Ret:
// - RETURNED:
// - -1: unexpected problem
static int WrhSynchronizeFirstMessagePart (WRH_HANDLE handle, const char *fp_messagePartBuffer,
  int messagePartLength) {
  m_DIGGY_BOLLARD_S()
  handle->messagePartsNumber = 0;
  int i = handle->messagePartsNumber++ ;
  handle->sp_messagePartsBuffer[i] = fp_messagePartBuffer ;
  handle->s_messagePartsLength[i] = messagePartLength ;

  m_DIGGY_RETURN(RETURNED)
} // WrhSynchronizeFirstMessagePart


// Ret:
// - RETURNED:
// - -1: unexpected problem
static int WrhSynchronizeNextMessagePart (WRH_HANDLE handle, const char *fp_messagePartBuffer,
  int messagePartLength) {
  m_DIGGY_BOLLARD_S()
  int i = handle->messagePartsNumber++ ;
  m_ASSERT(i < MAX_MESSAGE_PARTS_NUMBER)
  handle->sp_messagePartsBuffer[i] = fp_messagePartBuffer ;
  handle->s_messagePartsLength[i] = messagePartLength ;

  m_DIGGY_RETURN(RETURNED)
} // WrhSynchronizeNextMessagePart


// Ret:
// - RETURNED:
// - -1: unexpected problem
static int WrhPrepareWrite (WRH_HANDLE handle, const char **ap_writeBuffer, int *a_writeLength) {
  m_DIGGY_BOLLARD_S()
  int i = handle->writtenMessagePartsCount ;
  m_ASSERT(i >= 0 && i < handle->messagePartsNumber && 
    handle->messagePartWrittenLength <= handle->s_messagePartsLength[i])

  *ap_writeBuffer = handle->sp_messagePartsBuffer[i] + handle->messagePartWrittenLength ;
  *a_writeLength = handle->s_messagePartsLength[i] - handle->messagePartWrittenLength;

  m_DIGGY_RETURN(RETURNED)
} // WrhPrepareWrite


// Side effect: "reset" when complete message is written
//
// Ret: Is message entirely written
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int WrhCheckWrite (WRH_HANDLE handle, int writtenLength) {
  m_DIGGY_BOLLARD_S()
  m_RAISE_IF(writtenLength <= 0,ANOMALY__VALUE__FMT_D,writtenLength)
  handle->messagePartWrittenLength += writtenLength;
  int i = handle->writtenMessagePartsCount ;
  if (handle->messagePartWrittenLength == handle->s_messagePartsLength[i]) {
    m_TRACK_IF(handle->writtenMessagePartsCount >= handle->messagePartsNumber)
    handle->writtenMessagePartsCount++ ;
    handle->messagePartWrittenLength = 0;
  } // if

  if (handle->writtenMessagePartsCount < handle->messagePartsNumber) {
    m_DIGGY_RETURN(ANSWER__NO)
  } // if

  m_WRH_RESET(handle)
  m_DIGGY_RETURN(ANSWER__YES)
} //  WrhCheckWrite


//
#define b_WRH_RESIDUE(handle) \
  ((handle)->messagePartWrittenLength != 0 || (handle)->writtenMessagePartsCount != 0)


// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: a message partially written message was trailing (SHOULD NOT happen if all messages 
//   were succesfully "written") 
// - -1: unexpected problem
static int WrhReset (WRH_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK ; // a priori
  if (b_WRH_RESIDUE(handle)) {
    completed = COMPLETED__BUT;
  } // if

  m_WRH_RESET(handle)

  m_DIGGY_RETURN(completed)
} // WrhReset


// Ret:
// - RETURNED:
static int WrhDestroyInstance (WRH_HANDLE xh_handle) {
  m_DIGGY_BOLLARD_S()

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // WrhDestroyInstance


// ERW
// ===

static const struct WAITING_PLAN p_defaultErwWaitingPlan = {
  .waitingPlan = LIGHTLY_BLOCKING__WAITING_PLAN ,
  .c_deadline = 10 ,
} ;


struct ERW {
  BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle ;
  PD_HANDLE h_pdHandle;
  RDH_HANDLE nh_rdhHandle; 
  WRH_HANDLE h_wrhHandle;
  struct WAITING_PLAN defaultWaitingPlan ;
  int n_descriptor ;
  char cb_synchronizedDefaultWaitingPlan ;
} ;


// Public function : see description in .h
int ErwCreateInstance (ERW_HANDLE *azh_handle, BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle,
  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle, int n_readBufferSize, char *cnf_traditionalReadBuffer,
  CHECK_READ_FUNCTION cn_checkReadFunction, void *ccnfr_checkReadHandle,
  const struct WAITING_PLAN *nap_defaultWaitingPlan) {
  m_DIGGY_BOLLARD()
  struct ERW *ha_erw = (struct ERW *) UNDEFINED;
  m_MALLOC_INSTANCE(ha_erw)

  ha_erw->brokenPipeFixHandle = f_brokenPipeFixHandle ;
  m_TRACK_IF(PdCreateInstance(&ha_erw->h_pdHandle,nf_alarmSystemHandle) != RETURNED)

  if (n_readBufferSize != -1) {
    m_TRACK_IF(RdhCreateInstance(&ha_erw->nh_rdhHandle, n_readBufferSize, cnf_traditionalReadBuffer,
      cn_checkReadFunction, ccnfr_checkReadHandle) != RETURNED)
  } else {
    ha_erw->nh_rdhHandle = NULL;
  } // if

  m_TRACK_IF(WrhCreateInstance(&(ha_erw->h_wrhHandle)) != RETURNED)

  ha_erw->n_descriptor = -1 ;
  if (nap_defaultWaitingPlan != NULL) ha_erw->defaultWaitingPlan = *nap_defaultWaitingPlan;
  else ha_erw->defaultWaitingPlan = p_defaultErwWaitingPlan ;

  *azh_handle = ha_erw ;

  m_DIGGY_RETURN(RETURNED)
} // ErwCreateInstance 


// Public function : see description in .h
int ErwReset (ERW_HANDLE handle, int f_descriptor,
  const struct WAITING_PLAN *nap_defaultWaitingPlan) {
  m_DIGGY_BOLLARD()
  int completed = COMPLETED__OK ; // a priori

  switch (WrhReset(handle->h_wrhHandle)) {
  case COMPLETED__OK :
  break; case COMPLETED__BUT :
    completed = COMPLETED__BUT ;
  break; default:
    m_TRACK()
  } // switch 

  if (handle->nh_rdhHandle != NULL) {
    switch (RdhReset(handle->nh_rdhHandle)) { 
    case COMPLETED__OK :
    break; case COMPLETED__BUT :
      completed = COMPLETED__BUT ;
    break; default:
      m_TRACK()
    } // switch 
  } // if

  if (nap_defaultWaitingPlan != NULL) handle->defaultWaitingPlan = *nap_defaultWaitingPlan;
  handle->n_descriptor = f_descriptor ;
  handle->cb_synchronizedDefaultWaitingPlan = b_FALSE0 ;

  m_DIGGY_RETURN(completed)
} // ErwReset


// Ensure synchronization of description vis-a-vis waiting plan 
//
// Uses:
// - handle: (n_descriptor != -1) 
// - nap_waitingPlan: may be not specified (NULL pointer)
//
// Changed:
// - nap_waitingPlan: waiting plan to use (always != NULL)
#define m_CHECK_O_NONBLOCK_SYNCHRONIZATION() {\
  m_ASSERT(handle->n_descriptor >= 0) \
  if (nap_waitingPlan != NULL) {\
    m_TRACK_IF(SynchronizeONonblock(handle->n_descriptor,nap_waitingPlan) != RETURNED)\
    handle->cb_synchronizedDefaultWaitingPlan = b_FALSE0;\
  } else  {\
    nap_waitingPlan = &handle->defaultWaitingPlan;\
    if (! handle->cb_synchronizedDefaultWaitingPlan) {\
      m_TRACK_IF(SynchronizeONonblock(handle->n_descriptor,nap_waitingPlan) != RETURNED)\
      handle->cb_synchronizedDefaultWaitingPlan = b_TRUE;\
    } \
  }\
}

// Public function : see description in .h
int ErwRead (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan,
  char **na_messageBuffer, int *a_messageLength) {
  m_DIGGY_BOLLARD()
  m_ASSERT(handle->nh_rdhHandle != NULL)

  m_CHECK_O_NONBLOCK_SYNCHRONIZATION()

  int n_rwStatus = -1; // unknown status for the moment
  while (n_rwStatus == -1) {
    char *nf_newReadBuffer ;
    int c_newReadBufferSize ;

    m_TRACK_IF(RdhPrepareRead(handle->nh_rdhHandle,&nf_newReadBuffer,&c_newReadBufferSize) != RETURNED)
    if (nf_newReadBuffer != NULL) {
      int c_newReadLength ;

      switch (n_rwStatus = ProtectedRead2(handle->n_descriptor, handle->h_pdHandle,
        nap_waitingPlan, nf_newReadBuffer,c_newReadBufferSize, &c_newReadLength)) {
      case RW_STATUS__OK:
        m_TRACK_IF(RdhUpdateRead(handle->nh_rdhHandle,c_newReadLength) != RETURNED)
      break; case RW_STATUS__TERMINATING:
      case RW_STATUS__CONNECTION_LOST:
      case RW_STATUS__TRY_AGAIN:
      break; default:
        m_TRACK()
      } // switch
    } else {
      n_rwStatus = RW_STATUS__OK; // successfully read (from previous call)
    } // if 

    switch (n_rwStatus) {
    case RW_STATUS__OK: // Read data to check
      switch (RdhCheckRead(handle->nh_rdhHandle,na_messageBuffer,a_messageLength)) {
      case ANSWER__YES : // Message is complete
      break; case ANSWER__NO : // Message is not complete
        n_rwStatus = -1 ; // Need to stay in loop
      break; default:
        m_TRACK()
      } // switch 
    break; case RW_STATUS__TRY_AGAIN:
      *a_messageLength = 0 ;
    break; case RW_STATUS__TERMINATING:
    case RW_STATUS__CONNECTION_LOST:
      m_TRACK_IF(RdhFlush(handle->nh_rdhHandle,na_messageBuffer,a_messageLength) != RETURNED)
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,n_rwStatus)
    } // switch 
  } // while 

  m_DIGGY_RETURN(n_rwStatus)
} // ErwRead


// Public function : see description in .h
int ErwWrite2 (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan, const char *p_messageBuffer,
  int messageLength, ...) {
  m_DIGGY_BOLLARD()
  m_CHECK_O_NONBLOCK_SYNCHRONIZATION()

  m_TRACK_IF(WrhSynchronizeFirstMessagePart(handle->h_wrhHandle, p_messageBuffer,messageLength) !=
    RETURNED)

  { // Synchronize next message parts
    va_list ap ;
    va_start(ap,messageLength) ;

    while (b_TRUE) {
      const char *np_messagePartBuffer = va_arg(ap,const char *);
      if (np_messagePartBuffer != NULL) {
        int c_messagePartLength = va_arg(ap,int);

        m_TRACK_IF(WrhSynchronizeNextMessagePart(handle->h_wrhHandle,np_messagePartBuffer,
          c_messagePartLength) != RETURNED)
      } else {
    	break;
      } // if
    } // while

    va_end(ap);
  } // Synchronize next messages parts

  int n_rwStatus  = -1; // unknown status for the moment
  while (n_rwStatus == -1) {
    int c_writtenLength ;
    int writeLength ;
    const char *p_writeBuffer;

    m_TRACK_IF(WrhPrepareWrite(handle->h_wrhHandle, &p_writeBuffer,&writeLength) != RETURNED)

    n_rwStatus =
    ProtectedWrite2(handle->brokenPipeFixHandle,handle->n_descriptor, handle->h_pdHandle,
      nap_waitingPlan, p_writeBuffer,writeLength, &c_writtenLength) ;
    switch (n_rwStatus) {
    case RW_STATUS__OK:
      switch (WrhCheckWrite(handle->h_wrhHandle,c_writtenLength)) {
      case ANSWER__YES: // all message parts written
      break; case ANSWER__NO:
    	n_rwStatus = -1 ; // stay in writing loop
      break; default:
    	m_TRACK()
      } // switch
    break; case RW_STATUS__TRY_AGAIN:
    case RW_STATUS__TERMINATING:
    case RW_STATUS__CONNECTION_LOST:
    break; default:
      m_TRACK()
    } // switch 
  } // while

  m_DIGGY_RETURN(n_rwStatus)
} // ErwWrite2


// Public function : see description in .h
int ErwWrite (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan,
  const char *p_messageBuffer, int messageLength) {
  m_DIGGY_BOLLARD()
  int rwStatus = ErwWrite2 (handle,nap_waitingPlan,p_messageBuffer,messageLength,NULL) ;
  switch (rwStatus) {
  case RW_STATUS__OK:
  case RW_STATUS__TRY_AGAIN:
  case RW_STATUS__TERMINATING:
  case RW_STATUS__CONNECTION_LOST:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(rwStatus)
} // ErwWrite


// Public function : see description in .h
int ErwDestroyInstance (ERW_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  if (xh_handle->nh_rdhHandle != NULL) {
    m_TRACK_IF(RdhDestroyInstance(xh_handle->nh_rdhHandle) != RETURNED)
  } // if

  m_TRACK_IF(WrhDestroyInstance(xh_handle->h_wrhHandle) != RETURNED)
     
  m_TRACK_IF(PdDestroyInstance(xh_handle->h_pdHandle) != RETURNED)

  free(xh_handle) ;
 
  m_DIGGY_RETURN(RETURNED)
} // ErwDestroyInstance 
