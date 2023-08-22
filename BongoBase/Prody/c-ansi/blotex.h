// blotex.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// c) Puduku - 2023

//
// 
// NOTE 2: blotex is a "context free" blot functions library (do not impose specific BLOT_CONTEXT
// structure definition)


#ifndef __C_ANSI_BLOTEX_H_INCLUDED__
#define __C_ANSI_BLOTEX_H_INCLUDED__

//////////////////////
#include "c-ansi/blotex.topo"
//////////////////////

#include "c-ansi/blotcode.h"


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


// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <blotex>
int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle);


// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotex>
int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) ;

// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <blotex>
int BlotexlibExecutorDestroyInstance (void *xhr_handle) ;


// Destroy blotex library executor factory
//
// Passed:
// - xh_handle: 
//
// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) ;


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


// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlotreg (BLOTEXLIB_EXECUTOR_HANDLE handle,
  const char* p_blotregName, *G_STRINGS_HANDLE *ac_blotregHandle) ;


// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the register alread exists 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  const char* p_blotregName, *G_STRINGS_HANDLE *cac_blotregHandle);

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 



#endif // __BLOTEX_C_ANSI_H_INCLUDED__

