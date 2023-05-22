// c-ansi/c-string.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_C_STRING_H_INCLUDED
#define __C_ANSI_C_STRING_H_INCLUDED

#include <string.h>
#include <ctype.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"

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
struct STRING_PORTION { // #REF struct-STRING_PORTION
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

// #REF TRIVIAL_EMPTY_STRING_PORTION
// Remark: string == stop == NULL (called "trivial" empty string) is thus a VALID representation of
// empty string portions...



// Assigning string portions
// -------------------------

// Assign a string portion.
// 
// Passed:
// - m_stringPortion
// - p_string:
// - un_length: (once evaluated parameter)
//   + >= 0: raw string length (may include '\0' chars)
//   + -1 special value: '\0'-terminated C string
// 
// Assigned:
// - m_stringPortion: 
#define m_ASSIGN_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion,\
  /*const char* */p_string, /*int*/un_length) {\
  int em_length = (un_length);\
  if (em_length < 0) em_length = strlen(p_string);\
  (m_stringPortion).stop = ((m_stringPortion).string = (p_string)) + (em_length);\
}

// Assign a local string portion.
// 
// Passed:
// - m_stringPortion: local var to create
// (other params : see m_ASSIGN_STRING_PORTION() above) 
//
// Created (local var):
// - m_stringPortion: 
#define m_ASSIGN_LOCAL_STRING_PORTION(/*struct STRING_PORTION*/m_localStringPortion,\
  /*const char* */p_string, /*int*/un_length) \
  struct STRING_PORTION m_localStringPortion;\
  m_ASSIGN_STRING_PORTION(m_localStringPortion, p_string, un_length)

// Assign "empty string" to "string portion" 
// TODO: use "trivial" empty string instead 
// 
// Passed:
// - m_stringPortion: 
//
// Assigned:
// - m_stringPortion: 
#define m_ASSIGN_EMPTY_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion) \
  m_ASSIGN_STRING_PORTION(m_stringPortion, GOOD_OLD_EMPTY_C_STRING,\
    EMPTY_STRING_LENGTH0)

// Assign "empty string" to LOCAL "string portion" 
// See m_ASSIGN_EMPTY_STRING_PORTION() above.
#define m_ASSIGN_LOCAL_EMPTY_STRING_PORTION(/*struct STRING_PORTION*/m_localStringPortion) \
  struct STRING_PORTION m_localStringPortion;\
  m_ASSIGN_EMPTY_STRING_PORTION(m_localStringPortion)

// Assign simple "C-string" to "string portion" 
// 
// Passed:
// - m_stringPortion: 
// - p_cString: c string (buffer start address)
//
// Assigned:
// - m_stringPortion:
#define m_ASSIGN_C_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion,\
  /*const char* */p_cString) {\
  m_ASSIGN_STRING_PORTION(m_stringPortion,p_cString, -1)\
}

// Assign simple "C-string" to local "string portion" 
// 
// Passed:
// - m_stringPortion: local var to create
// (other params : see m_ASSIGN_STRING_PORTION() above) 
//
// Assigned (local var):
// - m_stringPortion:
#define m_ASSIGN_LOCAL_C_STRING_PORTION(/*struct STRING_PORTION*/m_localStringPortion,\
  /*const char* */p_cString) \
  struct STRING_PORTION m_localStringPortion;\
  m_ASSIGN_C_STRING_PORTION(m_localStringPortion,  p_cString)


// String portions properties
// -------------------------- 

// Get actual length of a string portion  
// Note: pathetic case in which stop ptr preceeds start ptr is considered as empty string...
//
// Passed:
// - ap_stringPortion: 
// 
// Ret: string portion length (ALWAYS >= 0)
static inline int m_StringPortionLength(const struct STRING_PORTION *ap_stringPortion) { 
  int length = ap_stringPortion->stop - ap_stringPortion->string;
  if (length < 0) length = 0;
  return length;
} // m_StringPortionLength

// Is a string portion empty ?
//
// Passed:
// - p_stringPortion: 
// 
// Ret:
// - TRUE: the stringPortion is empty (length <= 0)
// - FALSE: the stringPortion is not empty (length > 0)
#define b_EMPTY_STRING_PORTION(/*const struct STRING_PORTION*/p_stringPortion) \
  (m_StringPortionLength(&(p_stringPortion)) <= 0)

// Get length of a string portion  
// TODO: ag virer ???
//
// Passed:
// - p_stringPortion: 
// 
// Changed:
// - m_length: (ALWAYS >= 0)
#define m_GET_STRING_PORTION_LENGTH(/*const struct STRING_PORTION*/p_stringPortion, m_length) {\
  m_ASSERT( ((m_length) = m_StringPortionLength(&(p_stringPortion))) >= 0 ) \
}

// Provide buffer / length function parameters corresponding to string portion.
// 
// Passed:
// - p_stringPortion: string portion 
#define m_STRING_PORTION_2_BUFFER_LENGTH(/*const struct STRING_PORTION*/p_stringPortion) \
 (p_stringPortion).string , m_StringPortionLength(&(p_stringPortion))


// Verify if string portion corresponds well to C-string (i.e exempted of '\0' chars)
//
// Passed:
// - a_stringPortion : address of string portion data struct 
// 
// Changed: 
// - a_stringPortion : rectified with COMPLETED__BUT status... 
// - <a_stringPortion>->stop : delimit actual string length 
//
// Returned: 
// - COMPLETED__OK:
// - COMPLETED__BUT: rectified
// - -1: unexpected problem; anomaly is raised
int VerifyCStringPortion(struct STRING_PORTION *a_stringPortion);


// Using string portions with printf() family of functions...
 
// format directive for string portions:
#define FMT_STRING_PORTION "%.*s" 

// String portions wrapper usable with format directive above.
//
// Passed:
// - p_stringPortion: string portion passed in printf() subsequent argument(s) 
#define m_STRING_PORTION_2_FMT_ARGS(/*const struct STRING_PORTION*/p_stringPortion) \
 m_StringPortionLength(&(p_stringPortion)) , (p_stringPortion).string


// Copy string portions 
// --------------------

// Establish the optimal size for destination string buffer in string copy operation. 
// 
// Passed:
// - srcStringPortionLength : 'string' source string length 
// 
// Ret: optimal buffer size (> 0)
#define /*int*/OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(/*int*/srcStringPortionLength) \
  ((srcStringPortionLength) + 1) 


// Establish the optimal size for destination string buffer in string copy operation. 
// 
// Passed:
// - p_srcStringPortion : 'string' source string #SEE struct-STRING_PORTION 
//
// Changed:
// - m_optimalBufferSize: optimal buffer size (> 0)
// TODO: replace with m_OptimalBufferSize4StringPortionCopy()
#define m_GET_OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(\
  /*const struct STRING_PORTION*/p_srcStringPortion, /*int*/m_optimalBufferSize) {\
  (m_optimalBufferSize) = OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(m_StringPortionLength(\
    &(p_srcStringPortion))) ;\
  m_ASSERT((m_optimalBufferSize) > 0)\
}


// Same purpose as standard "strcpy" function, except that this function takes care never to
// overflow the target string.
//
// Passed:
// - dstRawString : target string (buffer address)
// - dstBufferSize : target string's buffer size; > 0 (CANNOT be == 0) ; see 
//   m_GET_OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY() macro above.
// - ap_srcStringPortion : source string portion 
//
// Modified:
// - dstString : copy of 'source string' (limited to dst buffer size); ==> ALWAYS '\0' terminated.
// - *na_dstStringPortion : (if explicit target used) target string portion 
//
// Returned:
// - >= 0 and < dstBufferSize: copied length (not taking into account the extra '\0'
//   ending char) (the copy is COMPLETE if and only 'destination' buffer is big enough) 
// - -1: unexpected problem; anomaly is raised
// TODO: supprimer ap_ ???
int CopyStringPortion(char *dstRawString, int dstBufferSize, 
  const struct STRING_PORTION *ap_srcStringPortion);


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


// #REF CompareStringPortions
// Comparison of raw strings:
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - otherwize, "char to char" comparison of raw strings.
//
// Passed:
// - ap_stringPortion1 : 1st string to compare #SEE struct-STRING_PORTION
// - ap_stringPortion2 : 2nd string to compare #SEE struct-STRING_PORTION
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
// - -1: unexpected problem; anomaly is raised
int CompareStringPortions(const struct STRING_PORTION *ap_stringPortion1, 
  const struct STRING_PORTION *ap_stringPortion2,  IS_CHAR_FUNCTION n_isNeutralCharFunction,
  TO_CHAR_FUNCTION n_toCharFunction) ; 

// CompareStringPortions() wrapper ; compare a string portion with a c-string...
static inline int m_CompareWithCString(const struct STRING_PORTION *ap_stringPortion1, const char *ap_cString2) {
  m_ASSIGN_LOCAL_C_STRING_PORTION(stringPortion2,ap_cString2)
  return CompareStringPortions(ap_stringPortion1, &stringPortion2,  NULL, NULL) ;
} 

// Paranoid wrapper of CompareStringPortions() (testing/critical embedded purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
int ParanoidCompareStringPortions(const struct STRING_PORTION *ap_stringPortion1,
  const struct STRING_PORTION *ap_stringPortion2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION n_toCharFunction) ; 


// Parsing string portions
// ----------------------- 

#define b_REGULAR_SCAN b_TRUE
#define b_REVERTED_SCAN b_FALSE0

#define b_PASS_CHARS_TILL b_TRUE
#define b_PASS_CHARS_WHILE b_FALSE0

// Simple string (char to char) "scanning" function 
// Locate first char having / NOT having a property
// NOTE: not be confused with SScanfStringPortion() declared below... 
//
// Passed:
// - ap_stringPortion : string portion to scan. 
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
// - != NULL: scanning position ; use b_SCAN_STRING_PORTION_LOCATED() below to check whether char
//   is actually located. 
// - NULL: unexpected problem; anomaly is raised
const char *ScanStringPortion(const struct STRING_PORTION *ap_stringPortion,
  char b_regularScan, char b_passCharsTill, IS_CHAR_FUNCTION n_isCharFunction, int c_char);


// String "scanning" function 
// Locate first matching sub-string based on 
// - "binary" comparison when possible (i.e memcmp()-based => more efficient) 
// - "char to char" comparison otherwise.
//
// Passed:
// - ap_stringPortion: string to scan #SEE struct-STRING_PORTION
// - ap_subStringPortion: sub string to locate
// - n_toCharFunction: 
//   + NULL: not provided; no conversion before  comparison 
//   + != NULL: conversion applied on (each char of) both strings before comparison ; "binary"
//     comparison is NOT possible
//
// Returned:
// - != NULL: scanning position; use b_SCAN_STRING_PORTION_LOCATED() below to check whether
//   sub-string is actually located. 
// - NULL: unexpected problem; anomaly is raised
const char *ScanStringPortionTillMatch(const struct STRING_PORTION *ap_stringPortion,
  const struct STRING_PORTION *ap_subStringPortion,  TO_CHAR_FUNCTION n_toCharFunction);


// Paranoid wrapper of ScanStringPortionTillMatch() (testing/critical embedded purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
const char *ParanoidScanStringPortionTillMatch(const struct STRING_PORTION *ap_stringPortion, 
  const struct STRING_PORTION *ap_subStringPortion,  TO_CHAR_FUNCTION n_toCharFunction) ;


// Boolean interpretation (located or not) of scan pointer returned by ScanStringPortion*()
// functions.
//
// Passed:
// - p_stringPortion: as passed to (and updated by) ScanStringPortion*()
// - scanPtr : offset returned by ScanStringPortion*()
//
// Ret:
// - TRUE: located at position indicated by scan pointer 
// - FALSE: not located, 
#define b_SCAN_STRING_PORTION_LOCATED(/*const struct STRING_PORTION*/p_stringPortion,\
  /*const char* */u_scanPtr)  ( (u_scanPtr) < (p_stringPortion).stop ) 

// Parse a string portion sequence according to scan pointer returned by ScanStringPortion*()
// functions. The scan pointer is the delimitation of the parsed lexeme (aka token) in the 
// sequence...
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
// - scanPtr: scan positon (returned by ScanStringPortion*()...)
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_SEQUENCE(/*struct STRING_PORTION*/m_sequence, /*const char* */scanPtr,\
  /*struct STRING_PORTION* */na_lexeme) {\
  if ((na_lexeme) != NULL) {\
    ((struct STRING_PORTION*)na_lexeme)->string = (m_sequence).string;\
    ((struct STRING_PORTION*)na_lexeme)->stop = scanPtr;\
  }\
  (m_sequence).string = scanPtr ;\
}

// Parse a string portion sequence according to chars scanned by ScanStringPortion() function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
// - b_regularScan 
//   + b_REGULAR_SCAN (TRUE) :
//   + b_REVERSED_SCAN (FALSE) :
// - b_passCharsTill:  
//   + b_PASS_CHARS_TILL (TRUE) : seek 1st character HAVING the property
//   + b_PASS_CHARS_WHILE (FALSE) : seek 1st character NOT HAVING the property
// - n_isCharFunction: property evaluation function (see IS_CHAR_FUNCTION) (NULL if not used) 
// - c_char: only significant if n_isCharFunction == NULL 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_PASS_CHARS(/*struct STRING_PORTION*/m_sequence, \
  b_regularScan, b_passCharsTill, /*IS_CHAR_FUNCTION*/n_isCharFunction, /*char*/c_char,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = ScanStringPortion(&m_sequence,b_regularScan,b_passCharsTill,\
    n_isCharFunction,c_char);\
  m_TRACK_IF(em_scanPtr == NULL)\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Parse a string portion sequence according to sub string scanned by ScanStringPortionTillMatch()
// function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
// - a_subStringPortion:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_TILL_MATCH(/*struct STRING_PORTION*/m_sequence, \
  /*struct STRING_PORTION*/p_subStringPortion, /*TO_CHAR_FUNCTION*/ n_toCharFunction,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = ScanStringPortionTillMatch(&m_sequence,&p_subStringPortion,\
    n_toCharFunction);\
  m_TRACK_IF(em_scanPtr == NULL)\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Parse a string portion sequence according to offset (as if was "scanned" position) 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
// - offset: typically , lenght of a token we want to "skip" in the sequence.
// TODO: what is exactly a valid offset  ? 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_OFFSET(/*struct STRING_PORTION*/m_sequence,  /*int*/offset,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string + offset;\
  m_TRACK_IF(em_scanPtr == NULL)\
  m_ASSERT(em_scanPtr >= (m_sequence).string)\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Converting string portions
// --------------------------

#define b_C_TERMINATED b_TRUE
#define b_NOT_C_TERMINATED b_FALSE0

// Simple string chars conversion function. 
// Possibly, "neutral" chars (white space, etc.) can be eliminated...
//
// Passed:
// - aep_stringPortion: string to convert #SEE struct-STRING_PORTION
// - b_cTerminated: 
//   + b_C_TERMINATED : explictely '\0'-terminated
//   + b_NOT_C_TERMINATED : pure string portion (not '\0'-terminated)
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before conversion
//   + != NULL: elimination of each "neutral" char in lieu of conversion
// - toCharFunction: 
//
// Modified:
// - aep_stringPortion: converted string portion ; 
//   string length may have been reduced after elimination of neutral chars.  
//   '\0' ending char is added if the string is explicitly NULL-terminated.
//
// Returned:
// - >=0: string length after conversion 
// - -1: unexpected problem; anomaly is raised
int ConvertStringPortion(struct STRING_PORTION *aep_stringPortion, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) ;

// Paranoid wrapper of ConvertStringPortion() (testing/critical embedded stuff purpose)
// Ensure that the optmized forms of the fonction (i.e called without filter) behave identically
// to general one... 
int ParanoidConvertStringPortion(struct STRING_PORTION *aep_stringPortion, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) ;


// Using string portions with standard string manipulation functions
// ----------------------------------------------------------------- 

// Max size of buffer in STACK when a local copy of a string portion parameter is required.
#define LOCAL_STRING_AUTO_BUFFER_SIZE 256

// Replace temporarilly string portion with an equivalent '\0'-terminated C string.
// 
// Passed:
// - m_stringPortion : #SEE struct-STRING_PORTION
//   must be a single variable name.
//
// Changed:
// - m_stringPortion: "hijacked" ; 
//   + string: points to adequate buffer (ALWAYS '\0'-terminated) :
//     . points to auto (stack) required buffer size <= LOCAL_STRING_AUTO_BUFFER_SIZE 
//     . otherwise, points to buffer created in heap
//   + stop: updated accordingly if necessary 
#define m_HIJACK_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion) \
  char emc_localString_ ## m_stringPortion [LOCAL_STRING_AUTO_BUFFER_SIZE] ;\
  char *emnh_heapString_ ## m_stringPortion = NULL;\
  int em_requiredBufferSize = UNDEFINED;\
  m_GET_OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(m_stringPortion,em_requiredBufferSize) \
  char* emep_string = (char*)UNDEFINED; \
  if (em_requiredBufferSize > LOCAL_STRING_AUTO_BUFFER_SIZE) {\
    m_MALLOC(emep_string = emnh_heapString_ ## m_stringPortion,  em_requiredBufferSize)\
  } else emep_string = emc_localString_ ## m_stringPortion;\
  m_ASSERT(CopyStringPortion(emep_string, em_requiredBufferSize,  &m_stringPortion) == \
    em_requiredBufferSize-1)\
  m_ASSIGN_STRING_PORTION(m_stringPortion,emep_string,em_requiredBufferSize-1)\

// 
// Passed:
// - mx_stringPortion : string portion used m_HIJACK_STRING_PORTION()  
//   must be a single variable name.
// 
// Changed:
// - mx_stringPortion : this string portion is no more usable 
// 
#define m_RELEASE_HIJACKED_STRING_PORTION(mx_stringPortion) \
if (emnh_heapString_ ## mx_stringPortion != NULL) free(emnh_heapString_ ## mx_stringPortion);


// Wraps sscanf() 
// NOTE: not be confused with ScanStringPortion() declared above... 
// 
// Ret:
// - 0 : number of param actualy read
// - -1: unexpected problem ; anomaly is raised
int SScanfStringPortion(struct STRING_PORTION stringPortion, const char *p_format, ...) ;


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
//   to the integral number. 
//
// Ret: Succeeded to parse a "long C" number ?
// - ANSWER__YES : successfully parsed (note: check *nac_parsedLength to see if some trailing
//   characters have been ignored) 
// - ANSWER__NO : C number not recognized or too big for long integer 
// - -1: unexpected problem ; anomaly is raised
int ReadCLongCString(const char *p_cString,  long *ac_value, int *nac_parsedLength);


// Wraps ReadCLongCString() above ;
// Allows parsing any kind of string portion (in lieu of '\0'-terminated C-string)...
//
// Passed:
// stringPortion : 
// (Other params : see ReadCLongCString() above)   
int ReadCLongStringPortion(struct STRING_PORTION stringPortion,  long *ac_value,
  int *nac_parsedLength);

#endif // __C_ANSI_C_STRING_H_INCLUDED
