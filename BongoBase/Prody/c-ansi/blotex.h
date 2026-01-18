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
#include "c-ansi/g-param.h"


// blotex library executor factory: 
// ================================

struct BLOTEXLIB_EXECUTOR_FACTORY ; // Private
typedef struct BLOTEXLIB_EXECUTOR_FACTORY *BLOTEXLIB_EXECUTOR_FACTORY_HANDLE; // Public
 

struct BLOTEXLIB_EXECUTOR; // Private structure 
typedef struct BLOTEXLIB_EXECUTOR *BLOTEXLIB_EXECUTOR_HANDLE; // Public handle

// Blottabs:
// ---------

// Actual blottabs are handled by external module(s).
// The blotex module simply manages the blottab identification within blot expressions:
// - "Genuine" blottabs are seen as some <entity> prefixed with '^' token) 
// - "Super" blottabs are seen as some <entity> prefixed with '^^' token) 
// - "Hyper" blottabs are seen as some <entity> prefixed with '^^^' token) 

#define b_SPOT b_TRUE
#define b_OPS !b_SPOT

// #REF l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
// Parse and compute 'l-value' blottab operations or spot:
// expect <blottab ref op set int> | <blottab ref op set str> (OPS) 
// expect <int blottab spot ref> | <str blottab spot ref> (SPOT) 
//
// Passed:
// - handle: 
// - b_spot: b_SPOT(TRUE) parse spot ; b_OPS(FALSE) parse operation 
// - *a_sequence: before parsing
// - blottabName: blot table name
// - nr_blottabHandle: NULL when not existing
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blottabSpotReference: only significant if "success" 
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' 'already exist' error; abandon processing 
// - -1: unexpected problem
typedef int (*l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION)(
  BLOTEXLIB_EXECUTOR_HANDLE handle, char b_spot, struct P_STRING *a_sequence,
  struct P_STRING blottabName, void* nr_blottabHandle,
  struct BLOTTAB_SPOT_REFERENCE *ac_blottabSpotReference, G_STRING_STUFF nc_abandonmentInfo) ;


// #REF l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
// Parse and compute 'r-value' blottab operations:
// expect <int blottab ops> | <str blottab ops> (OPS)
// expect <int blottab spot> | <str blottab spot> (SPOT)
//
// Passed:
// - handle: 
// - b_spot: b_SPOT(TRUE) parse spot ; b_OPS(FALSE) parse operation 
// - *a_sequence: before parsing
// - blottabsLabel:
// - blottabName: blot table name
// - nr_blottabHandle: NULL when not existing
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blotexValue: only significant if "success" ; value corresponding to blottab ops
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' 'already exist' error; abandon processing 
// - -1: unexpected problem
typedef int (*l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION)(
  BLOTEXLIB_EXECUTOR_HANDLE handle, char b_spot, struct P_STRING *a_sequence, int blottabslabel,
  struct P_STRING blottabName, void* nr_blottabHandle, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) ;

#define MAX_BLOTTAB_IMPLEMENTIONS_NUMBER 3
// 1st implementation: "genuine" blottabs
// 2nd implementation: "super" blottabs
// 3rd implementation: "hyper" blottabs

#define GENUINE_BLOTTABS_LABEL0 0

// Create blotex library executor factory; also register "Genuine" blottab implementation
// (GENUINE_BLOTTABS_LABEL0)
//
// Passed:
// - *azh_handle: "un-initialized" handle
// - l_blotexlibExecutorParseAndComputeLValueGenuineBlottabSetOpFunction:
// - l_blotexlibExecutorParseAndComputeRValueGenuineBlottabOpsFunction:
// - updateCurrentGenuineBlotsetFieldFunction:
// - genuineBlottabDestroyInstanceFunction:
//
// Changed:
// - *azh_handle: "factory" instance handle
//
// Returned:
// - RETURNED: OK, created and initialized
// - -1: unexpected problem
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueGenuineBlottabSetOpFunction,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueGenuineBlottabOpsFunction,
  UPDATE_BLOTTAB_SPOT_FUNCTION updateGenuineBlottabSpotFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION genuineBlottabDestroyInstanceFunction) ;

// Register another blottab implementation. 
// => Your are supposed to register ALL blotab implementations before linking blotex library...
//
// Passed:
// - l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction:
// - l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction:
// - updateBlottabSpotFunction:
//
// Returned:
// - >= 1: new EXTRA (not GENUINE)  
// - -1: unexpected problem
int BlotexlibExecutorFactoryRegisterBlottabImplementation(
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE handle,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction,
  UPDATE_BLOTTAB_SPOT_FUNCTION updateBlottabSpotFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction) ;

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


// Destroy blotex library executor factory
//
// Passed:
// - xh_handle: 
//
// Returned:
// - RETURNED: OK, destroyed 
// - -1: unexpected problem ; anomaly is raised
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) ;


// Interface for other blotlib executors:
// ======================================

// This function, which wraps BlotcodeExecutorGetBlotlibExecutorHandle(), retrieves the executor
// handle of =>blotex<= library.
// This function is useful to access to all blotex variables... 
//
// Passed:
// - handle:
// - blotexLibEntry: entry (label) of blotex library (see BlotcodeLinkBlotexlib() above) 
//
// Modified:
// - *a_blotexlibExecutorHandle: blotex executor library handle 
//
// Returned: 
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int l_BlotcodeExecutorGetBlotexlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle,int blotexlibEntry,
  BLOTEXLIB_EXECUTOR_HANDLE *a_blotexlibExecutorHandle) ;

// blotregs:
// ---------

#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING

// Notice: blot registers are based on g-params 
// blotvar <=> g-param 
// blotreg <=> g-params (i.e g-param collection)

// Retrieve some blot register.
// 
// Passed:
// - handle: see l_BlotcodeExecutorGetBlotexlibExecutorHandle()
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
int BlotexlibExecutorGetBlotreg (BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blotregName,
  g_BLOTREG_HANDLE *ac_blotregHandle) ;

// Create blot register.
// 
// Passed:
// - handle: see l_BlotcodeExecutorGetBlotexlibExecutorHandle()
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
  struct P_STRING blotregName, g_BLOTREG_HANDLE *na_blotregHandle);


// Interface for blottab implementations:
// ======================================

// These functions allow implementation of actual blottabs ("Genuine", "Super", "Hyper") 

// Blotex values:
// --------------

// Set blotex INITIAL value
//
// Passed:
// - handle:
// - asValue: AS__VALUE_INT / AS__VALUE_STR 
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
int BlotexlibExecutorSetBlotexValue(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct BLOTEX_VALUE *a_blotexValue, int asValue,
  gen_BLOTVAL c_blotval, const struct P_STRING* cap_str, char cb_fugaciousStr) ;


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

// Parsing blot expressions:
// -------------------------

// TODO: ParseBlotex() dans kitchen???
// Parse <blotex>
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "parsed/computed successfully" 
// - nc_abandonmentInfo: only significant if "parsing/computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
int BlotexlibExecutorParseAndComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) ;

// TODO: ParseStrPortion dans kitchen???
// Parse <str portion>
//
// Passed:
// - handle:
// - *a_sequence: before parsing; expect '{' [ <intex> ] [ ( ':' | '-' ) [ <intex> ] ] '}'
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_offset: >= 0; only significant if parsed successfully
// - *acn_length: (-1 special value for all rest of string) only significant if parsed successfully
// - nc_abandonmentInfo: only significant if "parsing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
int BlotexlibExecutorParseAndComputeStrPortion(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *ac_offset, int *acn_length, G_STRING_STUFF nc_abandonmentInfo);


// blottabs:
// ---------

// Add new blottab in blotex executor's blot tables.
// 
// Passed:
// - handle:
// - blottabsLabel: GENUINE_BLOTTABS_LABEL0; 1 => "Super" blottabs' label, etc.
// - blottabName: must correspond to NON-existing blottab 
// - hr_blottabHandle: new blottab's handle to be added
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorAddBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabsLabel,
  struct P_STRING blottabName, void* hr_blottabHandle) ;


// Retrieve some blot table of blotex executor...
// 
// Passed:
// - handle:
// - blottabsLabel: GENUINE_BLOTTABS_LABEL0; 1 => "Super" blottabs' label, etc.
// - blottabName: blot table's name 
//
// Changed:
// - *acr_blottabHandle: only significant when retrieved (RESULT__FOUND) 
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabsLabel,
  struct P_STRING blottabName, void* *acr_blottabHandle) ;


#endif // __C_ANSI_BLOTEX_H_INCLUDED__

