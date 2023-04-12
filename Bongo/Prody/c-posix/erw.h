// c-posix/erw.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: Facilitate design of POSIX programs based on read() and write() system calls...
// =======

#ifndef __C_POSIX_ERW_H_INCLUDED__
#define __C_POSIX_ERW_H_INCLUDED__

#include <unistd.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"

#include "c-posix/alarm.h"
#include "c-posix/tryagain.h"
#include "c-posix/rw.h"


// Enhanced Read/Write
// -------------------

// "Enhanced" => support read / write of "complete" messages with a (connected) file descriptor.

// "Enhanced Read/Write" :
//  =        =    =
struct ERW ; // private...
typedef struct ERW *ERW_HANDLE ;


// #REF CHECK_READ_FUNCTION  "Virtual" function definition.
//
// Passed:
// - nr_handle: "private" handle; see ccnfr_checkReadHandle param of ErwCreateInstance() function.
//   + NULL pointer: no "private" handle (generic function...)
//   + non-NULL : handle for "private" data...
// - p_readBuffer: buffer containing the read data to check
// - readLength: number of bytes read so far (> 0)
// - attemptsCount: number of attempts for getting the complete message.
//   1 -> validation of new data
//   2 -> you rejected the previous portion, we have completed it with fresh input data.
//   etc.
//
// Modified parameters :
// - *ac_messageLength: if the buffer contains a complete message, indicates its true length (> 0)
//
// Returned:
// - ANSWER__YES: message is complete ; corresponding parameter *ac_messageLength updated
// - ANSWER__NO: message is not complete
// - -1: unexpected problem; anomaly is raised
typedef int (*CHECK_READ_FUNCTION) (void *nr_handle, const char *p_readBuffer, int readLength,
  int attemptsCount, int *ac_messageLength) ;


// Create and initialize "enhanced read/write" handle.
//
// Passed:
// - azh_handle: address of handle to initialize
// - f_brokenPipeFixHandle: see BrokenPipeFixCreateInstance() in c-posix/rw.h
// - nf_alarmSystemHandle : alarm system used for temporizations (tied with "waiting plan") 
//   + NULL special pointer (not provided) ; limited functionality ; no temporization is possible ;
//     blocking waiting plans are de facto indefinite...
//   + non NULL pointer: actual alarm system (see c-posix/alarm.h)
// - n_readBufferSize: data buffer size (used to read() messages)
//   + -1 special value: not specified ; ErwRead() method cannot be called  
//   + > 0 (*) : size of (initial) buffer to read messages... 
//     (*) to avoid any ambivalence, 0 value is explicitly refused...
// - cnf_traditionalReadBuffer: only significant if read buffer size is provided
//   + NULL special pointer: not used; use the "automated reallocation" mechanism.
//     The read buffer size corresponds to the initial buffer size ; that size may be augmented
//     (by increments of that the initial size) if necessary...
//   + non NULL pointer: the fix-sized buffer you provide to read the messages ; that buffer is  
//     supposed to be large enough for any message to read()...
// - cn_checkReadFunction: only significant if read buffer size is provided
//   + NULL: no custom function ; trivial behaviour: read buffer size represents a message...
//   + non NULL: custom function for delimiting complete messages (while reading)
// - ccnfr_checkReadHandle: only significant if read buffer size AND custom function are provided:
//   Private handle passed to the "complete read message delimiting" function. Checking input 
//   data may require complex parsing of data. This parameter allows not to lose that work once 
//   ErwRead() function gets back the message.
//
// Modified:
// - *azh_handle: "enhanced r/w" handle initialized 
//
// Returned:
// - RETURNED: initialized
// - -1: unexpected problem; anomaly is raised
int ErwCreateInstance (ERW_HANDLE *azh_handle, BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle,
  ALARM_SYSTEM_HANDLE nf_alarmSystemHandle, int n_readBufferSize, char *cnf_traditionalReadBuffer,
  CHECK_READ_FUNCTION cn_checkReadFunction, void *ccnfr_checkReadHandle) ;


// Link file descriptor to "Enhanced R/W handle" and reset R/W.
//
// Passed:
// - handle: 
// - descriptor: "connected" descriptor (must stay open while linked) 
// - nap_defaultWaitingPlan: default "waiting plan" when reading / writing messages.
//   + NULL special pointer: not provided => no DEFAULT waiting plan
//   + non NULL pointer: DEFAULT waiting plan to be applied  
//
// Returned :
// - COMPLETED__OK: handle is "reset" (linked with new descriptor)
// - COMPLETED__BUT: only possible when you "recycle" the instance ; instance is "reset", BUT some 
//   "pending" data have been lost.
//   (Read) This case may occur if you call this function within an "active" connection (i.e before
//   ErwRead() returned with "TERMINATING / HANG UP" or "CONNECTION LOST")
//   (Write) This case might occur if you call this function before complete sending of a message
//   => in brief, should NOT happen UNLESS you "break" the connection 
// - -1: unexpected problem; anomaly is raised
int ErwReset (ERW_HANDLE handle, int descriptor, const struct WAITING_PLAN *nap_defaultWaitingPlan) ;
 

// If you do not expect ErwReset() to clean "pending data", you can raise that anomaly...
#define ANOMALY__TRAILING_DATA "trailing data"


// This function enhances ProtectedRead() (defined in c-posix/rw.h). 
// That is, the function ensures complete reading of messages.
//
// Passed:
// - handle: ERW instance, ENABLED for read() ; see ErwCreateInstance() 
// - nap_waitingPlan:
//   + NULL special pointer: take default waiting plan specified in ErwReset()
//   + non NULL address: #SEE struct-WAITING_PLAN@c-posix/tryagain.h <message to read>
// - na_messageBuffer: "address of message buffer" 
//   + NULL special pointer: when don't need to get the message buffer address (usage of
//     "traditional" buffer - see ErwCreateInstance())
//   + non NULL pointer: when need to get the message buffer address (automated reallocation)
//
// Modified:
// - *na_messageBuffer: starting address of buffer where the read message has been copied
// - *a_messageLength: length of available message; depending on rw status:
//   + RW_STATUS__OK: > 0 (length of complete message)
//   + RW_STATUS__TRY_AGAIN: == 0
//   + RW_STATUS__TERMINATING / RW_STATUS__CONNECTION_LOST: >= 0
//     . == 0: no residue
//     . > 0: length of residual message fragment (received so far)
//
// Returned :
// - RW_STATUS__OK: got fresh, complete, message...
// - RW_STATUS__TRY_AGAIN: Currently not possible / timeout ; message not available, a message
//   fragment may be internally present, but is not exploitable. #SEE RW_STATUS__TRY_AGAIN@c-posix/rw.h
//   Further ErwRead() calls would allow to get the complete message.
// #SEE ProtectedRead-RW_STATUS__TERMINATING-CONNECTION_LOST@c-posix/rw.h
int ErwRead (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan,
  char **na_messageBuffer, int *a_messageLength) ;


// This function enhances ProtectedWrite():
// That is the function helps sending message in "one shot"...
//
// Passed:
// - handle: ERW instance ; see ErwCreateInstance() 
// - nap_waitingPlan:
//   + NULL special pointer: default waiting plan - see ErwReset()
//   + non NULL address: #SEE struct-WAITING_PLAN@c-posix/tryagain.h <room for message to write>
// - p_messageBuffer: the message to write ; you are expected to present the same message after a
//   "TRY AGAIN!" status...
// - messageLength: > 0 value (*) 
//   (*) to avoid any ambivalence, 0 value is explicitly refused... 
// - ...: "variadic" parameters
//   + in simplest case (i.e message presented in one single part)
//     => set NULL address (meaning no extra message data)
//   + Useful in case your message is constructed in two (or more) parts (such as "header" and 
//     "body"); the passed two (or more) parts are treated as a single message.
//   + pass 2 params (buffer and length) for each part 
//   + Don't forget to set NULL to indicate the last part!
//  
// Returned :
// - RW_STATUS__OK: complete message successfully written.
// - RW_STATUS__TRY_AGAIN: "currently not possible / timeout" (pipe is full, file is locked, etc.).
//   message not yet (completely) written. Present your message again in next call!
//   #SEE RW_STATUS__TRY_AGAIN@c-posix/rw.h
// #SEE ProtectedWrite-RW_STATUS__TERMINATING-CONNECTION_LOST@c-posix/rw.h
int ErwWrite2 (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan,
  const char *p_messageBuffer, int messageLength, ...) ;

// This function wraps ErwWrite2() above.
// The function allows to write a message presented in ONE single part.
// (I.e there no variadic parameters ; hence no need add final NULL argument)
int ErwWrite (ERW_HANDLE handle, const struct WAITING_PLAN *nap_waitingPlan,
  const char *p_messageBuffer, int messageLength) ;



// Passed:
// - xh_handle: handle of instance to liquidate...
//
// Modified:
// - xh_handle: this handle is no more usable.
//
// Returned value:
// - RETURNED, instance properly removed
//   NB: some "pending" data may have been lost.
//   (Read) This case may occur if you call this function within an "active" connection (i.e before
//   EnrdRead() returned with "HANG UP" or "CONNECTION LOST")
//   (Write) This case might occur if you call this function before complete sending of a message
// - -1: unexpected problem; anomaly is raised
int ErwDestroyInstance (ERW_HANDLE xh_handle) ;


#endif // __C_POSIX_ERW_H_INCLUDED__
