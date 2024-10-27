// c-ansi/p-string.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_P_STRING_H_INCLUDED
#define __C_ANSI_P_STRING_H_INCLUDED

#include <string.h>
#include <ctype.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"

// SEE ALSO c-flaws.topo, Number -1 
// The very first aim of the module is to propose to stick once and for all to that CONVENTION.
// By #including that "module", you tacitly admit the following CONVENTION:
// #REF Buffer-size-NEVER-zero
// => CONVENTION <= : the size of a string buffer CANNOT be equal to 0.
//    ==========

// #REF GOOD_OLD_EMPTY_C_STRING 
// good old empty C string
#define GOOD_OLD_EMPTY_C_STRING ""
#define EMPTY_STRING_LENGTH0 0


// String portion ("compound" data struct) : 
struct P_STRING { // #REF struct-P_STRING
  const char *string; // "util" (raw) char array (start pointer)
  const char *stop; // explicit stop pointer delimitor - normally cannot point before string start
    // pointer ; otherwise, such (so-called "crazy") string portions are hold as EMPTY strings... 
  // NOTICE: zeroified string portion struct (in which string == stop == NULL) is yet a valid form 
  // of EMPTY string (just called "trivial" empty string portion...)  
} ;

// #REF Stop-means-stop
// stop pointer indicates 1st unsafe position ; depointing such address means possibility of SEGV. 
// That restriction is particulary important with empty string (stop === start)...
// Of course, when the string portion is '\0'-terminated (C-string), we can "safely" depoint stop 
// position (which must correspond to good old '\0' char...)  

// #REF TRIVIAL_EMPTY_P_STRING
// Remark: string == stop == NULL (called "trivial" empty string) is thus a VALID representation of
// empty string portions...



// Assigning string portions
// -------------------------

// Assign a string portion.
// 
// Passed:
// - mu_pString: (once evaluated parameter)
// - up_string: (once evaluated parameter)
// - un_length: (once evaluated parameter)
//   + >= 0: raw string length (may include '\0' chars)
//   + -1 special value: '\0'-terminated C string
// 
// Assigned:
// - mu_pString: 
#define m_ASSIGN_P_STRING(/*struct P_STRING*/mu_pString,\
  /*const char* */up_string, /*int*/un_length) {\
  struct P_STRING *em_pStringPtr = &(mu_pString);\
  int em_length = (un_length);\
  const char *emp_string = (up_string);\
  if (em_length < 0) em_length = strlen(emp_string);\
  em_pStringPtr->stop = (em_pStringPtr->string = emp_string) + (em_length);\
}

// Assign a local string portion.
// 
// Passed:
// - m_pString: local var to create
// (other params : see m_ASSIGN_P_STRING() above) 
//
// Created (local var):
// - m_pString: 
#define m_ASSIGN_LOCAL_P_STRING(/*struct P_STRING*/m_localPString,\
  /*const char* */p_string, /*int*/un_length) \
  struct P_STRING m_localPString;\
  m_ASSIGN_P_STRING(m_localPString, p_string, un_length)

// Assign "empty string" to "string portion" 
// TODO: use "trivial" empty string instead 
// 
// Passed:
// - m_pString: 
//
// Assigned:
// - m_pString: 
#define m_ASSIGN_EMPTY_P_STRING(/*struct P_STRING*/m_pString) \
  m_ASSIGN_P_STRING(m_pString, GOOD_OLD_EMPTY_C_STRING,\
    EMPTY_STRING_LENGTH0)

// Assign "empty string" to LOCAL "string portion" 
// See m_ASSIGN_EMPTY_P_STRING() above.
#define m_ASSIGN_LOCAL_EMPTY_P_STRING(/*struct P_STRING*/m_localPString) \
  struct P_STRING m_localPString;\
  m_ASSIGN_EMPTY_P_STRING(m_localPString)

// Assign simple "C-string" to "string portion" 
// 
// Passed:
// - mu_pString: 
// - p_cString: c string (buffer start address)
//
// Assigned:
// - m_pString:
#define m_ASSIGN_C_P_STRING(/*struct P_STRING*/mu_pString,\
  /*const char* */p_cString) {\
  m_ASSIGN_P_STRING(mu_pString,p_cString, -1)\
}

// Assign simple "C-string" to local "string portion" 
// 
// Passed:
// - m_localPString: local var to create
// (other params : see m_ASSIGN_P_STRING() above) 
//
// Assigned (local var):
// - m_localPString:
#define m_ASSIGN_LOCAL_C_P_STRING(/*struct P_STRING*/m_localPString,\
  /*const char* */p_cString) \
  struct P_STRING m_localPString;\
  m_ASSIGN_C_P_STRING(m_localPString,  p_cString)


// Assign simple "C-strings" to "string portions".
//
// Passed: 
// - s_pStrings: array of p-strings to assign 
// - stringsCount: (>0) number of strings
// - p_cString0: 1st c-string
// - ...: other c-string(s) 
//
// Assigned :
// - s_pStrings:
static inline int m_AssignPStrings(struct P_STRING* s_pStrings, int stringsCount,
  /*const char* p_cString0 ,*/ ...) {
  va_list cStrings;
  struct P_STRING *pStringsPtr = s_pStrings;
  va_start(cStrings,stringsCount);
  int i = 0; while(i++ < stringsCount) m_ASSIGN_P_STRING(*(pStringsPtr++),va_arg(cStrings,
    const char*),-1);
  va_end(cStrings);
  return RETURNED;
} // m_AssignPStrings


// Assign simple "C-strings" to LOCAL "string portions".
//
// Passed: 
// - m_localPStrings: 
// - stringsCount: (>0) number of strings; 
// - m_cString0: 1st c-string
// - ...: other c-string(s)
//
// Assigned (local var):
// - m_pString:
#define m_ASSIGN_LOCAL_P_STRINGS(/*struct P_STRING* */m_localPStrings,/*int*/ stringsCount,...)\
  struct P_STRING m_localPStrings[stringsCount]; m_ASSERT(m_AssignPStrings(m_localPStrings,\
  stringsCount,__VA_ARGS__) == RETURNED)



// String portions properties
// -------------------------- 

// Get actual length of a string portion  
// Note: pathetic case in which stop ptr preceeds start ptr is considered as empty string...
//
// Passed:
// TODO: why pass pointer ???
// - ap_pString: 
// 
// Ret: string portion length (ALWAYS >= 0)
static inline int m_PStringLength(const struct P_STRING *ap_pString) { 
  int length = ap_pString->stop - ap_pString->string;
  if (length < 0) length = 0;
  return length;
} // m_PStringLength

// Is a string portion empty ?
//
// Passed:
// - p_pString: 
// 
// Ret:
// - TRUE: the pString is empty (length == 0)
// - FALSE: the pString is not empty (length > 0)
#define b_EMPTY_P_STRING(/*const struct P_STRING*/p_pString) \
  (m_PStringLength(&(p_pString)) == 0)


// Extract sub string portion
//
// Passed:
// - pString: 
// - offset: >= 0 ; rectified if exceeds end of portion 
// - n_length: sub string length:
//  + -1 special value: end of string
//  + >=0: actual length (retified if exceeds end of portion) 
// 
// Ret: sub string portion 
static inline struct P_STRING m_SubPString(struct P_STRING pString,int offset,int n_length) {
  pString.string += offset;
  if (pString.string > pString.stop) pString.string = pString.stop; 
  if (n_length > 0 && n_length < (pString.stop - pString.string)) pString.stop = pString.string +
    n_length;

  return pString;
} // m_SubPString


// Provide buffer / length function parameters corresponding to string portion.
// 
// Passed:
// - p_pString: string portion 
#define m_P_STRING_2_BUFFER_LENGTH(/*const struct P_STRING*/p_pString) \
 (p_pString).string , m_PStringLength(&(p_pString))


// Verify if string portion corresponds well to C-string (i.e exempted of '\0' chars)
//
// Passed:
// - a_pString : address of string portion data struct 
// 
// Changed: 
// - a_pString : rectified with COMPLETED__BUT status... 
// - <a_pString>->stop : delimit actual string length 
//
// Returned: 
// - COMPLETED__OK:
// - COMPLETED__BUT: rectified
int VerifyCPString(struct P_STRING *a_pString);


// Using string portions with printf() family of functions...
 
// format directive for string portions:
#define FMT_P_STRING "%.*s" 

// String portions wrapper usable with format directive above.
//
// Passed:
// - p_pString: string portion passed in printf() subsequent argument(s) 
#define m_P_STRING_2_FMT_ARGS(/*const struct P_STRING*/p_pString) \
 m_PStringLength(&(p_pString)) , (p_pString).string


// Copy string portions 
// --------------------

// Establish the optimal size for destination string buffer in string copy operation. 
// 
// Passed:
// - srcPStringLength : 'string' source string length 
// 
// Ret: optimal buffer size (> 0)
#define /*int*/OPTIMAL_BUFFER_SIZE_4_P_STRING_COPY(/*int*/srcPStringLength) \
  ((srcPStringLength) + 1) 


// Same purpose as standard "strcpy" function, except that this function takes care never to
// overflow the target string.
// ATTENTION: source and destination MAY NOT overlap
// TODO: MovePString()
//
// Passed:
// - dstRawString : target string (buffer address)
// - dstBufferSize : target string's buffer size; > 0 (CANNOT be == 0) ; see 
//   OPTIMAL_BUFFER_SIZE_4_P_STRING_COPY() macro above.
// - ap_srcPString : source string portion 
//
// Modified:
// - dstString : copy of 'source string' (limited to dst buffer size); ==> ALWAYS '\0' terminated.
// - *na_dstPString : (if explicit target used) target string portion 
//
// Returned:
// - >= 0 and < dstBufferSize: copied length (not taking into account the extra '\0'
//   ending char) (the copy is COMPLETE if and only 'destination' buffer is big enough) 
// - -1: unexpected problem; anomaly is raised
// TODO: supprimer ap_ ???
int CopyPString(char *dstRawString, int dstBufferSize, 
  const struct P_STRING *ap_srcPString);


// Compare string portions
// -----------------------

// To match "int isdigit(int c); etc." family functions...
// Evaluate character property
//
// Passed: 
// - c: character to evaluate
// 
// Ret:
// - 0: "FALSE" ; the character does not have the property 
// - != 0 : "TRUE" ; the character has the property 
typedef int (*IS_CHAR_FUNCTION) (int c) ;

// to match "int toupper(int c); etc." family functions
// Convert a character
//
// Passed: 
// - c: character to evaluate
// 
// Ret: converted character
typedef int (*TO_CHAR_FUNCTION) (int c) ;


// #REF ComparePStrings
// Lexical comparison of raw strings:
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - otherwize, "char to char" comparison of raw strings.
//
// Passed:
// - ap_pString1 : 1st string to compare #SEE struct-P_STRING
// - ap_pString2 : 2nd string to compare #SEE struct-P_STRING
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before comparison 
//   + != NULL: elimination of each "neutral" char of both strings before comparison ; "binary"
//     comparison is NOT possible
// - n_toCharFunction:
//   + NULL: NO char conversion applied before comparison
//   + != NULL: conversion applied on each char of both strings before comparison ; "binary"
//     comparison is NOT possible
// 
// Returned:
// - >=0: "comparison" between string 1 and string 2: 
//   + LESS_THAN__COMPARISON : string 1 'less than' string 2
//   + EQUAL_TO__COMPARISON : strings are "identical"
//   + GREATER_THAN__COMPARISON : string 1 'greater than' string 2
int ComparePStrings(const struct P_STRING *ap_pString1, 
  const struct P_STRING *ap_pString2,  IS_CHAR_FUNCTION n_isNeutralCharFunction,
  TO_CHAR_FUNCTION n_toCharFunction) ; 

// ComparePStrings() wrapper ; compare a string portion with a c-string...
static inline int m_CompareWithCString(const struct P_STRING *ap_pString1, const char *ap_cString2) {
  m_ASSIGN_LOCAL_C_P_STRING(pString2,ap_cString2)
  return ComparePStrings(ap_pString1, &pString2,  NULL, NULL) ;
} 

// m_CompareWithCString() wrapper ; indicates whether string portion matches with a c-string...
// 
// Ret: boolean value (TRUE when strings matches)
static inline int mb_EqualToCString(const struct P_STRING *ap_pString1,
  const char *ap_cString2) {
  int comparison = m_CompareWithCString(ap_pString1,ap_cString2);
  switch (comparison) {
  case LESS_THAN__COMPARISON:
  case GREATER_THAN__COMPARISON:
  case EQUAL_TO__COMPARISON:
  break; default: 
    m_RAISE_FATAL(ANOMALY__VALUE__D,comparison) 
  } // switch
  return comparison == EQUAL_TO__COMPARISON; 
}  


// Paranoid wrapper of ComparePStrings() (testing/critical embedded purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
// 
// Ret:
// - >0 : LESS_THAN__COMPARISON / EQUAL_TO__COMPARISON / GREATER_THAN__COMPARISON 
// - -1: unexpected problem, anomaly is raised
int ParanoidComparePStrings(const struct P_STRING *ap_pString1,
  const struct P_STRING *ap_pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION n_toCharFunction) ; 


// Scanning string portions
// ------------------------ 

#define b_REGULAR_SCAN b_TRUE
#define b_REVERTED_SCAN b_FALSE0

#define b_PASS_CHARS_TILL b_TRUE
#define b_PASS_CHARS_WHILE b_FALSE0

// Simple string (char to char) "scanning" function 
// Locate first char having / NOT having a property
// NOTE: not be confused with SScanfPString() declared below... 
//
// Passed:
// - ap_pString : string portion to scan. 
// - b_regularScan 
//   + b_REGULAR_SCAN (TRUE) :
//   + b_REVERTED_SCAN (FALSE) :
// - b_passCharsTill:  
//   + b_PASS_CHARS_TILL (TRUE) : seek 1st character HAVING the property
//   + b_PASS_CHARS_WHILE (FALSE) : seek 1st character NOT HAVING the property
// - n_isCharFunction: property evaluation function (see IS_CHAR_FUNCTION) (NULL if not used) 
// - c_char: only significant if n_isCharFunction == NULL ; specific char to seek 
//
// Returned:
// (!= NULL) scanning position ; use b_SCAN_P_STRING_LOCATED() below to check whether char
// is actually located. 
const char *ScanPString(const struct P_STRING *ap_pString,
  char b_regularScan, char b_passCharsTill, IS_CHAR_FUNCTION n_isCharFunction, int c_char);


// String "scanning" function 
// Locate first matching sub-string based on 
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - "char to char" comparison otherwise.
//
// Passed:
// - ap_pString: string to scan #SEE struct-P_STRING
// - ap_subPString: sub string to locate
// - n_toCharFunction: 
//   + NULL: not provided; no conversion before  comparison 
//   + != NULL: conversion applied on (each char of) both strings before comparison ; "binary"
//     comparison is NOT possible
//
// Returned:
// (!= NULL) scanning position; use b_SCAN_P_STRING_LOCATED() below to check whether
// sub-string is actually located. 
const char *ScanPStringTillMatch(const struct P_STRING *ap_pString,
  const struct P_STRING *ap_subPString,  TO_CHAR_FUNCTION n_toCharFunction);


// Paranoid wrapper of ScanPStringTillMatch() (testing/critical embedded purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
// 
// Ret:
// - != NULL: scanning position... 
// - NULL: unexpected problem, anomaly is raised
const char *ParanoidScanPStringTillMatch(const struct P_STRING *ap_pString, 
  const struct P_STRING *ap_subPString,  TO_CHAR_FUNCTION n_toCharFunction) ;

// Extend ScanPStringTillMatch "scanning" function : 
// Locate first matching sub-string, among DIFFERENT sub-strings. 
//
// Passed:
// - ap_pString: string to scan #SEE struct-P_STRING
// - n_toCharFunction: 
//   + NULL: not provided; no conversion before  comparison 
//   + != NULL: conversion applied on (each char of) both strings before comparison ; "binary"
//     comparison is NOT possible
// - subStringsCount: >0
// - subPString0: 1st possible sub string to locate
// - other possible sub strings (...) : indicate all other possible sub-strings to match ;
//
// Changed:
// - *a_matchedEntry : entry for sub-string which was FIRST matched (may be any entry if none of 
//   sub-strings is located) 
//   
//
// Returned:
// (!= NULL) scanning position; use b_SCAN_P_STRING_LOCATED() below to check whether
// sub-string is actually located. 
const char *ScanPStringTillFirstMatch(const struct P_STRING *ap_pString,
  TO_CHAR_FUNCTION n_toCharFunction, int *a_matchedEntry, int subStringsCount,
  /*struct P_STRING subPString0,*/ ...);

// See ScanPStringTillFirstMatch()
// 
// Passed:
// - subPStrings:
// (Instead of:)
// - subPString0: 1st possible sub string to locate
// - other possible sub strings (...) : indicate all other possible sub-strings to match ;
const char *ScanPStringTillFirstMatchV(const struct P_STRING *ap_pString,
  TO_CHAR_FUNCTION n_toCharFunction, int *a_matchedEntry, int subStringsCount,
  va_list subPStrings);

// See ScanPStringTillFirstMatchV()
// 
// Passed:
// - sp_subPStrings:
// (Instead of:)
// - subPStrings:
const char *ScanPStringTillFirstMatchR(const struct P_STRING *ap_pString,
  TO_CHAR_FUNCTION n_toCharFunction, int *a_matchedEntry, int subStringsCount, 
  const struct P_STRING sp_subPStrings[]) ;

// Boolean interpretation (located or not) of scan pointer returned by ScanPString*()
// functions.
//
// Passed:
// - p_pString: as passed to (and updated by) ScanPString*()
// - scanPtr : offset returned by ScanPString*()
//
// Ret:
// - TRUE: located at position indicated by scan pointer 
// - FALSE: not located, 
#define b_SCAN_P_STRING_LOCATED(/*const struct P_STRING*/p_pString,\
  /*const char* */u_scanPtr)  ( (u_scanPtr) < (p_pString).stop ) 


// Converting string portions
// --------------------------

#define b_C_TERMINATED b_TRUE
#define b_NOT_C_TERMINATED b_FALSE0

// Simple string chars conversion function. 
// Possibly, "neutral" chars (white space, etc.) can be eliminated...
//
// Passed:
// - aep_pString: string to convert #SEE struct-P_STRING
// - b_cTerminated: 
//   + b_C_TERMINATED : explictely '\0'-terminated
//   + b_NOT_C_TERMINATED : pure string portion (not '\0'-terminated)
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before conversion
//   + != NULL: elimination of each "neutral" char in lieu of conversion
// - toCharFunction: 
//
// Modified:
// - aep_pString: converted string portion ; 
//   string length may have been reduced after elimination of neutral chars.  
//   '\0' ending char is added if the string is explicitly NULL-terminated.
//
// Returned:
// - >=0: string length after conversion 
// - -1: unexpected problem; anomaly is raised
int ConvertPString(struct P_STRING *aep_pString, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) ;

// Paranoid wrapper of ConvertPString() (testing/critical embedded stuff purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
int ParanoidConvertPString(struct P_STRING *aep_pString, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) ;


// Using string portions with standard string manipulation functions
// ----------------------------------------------------------------- 

// Max size of buffer in STACK when a local copy of a string portion parameter is required.
#define LOCAL_STRING_AUTO_BUFFER_SIZE 256

// Replace temporarilly string portion with an equivalent '\0'-terminated C string.
// 
// Passed:
// - m_pString : #SEE struct-P_STRING
//   must be a single variable name.
//
// Changed:
// - m_pString: "hijacked" ; 
//   + string: points to adequate buffer (ALWAYS '\0'-terminated) :
//     . points to auto (stack) required buffer size <= LOCAL_STRING_AUTO_BUFFER_SIZE 
//     . otherwise, points to buffer created in heap
//   + stop: updated accordingly if necessary 
#define m_HIJACK_P_STRING(/*struct P_STRING*/m_pString) \
  char emc_localString_ ## m_pString [LOCAL_STRING_AUTO_BUFFER_SIZE] ;\
  char *emnh_heapString_ ## m_pString = NULL;\
  int em_requiredBufferSize = OPTIMAL_BUFFER_SIZE_4_P_STRING_COPY(m_PStringLength(\
    &(m_pString))); \
  char* emep_string = (char*)UNDEFINED; \
  if (em_requiredBufferSize > LOCAL_STRING_AUTO_BUFFER_SIZE) {\
    m_MALLOC(emep_string = emnh_heapString_ ## m_pString,  em_requiredBufferSize)\
  } else emep_string = emc_localString_ ## m_pString;\
  m_ASSERT(CopyPString(emep_string, em_requiredBufferSize,  &m_pString) == \
    em_requiredBufferSize-1)\
  m_ASSIGN_P_STRING(m_pString,emep_string,em_requiredBufferSize-1)\

// 
// Passed:
// - mx_pString : string portion used m_HIJACK_P_STRING()  
//   must be a single variable name.
// 
// Changed:
// - mx_pString : this string portion is no more usable 
// 
#define m_RELEASE_HIJACKED_P_STRING(mx_pString) \
if (emnh_heapString_ ## mx_pString != NULL) free(emnh_heapString_ ## mx_pString);


// Wraps sscanf() 
// NOTE: not be confused with ScanPString() declared above... 
// 
// Ret:
// - 0 : number of param actualy read
// - -1: unexpected problem ; anomaly is raised
int SScanfPString(struct P_STRING pString, const char *p_format, ...) ;


// Recognize long signed integral numbers expressed according to C conventions (like 0x prefix for
// hexadecimal base).
// This function wraps strtol() ; check man pages for exact conventions.
//
// Passed:
// - p_cString : '\0'-terminated C-string containing the integral number expression
// - nac_parsedLength: NULL special value: not used
//
// Changed:
// - *ac_value: only significant if parsing is successful (ANSWER__YES)
// - *nac_parsedLength: (when used) ; with ANSWER__YES, actual number of parsed chars corresponding
//   to the integral number (> 0). 
//
// Ret: Succeeded to parse a "long C" number ?
// - ANSWER__YES : successfully parsed (note: check *nac_parsedLength to see if some trailing
//   characters have been ignored) 
// - ANSWER__NO : C number not recognized or too big for long integer 
// - -1: unexpected problem ; anomaly is raised
int ReadGenericIntegerCString(const char *p_cString,  GENERIC_INTEGER *ac_value, int *nac_parsedLength);


// Wraps ReadGenericIntegerCString() above ;
// Allows parsing any kind of string portion (in lieu of '\0'-terminated C-string)...
//
// Passed:
// pString : 
// (Other params : see ReadGenericIntegerCString() above)   
int ReadGenericIntegerPString(struct P_STRING pString,  GENERIC_INTEGER *ac_value,
  int *nac_parsedLength);


#endif // __C_ANSI_P_STRING_H_INCLUDED
