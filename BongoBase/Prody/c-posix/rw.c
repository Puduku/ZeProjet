// c-posix/rw.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-posix/rw.h"
#include "c-posix/tryagain.h"

#include "c-ansi/diggy.h"

#define BLAME__READ     "read()"
#define BLAME__WRITE    "write()"
#define BLAME__CLOSE    "close()"


// Public function : see .h
int ProtectedRead2 (int descriptor, PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
  char *readBuffer, int readBufferSize, int *ac_readLength) {
  m_DIGGY_BOLLARD()

  m_ASSERT(readBufferSize != 0)

  m_TRACK_IF(PdSetDeadline(pdHandle,ap_waitingPlan) < 0)

  int n_rwStatus = -1 ; // Unknown status for the moment
  while (n_rwStatus == -1) {
    int ret ;

    m_TRACK_IF(PdSetAlarm(pdHandle) != RETURNED)

    ret = read(descriptor,readBuffer,readBufferSize) ;
    if (ret == -1) {
      switch (errno) {
      case EINTR: // Interrupted by signal (blocking mode)
      case EAGAIN: // no data (non blocking mode) / socket's timeout (setsocktopt(SO_RCVTIMEO))
#if (EAGAIN != EWOULDBLOCK)
      case EWOULDBLOCK:
#endif
        switch (PdCheckTimeout(pdHandle)) {
        case ANSWER__YES: // timeout
          n_rwStatus = RW_STATUS__TRY_AGAIN ;
        break; case ANSWER__NO: // NO timeout
          if (b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno)) {
            SLEEP_JUST_TO_AVOID_CPU_HOGGING();
          } // if
        break; default:
          m_TRACK()
        } // switch
      break; case EIO: // "(let's say) disk I/O error" => UE2 ;
      // TODO: other UE2 errno values ???
      case ECONNABORTED: // UE4 
      case ECONNRESET: // Connection forcibly reset by peer => UE4
        n_rwStatus = RW_STATUS__CONNECTION_LOST ;
      break; default: // unexpected errno value
        m_PERROR(BLAME__READ)
      } // switch

    } else if (ret == 0) { // "EOF"
      n_rwStatus = RW_STATUS__TERMINATING ;
    } else { // Got data feed
      *ac_readLength = ret ;
      n_rwStatus = RW_STATUS__OK ;
    } // if
  } // while

  m_TRACK_IF(PdClearAlarm(pdHandle) != RETURNED)

  m_DIGGY_RETURN(n_rwStatus)
} // ProtectedRead2


// Public function : see .h
int ProtectedRead (int descriptor, PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
  char *readBuffer, int readBufferSize, int *ac_readLength) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(SynchronizeONonblock(descriptor,ap_waitingPlan) != RETURNED)

  int rwStatus =
  ProtectedRead2(descriptor,pdHandle,ap_waitingPlan,readBuffer,readBufferSize,ac_readLength);
  m_TRACK_IF(rwStatus < 0)

  m_DIGGY_RETURN(rwStatus)
} // ProtectedRead


struct BROKEN_PIPE_FIX {
  unsigned int dummy ;
} ; 

static BROKEN_PIPE_FIX_HANDLE n_uniqueBrokenFixHandle = NULL ;


// Public function : see .h
int BrokenPipeFixCreateInstance (BROKEN_PIPE_FIX_HANDLE *azdh_handle) {
  m_DIGGY_BOLLARD()
  static struct BROKEN_PIPE_FIX uniqueBrokenPipeFix = { .dummy = UNDEFINED } ;
 
  m_ASSERT(n_uniqueBrokenFixHandle == NULL)

  m_PERROR_VERBATIM_IF(signal(SIGPIPE,SIG_IGN) == SIG_ERR)

  *azdh_handle = n_uniqueBrokenFixHandle = &uniqueBrokenPipeFix; 

  m_DIGGY_RETURN(RETURNED)
} // BrokenPipeFixCreateInstance


// Public function : see .h
int BrokenPipeFixDestroyInstance (BROKEN_PIPE_FIX_HANDLE xdh_handle) {
  m_DIGGY_BOLLARD()
  n_uniqueBrokenFixHandle = NULL; 
  m_DIGGY_RETURN(RETURNED)
} // BrokenPipeFixDestroyInstance


// Public function : see description in .h
int ProtectedWrite2 (BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle, int descriptor,
  PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
  const char *p_writeBuffer, int writeLength, int *ac_writtenLength) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PdSetDeadline(pdHandle,ap_waitingPlan) < 0) 

  m_ASSERT(writeLength != 0)

  int n_rwStatus = -1; // unknown status for the moment...
  while (n_rwStatus == -1) {
    int ret ;

    m_TRACK_IF(PdSetAlarm(pdHandle) != RETURNED) 
 
    ret = write(descriptor,p_writeBuffer,writeLength) ;
    if (ret == -1) {
      switch (errno) {
      case ENOSPC: // UE1
      case EPIPE: // UE3
        n_rwStatus = RW_STATUS__TERMINATING; // (rough) session termination
      break; case EIO: // UE2
      case ECONNABORTED: // UE4
      case ECONNRESET: // "Connection forcibly reset by peer" => UE4
        n_rwStatus = RW_STATUS__CONNECTION_LOST ; 
      break; case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
      case EWOULDBLOCK: // stream is full (non blocking mode)
#endif
      case EINTR: // Interrupted by signal (blocking mode)
        switch (PdCheckTimeout(pdHandle)) {
        case ANSWER__YES:
          n_rwStatus = RW_STATUS__TRY_AGAIN ; // timeout
          break;
        case ANSWER__NO:
          if (b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno)) {
            SLEEP_JUST_TO_AVOID_CPU_HOGGING();
          } // if
        break; default:
          m_TRACK()
        } // switch
      break; default:
        m_PERROR(BLAME__WRITE)
      } // switch

    } else if (ret == 0) {
      m_RAISE(ANOMALY__VALUE__FMT_D,ret)
    } else { // Some piece of data has been written 
      n_rwStatus = RW_STATUS__OK ; 
      *ac_writtenLength = ret ;
    } // if
  } // while

  m_TRACK_IF(PdClearAlarm(pdHandle) != RETURNED) 

  m_DIGGY_RETURN(n_rwStatus)
} // ProtectedWrite2

// Public function: see .h
int ProtectedWrite (BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle, int descriptor,
  PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
  const char *p_writeBuffer, int writeLength, int *ac_writtenLength) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(SynchronizeONonblock(descriptor,ap_waitingPlan) != RETURNED)

  int rwStatus =
  ProtectedWrite2(brokenPipeFixHandle,descriptor,pdHandle,ap_waitingPlan,
    p_writeBuffer,writeLength,ac_writtenLength);
  switch (rwStatus) {
  case RW_STATUS__OK :
  case RW_STATUS__TRY_AGAIN:
  case RW_STATUS__TERMINATING:
  case RW_STATUS__CONNECTION_LOST:
  break; default :
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(rwStatus)
} // ProtectedWrite

// close() wrapper
// --------------

// Public function: see description in .h
int ProtectedClose (int xh_descriptor) {
  m_DIGGY_BOLLARD()
  int rwCloseStatus = RW_CLOSE_STATUS__OK ; // a priori...

  if (close(xh_descriptor) == -1) {
    switch (errno) {
    case ENOSPC:
    case EIO: // such case may occur with NFS mount (in particular)
      // TODO: what is not clear is whether descriptor is really closed ??
      rwCloseStatus = RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST ;
    break; case EINTR:
    case EAGAIN:
#if (EAGAIN != EWOULDBLOCK)
    case EWOULDBLOCK: 
#endif
      rwCloseStatus = RW_CLOSE_STATUS__CANNOT_HANG_UP ;
    break; default:
      m_PERROR(BLAME__CLOSE)
    } // switch 
  } // if

  m_DIGGY_RETURN(rwCloseStatus)
} // ProtectedClose 




