// c-ansi/blotcode.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2002
//
// Purpose: Dynamic construction of pages, based on templates.

#ifndef __C_ANSI_BLOTCODE_H_INCLUDED__
#define __C_ANSI_BLOTCODE_H_INCLUDED__

///////////////////////////////
#include "c-ansi/blotcode.topo"
///////////////////////////////
#include "c-ansi/types.h"
#include "c-ansi/stderr.h"
#include "c-ansi/g-string.h"
#include "c-ansi/sucker.h"


// "blotcode" instance 
// ------------------- 


struct BLOTCODE ; // Private!
typedef struct BLOTCODE *BLOTCODE_HANDLE;


// Create a "blocode instance".
//
// Passed:
// - *azh_handle: "un-initialized" handle
//
// Changed:
// - *azh_handle: "blotcode" instance handle
//
// Returned:
// - RETURNED: OK, created and initialized
// - -1: unexpected problem
int BlotcodeCreateInstance(BLOTCODE_HANDLE *azh_handle); 


// #REF BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <blotlib>
//  <blotlib> library executor constructor (virtual function)
// 
// 
// Passed:
// - pr_handle: factory handle (protected instance)
// - azhr_blotlibExecutorHandle:
//
// Modified:
// - *azhr_blotlibExecutorHandle: instance's handle
//
// Returned:
// - RETURNED: done
// - -1: unexpected problem ; anomaly is raised
typedef int (*BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION)(void *pr_handle,
  void **azhr_blotlibExecutorHandle);


struct BLOTFUNC_CALL {
  struct P_STRING referral;
  struct P_STRING arguments; // arguments list passed to the blot function
  // (in the template), excluding initial '(' and  final ')')
} ;

struct BLOTFUNC_ENTRY {
  int blotlibEntry;
  int localBlotfuncNameEntry; 
} ;

struct BLOTFUNC {
  struct BLOTFUNC_ENTRY entry; 
  struct BLOTFUNC_CALL call;
} ;


typedef GENERIC_INTEGER gen_BLOTVAL; 


#define  TRUE__BLOTVAL0 0
#define FALSE__BLOTVAL  1

#define ITERATE_LOOP__BLOTVAL0  TRUE__BLOTVAL0
#define   BREAK_LOOP__BLOTVAL  FALSE__BLOTVAL


// #REF BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotlib>
// C function implementing the <blotlib> function.
//
// Passed:
// - r_handle:
// - ap_blotfunc: identification / localization of the function call in the template
// - c_surrogate: "empty" string
// - *ac_blotval: default value is TRUE__BLOTVAL0 (aka ITERATE_LOOP__BLOTVAL0) 
// - nc_abandonmentInfo: (NULL special value if not used) : "empty" string"
//
// Modified :
// - c_surrogate: only significant with ANSWER__YES ; content
//   corresponding to the emplacement of the blot containing the function call in the template
// - *ac_blotval: only significant with ANSWER__YES ; POSITIVE number.
//   the blot value is used in particular by "loop" and "switch" blot control statement for
//   dynamic construction of the response content.   
//   In "loop" blot control statement:
//   + ITERATE_LOOP__BLOTVAL0 instructs to stay in the loop ; 
//   + any value != ITERATE_LOOP__BLOTVAL0 (like BREAK_LOOP__BLOTVAL1) instructs to leave the
//     loop. 
// - nc_abandonmentInfo: (if used) only significant with ANSWER__NO
//
// Returned: successful execution ? 
// - ANSWER__YES: blot function execution completed
// - ANSWER__NO: order to abandon dynamic page construction
// - -1: unexpected problem ; anomaly is raised
typedef int (*BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION) (void *r_handle,
  const struct BLOTFUNC *ap_blotfunc, G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval,
  G_STRING_STUFF nc_abandonmentInfo) ;



// #REF BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION 
// BLOT library executor destructor (virtual function)
// 
// Passed:
// - xhr_handle: instance to liquidate
//
//- Modified:
// - xhr_handle: this handle is no more valid
//  
//
// Returned:
// - RETURNED: done
// - -1: unexpected problem ; anomaly is raised
typedef int (*BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION) (void *xhr_handle) ;


#define BLOTLIB_ENTRY0 0

// This function adds a new library of blot functions.
// Note: actually, the function just "pre-link" the library. The "final link" will be completed 
// by BlotcodeFreeze() function below... 
//
// Passed:
// - ep_handle: see BlotcodeCreateInstance() (instance is NOT protected) 
// - nfp_blotlibPrefix: 
//   + NULL: special value : anonymous library
//   + non NULL: 
// - localBlotfuncNamesNumber: >= 0
// - cfp_localBlotfuncNames: only significant if localBlotfuncNamesNumber > 0 
// - blotlibExecutorFactoryCreateProductInstanceFunction: blotlib executor creation function
//   (i.e "factory") ; 
// - fpr_blotlibExecutorFactoryHandle: factory's handle (protected instance)
// - blotlibExecutorExecuteCFunction,
// - blotlibExecutorDestroyInstanceFunction,
//
// Returned: 
// - >= 0: entry (label) of library (BLOTLIB_ENTRY0 for 1st blotlib, etc.)
// - -1: unexpected problem ; anomaly is raised
int BlotcodeLinkBlotlib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  int localBlotfuncNamesNumber, const char** cfp_localBlotfuncNames, 
  BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION 
  blotlibExecutorFactoryCreateProductInstanceFunction,  void *fpr_blotlibExecutorFactoryHandle,
  BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION blotlibExecutorExecuteCFunction,
  BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION blotlibExecutorDestroyInstanceFunction) ;


// Finalize linkage of blotlibs and freeze the blotcode instance (i.e make instance "protected"). 
// This action is necessary before creating blotcode executor(s).  
// To be called once you've finished linking blotlibs. 
//
// Passed:
// - ep_handle: see BlotcodeCreateInstance() (instance is NOT protected) 
//
// Returned:
// - RETURNED: OK, ready to create blotcode executor(s)...
// - -1: unexpected problem ; anomaly is raised
int BlotcodeFreeze(BLOTCODE_HANDLE ep_handle) ;


// General release
//
// Passed:
// - xh_handle: 
//
// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int BlotcodeDestroyInstance(BLOTCODE_HANDLE xh_handle) ;


// "blotcode executor" : allows execution of blot code 
// ------------------- 

struct BLOTCODE_EXECUTOR ; // Private!
typedef struct BLOTCODE_EXECUTOR *BLOTCODE_EXECUTOR_HANDLE ;


// Create a "blotcode executor".
// 
// Passed:
// - azh_handle:
// - f_frozenBlotcodeHandle: "frozen" blotcode instance's handle 
// 
// Modified:
// - *azh_handle: initialized 
// 
// Returned value :
// - RETURNED: OK , "execution" instance created and initialized
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorCreateInstance(BLOTCODE_EXECUTOR_HANDLE *azh_handle,
  BLOTCODE_HANDLE f_frozenBlotcodeHandle);


// This function retrieves the executor handle of a given library.
// This function is useful when a library can share some functionalities
// with other libraries (see for instance the "blotex" library)
//
// Passed:
// - handle: see BlotcodeExecutorCreateInstance()
// - blotLibEntry: entry (label) of library (see BlotcodeLinkBlotlib() above) 
//
// Modified:
// - *acr_blotlibExecutorHandle: modified if library found (RESULT__FOUND)  
//
// Returned: found library ?
// - RESULT__FOUND: OK library identified, executor handle updated
// - RESULT__NOT_FOUND: not found  
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorGetBlotlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int blotlibEntry,
  void **acr_blotlibExecutorHandle) ;
												  

// Blot code parsing function.
// This function parses blot code contained in template.
//
// Passed:
// - handle: see BlotcodeExecutorCreateInstance()
// - fp_template:
// - nac_parsingErrorLocalization: pass NULL pointer if not used 
// - nc_parsingErrorInfo: pass NULL pointer if not used 
//
// Modified:
// - *nac_parsingErrorLocalization: (if used) only significant in case of parsing error
// - nc_parsingErrorInfo: (if used) only significant in case of parsing error
//
// Returned: parsing OK ?
// - ANSWER__YES: parsing successful ; ready for generating output page, see
//   BlotcodeExecutorConstructPage()  
// - ANSWER__NO: Blot parsing error:
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorParseTemplate(BLOTCODE_EXECUTOR_HANDLE handle,
  struct P_STRING fp_template, struct P_STRING *nac_parsingErrorLocalization,
  G_STRING_STUFF nc_parsingErrorInfo) ;


enum {
  BLOTCODE_CONSTRUCTION_STATUS__OK,
  BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED,
  BLOTCODE_CONSTRUCTION_STATUS__OUTPUT_FLOP
} ;

// Execute the blot code parsed with BlotcodeExecutorParseTemplate() and construct the dynamic
// page (in sucker's "destination" butt)
// Notice1: BlotcodeExecutorParseTemplate() must have been called succesfully beforehand.
// Notice2: the sucker must be ready to collect output in "destination" butt. 
//
//
// Passed:
// - handle: see BlotcodeExecutorCreateInstance();
// - outputSuckerHandle: output "sucker" ; must be plugged with "destination butt" (see
//   SuckerPlugDButt() )
// - nac_abandonmentBlotfunc: NULL pointer if not used
// - nc_abandonmentInfo: NULL pointer if not used 
//
// Modified:
// - outputSuckerHandle: "destination" butt :
//   + BLOTCODE_CONSTRUCTION_STATUS__OK: contains complete page
//   + BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED: contains page produced so far till blot function
//     instructed to abandon execution...
//   + BLOTCODE_CONSTRUCTION_STATUS__OUTPUT_FLOP: contains page produced so far till "flop"
//     occured (see sucker)...
// - *nac_abandonmentBlotfunc: (if used) ; only significant with 
//   BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED status
// - nc_abandonmentInfo: (if used) only significant with BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED
//   status 
//
// Returned: 
// - BLOTCODE_CONSTRUCTION_STATUS__OK: dynamic page fully constructed 
// - BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED: a blot function instructed to abandon dynamic
//   construction; abandon execution...
// - BLOTCODE_CONSTRUCTION_STATUS__OUTPUT_FLOP: "flop" occured during output:
//   could not open file, descriptor closed by other peer, etc.
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorConstructPage(BLOTCODE_EXECUTOR_HANDLE handle,
  SUCKER_HANDLE outoutSuckerHandle,  struct BLOTFUNC *nac_abandonmentBlotfunc,
  G_STRING_STUFF nc_abandonmentInfo) ;


// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int BlotcodeExecutorDestroyInstance(BLOTCODE_EXECUTOR_HANDLE xh_handle) ;


#endif // __C_ANSI_BLOTCODE_H_INCLUDED__

