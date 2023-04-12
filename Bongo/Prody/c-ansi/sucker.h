// c-ansi/sucker.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: "universal" transfer agent
// =======

#ifndef __C_ANSI_SUCKER_H_INCLUDED__
#define __C_ANSI_SUCKER_H_INCLUDED__

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/c-string.h"
#include "c-ansi/g-string.h"


// This C-Ansi version is "conceptual" : most of declarations
// (excepted SuckerSuckOut() "core" function) have no definition (in that "naked" c-ansi module). 
// See c-posix/sucker for "complete" implementation... 
// TODO: "pure" C-Ansi (G-strings-based) implementation 

// Sucker...
// ------

struct SUCKER ; // private...
typedef struct SUCKER *SUCKER_HANDLE ;

struct SUCKER_BUTT ; // defined in c-posix/sucker...


// Just a declaration (see actual implementation in c-posix/sucker)
// Plug a "source" butt ; 
//
// Passed:
// - handle: sucker 
// - af_sSuckerButt: "source" butt 
//
// Return:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int SuckerPlugSButt(SUCKER_HANDLE handle, const struct SUCKER_BUTT *af_sSuckerButt);


// Just a declaration (see actual implementation in c-posix/sucker)
// Plug a "destination butt" 
//
// Passed:
// - handle: sucker 
// - af_dSuckerButt: "destination" butt ; 
//
// Return:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int SuckerPlugDButt(SUCKER_HANDLE handle, const struct SUCKER_BUTT *af_dSuckerButt);


// Just a declaration (see actual implementation in c-posix/sucker)
// #REF SuckerSuckSButt <generic status>
// Suck content of the "plugged" source
// 
// Passed:
// - handle: sucker (with plugged "source butt" - see SuckerPlugSButt() above)
// - nar_flopCause: NULL pointer if not used
// - *nar_flopCause: (if used) default value
//
// Changed:
// - *ac_chunk: sucked data: not significant in case of "flop"
//   + empty portion (length == 0): end of input ; no more can be sucked
//   + non empty portion (length > 0): sucked data 
// - *nar_flopCause: (if asked) <generic status> ; cause of "flop"
//
// Ret: sucked OK ? 
// - ANSWER__YES: OK - "empty" data chunk indicates "end of input" 
// - ANSWER__NO: "flop" during sucking 
// - -1 : unexpected problem; anomaly is raised
int SuckerSuckSButt(SUCKER_HANDLE handle, struct STRING_PORTION *ac_chunk, int *nar_flopCause);


// Just a declaration (see actual implementation in c-posix/sucker)
// #REF SuckerFillDButt <generic status>
// feed "destination butt" with content
//
// Passed:
// - handle: sucker (with plugged "destination butt" - see SuckerPlugDButt() above)
// - ap_chunk:  
// - nar_flopCause: NULL pointer if not used
// - *nar_flopCause: (if used) default value
//
// Changed:
// - *nar_flopCause: cause of "flop"
//
// Ret: padded to output ? 
// - ANSWER__YES: passed chunk is empty or chunk successully padded  
// - ANSWER__NO: "flop" during padding 
// - -1 : unexpected problem; anomaly is raised
int SuckerFillDButt(SUCKER_HANDLE handle, struct STRING_PORTION chunk, int *nar_flopCause);

// Wraps SuckerFillDButt() above
// 
// Passed: 
// - p_chunkGStringStuff: g-string containing the chunk
static inline int m_SuckerFillDButt_GString(SUCKER_HANDLE handle, G_STRING_STUFF p_chunkGStringStuff,
  int *nar_flopCause) {
  m_ASSIGN_LOCAL_STRING_PORTION__G_STRING(chunk, p_chunkGStringStuff) 
  return SuckerFillDButt(handle, chunk, nar_flopCause);
} // SuckerFillDButt_GString



// Just a declaration (see actual implementation in c-posix/sucker)
// #REF SuckerGetSuckingLimit
//
// Return: 
// - >= 0: allowed limit for sucking data (0 for no limit)
// - -1 : unexpected problem; anomaly is raised
int SuckerGetSuckingLimit (SUCKER_HANDLE handle) ;


enum { // #REF enum-SUCKER_STATUS
  SUCKER_STATUS__OK, // 100% sucked
  SUCKER_STATUS__LIMIT_BREACHED, // breached allowed limit of suckable data
  SUCKER_STATUS__S_FLOP, // "flop" occurred on source
  SUCKER_STATUS__D_FLOP, // "flop" occurred on destination
} ;

// The "Core" function of that "C-ansi" module...
// Plug "source" & "destination" butts and suck up content of "source butt" into "destination butt".
//
// Passed:
// - handle: sucker 
// - ap_sSuckerButt: (address of) "source" butt to plug 
// - ap_dSuckerButt: (address of) "destination" butt to plug 
// - na_suckedOutLength: NULL pointer if not used
// - nar_flopCause: NULL pointer if not used
// - *nar_flopCause: (if used) default value
//
// Modified:
// - *na_suckedOutLength: (if asked) amount of data sucked (in bytes)
// - *nar_flopCause: <generic status> ; cause of "flop"
//
// Return:
// #SEE enum-SUCKER_STATUS
// - -1 : unexpected problem; anomaly is raised
int SuckerSuckOut (SUCKER_HANDLE handle, const struct SUCKER_BUTT *ap_sSuckerButt,
  const struct SUCKER_BUTT *ap_dSuckerButt,  int *na_suckedOutLength, int *nar_flopCause) ;


#endif // __C_ANSI_SUCKER_H_INCLUDED__
