// karl.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#ifndef __C_ANSI_KARL_H_INCLUDED__
#define __C_ANSI_KARL_H_INCLUDED__

/////////////////////////////
#include "c-ansi/karl.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/g-string.h"


// karl library executor 
// =======================

struct KARLLIB_EXECUTOR_FACTORY ; // Private
typedef struct KARLLIB_EXECUTOR_FACTORY *KARLLIB_EXECUTOR_FACTORY_HANDLE; // Public

 
// Create karl library executor factory
//
// Passed:
// - *azh_handle: "un-initialized" handle
// - blotexlibLabel: label of linked blotex lib (see m_BlotcodeLinkBlotexlib() in c-ansi/blotex.h)
//
// Changed:
// - *azh_handle: "factory" instance handle
//
// Returned:
// - RETURNED: OK, created and initialized
// - -1: unexpected problem
int KarllibExecutorFactoryCreateInstance(KARLLIB_EXECUTOR_FACTORY_HANDLE *azh_handle, int blotexlibLabel);


// Destroy karl library executor factory
//
// Passed:
// - xh_handle: 
//
// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int KarllibExecutorFactoryDestroyInstance(KARLLIB_EXECUTOR_FACTORY_HANDLE xh_handle) ;


// Link karl (blotpam) library to blotcode instance.
// 
// Passed:
// - ep_handle: blotcode instance handle created by BlotcodeCreateInstance()
// - nfp_blotlibPrefix: prefix used for karl library (NULL if NO prefix)
// - fp_karllibExecutorFactoryHandle: see KarllibExecutorFactoryCreateInstance()
//
// Returned: 
// - >= 0: entry (label) of library (BLOTLIB_ENTRY0 for 1st blotlib, etc.)
// - -1: unexpected problem ; anomaly is raised
int l_BlotcodeLinkKarllib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  KARLLIB_EXECUTOR_FACTORY_HANDLE fp_karllibExecutorFactoryHandle) ;



// Interface with other blotlibs
// =============================

// Allow access to karl variables... 

struct KARLLIB_EXECUTOR; // Private structure 
typedef struct KARLLIB_EXECUTOR *KARLLIB_EXECUTOR_HANDLE; // Public handle

// This function, which wraps BlotcodeExecutorGetBlotlibExecutorHandle(), retrieves the executor
// handle of =>karl<= library.
// This function is useful to access to all pamphlets... 
//
// Passed:
// - handle: see BlotcodeExecutorCreateInstance() 
// - karlLibEntry: entry (label) of karl library (see BlotcodeLinkKarllib() above) 
//
// Modified:
// - *a_karllibExecutorHandle: karl executor library handle 
//
// Returned: 
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int l_BlotcodeExecutorGetKarllibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int karllibEntry,
  KARLLIB_EXECUTOR_HANDLE *a_karllibExecutorHandle) ;
												  

#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 

// Retrieve some blot pamphlet.
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
int KarllibExecutorGetBlotpam (KARLLIB_EXECUTOR_HANDLE handle,
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
int KarllibExecutorCreateBlotpam(KARLLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottabName, G_STRINGS_HANDLE *na_pointsHandle);


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
int KarllibExecutorGetBlottab (KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
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
// - fieldsNumber:
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
int KarllibExecutorCreateBlottab(KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexTypes,
  G_STRINGS_HANDLE *na_tableHandle, G_STRINGS_HANDLE *nap_fieldsHandle) ;



#endif // __KARL_C_ANSI_H_INCLUDED__
