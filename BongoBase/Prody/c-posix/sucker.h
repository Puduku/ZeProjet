// c-posix/sucker.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: "universal" transfer agent: stream butt spotter
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


// STREAM_BUTT_SPOTTER : handle "stream"

struct STREAM_BUTT_SPOTTER ; // Private
typedef struct STREAM_BUTT_SPOTTER *STREAM_BUTT_SPOTTER_HANDLE; // Public handle


// Passed:
// - azh_handle:
// - nf_alarmSystemHandle : alarm system used for temporizations (see c-posix/alarm.h)
//   + NULL special pointer (not provided) ; limited functionality ; no temporization is possible ;
//     blocking deadlines are de facto indefinite...
//   + non NULL pointer: actual alarm system ; full temporization support
// - f_brokenPipeFixHandle: see BrokenPipeFixCreateInstance() in c-posix/rw.h
// - waitingPlan: #SEE struct-WAITING_PLAN <butt> ; if the openDescriptor is a regular file,
//   you can take p_fileStreamSimpleWaitingPlan... 
// - n_readBufferSize: 
//   +  -1 special value: 'd' butt
//   +  >0: 's'(sucked) butt; size (in bytes) of read buffer. If you have no idea, you can specify
//      BATEAU__FILE_BUTT_SPOTTER__READ_BUFFER_SIZE...
// - nf_openDescriptor: 
//   + -1 special value: not provided
//   + >=0: connected socket / open regular file,
// 
// Changed:
// - *azh_handle: new instance's "head" handle... 
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int StreamButtSpotterCreateInstance(STREAM_BUTT_SPOTTER_HANDLE *azh_handle,
  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle, BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle,
  struct WAITING_PLAN waitingPlan, int n_readBufferSize, int nf_openDescriptor) ;


// Kick spotter's "stream" butt (that is provide "fresh" stream)
//
// Passed: 
// - handle:
// - f_openDescriptor: connected socket / open regular file,
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int StreamButtSpotterKick(STREAM_BUTT_SPOTTER_HANDLE handle, int f_openDescriptor) ;


enum { // #REF STREAM_FLOP_CAUSE-enum 
  STREAM_FLOP_CAUSE__NONE0 = FLOP_CAUSE__NONE0,
  STREAM_FLOP_CAUSE__NOT_CONNECTED, // or could not open file 
  STREAM_FLOP_CAUSE__TIMEOUT, // corresponds to RW_STATUS__TRY_AGAIN 
  STREAM_FLOP_CAUSE__BROKEN_PIPE, // corresponds to RW_STATUS__TERMINATING 
  STREAM_FLOP_CAUSE__CONNECTION_LOST, // corresponds to RW_STATUS__CONNECTION_LOST 
} ;


// #SEE BUTT_SPOTTER_SUCK_FUNCTION@c-ansi/sucker.h <stream>
// #SEE STREAM_FLOP_CAUSE-enum
int StreamButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
   int *nar_flopCause) ;


// #SEE BUTT_SPOTTER_FILL_FUNCTION@c-ansi/sucker.h <stream>
// #SEE STREAM_FLOP_CAUSE-enum
int StreamButtSpotterFill(void *r_handle, struct STRING_PORTION chunk, int *nar_flopCause);


// Destroy stream butt spotter...
//
// Passed: 
// -  xh_handle: the stream butt spooter to liquidate
//
// Changed:
// - xh_handle: this handle is no more usable
//
// Ret:
// - RETURNED: OK
// - -1 : unexpected problem; anomaly is raised
int StreamButtSpotterDestroyInstance(STREAM_BUTT_SPOTTER_HANDLE xh_handle);


// Sucker: stream-based file copy
// -------

// Copy source file into destination file. This function simply wraps SuckerSuckOut() with two
// "stream" butts (open regular files). That is, this function is a C-Posix implementation of
// C-Ansi's SuckerFCopy()... 
//
// Passed: 
// - handle: see SuckerCreateInstance()
// - n_alarmSystemHandle:
// - brokenPipeFixHandle:
// - p_sFilePathname: source (regular) file's pathname
// - p_dFilePathname: destination (regular) file's pathname
// - na_copiedLength: NULL pointer if not used
// - na_streamFlopCause: NULL pointer if not used
//
// Modified:
// - *na_copiedLength: (if asked) amount of data copied (in bytes)
// - *na_streamFlopCause: (if asked) : in case of "flop", indicates problem during file reading or
//   writing ; #SEE STREAM_FLOP_CAUSE
//
// Return:
// #SEE enum-SUCKER_STATUS @ c-ansi/sucker.h
// - -1 : unexpected problem; anomaly is raised
int SuckerCopy (SUCKER_HANDLE handle,  ALARM_SYSTEM_HANDLE n_alarmSystemHandle,
   BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle,  
   const char *p_sFilePathname, const char *p_dFilePathname,
   int *na_copiedLength, int *na_streamFlopCause) ;



#endif // __C_POSIX_SUCKER_H_INCLUDED__
