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

const struct P_STRING *ap_aTrivialEmptyPString ;  

// Assigning string portions
// -------------------------

#define UNDEFINED_P_STRING { (const char*)UNDEFINED } 


// Establish (regular) string portion.
// 
// Passed:
// - p_string: raw string (pointer)
// - n_length: 
//   + >= 0: raw string length (may include '\0' chars)
//   + -1 special value: '\0'-terminated C string
static inline struct P_STRING m_PString2(const char *p_string, int n_length) {
  struct P_STRING pString = { .string = p_string, .stop = p_string + (n_length<0? strlen(p_string):
    n_length), } ;
  return pString; 
} // m_PString2

// Establish (regular) string portion.
// 
// Passed:
// - p_cString: '\0'-terminated C-string (pointer)
static inline struct P_STRING m_PString(const char *p_cString) {
  struct P_STRING pString = { .string = p_cString, .stop = p_cString + strlen(p_cString), } ;
  return pString; 
} // m_PString


// Assign simple "C-strings" to "string portions".
//
// Passed: 
// - s_pStrings: array of p-strings to assign 
// - stringsCount: (>0) number of strings
// - p_cString0: 1st c-string
// - ...: other c-string(s) / id(s) 
//
// Assigned :
// - s_pStrings:
static inline int m_AssignPStrings(struct P_STRING* s_pStrings, int stringsCount,
  /*const char* p_cString0, */ ...) {
  va_list cStringsIds;
  struct P_STRING *pStringsPtr = s_pStrings;
  va_start(cStringsIds,stringsCount);
  int i = 0; while(i++ < stringsCount) {
    *(pStringsPtr++) = m_PString(va_arg(cStringsIds, const char*));
  } // while
  va_end(cStringsIds);
  return RETURNED;
} // m_AssignPStrings

// Assign simple "C-strings" to LOCAL "string portions". 
// (Wraps m_AssignPStringsIds()) 
//
// Passed: 
// - ms_localPStrings: 
// - stringsCount: (>0) number of strings; 
// - p_cString0: 1st c-string
// - ...: other c-string(s) / id(s)
//
// Assigned (local var):
// - m_pString:
#define m_ASSIGN_LOCAL_P_STRINGS(/*struct P_STRING* */ms_localPStrings,/*int*/stringsCount,\
  /*const char* p_cString0, */ ...)\
  struct P_STRING ms_localPStrings[stringsCount];\
  m_ASSERT(m_AssignPStrings(ms_localPStrings, stringsCount,__VA_ARGS__) == RETURNED)

// Assign simple "C-strings" to "string portions" (with ids).
//
// Passed: 
// - s_pStrings: array of p-strings to assign 
// - sn_ids: array of string ids to assign 
// - stringsCount: (>0) number of strings
// - p_cString0: 1st c-string
// - n_id0: 1st c-string (-1 special value to SKIP item)
// - ...: other c-string(s) / id(s) 
//
// Assigned :
// - s_pStrings:
static inline int m_AssignPStringsIds(struct P_STRING* s_pStrings, int* sn_ids, int stringsCount,
  /*const char* p_cString0 ,int n_id0, */ ...) {
  va_list cStringsIds;
  struct P_STRING *pStringsPtr = s_pStrings;
  int *n_idPtr = sn_ids;  
  va_start(cStringsIds,stringsCount);
  int i = 0; while(i++ < stringsCount) {
    *(pStringsPtr++) = m_PString(va_arg(cStringsIds, const char*));
    *(n_idPtr)++ = va_arg(cStringsIds, int);
  } // while
  va_end(cStringsIds);
  return RETURNED;
} // m_AssignPStringsIds


// Assign simple "C-strings" to LOCAL "string portions" (with ids).
// (Wraps m_AssignPStringsIds()) 
//
// Passed: 
// - ms_localPStrings: 
// - ms_localIds: 
// - stringsCount: (>0) number of strings; 
// - p_cString0: 1st c-string
// - n_id0: 1st c-string's id
// - ...: other c-string(s) / id(s)
//
// Assigned (local var):
// - m_pString:
#define m_ASSIGN_LOCAL_P_STRINGS_IDS(/*struct P_STRING* */ms_localPStrings,/*int* */ms_localIds,\
  /*int*/ stringsCount,/*const char* p_cString0 ,int n_id0, */ ...)\
  struct P_STRING ms_localPStrings[stringsCount]; int ms_localIds[stringsCount];\
  m_ASSERT(m_AssignPStringsIds(ms_localPStrings,ms_localIds, stringsCount,__VA_ARGS__) == RETURNED)



// String portions properties
// -------------------------- 

// Get actual length of a string portion  
// Note: pathetic case in which stop ptr preceeds start ptr is considered as empty string...
//
// Passed:
// - pString: 
// 
// Ret: string portion length (ALWAYS >= 0)
static inline int m_PStringLength(struct P_STRING pString) { 
  int length = pString.stop - pString.string;
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
  (m_PStringLength(p_pString) == 0)


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
 (p_pString).string , m_PStringLength(p_pString)


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
 m_PStringLength(p_pString) , (p_pString).string


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
// - srcPString : source string portion 
//
// Modified:
// - dstString : copy of 'source string' (limited to dst buffer size); ==> ALWAYS '\0' terminated.
//
// Returned:
// - >= 0 and < dstBufferSize: copied length (not taking into account the extra '\0'
//   ending char) (the copy is COMPLETE if and only 'destination' buffer is big enough) 
// - -1: unexpected problem; anomaly is raised
int CopyPString(char *dstRawString, int dstBufferSize, struct P_STRING srcPString);


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

#define b_SUB_STRING_2 b_TRUE

// #REF ComparePStrings
// Lexical comparison of raw strings:
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - otherwize, "char to char" comparison of raw strings.
//
// Passed:
// - pString1 : 1st string to compare #SEE struct-P_STRING
// - pString2 : 2nd string to compare #SEE struct-P_STRING
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before comparison 
//   + != NULL: elimination of each "neutral" char of both strings before comparison ; "binary"
//     comparison is NOT possible
// - n_toCharFunction:
//   + NULL: NO char conversion applied before comparison
//   + != NULL: conversion applied on each char of both strings before comparison ; "binary"
//     comparison is NOT possible
// - b_subString2: + b_SUB_STRING2 (TRUE): compare 2nd string candidates as a sub-string of 1st
//   string
// 
// Returned:
// - >=0: "comparison" between string 1 and string 2: 
//   + LESS_THAN__COMPARISON : string 1 'less than' string 2
//   + EQUAL_TO__COMPARISON : strings are "identical"
//   + GREATER_THAN__COMPARISON : string 1 'greater than' string 2
int ComparePStrings(struct P_STRING pString1, struct P_STRING pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2);

// ComparePStrings() wrapper ; indicates whether string portion matches with a c-string...
// 
// Ret: boolean value (TRUE when strings matches)
int b_EqualToCString(struct P_STRING pString1, const char *p_cString2) ;

// Paranoid wrapper of ComparePStrings() (testing/critical embedded purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
// 
// Ret:
// - >0 : LESS_THAN__COMPARISON / EQUAL_TO__COMPARISON / GREATER_THAN__COMPARISON 
// - -1: unexpected problem, anomaly is raised
int ParanoidComparePStrings(struct P_STRING pString1, struct P_STRING pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2) ;

// #REF Strings-First-Match 
// To avoid wrong match, always sort list of candidates by length (longuest strings first) 

// Lexical comparison of strings (among a list):
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - otherwize, "char to char" comparison of raw strings.
//
// Passed:
// - pString1 : 1st string to compare #SEE struct-P_STRING
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before comparison 
//   + != NULL: elimination of each "neutral" char of both strings before comparison ; "binary"
//     comparison is NOT possible
// - n_toCharFunction:
//   + NULL: NO char conversion applied before comparison
//   + != NULL: conversion applied on each char of both strings before comparison ; "binary"
//     comparison is NOT possible
// - navn_matchedEntry: NULL pointer if not used 
// - cnavn_matchedId: NULL pointer if not used 
// - b_subString2: + b_SUB_STRING2 (TRUE): compare 2nd string candidates as a sub-string of 1st
//   string
// - string2sCount: number of 2nd string candidates... (> 0) 
// - sp_pString2s : array of 2nd strings candidates used for comparison #SEE struct-P_STRING
// #SEE Strings-First-Match 
// - nsn_ids : ids of strings candidates used for comparison (-1 special value: SKIP
//   corresponding candidate)
// 
// Changed:
// - *navn_matchedEntry: (if used) (set to -1 special value if no match found) entry for 2nd string
//   candidate which (first) matched 
// - *cnavn_matchedId: (if used) (set to -1 special value if no match found) entry for 2nd string
//   candidate which (first) matched 
// 
// Returned: 
// - >=0: "comparison" between 1st string and matched 2nd (sub-)string: 
//   + LESS_THAN__COMPARISON : 1st string 'lexilically before' 2nd (sub-)string
//   + EQUAL_TO__COMPARISON : 1st string and 2nd (sub)-string "match" 
//   + GREATER_THAN__COMPARISON : string 1 'lexilically  after' (sub-)string 2
int ComparePStringsAmongR(struct P_STRING pString1, 
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2,
  int *navn_matchedEntry, int *cnavn_matchedId, int string2sCount, const struct P_STRING sp_pString2s[],
  int nsn_ids[]) ;


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
// - pString : string portion to scan. 
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
const char *ScanPString(struct P_STRING pString,
  char b_regularScan, char b_passCharsTill, IS_CHAR_FUNCTION n_isCharFunction, int c_char);


// String "scanning" function 
// Locate first matching sub-string based on 
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - "char to char" comparison otherwise.
//
// Passed:
// - pString: string to scan #SEE struct-P_STRING
// - subPString: sub string to locate
// - n_toCharFunction: 
//   + NULL: not provided; no conversion before  comparison 
//   + != NULL: conversion applied on (each char of) both strings before comparison ; "binary"
//     comparison is NOT possible
//
// Returned:
// (!= NULL) scanning position; use b_SCAN_P_STRING_LOCATED() below to check whether
// sub-string is actually located. 
const char *ScanPStringTillMatch(struct P_STRING pString, struct P_STRING subPString,
  TO_CHAR_FUNCTION n_toCharFunction);


// Extend ScanPStringTillMatch "scanning" function : 
// Locate first matching sub-string, among DIFFERENT sub-strings. 
//
// Passed:
// - pString: string to scan #SEE struct-P_STRING
// - n_toCharFunction: 
//   + NULL: not provided; no conversion before  comparison 
//   + != NULL: conversion applied on (each char of) both strings before comparison ; "binary"
//     comparison is NOT possible
// - navn_matchedEntry: NULL pointer if not used 
// - cnavn_matchedId: NULL pointer if not used 
// - subStringsCount: >0
// - subPString0: 1st possible sub string to locate
// - n_id0: id of 1st possible sub string to locate (-1 special value to SKIP that sub-string)
// #SEE Strings-First-Match 
// - other possible sub strings (...) : indicate all other possible sub-strings to match ;
//
// Changed:
// - *navn_matchedEntry: (if used) (set to -1 special value if no sub-string located) entry for
//   sub-string which was FIRST matched 
// - *cnavn_matchedId: (if used) (set to -1 special value if no sub-string located) id for 2nd
//   string candidate which (first) matched 
//
// Returned:
// (!= NULL) scanning position; use b_SCAN_P_STRING_LOCATED() below to check whether
// sub-string is actually located. 
const char *ScanPStringTillFirstMatch(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *navn_matchedId, int subStringsCount, /*struct P_STRING subPString0,
  int sn_id0, */ ...);

// See ScanPStringTillFirstMatch()
// 
// Passed:
// - subPStringsIds:
// (Instead of:)
// - subPString0: 1st possible sub string to locate
// - n_id0: id of 1st possible sub string to locate (-1 special value to SKIP that sub-string)
// - other possible sub strings (...) : indicate all other possible sub-strings to match ;
const char *ScanPStringTillFirstMatchV(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *navn_matchedId, int subStringsCount, va_list subPStringsIds);

// See ScanPStringTillFirstMatchV()
// 
// Passed:
// - sp_subPStrings:
// - nsn_ids:
// (Instead of:)
// - subPStringsIds:
const char *ScanPStringTillFirstMatchR(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *navn_matchedId, int subStringsCount,
  const struct P_STRING sp_subPStrings[], int nsn_ids[]) ;

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
    m_pString)); \
  char* emep_string = (char*)UNDEFINED; \
  if (em_requiredBufferSize > LOCAL_STRING_AUTO_BUFFER_SIZE) {\
    m_MALLOC(emep_string = emnh_heapString_ ## m_pString,  em_requiredBufferSize)\
  } else emep_string = emc_localString_ ## m_pString;\
  m_ASSERT(CopyPString(emep_string, em_requiredBufferSize, m_pString) == \
    em_requiredBufferSize-1)\
  m_pString = m_PString2(emep_string,em_requiredBufferSize-1);\

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
int ReadGenericIntegerCString(const char *p_cString,  GENERIC_INTEGER *ac_value,
  int *nac_parsedLength);


// Wraps ReadGenericIntegerCString() above ;
// Allows parsing any kind of string portion (in lieu of '\0'-terminated C-string)...
//
// Passed:
// pString : 
// (Other params : see ReadGenericIntegerCString() above)   
int ReadGenericIntegerPString(struct P_STRING pString,  GENERIC_INTEGER *ac_value,
  int *nac_parsedLength);


#endif // __C_ANSI_P_STRING_H_INCLUDED
