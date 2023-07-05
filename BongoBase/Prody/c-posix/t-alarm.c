#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "c-posix/t-alarm.h"
#include "c-ansi/stderr.h"
#include "c-ansi/att.h"
#include "c-ansi/types.h"
#include "c-ansi/green.h"
#include "c-posix/att-glue.h"
#include "c-posix/tid-glue.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// TODO: Et si on for(ni)k() ? les threads restent ???? avec le mesme tid (ce serait bizarre) ????!!!!!

// Inventory of threads intervening in the alarm system:
// ----------------------------------------------------
// - "initiator" & "liquidator" threads: initialization / liquidation of the alarm system
//   By definition:
//   + "initiator" == the thread calling PthreadAlarmSystemCreateInstance() public function.
//   + "liquidator" == the thread calling PthreadAlarmSystemDestroyInstance() public function.
//   => Typically, the "main" thread of the process acts both as "initiator" AND "liquidator"...
// - "custodians": dedicated threads to support the alarm system (of the process) ;
//   there are actually two custodians:
//   + "the" custodian: handles the alarms timetable (client requests) in "real time"...
//   + "assistant custodian" : helps "the custodian" to recover in case of system time "leap"
// - "client" : any thread (custodians excepted...) using the alarm system - i.e calling SetAlarm()
//   public function...

enum {
        THE_CUSTODIAN,
  ASSISTANT_CUSTODIAN,
    CUSTODIANS_NUMBER
} ;

// ==> Technical note: reason for presence of assistant...
// workaround for pthread_cond_timedwait() (unfortunate) "feature":
// the timeouts are based on the system time (rather than explicit delay)
// the timeouts are thus affected by any system time change.
// In particular, if system time goes far in "the past", all the timeouts are de facto disabled!
// The assistant simply watches out the evolution of the system time, and books the difference 
// vis-a-vis the initial system time (which remains the reference time of the alarm system) 

struct ALARM_SYSTEM {
  pthread_mutex_t hj_mutex;
  // Note: "h_cond" is, stricto sensu, NOT tied to the mutex;
  // however:
  // - pthread_cond_wait() technically imposes mutex (passed in argument) to be locked;
  // - though pthread_cond_signal() doesn't technically impose a locked mutex, that disposition
  //   enforces efficiency of condition raising mechanism (that is, the condition checked by the
  //   "awaken" thread corresponds to that the "waker" thread wants to signal)
  // Practical usage:
  // - in "nominal" situation, "THE" custodian uses "cond" to wait for condition changes
  //   raised by "liquidator (alias main)", "client" or "assistant custodian" thread
  pthread_cond_t h_cond ;

  // All data shared by the threads; only accessible if mutex is locked:
  struct {
    ATT_GLUE_HANDLE h_attGlueHandle;
    TID_GLUE_HANDLE h_tidGlueHandle;
    ALARM_TIMETABLE_HANDLE h_alarmTimetableHandle;
    time_t wakeupEpochTime; // updated by "the custodian" ;
    // used by "clients" ; meaning:
    // programmed "custodian" 's wake up (real system) time
    int systemTimeAdvancement ; // updated by "the assistant"; 
    // used by "the custodian" and "clients" : 
    // - meaning for "the custodian" and the "clients": difference between current system time and
    //   alarm system's reference time ; hence this value is  
    //   + positive when current system time has been advanced vis-a-vis the initial system time 
    //   + negative when current system time has been moved back (in the past) vis-a-vis the 
    //     initial system time
    // - meaning for "the assistant": accumulation of all system time changes observed since 
    //   the "beginning"...       
  } c_shared ; 
  struct { // Private to...
    int phantomPthreadsCount ; // ..."the custodian"
    pthread_t custodianPthreads[CUSTODIANS_NUMBER] ; // ..."main" thread
  } private ;
} ;


// => "General utils" (bah!)
// -------------------------

// Wrapping "pthread" API primitives that are NOT supposed to fail...
#define m_PTHREAD_CALL(m_call) {\
  int tc_ret = m_call;\
  m_RAISE_IF(tc_ret != 0,  #m_call " ret =%d",tc_ret)\
}

// => Code run by "Initiator" thread
// ---------------------------------

// These functions are intended as start_routine parameter for pthread_create() function, in order to 
// start the "custodians" threads.
//
// Returned:
// NULL pointer
static void *TheCustodianPthreadRoutine (void *r_alarmSystemHandle) ;
static void *AssistantCustodianPthreadRoutine (void *r_alarmSystemHandle) ;


// Passed:
// - handle:
//
// Return:
// - RETURNED:
// - -1: unexpected problem; anomaly is raised
static int LaunchCustodians (ALARM_SYSTEM_HANDLE handle) {
  pthread_attr_t attr;

  m_PTHREAD_CALL(pthread_attr_init(&attr))
  m_PTHREAD_CALL(pthread_attr_setstacksize(&attr,32*1024))
  m_PTHREAD_CALL(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE))

  m_PTHREAD_CALL(pthread_create(&handle->private.custodianPthreads[THE_CUSTODIAN],&attr,
                 TheCustodianPthreadRoutine,(void *)handle))
  m_PTHREAD_CALL(pthread_create(&handle->private.custodianPthreads[ASSISTANT_CUSTODIAN],&attr,
                 AssistantCustodianPthreadRoutine,(void *)handle))

  m_PTHREAD_CALL(pthread_attr_destroy(&attr))

  return RETURNED;
} // LaunchCustodians



// Initialize alarm system structure
//
// Returned:
// - RETURNED: initialized
// - -1: unexpected problem ; anomaly is raised
static int CreateAlarmSystemData(ALARM_SYSTEM_HANDLE z_handle) {
  // 1. Initialize static data

  m_TRACK_IF(AttGlueCreateInstance(&z_handle->c_shared.h_attGlueHandle) != RETURNED)
  m_TRACK_IF(TidGlueCreateInstance(&z_handle->c_shared.h_tidGlueHandle,b_PORTABLE) != RETURNED)
  m_TRACK_IF(AlarmTimetableCreateInstance(&z_handle->c_shared.h_alarmTimetableHandle) != RETURNED)

  pthread_mutexattr_t mutexattr;

  m_PTHREAD_CALL(pthread_mutexattr_init(&mutexattr))
  m_PTHREAD_CALL(pthread_mutexattr_settype(&mutexattr,PTHREAD_MUTEX_ERRORCHECK))
#ifndef __APPLE__
  m_PTHREAD_CALL(pthread_mutexattr_setrobust(&mutexattr,PTHREAD_MUTEX_ROBUST))
#endif
  m_PTHREAD_CALL(pthread_mutex_init(&z_handle->hj_mutex,&mutexattr))
  m_PTHREAD_CALL(pthread_mutexattr_destroy(&mutexattr))

  m_PTHREAD_CALL(pthread_cond_init(&z_handle->h_cond,NULL))

  z_handle->c_shared.wakeupEpochTime = time(NULL);
  z_handle->c_shared.systemTimeAdvancement = 0;

  return RETURNED;
} // CreateAlarmSystemData


// sighandler_t handler (see in <signal.h>)
static void  SigalrmHandler (int signum) {
  // uniqueAlarmSystem.private.sigalrmCounter++;
} //SigalrmHandler


static ALARM_SYSTEM_HANDLE n_uniqueAlarmSystemHandle = NULL;


// Public function: see description in .h
int PthreadAlarmSystemCreateInstance (ALARM_SYSTEM_HANDLE *azdh_handle) {
  m_DIGGY_BOLLARD()
  static struct ALARM_SYSTEM uniqueAlarmSystem ;

  const struct sigaction sigAction = {
    .sa_handler = SigalrmHandler ,
    //.sa_mask = 0 ,
    .sa_flags = SA_RESTART ,
   } ;

  m_ASSERT(n_uniqueAlarmSystemHandle == NULL)

  m_PERROR_VERBATIM_IF(sigaction(SIGALRM,&sigAction,NULL) < 0);

  *azdh_handle = n_uniqueAlarmSystemHandle = &uniqueAlarmSystem ;

  m_TRACK_IF(CreateAlarmSystemData(n_uniqueAlarmSystemHandle) != RETURNED)

  m_TRACK_IF(LaunchCustodians(n_uniqueAlarmSystemHandle) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // PthreadAlarmSystemCreateInstance


// => Macros used in code for "clients" & "the custodian" threads
// --------------------------------------------------------------

#define ALARM_2_REAL_SYSTEM_TIME(/*ALARM_SYSTEM_HANDLE*/m_handle, /*gen_ATT_TIME_long*/attTime) \
  (ATT_2_EPOCH_TIME((m_handle)->c_shared.h_attGlueHandle,attTime) + \
  (m_handle)->c_shared.systemTimeAdvancement)

#define REAL_SYSTEM_2_ALARM_TIME(/*ALARM_SYSTEM_HANDLE*/m_handle, /*time_t*/epochTime) \
  (EPOCH_2_ATT_TIME((m_handle)->c_shared.h_attGlueHandle,epochTime - \
  (m_handle)->c_shared.systemTimeAdvancement))   


// => Code run by "client" threads
// -------------------------------

// Public function; see description in .h 
int SetAlarm (ALARM_SYSTEM_HANDLE p_handle, int seconds) {
  m_DIGGY_BOLLARD()
  time_t nowEpochTime = time(NULL);
  int ret = 0 ; // a priori

  m_PTHREAD_CALL(pthread_mutex_lock(&p_handle->hj_mutex))

  { // pipi
    gen_ATT_TIME_long c_ancientAttTime = UNDEFINED;
    tid_t tid = (tid_t) UNDEFINED;

    m_TRACK_IF(TidGlueSelfPthread2Tid(p_handle->c_shared.h_tidGlueHandle, &tid) != RETURNED)

    switch (AlarmTimetableProgram(p_handle->c_shared.h_alarmTimetableHandle,
      TID_2_ATT_ID(p_handle->c_shared.h_attGlueHandle,tid),seconds > 0,
      REAL_SYSTEM_2_ALARM_TIME(p_handle,nowEpochTime) + seconds, &c_ancientAttTime)) {
    case RESULT__FOUND: // ancient alarm updated or removed
      ret = (int) (ALARM_2_REAL_SYSTEM_TIME(p_handle,c_ancientAttTime) - nowEpochTime) ;
      if (ret < 0) { // Not relevant, doesn't matter...
        ret = 0 ;
      } // if
    break; case RESULT__NOT_FOUND: // New alarm
    break; default:
      m_TRACK()
    } // switch
  } // pipi

  { // caca 
    gen_ATT_TIME_long c_refreshAttTime = UNDEFINED;
 
    switch (AlarmTimetableGetRefreshTime(p_handle->c_shared.h_alarmTimetableHandle,
      &c_refreshAttTime)) {
    case RESULT__FOUND: // There is at least one alarm in the timetable
      if (ALARM_2_REAL_SYSTEM_TIME(p_handle,c_refreshAttTime) < p_handle->c_shared.wakeupEpochTime) {
        m_PTHREAD_CALL(pthread_cond_signal(&p_handle->h_cond))
      } // if
    break; case RESULT__NOT_FOUND: // timetable is empty
    break; default:
      m_TRACK()
    } // switch
  } // caca

  m_PTHREAD_CALL(pthread_mutex_unlock(&p_handle->hj_mutex))

  m_DIGGY_RETURN(ret)
} // SetAlarm 


// => Code run by "The custodian":
// -------------------------------

// #SEE RING_ALARM_FUNCTION@c-ansi/att.h 
static int RingAlarm (void *pr_handle, unsigned int attId, unsigned long delay) {
  m_DIGGY_BOLLARD_S()
  ALARM_SYSTEM_HANDLE p_handle = (ALARM_SYSTEM_HANDLE) pr_handle;
  pthread_t pthread = (pthread_t) UNDEFINED;

  int result = TidGlueTid2Pthread(p_handle->c_shared.h_tidGlueHandle,
    ATT_ID_2_TID(p_handle->c_shared.h_attGlueHande,attId),&pthread);
  m_TRACK_IF(result < 0)
  m_ASSERT(result == RESULT__FOUND)

  int ret = pthread_kill(pthread,SIGALRM) ;

  // TODO: Linux: segfault if no thread (in lieu of ESRCH)
  switch (ret) {
  case 0: // OK
  break; case ESRCH: // No such thread
    p_handle->private.phantomPthreadsCount++ ;
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,ret)
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // RingAlarm


// Handle the alarm timetable...
//
// Ret:
// - RETURNED: terminated (*) 
// (*) : actually in current design, the custodian should never "terminate" (the "main" thread
//       rather "cancels" the custodians)
// - -1: unexpected problem ; anomaly is raised
static int TheCustodianMain (ALARM_SYSTEM_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  handle->private.phantomPthreadsCount = 0;

  m_PTHREAD_CALL(pthread_mutex_lock(&handle->hj_mutex))

  while (b_TRUE) {
    struct timespec timeSpecification ;
    clock_gettime(CLOCK_REALTIME,&timeSpecification) ; 
    
    int count = AlarmTimetableRefresh(handle->c_shared.h_alarmTimetableHandle,
      ALARM_2_REAL_SYSTEM_TIME(handle,timeSpecification.tv_sec), RingAlarm,handle) ;
    m_TRACK_IF(count < 0)

    { // prout
      gen_ATT_TIME_long c_firstExpiringAttTime;

      switch (AlarmTimetableGetRefreshTime(handle->c_shared.h_alarmTimetableHandle,
        &c_firstExpiringAttTime)) {
      case RESULT__FOUND: // It remains alarm(s) in the timetable
        if (ALARM_2_REAL_SYSTEM_TIME(handle,c_firstExpiringAttTime) <= timeSpecification.tv_sec) {
          timeSpecification.tv_sec++ ;
        } else {
          timeSpecification.tv_sec = ALARM_2_REAL_SYSTEM_TIME(handle,c_firstExpiringAttTime) ;
        } // if
      break; case RESULT__NOT_FOUND: // The timetable is empty
        timeSpecification.tv_sec += 10 ; 
      break; default:
        m_TRACK()
      } // switch
    } // prout
  
    handle->c_shared.wakeupEpochTime = timeSpecification.tv_sec;

    { // zzz...
      int ret = pthread_cond_timedwait(&handle->h_cond,&handle->hj_mutex,
                                       &timeSpecification) ;
      m_RAISE_IF(ret != 0 && ret != ETIMEDOUT, ANOMALY__VALUE__FMT_D,ret)
    } // zzz...
  } // while 

  // Currently, there is no termination mechanism (the thread is canceled)
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)

  m_PTHREAD_CALL(pthread_mutex_unlock(&handle->hj_mutex))
  m_DIGGY_RETURN(RETURNED)
} // TheCustodianMain


// Description: see above...
static void *TheCustodianPthreadRoutine (void *r_alarmSystemHandle) {
  ALARM_SYSTEM_HANDLE handle = (ALARM_SYSTEM_HANDLE) r_alarmSystemHandle ;
  if (TheCustodianMain(handle) != RETURNED) {
    m_TRACK_AND(exit(-1);)
  } // if

  return NULL;
} // TheCustodianPthreadRoutine



// => Code run by "assistant custodian"
// ------------------------------------

//
// Ret:
// - RETURNED: terminated (*)
// (*) : actually in current design, the assistant should never "terminate" (the "main" thread
//       rather "cancels" the custodians)
// - -1 : unexpected problem ; anomaly is raised
static int AssistantCustodianMain (ALARM_SYSTEM_HANDLE handle) {
  m_DIGGY_BOLLARD_S()

  time_t lastEpochTime = time(NULL) ;

  while (b_TRUE) {
    int ret = sleep(1);
    m_TRACK_IF(ret < 0)
    int sleepDuration = 1 - ret ;
    
    { // pif...
      time_t epochTime = time(NULL);
      int localSystemTimeAdvancement = (int) epochTime - (lastEpochTime + sleepDuration) ;
      
      if (localSystemTimeAdvancement < -2 || localSystemTimeAdvancement > 2) { // Noticeable system time change
        m_PTHREAD_CALL(pthread_mutex_lock(&handle->hj_mutex))
         handle->c_shared.systemTimeAdvancement += localSystemTimeAdvancement ;
        // Warn "the custodian"
        m_PTHREAD_CALL(pthread_cond_signal(&handle->h_cond))
        m_PTHREAD_CALL(pthread_mutex_unlock(&handle->hj_mutex))
      } // if

      lastEpochTime = epochTime ; // Note: we assume that "pif" duration is negligible vis-a-vis
                                  // our time (rough...) measurements (that is << 1 sec)
    } // pif...
  } // while

  // Currently, there is no termination mechanism:
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)

  m_DIGGY_RETURN(RETURNED)
} // AssistantCustodianMain


// Description: see above...
static void *AssistantCustodianPthreadRoutine (void *r_alarmSystemHandle) {
  ALARM_SYSTEM_HANDLE handle = (ALARM_SYSTEM_HANDLE) r_alarmSystemHandle ;

  if (AssistantCustodianMain(handle) < 0) {
    m_TRACK_AND(exit(-1);)
  } // if

  return NULL;
} // AssistantCustodianPthreadRoutine


// => Code run by "Liquidator" (alias "main") thread
// =================================================

// Public function; see description in .h 
int PthreadAlarmSystemDestroyInstance (ALARM_SYSTEM_HANDLE xhj_handle) {
  m_DIGGY_BOLLARD()
  int i ;

  // 1. Liquidate "custodians" 

  for (i = 0; i < CUSTODIANS_NUMBER; i++) {
    void *r_status = (void *) UNDEFINED;

    m_PTHREAD_CALL(pthread_cancel(xhj_handle->private.custodianPthreads[i]))
    m_PTHREAD_CALL(pthread_join(xhj_handle->private.custodianPthreads[i],&r_status))
    m_RAISE_IF(r_status != PTHREAD_CANCELED, ANOMALY__VALUE__FMT_P,r_status)
  } // for

  // 2. Liquidate stuff

  m_TRACK_IF(AlarmTimetableDestroyInstance(xhj_handle->c_shared.h_alarmTimetableHandle) != RETURNED)
  m_TRACK_IF(TidGlueDestroyInstance(xhj_handle->c_shared.h_tidGlueHandle) != RETURNED)
  m_TRACK_IF(AttGlueDestroyInstance(xhj_handle->c_shared.h_attGlueHandle) != RETURNED)
  m_PTHREAD_CALL(pthread_cond_destroy(&xhj_handle->h_cond))
#ifndef __APPLE__
  m_PTHREAD_CALL(pthread_mutex_destroy(&xhj_handle->hj_mutex))
#endif

  m_TRACK_IF(n_uniqueAlarmSystemHandle != xhj_handle)
  n_uniqueAlarmSystemHandle = NULL; 

  m_DIGGY_RETURN(RETURNED)
} // PthreadAlarmSystemDestroyInstance
