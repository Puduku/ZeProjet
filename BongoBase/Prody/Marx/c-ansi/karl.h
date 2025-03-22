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
#include "c-ansi/engels.h"


// karl library executor 
// =======================

struct KARLLIB_EXECUTOR_FACTORY ; // Private
typedef struct KARLLIB_EXECUTOR_FACTORY *KARLLIB_EXECUTOR_FACTORY_HANDLE; // Public

 
// Create karl library executor factory
//
// Passed:
// - *azh_handle: "un-initialized" handle
// - blotexlibLabel: label (entry) of linked blotex lib (see l_BlotcodeLinkBlotexlib() in
//   c-ansi/blotex.h)
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
// - >= 0: (label) (entry) of library (BLOTLIB_ENTRY0 for 1st blotlib, etc.)
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
// - karllibLabel: label (entry) of karl library (see l_BlotcodeLinkKarllib() above) 
//
// Modified:
// - *a_karllibExecutorHandle: karl executor library handle 
//
// Returned: 
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int l_BlotcodeExecutorGetKarllibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int karlibLabel,
  KARLLIB_EXECUTOR_HANDLE *a_karllibExecutorHandle) ;
												  

// Retrieve some blot pamphlet.
// 
// Passed:
// - handle:
// - blotpamName: blotpam's name 
//
// Changed:
// - *ac_blotpamHandle: (only significant when retrieved : RESULT__FOUND) 
//   blotpam's handle
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int KarllibExecutorGetBlotpam (KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blotpamName,
  PAMPHLET_EXAMPLAR_HANDLE *ac_blotpamHandle) ;

// Create blot register.
// Note: blot registers are based g-params (specialised g-string set collections) 
// 
// Passed:
// - handle:
// - blotpamName: blotpam's name 
//
// Changed:
// - *a_blotpamHandle: blotpam's handle
//
// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the register already exists 
// - -1: unexpected problem; anomaly is raised
int KarllibExecutorCreateBlotpam(KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blotpamName,
  PAMPHLET_EXAMPLAR_HANDLE *a_blotpamHandle);


#endif // __C_ANSI_KARL_H_INCLUDED__
