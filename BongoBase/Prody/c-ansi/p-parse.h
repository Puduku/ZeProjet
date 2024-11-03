// c-ansi/p-parse.h, version 1.93 (ANSI)
// (c) Puduku - 2023

#ifndef __C_ANSI_P_PARSE_H_INCLUDED
#define __C_ANSI_P_PARSE_H_INCLUDED

#include "c-ansi/p-string.h"


// Parse a string portion sequence according to scan pointer corresponding to the delimitation of
// the parsed lexeme (aka token) in the sequence...
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanPString*() function
// - scanPtr: scan positon (returned, for instance, by ScanPString*()...)
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_SEQUENCE(/*struct P_STRING*/m_sequence, /*const char* */scanPtr,\
  /*struct P_STRING* */na_lexeme) {\
  m_TRACK_IF(scanPtr == NULL)\
  if ((na_lexeme) != NULL) {\
    ((struct P_STRING*)na_lexeme)->string = (m_sequence).string;\
    ((struct P_STRING*)na_lexeme)->stop = scanPtr;\
  }\
  (m_sequence).string = scanPtr ;\
}

// Parse a single char within string portion sequence. 
//
// Passed:
// - m_sequence: as passed 
// - n_isCharFunction: (NULL pointer if not used) 
// - c_char: only significant if n_isCharFunction == NULL 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_PASS_SINGLE_CHAR(/*struct P_STRING*/m_sequence, \
  /*IS_CHAR_FUNCTION*/n_isCharFunction, /*char*/c_char,\
  /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string;\
  if (em_scanPtr < (m_sequence).stop) {\
    if (n_isCharFunction != NULL) {\
      if (((IS_CHAR_FUNCTION)n_isCharFunction)(*em_scanPtr)) em_scanPtr++;\
    } else {\
      if (*em_scanPtr == (c_char)) em_scanPtr++;\
    }\
  }\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Parse a string portion sequence according to chars scanned by ScanPString() function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanPString*() function
// - b_regularScan 
//   + b_REGULAR_SCAN (TRUE) :
//   + b_REVERTED_SCAN (FALSE) :
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
#define m_PARSE_PASS_CHARS(/*struct P_STRING*/m_sequence, \
  b_regularScan, b_passCharsTill, /*IS_CHAR_FUNCTION*/n_isCharFunction, /*char*/c_char,\
  /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = ScanPString(&(m_sequence),b_regularScan,b_passCharsTill,\
    n_isCharFunction,c_char);\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Parse a string portion sequence : pass all white spaces... 
//
// Passed:
// - m_sequence: 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka white spaces) 
#define m_PARSE_PASS_SPACES(/*struct P_STRING*/m_sequence, \
  /*struct P_STRING* */na_lexeme)  m_PARSE_PASS_CHARS(m_sequence,b_REGULAR_SCAN,\
  b_PASS_CHARS_WHILE,isspace,UNDEFINED, na_lexeme)

// Parse twice a string portion sequence :
// 1. pass all (leading) white spaces... 
// 2. remove ending white space of sequence 
//
// Passed:
// - m_sequence: 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: points to ending white spaces of initial sequence 
// - *na_lexeme: (if used) parsed lexeme (stripped of ending and leading white spaces) 
#define m_PARSE_STRIP_SPACES(/*struct P_STRING*/m_sequence, \
  /*struct P_STRING* */na_lexeme) {\
  m_PARSE_PASS_SPACES(m_sequence,NULL)\
  m_PARSE_PASS_CHARS(m_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,isspace,UNDEFINED,na_lexeme)\
}


// Parse a string portion sequence according to sub string scanned by ScanPStringTillMatch()
// function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanPString*() function
// - a_subPString:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_TILL_MATCH(/*struct P_STRING*/m_sequence, \
  /*struct P_STRING*/p_subPString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = ScanPStringTillMatch(&m_sequence,&p_subPString,\
    n_toCharFunction);\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Wrap m_PARSE_TILL_MATCH above
#define m_PARSE_TILL_MATCH_C(/*struct P_STRING*/m_sequence, \
  /*const char* */p_subCString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct P_STRING* */na_lexeme) {\
  m_ASSIGN_LOCAL_C_P_STRING(em_localSubPString,p_subCString) \
  m_PARSE_TILL_MATCH(m_sequence,em_localSubPString,n_toCharFunction,na_lexeme) \
}   

// Parse a string portion sequence according to sub string list scanned by
// ScanPStringTillFirstMatchR() function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanPString*() function
// - n_toCharFunction:
// - na_lexeme: NULL if not used
// - subStringsCount: (>0) number of sub-strings 
// - sp_subPStrings: possible sub-strings (p-strings) 
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - mc_matchedEntry: entry for sub-string which was FIRST matched 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_TILL_FIRST_MATCH_R(/*struct P_STRING*/m_sequence,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction, /*int*/mc_matchedEntry, /*struct P_STRING* */na_lexeme,\
  /*int*/subStringsCount, /*struct P_STRING* */sp_subPStrings) {\
  const char *em_scanPtr = ScanPStringTillFirstMatchR(m_sequence,n_toCharFunction,\
    &(mc_matchedEntry),subStringsCount,sp_subPStrings);\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Wrap m_PARSE_TILL_FIRST_MATCH_R above: use C-strings as sub-strings in lieu of string portions.
//
// Passed: 
// - subCString0: 1st C sub-string 
// - other C sub-strings 
// (Instead of:)
// - sp_subPStrings: possible sub-strings (p-strings) 
#define m_PARSE_TILL_FIRST_MATCH_C(/*struct P_STRING*/ m_sequence,\
  /*TO_CHAR_FUNCTION*/ n_toCharFunction,/*int*/mc_matchedEntry, /*struct P_STRING* */na_lexeme,\
  /*int*/ subStringsCount, /*const char* p_subCString0 ,*/ ...) {\
  m_ASSIGN_LOCAL_P_STRINGS(sp_subPStrings,subStringsCount,__VA_ARGS__)\
  m_PARSE_TILL_FIRST_MATCH_R(m_sequence,n_toCharFunction,mc_matchedEntry,na_lexeme,\
    subStringsCount, sp_subPStrings)\
} 


// Parse a string portion sequence: match token 
//
// Passed:
// - m_sequence: as passed 
// - p_token:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_MATCH(/*struct P_STRING*/m_sequence, /*const struct P_STRING*/p_token,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction, /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string;\
  int em_length = m_PStringLength(&(p_token));\
  struct P_STRING em_subSequence = m_SubPString(m_sequence,0,em_length);\
  switch (ComparePStrings(&(em_subSequence),&p_token,NULL,n_toCharFunction,!b_SUB_STRING_2)) {\
  case LESS_THAN__COMPARISON :\
  case GREATER_THAN__COMPARISON :\
  break; case EQUAL_TO__COMPARISON :\
    em_scanPtr += em_length;\
  break; default:\
    m_TRACK()\
  } \
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Wrap m_PARSE_MATCH above
#define m_PARSE_MATCH_C(/*struct P_STRING*/m_sequence, \
  /*const char* */p_token, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct P_STRING* */na_lexeme) {\
  m_ASSIGN_LOCAL_C_P_STRING(em_localToken,p_token) \
  m_PARSE_MATCH(m_sequence,em_localToken,n_toCharFunction,na_lexeme) \
}   

// Parse a string portion sequence according to offset (as if was "scanned" position) 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanPString*() function
// - offset: typically , lenght of a token we want to "skip" in the sequence.
// TODO: what is exactly a valid offset  ? 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_OFFSET(/*struct P_STRING*/m_sequence,  /*int*/offset,\
  /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string + offset;\
  m_ASSERT(em_scanPtr >= (m_sequence).string)\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}


// Parse a C integer value within string portion sequence. 
//
// Passed:
// - m_sequence: as passed 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - mc_value: only significant if succesfully parsed (lexeme's length > 0) 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_GENERIC_INTEGER(/*struct P_STRING*/m_sequence, /*GENERIC_INTEGER*/mc_value,\
  /*struct P_STRING* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string;\
  int emc_parsedLength;\
  int em_answer;\
  switch (em_answer = ReadGenericIntegerPString(m_sequence, &(mc_value),&emc_parsedLength)) {\
  case ANSWER__YES:\
    em_scanPtr += emc_parsedLength;\
  break; case ANSWER__NO:\
  break; default:\
    m_TRACK()\
  }\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

#endif // __C_ANSI_P_PARSE_H_INCLUDED
