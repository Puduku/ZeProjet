// c-posix/rw.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: Facilitate design of POSIX programs based on read() and write() system calls...
// =======

#ifndef __C_POSIX_RW_H_INCLUDED__
#define __C_POSIX_RW_H_INCLUDED__

//////////////////////////
#include "c-posix/rw.topo"
//////////////////////////
#include <unistd.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"

#include "c-posix/alarm.h"
#include "c-posix/tryagain.h"


// => Read/write functions possible status: 
// => These status have been defined regarding the GENERIC peer-2-peer communication paradigm 
//    (see c-posix/rw.topo)...
enum { 
  // => The two statuses below say "HOLD THE LINE" : you can keep working with descriptor...
  RW_STATUS__OK, 

  RW_STATUS__TRY_AGAIN, 
  // #REF RW_STATUS__TRY_AGAIN - timeout
  // socket case: Remark:
  // if a timeout has been set at socket level (setsockopt(SO_RCVTIMEO)), then
  // - in LIGHTLY BLOCKING mode, socket's timeout prevails vis-a-vis waiting plan's deadline
  // - in DEEPLY_BLOCLING mode, waiting plan's deadine prevails vis-a-vis socket's timeout

  // => The two statuses below say "CONNECTION IS BROKEN" : you can nothing but
  //    close() descriptor... - SEE ALSO REMARK ==> (*) BELOW:
  RW_STATUS__TERMINATING, 
  // #REF RW_STATUS__TERMINATING
  // "connection is terminating" : the other peer has hung up ; now it's your turn to hang up...
  // #ENDREF
  // Depending on your current "rank", we qualify the termination as either "smooth" or "rough".
  // - If you are the "slave" (see read() wrappers), the termination is "smooth" ; in particular,
  //   when reading a regular file, this case simply corresponds to the Eof (end of file) status...
  // - If you are the "master" (see write() wrappers) or share "equal" rank with the other peer,
  //   the termination is "rough".
  // ==> (*) Of course, with (but only with) a regular file, specific actions (that are out of the
  // "p2p communcation" paradigm) are possible to "revive" the stream even after an "eof", such as
  // repositioning the file offset (rewind(), etc.), or letting some "extra process" completing the
  // file in background...

  RW_STATUS__CONNECTION_LOST, 
  // #REF RW_STATUS__CONNECTION_LOST - "connection has been lost" :
  // - if descriptor is a regular file: problem with the "underlying" file system ("I/O error"...) ; the 
  //   "good" behavior of your program fundamentally depends on the nature of the file system: 
  //   + on external/removable file system (USB stick, CD/ROM, NFS mount...) : that case is banal;
  //     trivial scenario: the fool user removes "without reason" the media from its support... ;
  //     your program ought to react GRACEFULLY in such situation...
  //   + on internal file system (root disk) : you may consider that case as an anomaly - and
  //     abort program execution... (see ANOMALY__CONNECTION_LOST) 
  // - if descriptor is a tcp/ip socket: "connection reset" : problem on the network (or, on occasion,
  //   the other peer has deliberately messed up the connection)
  // - if the descriptor is a pipe or FIFO: this case should not happen in practical situation =>
  //   consider it's not normal... (see ANOMALY__CONNECTION_LOST)
} ;

// In the case a "connection lost" is abnormal, you  can raise that anomaly:"
#define ANOMALY__CONNECTION_LOST "Connection lost ???" 

// read() wrapper
// --------------

// This function wraps read() system call:
// - protects against signal interruption in blocking waiting plan (with timeout)
// - Normalize "error" handling according to "peer-to-peer" connection paradigm described in
//   c-posix/rw.topo
// NOTICE: this function systematically synchronizes O_NONBLOCK flag of the descriptor regarding
// the passed "waiting plan", which is superfluous when you recall the function with the same
// waiting plan. ProtectedRead2() function below is here to allow you avoiding needless
// synchronization operations...
//
// Passed:
// - descriptor: file to "read()"
// - pdHandle: linked with file descriptor to "read()"...
// - ap_waitingPlan: #SEE struct-WAITING_PLAN @ c-posix/tryagain.h <data to read>
// - readBuffer: data buffer (fixed size) for getting the message part
// - readBufferSize: above buffer's size (> 0 (*)) 
//   (*) in order to avoid ambivalent behavior, the function explicitly refuses 0 value 
//
// Modified:
// - readBuffer[]: available data
// - *ac_readLength: (> 0) length of available data; only significant with RW_STATUS__OK status
//
// Returned: 
// - RW_STATUS__OK: got fresh data
//   please note the read "message" may be not complete ; it corresponds simply to the first portion of
//   data successfully read on the stream (that is: at least one byte)
// - RW_STATUS__TRY_AGAIN: currently no data available #SEE RW_STATUS__TRY_AGAIN
// #REF ProtectedRead-RW_STATUS__TERMINATING-CONNECTION_LOST
// - RW_STATUS__TERMINATING: "smooth" (*) termination #SEE RW_STATUS__TERMINATING
//   + with a regular file : it's the good old "EOF"
//   + with a socket (or pipe...) : the other peer has closed the connection... 
//   (*) The termination is "smooth" given peers communicate in "half-duplex". In "full-duplex"
//   exchange mode, during which no peer is supposed to close the session, the status corresponds
//   to a "ROUGH" termination.
// - RW_STATUS__CONNECTION_LOST: #SEE RW_STATUS__CONNECTION_LOST
// - -1 : unexpected problem ; anomaly is raised
int ProtectedRead (int descriptor,
                   PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                   char *readBuffer, int readBufferSize,
                   int *ac_readLength) ;


// Same function but DO NOT synchronize O_NONBLOCK flag of the descriptor...
int ProtectedRead2 (int descriptor,
                    PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                    char *readBuffer, int readBufferSize,
                    int *ac_readLength) ;




// write() wrapper
// ---------------

// SIGPIPE CATCHING:

// Private stuff:
struct BROKEN_PIPE_FIX ;
typedef const struct BROKEN_PIPE_FIX * BROKEN_PIPE_FIX_HANDLE ;


// Basic handling of SIGPIPE:
// In POSIX, SIGPIPE signal is sent to each process making write() onto a "broken pipe". And the
// default action of SIGPIPE is to kill the process...
// So, the default action of SIGPIPE must be disabled :
// - either by installing a "gentle" (i.e no destructive) handler
// - either by ignoring the signal (SIG_IGN)
// In fact, that function simply make SIGPIPEs "ignored" by process / threads...
// NOTE: We simply assert that (simplistic) installation will not clash with other components of
// your program.
// NOTE: singleton; only one "global" (thread-safe) instance is allowed per process...
//
// Passed:
// - azdh_handle: address of handle to initialize
//
// Modified:
// - *azdh_handle: new handle initialized (thread-safe)
//
// Return:
// - RETURNED: SIG_IGN handler "installed" ;
// - -1 : unexpected problem ; anomaly is raised
int BrokenPipeFixCreateInstance (BROKEN_PIPE_FIX_HANDLE *azdh_handle) ;


// Passed:
// - xdh_handle:
//
// Modified:
// - xdh_handle: this handle is no more usable
//
// Return:
// - RETURNED: SIG_IGN handler "removed" ;
// - -1 : unexpected problem ; anomaly is raised
int BrokenPipeFixDestroyInstance (BROKEN_PIPE_FIX_HANDLE xdh_handle) ;



// This function wraps write() POSIX system call:
// - protects against signal interruption in blocking waiting plan (with timeout)
// - Normalize "error" handling according to "peer-to-peer" connection paradigm described in
//   c-posix/rw.topo
// NOTICE: this function systematically synchronizes O_NONBLOCK flag of the descriptor regarding
// the passed "waiting plan", which is superfluous when you recall the function with the same
// waiting plan. ProtectedWrite2() function below is here to allow you avoiding needless
// synchronization operations...
//
// Passed:
// - brokenPipeFixHandle: this handle is necessary to protect your process against SIGPIPE...
// - descriptor:
// - pdHandle: linked with file descriptor to "write()"...
// - ap_waitingPlan: #SEE struct-WAITING_PLAN @ c-posix/tryagain.h <room for data>
// - p_writeBuffer: the message to write
// - writeLength: > 0 (*) ;
//   (*) in order to avoid ambivalent behaviour (or infernal loops hogging the CPU...), the 
//   function explicitly refuses 0 value 
//
// Modified:
// - *ac_writtenLength: only modified with RW_STATUS__OK status: actual number of bytes
//   successfully written so far (> 0)
//
// Returned value: 
// - RW_STATUS__OK: SOME data (i.e at least one byte...) successfully written.
// - RW_STATUS__TRY_AGAIN: #SEE RW_STATUS__TRY_AGAIN
//   "currently not possible / timeout" (pipe is full, file is locked, etc.). NO data written.
// #REF ProtectedWrite-RW_STATUS__TERMINATING-CONNECTION_LOST
// - RW_STATUS__TERMINATING: #SEE RW_STATUS__TERMINATING - "rough termination":
//   More precisely:
//   + writing on regular file: either there is a quota on the file system or the device (USB
//     stick...) containing the file has no room for the data
//   + otherwize (fifo, tcp/ip socket...): "broken pipe" ; the other peer (supposedly the "slave")
//     has closed the connection (or the pipe)
// - RW_STATUS__CONNECTION_LOST: #SEE RW_STATUS__CONNECTION_LOST
// - -1 : unexpected problem ; anomaly is raised
int ProtectedWrite (BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle,
                    int descriptor,
                    PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                    const char *p_writeBuffer, int writeLength,
                    int *ac_writtenLength) ;


// Same function but DO NOT synchronize O_NONBLOCK flag of the descriptor...
int ProtectedWrite2 (BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle,
                     int descriptor,
                     PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                     const char *p_writeBuffer, int writeLength,
                     int *ac_writtenLength) ;



// close() wrapper
// ---------------

enum {
  RW_CLOSE_STATUS__OK,
  RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST,
  RW_CLOSE_STATUS__CANNOT_HANG_UP,
} ;

// This function should help to close() a descriptor more properly... 
// Indeed it's a current shortcoming in programs not to care about close() status...
// (but close() errno values are somewhat difficult to interpret at first glance...) 
// In typical "peer-2-peer" communication paradigm (see c-ansi/rw.topo), close() corresponds to
// elementary "hang up" action.
//
// Passed:
// - hcx_descriptor: the descriptor to close().
//
// Modified:
// - hcx_descriptor: the descriptor is technically closed with RW_CLOSE_STATUS__OK and 
//   RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST status ; in the pathetic 
//   RW_CLOSE_STATUS__CANNOT_HANG_UP situation, the descriptor remains technically "open"...
//
// Returned: 
// - RW_CLOSE_STATUS__OK: "technically" closed.
//   Note: such status is NOT a warranty (*) that "everything went well" at the end of the
//   connection (in particular, the "other peer" may have shut the connection before reading the
//   last message you wrote...).
//   (*) but of course, in ordinary cases of closing of local devices like regular file, 
//       pipe/fifo, etc., the warranty is de facto given by the os. 
// - RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST: descriptor is closed, BUT there has been finally a
//   problem you were not aware of ; i.e that status amends the final status of the connection
//   (after a successful write()).
//   Interpretation: though your last write() seemed "successful", actually it was not ;
//   functionally, that is it's EXACTLY AS THOUGH your last write() had returned a "connection
//   loss"...
//   Practical notice: it seems that such situation might occur when closing files on remote
//   file system (NFS). However, you'd better consider that corner case in all circumstances...
//   REMARK: this case can only occur with a "connected" descriptor (initialized via open(),
//   connect(), accept(), etc.).
//   Hence, this case is totally unexpected for "non-connected" descriptors - like a bare
//   socket()... (see ANOMALY__CONNECTION_LOST)
// - RW_CLOSE_STATUS__CANNOT_HANG_UP: descriptor cannot be closed for the moment (EINTR / EAGAIN)
//   Direct notice: this is really a "pathetic case"... Problem is the "doc" is somewhat
//   contradictory.
//   On one hand, one can understand that, in any case, close() simply "registers" a close "demand",
//   allowing to return directly ; the actual "closure" is pursued "behind the scene" by the system.
//   On the other hand, on certain platforms, the doc suggests that such problem might arise with
//   NFS mount (again!)...
//   => simplest - and recommended - disposition: treat this case as an "unexpected problem" (i.e
//   report an anomaly like ANOMALY__CANNOT_HANG_UP and abort program execution... )
//   => Otherwize, anything you can undertake is to try to close() the descriptor again. But of
//   course, there is absolutely no warranty of success with that strategy... And ACHTUNG not
//   hogging the CPU in an infernal loop! (see b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION() macro)
// - -1 : unexpected problem ; anomaly is raised
int ProtectedClose (int hcx_descriptor) ;


// If you do not expect ProtectedClose() to "block", you can raise that anomaly...
#define ANOMALY__CANNOT_HANG_UP		"Cannot hang up ???"


// This macro wraps ProtectClose() function above ; 
//
// Passed:
// - hcx_descriptor: the descriptor to close().
// - m_connectionLostAction: action in case RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST... 
#define m_PROTECTED_CLOSE(/*int*/ hcx_descriptor, m_connectionLostAction) \
switch (ProtectedClose(hcx_descriptor)) {\
case RW_CLOSE_STATUS__OK:\
break; case RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST:\
  m_connectionLostAction;\
break; case RW_CLOSE_STATUS__CANNOT_HANG_UP:\
  m_RAISE(ANOMALY__CANNOT_HANG_UP)\
break; default :\
  m_TRACK()\
} 

#endif // __C_POSIX_RW_H_INCLUDED__
