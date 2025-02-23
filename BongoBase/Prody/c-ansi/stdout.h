// c-ansi/stdout.h, version 1.93
// (c) Atos-Euronext Belgium - 2003
// (c) Puduku - 2024

#ifndef __C_ANSI_STDOUT_H_INCLUDED__
#define __C_ANSI_STDOUT_H_INCLUDED__


/////////////////////////////
#include "c-ansi/stdout.topo"
/////////////////////////////

#include "c-ansi/types.h"


struct STD_INFO; // Private...
typedef struct STD_INFO* STD_INFO_HANDLE;


// Ret:
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int StdInfoCreateInstance(STD_INFO_HANDLE *azh_handle);

// Passed:
// - handle:
// - p_info: "smart" '\n' management
// - ...: variadic parameters ("a la" printf()) of format string
//
// Ret:
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int StdInfoPrint(STD_INFO_HANDLE handle, const char *p_anomaly, ...) ;

// Ret:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int StdInfoDestroyInstance(G_STRINGS_HANDLE xh_handle);




#endif // __C_ANSI_STDOUT_H_INCLUDED__
