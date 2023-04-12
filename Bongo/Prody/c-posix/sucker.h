// c-posix/sucker.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: "universal" transfer agent
// =======

#ifndef __C_POSIX_SUCKER_H_INCLUDED__
#define __C_POSIX_SUCKER_H_INCLUDED__

#include <unistd.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/g-string.h"
#include "c-ansi/sucker.h"

#include "c-posix/tryagain.h"
#include "c-posix/rw.h"
#include "c-posix/rwf.h"

// Sucker...
// ------

struct SUCKER ; // private...
typedef struct SUCKER *SUCKER_HANDLE ;


#define BATEAU__S_STREAM_READ_BUFFER_SIZE 4096

// Create a sucker...
//
// Passed:
// - azh_handle: address of sucker handle to initialize
// - nf_alarmSystemHandle : alarm system used for temporizations (see c-posix/alarm.h)
//   + NULL special pointer (not provided) ; limited functionality ; no temporization is possible ;
//     blocking deadlines are de facto indefinite...
//   + non NULL pointer: actual alarm system ; full temporization support
// - f_brokenPipeFixHandle: see BrokenPipeFixCreateInstance() in c-posix/rw.h
// - sStreamReadBufferSize: >0 ; size (in bytes) of read buffer (for stream source butt). If you 
//   have no idea, specify BATEAU__S_STREAM_READ_BUFFER_SIZE...
// - suckingLimit: limit (in bytes) of data volume that can be sucked (0 for no limit)
//
// Changed:
// - *azh_handle: "sucker" handle initialized.
//
// Ret:
// - RETURNED: OK
// - -1 : unexpected problem; anomaly is raised
int SuckerCreateInstance (SUCKER_HANDLE *azh_handle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle,
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle, int sStreamReadBufferSize, int suckingLimit) ;


// #SEE SuckerGetSuckingLimit @ c-ansi/sucker.h
int SuckerGetSuckingLimit (SUCKER_HANDLE handle) ;


enum {
// #REF enum-SUCKER_BUTT
    STREAM__SUCKER_BUTT, // any "connected" descriptor...
  G_STRING__SUCKER_BUTT, // g-string used as "buffer"
};


// #REF STREAM_WAITING_PLAN 
// int waitingPlan; => choose your plan. If connected descriptor is currently not available,
//  sucker...
//  - NON_BLOCKING__WAITING_PLAN: ...flops "immediately" !
//  - LIGHTLY_BLOCKING__WAITING_PLAN: ...blocks till stream is available, BUT
//    flops directly if the process / thread is "interrupted"
//  - DEEPLY_BLOCKING__WAITING_PLAN: ...blocks till stream is available ;
//    flops after a STRICT deadline expiration.
//  int c_deadline; => Only significant for blocking waiting plan ;
//  delay, in seconds of timeout (0 for infinite, that is "no deadline")


struct SUCKER_BUTT {
  // #REF struct-SUCKER_BUTT
  int suckerButt ;  // #SEE enum-SUCKER_BUTT
  union { //
    struct { // [[ With STREAM__SUCKER_BUTT ]]
      int descriptor ; // e.g. connected socket
      struct WAITING_PLAN waitingPlan ; // #SEE STREAM_WAITING_PLAN
    } c_stream ; //
    G_STRING_STUFF c_gStringStuff; // [[ With G_STRING__SUCKER_BUTT ]]
  } select; //
} ;


// Initialize a stream sucker butt.
// 
// Passed:
// - m_suckerButt: structure to initialize
// - m_descriptor: any connected descriptor
// - streamWaitingPlan: 
//   #SEE STREAM_WAITING_PLAN
#define m_ASSIGN_SUCKER_BUTT__STREAM(/*struct SUCKER_BUTT*/ m_suckerButt,\
  /*int*/m_descriptor, /*struct WAITING_PLAN*/p_streamWaitingPlan) {\
  (m_suckerButt).suckerButt = STREAM__SUCKER_BUTT;\
  (m_suckerButt).select.c_stream.descriptor = m_descriptor;\
  (m_suckerButt).select.c_stream.waitingPlan = p_streamWaitingPlan;\
}

extern const struct WAITING_PLAN p_suckerFileStreamWaitingPlan; 

// Initialize a "file" stream sucker butt.
// 
// Passed:
// - m_suckerButt: structure to initialize
// - fileDescriptor: descriptor, supposed a regular open file (see c-posix/rw.h)
//   must stay open while butt is in use...
#define m_ASSIGN_SUCKER_BUTT__FILE_STREAM(/*struct SUCKER_BUTT*/m_suckerButt,\
  /*int*/fileDescriptor) \
m_ASSIGN_SUCKER_BUTT__STREAM(m_suckerButt, fileDescriptor, p_suckerFileStreamWaitingPlan)


// Initialize a g-string sucker butt.
// 
// Passed:
// - m_suckerButt: structure to initialize
// - gStringStuff: g-string used as buffer
#define m_ASSIGN_SUCKER_BUTT__G_STRING(/*struct SUCKER_BUTT*/ m_suckerButt,\
  /*G_STRING_STUFF*/ gStringStuff) {\
  (m_suckerButt).suckerButt = G_STRING__SUCKER_BUTT;\
  (m_suckerButt).select.c_gStringStuff = gStringStuff;\
}

#define m_ASSIGN_LOCAL_SUCKER_BUTT__G_STRING(/*struct SUCKER_BUTT*/ m_localSuckerButt,\
  /*G_STRING_STUFF*/ gStringStuff) \
  struct SUCKER_BUTT m_localSuckerButt;\
  m_ASSIGN_SUCKER_BUTT__G_STRING(m_localSuckerButt,gStringStuff)


// #SEE SuckerPlugSButt@c-ansi/sucker.h
int SuckerPlugSButt(SUCKER_HANDLE handle, const struct SUCKER_BUTT *af_sSuckerButt);


// #SEE SuckerPlugDButt@c-ansi/sucker.h
int SuckerPlugDButt(SUCKER_HANDLE handle, const struct SUCKER_BUTT *af_dSuckerButt);

// Wraps SuckerPlugDButt() 
// Plug g-string destination butt...
static inline int m_SuckerPlugDButt_GString(SUCKER_HANDLE handle, G_STRING_STUFF f_dGStringStuff) {
  m_ASSIGN_LOCAL_SUCKER_BUTT__G_STRING(dSuckerButt,f_dGStringStuff) 
  return SuckerPlugDButt(handle,&dSuckerButt) ;
} // SuckerPlugDButtGString 



// #SEE SuckerSuckSButt @ c-ansi/sucker.h <RW status>
// Note: "flop" can only occur with "stream" butt ; the possible RW status is:
// RW_STATUS__TRY_AGAIN or RW_STATUS__CONNECTION_LOST
int SuckerSuckSButt(SUCKER_HANDLE handle, struct STRING_PORTION *ac_chunk,
  int *na_flopCauseRwStatus);


// #SEE SuckerFillDButt @ c-ansi/sucker.h <RW status>
// Note: "flop" can only occur with "stream" butt ; the possible RW status is:
// RW_STATUS__TRY_AGAIN, RW_STATUS__TERMINATING or RW_STATUS__CONNECTION_LOST
int SuckerFillDButt(SUCKER_HANDLE handle, struct STRING_PORTION chunk, int *na_flopCauseRwStatus);


// Just a declaration (actual implementation in c-ansi/sucker)
// #SEE SuckerSuckOut @ c-ansi/sucker.h <RW status>
// Note: "flop" can only occur with "stream" butt ; the possible RW status is:
// RW_STATUS__TRY_AGAIN, RW_STATUS__TERMINATING or RW_STATUS__CONNECTION_LOST
int SuckerSuckOut (SUCKER_HANDLE handle, const struct SUCKER_BUTT *ap_sSuckerButt,
  const struct SUCKER_BUTT *ap_dSuckerButt,  int *na_suckedOutLength, int *na_flopCauseRwStatus) ;


// Copy source file into destination file. This function simply wraps SuckerSuckOut() with two
// "FILE" butts...
//
// Passed: 
// - handle: see SuckerCreateInstance()
// - p_sFilePathname: source (regular) file's pathname
// - p_dFilePathname: destination (regular) file's pathname
// - nonPosixOpenFlags: see c-posix/rwf.h
// - na_copiedLength: NULL pointer if not used
// - na_flopCauseRwStatus: NULL pointer if not used
// - na_openErrno: NULL pointer if not used
//
// Modified:
// - *na_copiedLength: (if asked) amount of data copied (in bytes)
// - *na_flopCauseRwStatus: (if asked) : in case of "flop", != RW_STATUS__OK value indicates
//   problem during file reading or writing (see c-posix/rw.h)... 
// - *na_openErrno: reflects underlying open()'errno status in case of "flop" during "file opening"
//
// Return:
// #SEE enum-SUCKER_STATUS @ c-ansi/sucker.h
// - -1 : unexpected problem; anomaly is raised
int SuckerCopy (SUCKER_HANDLE handle, const char *p_sFilePathname, const char *p_dFilePathname,
  int nonPosixOpenFlags, int *na_copiedLength, int *na_flopCauseRwStatus, int *na_openErrno) ;


// Destroy sucker...
//
// Passed: 
// -  xh_handle: the sucker to liquidate
//
// Changed:
// - xh_handle: this handle is no more usable
//
// Ret:
// - RETURNED: OK
// - -1 : unexpected problem; anomaly is raised
int SuckerDestroyInstance (SUCKER_HANDLE xh_handle) ;


#endif // __C_POSIX_SUCKER_H_INCLUDED__
