// c-ansi/p-parse.h, version 1.93 (ANSI)
// (c) Puduku - 2023

#ifndef __C_ANSI_P_PARSE_H_INCLUDED
#define __C_ANSI_P_PARSE_H_INCLUDED

#include "c-ansi/p-string.h"


// Parse a string portion sequence according to scan pointer corresponding to the delimitation of
// the parsed lexeme (aka token) in the sequence...
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
// - scanPtr: scan positon (returned, for instance, by ScanStringPortion*()...)
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_SEQUENCE(/*struct STRING_PORTION*/m_sequence, /*const char* */scanPtr,\
  /*struct STRING_PORTION* */na_lexeme) {\
  m_TRACK_IF(scanPtr == NULL)\
  if ((na_lexeme) != NULL) {\
    ((struct STRING_PORTION*)na_lexeme)->string = (m_sequence).string;\
    ((struct STRING_PORTION*)na_lexeme)->stop = scanPtr;\
  }\
  (m_sequence).string = scanPtr ;\
}

// Parse a single char within string portion sequence. 
//
// Passed:
// - m_sequence: as passed 
// - c_char: only significant if n_isCharFunction == NULL 
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_PASS_SINGLE_CHAR(/*struct STRING_PORTION*/m_sequence, \
  /*IS_CHAR_FUNCTION*/n_isCharFunction, /*char*/c_char,\
  /*struct STRING_PORTION* */na_lexeme) {\
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

// Parse a string portion sequence according to chars scanned by ScanStringPortion() function. 
//
// Passed:
// - m_sequence: as passed to (and updated by) ScanStringPortion*() function
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
#define m_PARSE_PASS_CHARS(/*struct STRING_PORTION*/m_sequence, \
  b_regularScan, b_passCharsTill, /*IS_CHAR_FUNCTION*/n_isCharFunction, /*char*/c_char,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = ScanStringPortion(&m_sequence,b_regularScan,b_passCharsTill,\
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
#define m_PARSE_PASS_SPACES(/*struct STRING_PORTION*/m_sequence, \
  /*struct STRING_PORTION* */na_lexeme)  m_PARSE_PASS_CHARS(m_sequence,b_REGULAR_SCAN,\
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
#define m_PARSE_STRIP_SPACES(/*struct STRING_PORTION*/m_sequence, \
  /*struct STRING_PORTION* */na_lexeme) {\
  m_PARSE_PASS_SPACES(m_sequence,NULL)\
  m_PARSE_PASS_CHARS(m_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,isspace,UNDEFINED,na_lexeme)\
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
  /*struct STRING_PORTION*/p_subStringPortion, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = ScanStringPortionTillMatch(&m_sequence,&p_subStringPortion,\
    n_toCharFunction);\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Wrap m_PARSE_TILL_MATCH above
#define m_PARSE_TILL_MATCH_C(/*struct STRING_PORTION*/m_sequence, \
  /*const char* */p_subCString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct STRING_PORTION* */na_lexeme) {\
  m_ASSIGN_LOCAL_C_STRING_PORTION(em_localSubStringPortion,p_subCString) \
  m_PARSE_TILL_MATCH(m_sequence,em_localSubStringPortion,n_toCharFunction,na_lexeme) \
}   

// Parse a string portion sequence: match sub string.
//
// Passed:
// - m_sequence: as passed 
// - a_subStringPortion:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - m_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
#define m_PARSE_MATCH(/*struct STRING_PORTION*/m_sequence, \
  /*struct STRING_PORTION*/p_subStringPortion, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string;\
  switch (CompareStringPortions(&m_sequence,&p_subStringPortion,NULL,n_toCharFunction)) {\
  case LESS_THAN__COMPARISON :\
  case GREATER_THAN__COMPARISON :\
  break; case EQUAL_TO__COMPARISON :\
    em_scanPtr += m_StringPortionLength(&(p_subStringPortion));\
  break; default:\
    m_TRACK()\
  } \
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

// Wrap m_PARSE_MATCH above
#define m_PARSE_MATCH_C(/*struct STRING_PORTION*/m_sequence, \
  /*const char* */p_subCString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*struct STRING_PORTION* */na_lexeme) {\
  m_ASSIGN_LOCAL_C_STRING_PORTION(em_localSubStringPortion,p_subCString) \
  m_PARSE_MATCH(m_sequence,em_localSubStringPortion,n_toCharFunction,na_lexeme) \
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
#define m_PARSE_GENERIC_INTEGER(/*struct STRING_PORTION*/m_sequence, /*GENERIC_INTEGER*/mc_value,\
  /*struct STRING_PORTION* */na_lexeme) {\
  const char *em_scanPtr = (m_sequence).string;\
  int emc_parsedLength;\
  int em_answer;\
  switch (em_answer = ReadGenericIntegerStringPortion(m_sequence, &(mc_value),&emc_parsedLength)) {\
  case ANSWER__YES:\
    em_scanPtr += emc_parsedLength;\
  break; case ANSWER__NO:\
  break; default:\
    m_TRACK()\
  }\
  m_PARSE_SEQUENCE(m_sequence,em_scanPtr, na_lexeme)\
}

#endif // __C_ANSI_P_PARSE_H_INCLUDED
