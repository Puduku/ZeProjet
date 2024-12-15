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
// - ep_handle: blotcode instance handle created by BlotcodeCreateInstance()
// - nfp_blotlibPrefix:
// - fp_blotexlibExecutorFactoryHandle: see BlotexlibExecutorFactoryCreateInstance()
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

// Allow access to blotex variables... 

struct BLOTEXLIB_EXECUTOR; // Private structure 
typedef struct BLOTEXLIB_EXECUTOR *BLOTEXLIB_EXECUTOR_HANDLE; // Public handle

// This function, which wraps BlotcodeExecutorGetBlotlibExecutorHandle(), retrieves the executor
// handle of =>blotex<= library.
// This function is useful to access to all blotex variables... 
// TODO: indiquer que cette medthode n'est pas "native" de blotcode executor ???
//
// Passed:
// - handle: see BlotcodeExecutorCreateInstance() 
// - blotexLibEntry: entry (label) of blotex library (see BlotcodeLinkBlotexlib() above) 
//
// Modified:
// - *a_blotexlibExecutorHandle: blotex executor library handle 
//
// Returned: 
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorGetBlotexlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int blotexlibEntry,
  BLOTEXLIB_EXECUTOR_HANDLE *a_blotexlibExecutorHandle) ;
												  

#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 

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
  struct P_STRING blotregName, G_STRINGS_HANDLE *ac_blotregHandle) ;

// Create blot register.
// Note: blot registers are based g-params (specialised g-string set collections) 
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
  struct P_STRING blottabName, G_STRINGS_HANDLE *na_blotregHandle);


// Retrieve some blot table.
// 
// Passed:
// - handle:
// - blottabName: blot table's name 
//
// Changed:
// - *ac_tableHandle: (only significant when retrieved : RESULT__FOUND) 
//   blot table's handle
// - *acp_fieldsHandle: (only significant when retrieved : RESULT__FOUND)
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlottab (BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  G_STRINGS_HANDLE *ac_tableHandle, G_STRINGS_HANDLE *acp_fieldsHandle);


enum { // #REF BLOTTAB_INDEX_TYPE
  NONE__BLOTTAB_INDEX_TYPE,
  STR__BLOTTAB_INDEX_TYPE,
  INT__BLOTTAB_INDEX_TYPE,
} ;

// Create blot table.
// Note: blot tables are based on g-string set collections. 
// 
// Passed:
// - handle:
// - blottabName: blot table's name 
// - elementsNumber:
// - s_names:
// - s_blottabIndexTypes: #SEE BLOTTAB_INDEX_TYPE 
// - na_tableHandle: NULL special pointer: not used
// - nap_fieldsHandle: NULL special pointer: not used
//
// Changed:
// - *na_tableHandle: (if used) blot table 's handle
// - *nap_fieldsHandle: (if used)
//
// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the table already exists 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int elementsNumber, struct P_STRING* s_names, int* s_blottabIndexTypes,
  G_STRINGS_HANDLE *na_tableHandle, G_STRINGS_HANDLE *nap_fieldsHandle) ;



#endif // __BLOTEX_C_ANSI_H_INCLUDED__
