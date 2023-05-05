// c-posix/tryagain.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"

#include "c-posix/tryagain.h"
#include "c-posix/alarm.h"

#include "c-ansi/diggy.h"


// Public function : see description in .h
int SynchronizeONonblock (int descriptor, const struct WAITING_PLAN *afp_waitingPlan) {
  m_DIGGY_BOLLARD()
  unsigned int descriptorFlags, newFlags;

  m_PERROR_VERBATIM_IF((newFlags = descriptorFlags = fcntl(descriptor,F_GETFL)) == -1)

  if (afp_waitingPlan->waitingPlan == NON_BLOCKING__WAITING_PLAN) {
    newFlags &= ~O_NONBLOCK ; // Remove flag
  } else {
    newFlags |= O_NONBLOCK ; // enable flag
  } // if
  if (newFlags != descriptorFlags) {
    m_PERROR_VERBATIM_IF(fcntl(descriptor,F_SETFL,newFlags) == -1)
  } // if

  m_DIGGY_RETURN(RETURNED)
} // SynchronizeONonblock



struct _sps2_PD {
  ALARM_SYSTEM_HANDLE n_alarmSystemHandle;

  const struct WAITING_PLAN *nap_waitingPlan ; 
  struct {
    time_t cc_baseEpochTime; // Only significant in deep blocking waiting plan with a finite deadline ;
                             // value of "base" epoch time
    int alarmDeliveryCorrection; // correction for alarm delivery
  } c_control ; // Only significant when waiting plan is initialized
};


// Public function : see description in .h
int PdCreateInstance (PD_HANDLE *azh_handle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)

  (*azh_handle)->n_alarmSystemHandle = nf_alarmSystemHandle;
  (*azh_handle)->nap_waitingPlan = NULL;  

  m_DIGGY_RETURN(RETURNED)
} // PdCreateInstance


// Public function : see description in .h
int PdSetDeadline (PD_HANDLE handle, const struct WAITING_PLAN *afp_waitingPlan) {
  m_DIGGY_BOLLARD()
  handle->nap_waitingPlan = afp_waitingPlan ;

  if (afp_waitingPlan->waitingPlan == DEEPLY_BLOCKING__WAITING_PLAN &&
      afp_waitingPlan->c_deadline > 0) {
    // Reference time for timeout:
   handle->c_control.cc_baseEpochTime = time(NULL) ;
  } // if
  handle->c_control.alarmDeliveryCorrection = 0 ;
  // assert "ASS" below is verified...

  m_DIGGY_RETURN(RETURNED)
} // PdSetDeadline


// Public function : see description in .h
int PdSetAlarm (PD_HANDLE handle) {
  m_DIGGY_BOLLARD()
  if (handle->nap_waitingPlan != NULL &&
      handle->nap_waitingPlan->waitingPlan != NON_BLOCKING__WAITING_PLAN && 
      handle->nap_waitingPlan->c_deadline > 0 && 
      handle->n_alarmSystemHandle != NULL) { 
    if (SetAlarm(handle->n_alarmSystemHandle,
                 // assert "ASS" : when c_deadline > 0,
                 // handle->nap_waitingPlan->c_deadline - 
                 // handle->c_control.alarmDeliveryCorrection > 0
                 handle->nap_waitingPlan->c_deadline - 
                 handle->c_control.alarmDeliveryCorrection) < 0) {
      m_TRACK()
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // PdSetAlarm


// Public function : see description in .h
int PdCheckTimeout (PD_HANDLE handle) {
  m_DIGGY_BOLLARD()
  int answer = ANSWER__YES ; // "timeout" a priori

  if (handle->nap_waitingPlan != NULL &&
      handle->nap_waitingPlan->waitingPlan == DEEPLY_BLOCKING__WAITING_PLAN) {
    answer = ANSWER__NO ; // no timeout for the moment...

    if (handle->nap_waitingPlan->c_deadline > 0) {
      time_t now ;

      time(&now) ;
      if (now >= handle->c_control.cc_baseEpochTime + handle->nap_waitingPlan->c_deadline || // timeout
          now < handle->c_control.cc_baseEpochTime) { // system "returns in the past" ! => provoke timeout
        answer = ANSWER__YES ;
      } else {
        // => now < handle->c_control.cc_baseEpochTime + handle->nap_waitingPlan->c_deadline
        //    (and now >= handle->c_control.cc_baseEpochTime)
        // => now - handle->c_control.cc_baseEpochTime < handle->nap_waitingPlan->c_deadline
        //    (and now - handle->c_control.cc_baseEpochTime >= 0)
        handle->c_control.alarmDeliveryCorrection = now - handle->c_control.cc_baseEpochTime ;
        // => handle->c_control.alarmDeliveryCorrection < handle->nap_waitingPlan->c_deadline
        //    (and handle->c_control.alarmDeliveryCorrection >= 0)
        // => handle->nap_waitingPlan->c_deadline - handle->c_control.alarmDeliveryCorrection > 0 
        // => assert "ASS" above is still verified...
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(answer)
} // PdCheckTimeout


// Public function : see description in .h
int PdClearAlarm (PD_HANDLE handle) {
  m_DIGGY_BOLLARD()
  if (handle->nap_waitingPlan != NULL &&
      handle->nap_waitingPlan->waitingPlan != NON_BLOCKING__WAITING_PLAN &&
      handle->nap_waitingPlan->c_deadline > 0 &&
      handle->n_alarmSystemHandle != NULL) {
    m_TRACK_IF(SetAlarm(handle->n_alarmSystemHandle,0) < 0)
  } // if

  m_DIGGY_RETURN(RETURNED)
} // PdClearAlarm



// Public function : see description in .h
int PdDestroyInstance (PD_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle); 
  m_DIGGY_RETURN(RETURNED)
} // PdDestroyInstance
