// c-ansi/inifile.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2025
//

#ifndef __C_ANSI_INIFILE_H_INCLUDED__
#define __C_ANSI_INIFILE_H_INCLUDED__

//////////////////////////////
#include "c-ansi/inifile.topo"
//////////////////////////////


#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/g-param.h"
#include "c-ansi/sucker.h"


struct INICONF ; // Private!
typedef struct INICONF *INICONF_HANDLE ;


// Create (and initialize) iniconf.
//
// Passed:
// *azh_handle: UNINITIALIZED handle
//
// Modified:
// *azh_handle: new iniconf initialized; its handle
//
// Returned:
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int IniconfCreateInstance (INICONF_HANDLE *azh_handle) ;

enum {
  CONCISE__INIFILE_FORMAT, // supposed to corresponds to the .ini file format "consensus" 
  JSON__INIFILE_FORMAT,
} ;

// Attach actual ini file. 
//
// Passed:
// - handle: see IniconfCreateInstance()
// - inifileFormat: TODO: let the module "guess" the format ???
// - f_sourceSuckerHandle: must be plugged with "source butt" (see SuckerPlugSDButts() )
// 
// Modified:
//
// Ret: Has alarm been updated ?
// - -1: unexpected problem ; anomaly is raised
int IniconfAttach (INICONF_HANDLE handle, int iniFileFormat, SUCKER_HANDLE f_sourceSuckerHandle);
 

// Read a "config" within a ini file... 
// 
// Passed:
// - handle: must be attached with actual ini file (see above) 
// - className: class name (may be empty)
//
// Modified:
// - gParamsHandle: g-params correcponding to the "config" (emptied the class is not found).
//
// Ret: 
// - RESULT__FOUND:
// - RESULT__NOT_FOUND:
// - -1: unexpected problem ; anomaly is raised
int IniconfReadConfig (INICONF_HANDLE handle, struct P_STRING className,
  G_STRINGS_HANDLE gParamsHandle) ;


// Liquidate the iniconf and release the memory.
// NOTE: pending alarms are NOT rung...
//
// Passed:
// - xh_handle: the instance to liquidate...
// 
// Modified:
// - xh_handle: this handle is no more usable
//
// Returned:
// - RETURNED:
// - -1: unexpected problem ; anomaly is raised
int IniconfDestroyInstance (INICONF_HANDLE xh_handle) ;

#endif
