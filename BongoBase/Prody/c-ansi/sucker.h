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


// This C-Ansi version is "limited" : only "c-posix/sucker" supports "stream"-based butts...

// Sucker...
// ------

struct SUCKER ; // private...
typedef struct SUCKER *SUCKER_HANDLE ;


// Create a sucker...
//
// Passed:
// - azh_handle: address of sucker handle to initialize
// - nf_alarmSystemHandle : alarm system used for temporizations (see c-posix/alarm.h)
//   + NULL special pointer (not provided) ; limited functionality ; no temporization is possible ;
//     blocking deadlines are de facto indefinite...
//   + non NULL pointer: actual alarm system ; full temporization support
// - f_brokenPipeFixHandle: see BrokenPipeFixCreateInstance() in c-posix/rw.h
// - suckingLimit: limit (in bytes) of data volume that can be sucked (0 for no limit)
//
// Changed:
// - *azh_handle: "sucker" handle initialized.
//
// Ret:
// - RETURNED: OK
// - -1 : unexpected problem; anomaly is raised
int SuckerCreateInstance(SUCKER_HANDLE *azh_handle, int suckingLimit);


// #REF BUTT_SPOTTER_SUCK_FUNCTION <generic status>
// Suck content of the "plugged" source
// 
// Passed:
// - handle: (private) handle
// - b_kickButt: "TRUE" => "prime the pump"... 
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
typedef int (*BUTT_SPOTTER_SUCK_FUNCTION) (void *r_handle, char b_kickButt,
  struct STRING_PORTION *ac_chunk, int *nar_flopCause);


// #REF BUTT_SPOTTER_FILL_FUNCTION <generic status>
// Fill "destination butt" with content
//
// Passed:
// - r_handle: (private) handle
// - chunk: (length >= 0) 
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
typedef int (*BUTT_SPOTTER_FILL_FUNCTION) (void *r_handle, struct STRING_PORTION chunk,
   int *nar_flopCause);


// Plug "source" and "destination" butts 
//
// Passed:
// - handle: sucker 
// - n_sButtSpotterSuckFunction:
// - cfr_sButtSpotterSuckHandle:
// - n_dButtSpotterFillFunction: 
// - cfr_dButtSpotterFillHandle:
//
// Return:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int SuckerPlugSDButts(SUCKER_HANDLE handle, 
  BUTT_SPOTTER_SUCK_FUNCTION n_sButtSpotterSuckFunction, void *cfr_sButtSpotterSuckHandle,
  BUTT_SPOTTER_FILL_FUNCTION n_dButtSpotterFillFunction,  void *cfr_dButtSpotterFillHandle);


// Suck content of the "plugged" source
// 
// Passed:
// - handle: sucker (with plugged "source butt" - see SuckerPlugSDButts() above)
// - b_kickButt: "TRUE" => "prime the pump" 
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
int SuckerSuckSButt(SUCKER_HANDLE handle, char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *nar_flopCause);


// #REF SuckerFillDButt <generic status>
// feed "destination butt" with content
//
// Passed:
// - handle: sucker (with plugged "destination butt" - see SuckerPlugSDButts() above)
// - chunk:  
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


enum { // #REF enum-SUCKER_STATUS
  SUCKER_STATUS__OK, // 100% sucked
  SUCKER_STATUS__LIMIT_BREACHED, // breached allowed limit of suckable data
  SUCKER_STATUS__S_FLOP, // "flop" occurred on source
  SUCKER_STATUS__D_FLOP, // "flop" occurred on destination
} ;

#define FLOP_CAUSE__NONE0 0

// Suck up content of "plugged source butt" into "plugged destination butt".
// See SuckerPlugSDButts() above for "plugging butts"...
//
// Passed:
// - handle: sucker 
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
int SuckerSuckOut (SUCKER_HANDLE handle, int *na_suckedOutLength, int *nar_flopCause);


// Copy source file into destination file. This function simply wraps SuckerSuckOut() with two
// "FILE" butts...
//
// Passed: 
// - handle: see SuckerCreateInstance()
// - p_sFilePathname: source (regular) file's pathname
// - p_dFilePathname: destination (regular) file's pathname
// - na_copiedLength: NULL pointer if not used
// - na_fileFlopCause: NULL pointer if not used
//
// Modified:
// - *na_copiedLength: (if asked) amount of data copied (in bytes)
// - *na_fileFlopCause: (if asked) : in case of "flop", indicates
//   problem during file reading or writing #SEE FILE_FLOP_CAUSE-enum 
//
// Return:
// #SEE enum-SUCKER_STATUS
// - -1 : unexpected problem; anomaly is raised
int SuckerFCopy (SUCKER_HANDLE handle, const char *p_sFilePathname, const char *p_dFilePathname,
  int *na_copiedLength, int *na_fileFlopCause) ;


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


// G_STRING_BUTT_SPOTTER : handle g-string butts
//----------------------

struct G_STRING_BUTT_SPOTTER ; // Private
typedef struct G_STRING_BUTT_SPOTTER *G_STRING_BUTT_SPOTTER_HANDLE; // Public handle


// Create "file" butt spotter instance.
//
// Passed:
// - azh_handle:
// - f_gStringStuff: 
// 
// Changed:
// - *azh_handle: initialized
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int GStringButtSpotterCreateInstance(G_STRING_BUTT_SPOTTER_HANDLE *azh_handle,
  G_STRING_STUFF f_gStringStuff) ;


// BUTT_SPOTTER_SUCKER_FUNCTION
// #SEE BUTT_SPOTTER_SUCK_FUNCTION <g-string>
int GStringButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *na_flopCause) ;

// BUTT_SPOTTER_FILLER_FUNCTION
// #SEE BUTT_SPOTTER_FILL_FUNCTION <g-string>
int GStringButtSpotterFill(void *r_handle, struct STRING_PORTION chunk, int *na_flopCause);


// Destroy g-string butt spotter instance.
//
// Passed:
// - xh_handle: instance to liquidate...
//
// Changed:
// - xh_handle: this handle is no more valid
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int GStringButtSpotterDestroyInstance(G_STRING_BUTT_SPOTTER_HANDLE xh_handle);


// FILE_BUTT_SPOTTER: handle "file" butts 
//------------------

struct FILE_BUTT_SPOTTER ; // Private
typedef struct FILE_BUTT_SPOTTER *FILE_BUTT_SPOTTER_HANDLE; // Public handle

#define BATEAU__FILE_BUTT_SPOTTER__READ_BUFFER_SIZE 4096

// Create "file" butt spotter instance.
//
// Passed:
// - azh_handle:
// - filePathname:
// - n_readBufferSize: 
//   +  -1 special value: 'd' butt
//   +  >0: 's'(sucked) butt; size (in bytes) of read buffer. If you have no idea, you can specify
//      BATEAU__FILE_BUTT_SPOTTER__READ_BUFFER_SIZE...
// 
// Changed:
// - *azh_handle: initialized
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int FileButtSpotterCreateInstance(FILE_BUTT_SPOTTER_HANDLE *azh_handle,
  const char *p_filePathname, int n_readBufferSize) ;


enum { // #REF FILE_FLOP_CAUSE-enum 
  FILE_FLOP_CAUSE__NONE0 = FLOP_CAUSE__NONE0,
  FILE_FLOP_CAUSE__COULD_NOT_OPEN_FILE, 
} ;

// #see BUTT_SPOTTER_SUCK_FUNCTION
// #see FILE_FLOP_CAUSE-enum 
int FileButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *na_flopCause) ;

// #see BUTT_SPOTTER_FILL_FUNCTION
// #see FILE_FLOP_CAUSE-enum 
int FileButtSpotterFill(void *r_handle, struct STRING_PORTION chunk, int *na_flopCause);


// Destroy "file" butt spotter instance.
//
// Passed:
// - xh_handle: instance to liquidate...
//
// Changed:
// - xh_handle: this handle is no more valid
//
// Ret:
// - RETURNED: Ok
// - -1 : unexpected problem; anomaly is raised
int FileButtSpotterDestroyInstance(FILE_BUTT_SPOTTER_HANDLE xh_handle);



#endif // __C_ANSI_SUCKER_H_INCLUDED__
