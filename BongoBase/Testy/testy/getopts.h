// testy/getopts.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// Aim: TESTY program support (simple DIGGY-based module testing framework)  

#ifndef __TESTY_GETOPTS_H_INCLUDED
#define __TESTY_GETOPTS_H_INCLUDED

#include "diggy/getopts.h"


#define    MULTITHREADED__TESTY_FLAG 0x01
#define          SANDBOX__TESTY_FLAG 0x02
#define          DATADIR__TESTY_FLAG 0x04

// Parse command options for general TESTY program.
// Automatically exit process with NON_EXECUTIVE__EXIT_STATUS when necessary (in such a case,
// the explanation is displayed in standard error before exiting)
// If sandbox is required, automatically set the sandbox as process's working directory. 
// 
// Passed:
// - argc:
// - p_argv:
// - testyFlags: "or-ization" of the following flags:  
//   + MULTITHREADED__TESTY_FLAG:
//   + SANDBOX__TESTY_FLAG: SB argument must be present in program arguments to confirm ,that the
//     working directory is a well a sandbox  
//   + DATADIR__TESTY_FLAG: test data dir must be specified in program arguments (will update
///    p_testyDatadirPath below) 
//
// Ret:
// DG flags are set as requested
// #see dg-flags @ diggy/diggy-kitchen.h
unsigned int ParseTestyCommandArguments (int argc, char **p_argv, unsigned int testyFlags) ; 

#define DEFAULT_TESTY_DATADIR_PATH "./"

// tests' data files "central" directory...
extern const char *p_testyDatadirPath ;


#endif // __TESTY_GETOPTS_H_INCLUDED
