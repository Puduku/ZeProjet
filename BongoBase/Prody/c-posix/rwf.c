// c-posix/rwf.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/types.h"
#include "c-ansi/stderr.h"

#include "c-posix/tryagain.h"
#include "c-posix/rwf.h"

#include "c-ansi/diggy.h"


#define BLAME__OPEN		"open()"
#define BLAME__FCNTL	"fcntl()"
#define BLAME__FLOCK	"flock()"

const struct WAITING_PLAN p_fileStreamSimpleWaitingPlan = { .waitingPlan = 
  DEEPLY_BLOCKING__WAITING_PLAN, .c_deadline = 10, };

// Public function : see .h
int ProtectedOpenFile (const char *p_filePathname, int flags, mode_t creationMode,
                       int *aczh_fileDescriptor, int *na_openErrno) {
  m_DIGGY_BOLLARD()
  int answer = ANSWER__YES; // a priori
  int ret = UNDEFINED;

  if (na_openErrno != NULL) {
    *na_openErrno = 0;
  } // if

  ret = open(p_filePathname,flags,creationMode) ;
  if (ret == -1) {
    if (na_openErrno != NULL) {
      *na_openErrno = errno ;
    } // if
    switch (errno) {
    case ENOENT: 
    case ENOTDIR:
    case EACCES: 
    case EROFS:
    case EEXIST: 
    case EISDIR:
    case ENXIO:
    case ETXTBSY:
    case EAGAIN: // "non blocking" open would have blocked
#if (EAGAIN != EWOULDBLOCK)
    case EWOULDBLOCK:
#endif
    case EINTR: // open() was blocked and has been interrupted
      answer = ANSWER__NO ; 

    break; default: // unexpected problem
      m_PERROR(BLAME__OPEN)
    } // switch 

  } else {
    *aczh_fileDescriptor = ret ;
  } // if

  m_DIGGY_RETURN(answer)
} // ProtectedOpenFile


// Public function : see .h
int ProtectedFcntlFSetlk (int fileDescriptor, PD_HANDLE pdHandle, struct WAITING_PLAN waitingPlan,
  struct flock *a_flock) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PdSetDeadline(pdHandle,waitingPlan) < 0)

  int request = F_SETLKW ;
  if (waitingPlan.waitingPlan == NON_BLOCKING__WAITING_PLAN) {
    request = F_SETLK ;
  } // if

  // Attempt to set the lock
  int n_rwfFsetLkStatus = -1 ; // Unknown for the moment
  while (n_rwfFsetLkStatus == -1) {
    m_TRACK_IF(PdSetAlarm(pdHandle) != RETURNED)

    int ret = fcntl(fileDescriptor,request,a_flock) ;
    if (ret == -1) {
      switch (errno) {
      case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
      case EWOULDBLOCK:
#endif
      case EACCES: // // Cannot lock immediately (F_SETLK)
      case EINTR: // Interrupted by signal (F_SETLKW or F_SETLK on remote file)
        switch (PdCheckTimeout(pdHandle)) {
        case ANSWER__YES: // timeout
          n_rwfFsetLkStatus = RWF_FSET_LK_STATUS__TRY_AGAIN ;
        break; case ANSWER__NO: // NO timeout
          if (b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno)) {
            SLEEP_JUST_TO_AVOID_CPU_HOGGING();
          } // if
        break; default:
          m_TRACK()
        } // switch
      break; case EDEADLK: //
        n_rwfFsetLkStatus = RWF_FSET_LK_STATUS__DEADLOCK ;
      break; case EBADF:  // lock is not compatible with file descriptor
      default: // unexpected problem
        m_PERROR(BLAME__FCNTL)
      } // switch
    } else {
      n_rwfFsetLkStatus = RWF_FSET_LK_STATUS__OK ;
    } // if
  } // while 

  m_TRACK_IF(PdClearAlarm(pdHandle) != RETURNED)
  
  m_DIGGY_RETURN(n_rwfFsetLkStatus)
} // ProtectedFcntlFSetlk


// Public function : see .h
int ProtectedFlock (int fileDescriptor, PD_HANDLE pdHandle, struct WAITING_PLAN waitingPlan,
  int operation) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PdSetDeadline(pdHandle,waitingPlan) != RETURNED)

  if (waitingPlan.waitingPlan == NON_BLOCKING__WAITING_PLAN) {
    operation |= LOCK_NB;
  } // if
  // Attempt to set the lock
  int n_attempt = -1 ; // Unknown for the moment
  while (n_attempt == -1) {
    m_TRACK_IF(PdSetAlarm(pdHandle) != RETURNED)

    if (flock(fileDescriptor,operation) == -1) {
      switch (errno) {
      case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
      case EWOULDBLOCK:
#endif
      case EACCES: // // Cannot lock immediately (F_SETLK)
      case EINTR: // Interrupted by signal (F_SETLKW or F_SETLK on remote file)
        switch (PdCheckTimeout(pdHandle)) {
        case ANSWER__YES: // timeout
          n_attempt = ATTEMPT__TRY_AGAIN ;
        break; case ANSWER__NO: // NO timeout
          if (b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno)) {
            SLEEP_JUST_TO_AVOID_CPU_HOGGING();
          } // if
        break; default:
          m_TRACK()
        } // switch
      break; case EDEADLK: //
      case EBADF:  // lock is not compatible with file descriptor
      default: // unexpected problem
        m_PERROR(BLAME__FLOCK)
      } // switch
    } else {
      n_attempt = ATTEMPT__SUCCESSFUL ;
    } // if
  } // while

  m_TRACK_IF(PdClearAlarm(pdHandle) != RETURNED)
  m_DIGGY_RETURN(n_attempt)
} // ProtectedFlock
