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

// Prepare parsing function for potential abandonment.   
//
// Passed:
// - a_sequence: parsed sequence address
// - p_sequenceType: expected sequence type 
#define m_PREPARE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  struct P_STRING *ema_sequence = (a_sequence);\
  const char *emp_sequenceType = (p_sequenceType);\
  
// Precise sequence description of parsing function.
//
// Passed:
// - a_sequence: parsed sequence address 
// - p_sequenceType: expected sequence type 
#define m_PRECISE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  ema_sequence = (a_sequence);\
  emp_sequenceType = (p_sequenceType);\


#define SYNTAX_ERROR__ABANDONMENT_CAUSE "Syntax error"
#define NOT_PARSABLE__ABANDONMENT_CAUSE "Not parsable"
#define VALUE_ERROR__ABANDONMENT_CAUSE "Value error"
#define EXPECT_STREX__ABANDONMENT_CAUSE "Expect strex value"
#define EXPECT_INTEX__ABANDONMENT_CAUSE "Expect strex value"
#define UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_CAUSE "Unknown blotreg"
#define INVALID_FORMAT__ABANDONMENT_CAUSE "Invalid format"
#define NOT_EXISTING__ABANDONMENT_CAUSE "Not existing l-value"

// Make function abandon parsing (return ANSWER__NO)
//
// Passed:
// - p_cause : parsing abandonment cause format 
// - ... : parsing abandonment cause:
//  + mandatory const char* : parsing abandonment cause format
//  + cause format's optional variable arguments
// - ema_sequence : implicit variable 
// - emp_sequenceType: implicit variable
//
// Changed (Implicit) variables:
// - nc_abandonmentInfo:
#define m_ABANDON(...) {\
  if (nc_abandonmentInfo != NULL) {\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,0,"In %s [" FMT_P_STRING "] : ",\
      emp_sequenceType, m_P_STRING_2_FMT_ARGS((*ema_sequence))) < 0)\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,-1, __VA_ARGS__) < 0)\
  } \
  m_DIGGY_RETURN(ANSWER__NO)\
}
 
// IS_CHAR_FUNCTION:
// Recognize any character corresponding to <entity>
int IsEntityNameChar(int c) ;

enum {
  AS__R_VALUE__ENTRY,// '!#' 
  AS__NAME,          // '!$'
  AS__VALUE_INT,     // [ '#' ]
  AS__ID,            // '!'
  AS__VALUE_STR,     // '$'
} ;

// Parse "as" "value" specifier if present
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_as: (>=0) corresponding "as" "value" specifier (-1 if not present) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseAsValue(struct P_STRING *a_sequence, int *an_as) ;


// Parse comparison operator in register/table request, if present... 
//
// Passed:
// - *a_sequence: expect <str comp op> | <comp op> 
// - b_str : TRUE => expect <str comp op> ; FALSE => expect <comp op>
//
// Changed:
// - *a_sequence: after parsing 
// - *an_indexSeekFlags: + -1 special value: comparison operator not present
//   + >= 0: actual seek flags corresponding to comparison operator ; may be NOT 
//     INDEX_SEEK_FLAG__ANY
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseRequestCompOp(struct P_STRING *a_sequence, char b_str, int *an_indexSeekFlags);

// Parse <logical 2op>  
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *a_criteriaFlags: corresponding op. flags (ALL_FLAG_OFF0 special value if not found) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseLogical2Op(struct P_STRING *a_sequence, int *a_criteriaOpFlags);


struct BLOTEX_VALUE {
  char b_strex;
  union {
    gen_BLOTVAL c_blotval;
    struct P_STRING c_str;
  } select ;
} ;

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
// - *cac_asValue: only significant if "success" (l-value) 
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
// - as: 
// - blotexValue:
// 
// Ret:
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem; anomaly is raised
int UpdateCurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottabFieldReference, int as,
 struct BLOTEX_VALUE blotexValue) ;

#endif // __BLOTEX_C_ANSI_H_INCLUDED__
