// blotex.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

#ifndef __C_ANSI_BLOTEX_H_INCLUDED__
#define __C_ANSI_BLOTEX_H_INCLUDED__

/////////////////////////////
#include "c-ansi/blotex.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/g-string.h"


// blotex library executor 
// =======================

struct BLOTEXLIB_EXECUTOR_FACTORY ; // Private
typedef struct BLOTEXLIB_EXECUTOR_FACTORY *BLOTEXLIB_EXECUTOR_FACTORY_HANDLE; // Public

 
// Create blotex library executor factory
//
// Passed:
// - *azh_handle: "un-initialized" handle
//
// Changed:
// - *azh_handle: "factory" instance handle
//
// Returned:
// - RETURNED: OK, created and initialized
// - -1: unexpected problem
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle);


// Destroy blotex library executor factory
//
// Passed:
// - xh_handle: 
//
// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) ;


// Link blotex library
// TODO: indiquer que cette medthode n'est pas "native" de blotcode ???
// 
// Passed:
// - ep_handle:
// - nfp_blotlibPrefix:
// - fp_blotexlibExecutorFactoryHandle:
//
// Returned: 
// - >= 0: entry (label) of library (BLOTLIB_ENTRY0 for 1st blotlib, etc.)
// - -1: unexpected problem ; anomaly is raised
int BlotcodeLinkBlotexlib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE fp_blotexlibExecutorFactoryHandle) ;

// VARIANTE?:
//int BlotexlibExecutorFactoryLinkBlotexlib(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE fp_handle,
//  BLOTCODE_HANDLE ep_blotcodeHandle, const char* nfp_blotlibPrefix);



// Interface with other blotlibs
// =============================

// Allow exchange of variable values between blotex lib and other blot libraries. 

// Check handle retrieved by BlotcodeExecutorGetBlotlibExecutorHandle() function.
//
// Passed:
// r_handle: see BlotcodeExecutorGetBlotlibExecutorHandle()
//
// Returned :
// - RETURNED: OK
// - -1: unexpected problem 
int BlotexlibExecutorCheckHandle(void *r_handle) ;


#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 

struct BLOTEXLIB_EXECUTOR; // Private structure 
typedef struct BLOTEXLIB_EXECUTOR *BLOTEXLIB_EXECUTOR_HANDLE; // Public handle

// Retrieve some blot register.
// 
// Passed:
// - handle:
// - blotregName: blot register's name 
//
// Changed:
// - *ac_blotregHandle: (only significant when retrieved : RESULT__FOUND) 
//   blot register's handle
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlotreg (BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotregName, G_STRINGS_HANDLE *ac_blotregHandle) ;

// Create blot register.
// 
// Passed:
// - handle:
// - blotregName: blot register's name 
// - na_blotregHandle: NULL special pointer: not used
//
// Changed:
// - *na_blotregHandle: (if used) blot register 's handle
//
// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the register already exists 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotregName, G_STRINGS_HANDLE *na_blotregHandle);


#endif // __BLOTEX_C_ANSI_H_INCLUDED__
