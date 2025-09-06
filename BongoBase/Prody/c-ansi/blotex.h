// blotex.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

#ifndef __C_ANSI_BLOTEX_H_INCLUDED__
#define __C_ANSI_BLOTEX_H_INCLUDED__

/////////////////////////////
#include "c-ansi/blotex.topo"
/////////////////////////////

#include "c-ansi/blotex-kitchen.h"
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
// - blottabDestroyInstanceFunction:
//
// Changed:
// - *azh_handle: "factory" instance handle
//
// Returned:
// - RETURNED: OK, created and initialized
// - -1: unexpected problem
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction) ;


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
// (Hides "details" for linking blotex lib...) 
// 
// Passed:
// - ep_handle: blotcode instance handle created by BlotcodeCreateInstance()
// - nfp_blotlibPrefix: prefix used for blotex library (NULL if NO prefix) 
// - fp_blotexlibExecutorFactoryHandle: see BlotexlibExecutorFactoryCreateInstance()
//
// Returned: 
// - >= 0: entry (label) of library (BLOTLIB_ENTRY0 for 1st blotlib, etc.)
// - -1: unexpected problem ; anomaly is raised
int l_BlotcodeLinkBlotexlib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE fp_blotexlibExecutorFactoryHandle) ;



// Interface with other blotlibs
// =============================

struct BLOTEXLIB_EXECUTOR; // Private structure 
typedef struct BLOTEXLIB_EXECUTOR *BLOTEXLIB_EXECUTOR_HANDLE; // Public handle

// This function, which wraps BlotcodeExecutorGetBlotlibExecutorHandle(), retrieves the executor
// handle of =>blotex<= library.
// This function is useful to access to all blotex variables... 
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
int l_BlotcodeExecutorGetBlotexlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int blotexlibEntry,
  BLOTEXLIB_EXECUTOR_HANDLE *a_blotexlibExecutorHandle) ;
												  

#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 


// blotregs:
// ---------

// Notice: blot registers are based on g-params 
// blotvar <=> g-param 
// blotreg <=> g-params (i.e g-param collection)

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
  struct P_STRING blotregName, G_STRINGS_HANDLE *na_blotregHandle);


// Parsing blot expressions: helpers
// ---------------------------------


struct BLOTEX_VALUE {
  char b_strex;
  union {
    gen_BLOTVAL c_blotval;
    struct {
      int workingGStringEntry; 
      struct P_STRING v_str;
    } c_strex ;
  } select ;
} ;

#define UNDEFINED_BLOTEX_VALUE { (char) UNDEFINED } 

#define b_FUGACIOUS_STR b_TRUE

// Set blotex INITIAL value
// TODO: manage double initialization ???
//
// Passed:
// - handle:
// - b_strex: (TRUE/FALSE) 
// - c_blotval:
// - cap_str:
// - cb_fugaciousStr: (TRUE/FALSE) NOTICE: if you intialize TWICE a
//   fugacious string, the first working string buffer is "lost"  
// 
// changed:
// - a_blotexValue: 
//
// Ret: 
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorSetBlotexValue(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_strex,
  gen_BLOTVAL c_blotval, const struct P_STRING* cap_str, char cb_fugaciousStr,
  struct BLOTEX_VALUE *a_blotexValue) ;


// Passed:
// - handle:
// - ac_blotexValue: 
// - a_strexValue1: 
// - p_str2:
// 
// changed:
// - a_strtexValue1: 
//
// Ret: 
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorConcatenateStrexValue(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct BLOTEX_VALUE *a_strexValue1, struct P_STRING p_str2) ;


// Parse <blotex>
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING *a_sequence,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) ;



// blottabs:
// ---------

// Notice: blot tables "details" are actually handled by external (so-called "blottab") module.
// The blotex module simply manages the blottab identification within blot expressions (seen as
// some <entity> prefixed with '^' token). 


// ===> To be implemented by blottab module:
struct BLOTTAB ;
typedef struct BLOTTAB* BLOTTAB_HANDLE;

// Add new blottab in blotex executor's blot tables.
// 
// Passed:
// - handle:
// - blottabName:
// - h_blottabHandle: new blotab handle 
//
// Changed:
// - *a_blottabHandle: created blottab's handle 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorAddBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE h_blottabHandle) ;


// Retrieve some blot table of blotex executor...
// 
// Passed:
// - handle:
// - blottabName: blot table's name 
//
// Changed:
// - *ac_blottabHandle: (only significant when retrieved : RESULT__FOUND) 
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE *ac_blottabHandle) ;


struct BLOTTAB_FIELD_REFERENCE {
  int element; // field identification
  BLOTTAB_HANDLE blottabHandle; // Current 
} ;


// ===> To be implemented by blottab module:
// #REF l_BlotexlibExecutorComputeBlottabOps
// Parse and compute blottab operations:
// - as r-value of blotex atom => expect <int blottab ops> | <str blottab ops>
// or
// - as l-value of blotex reference => expect <blottab ref op set int> | <blottab ref op set str> 
//
// Passed:
// - handle: 
// - b_lValue: true=>l-value ; false=>r-value
// - *a_sequence: before parsing
// - blottabName: blot table name
// - cac_blotexValue: only significant with (r-value) 
// - cac_blotvarReference: only significant with (l-value)
// - cac_asValue: only significant with (l-value) 
//
// Changed:
// - *a_sequence: after parsing 
// - *cac_blotexValue: only significant if "success" (r-value) ; value corresponding to blottab ops
// - *cac_blottabFieldReference: only significant if "success" (l-value) 
// - *cac_asValue: only significant if "success" (l-value) either AS__VALUE_INT or AS__VALUE_STR 
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' 'already exist' error; abandon processing 
// - -1: unexpected problem
int l_BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTTAB_FIELD_REFERENCE *cac_blottabFieldReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) ;


// ===> To be implemente by blottab module
// #REF UpdateBlotsetField
// Update some field of current blotset of a blottab.
//
// Passed:
// - blottabFieldReference: 
// - asValue: AS__VALUE_INT / AS__VALUE_STR 
// - blotexValue:
// 
// Ret:
// - RESULT__FOUND:
// - RESULT__NOT_FOUND: current blotset not available
// - -1: unexpected problem; anomaly is raised
int UpdateCurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottabFieldReference, int asValue,
 struct BLOTEX_VALUE blotexValue) ;

#endif // __C_ANSI_BLOTEX_H_INCLUDED__
