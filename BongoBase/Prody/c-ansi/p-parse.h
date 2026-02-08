// c-ansi/p-parse.h, version 1.93 (ANSI)
// (c) Puduku - 2023

#ifndef __C_ANSI_P_PARSE_H_INCLUDED
#define __C_ANSI_P_PARSE_H_INCLUDED

//////////////////////////////
#include "c-ansi/p-parse.topo"
//////////////////////////////

#include "c-ansi/p-string.h"

// 1. "Core" parsing function 

// Parse a string portion sequence according to scan pointer corresponding to the delimitation of
// the parsed lexeme (aka token) in the sequence...
//
// Passed:
// - *a_sequence: as passed to (and updated by) "scanning"  function
// - scanPtr: scan positon (returned by "scanning" ...)
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseSequence(struct P_STRING *a_sequence, const char* scanPtr, struct P_STRING* na_lexeme); 

// 2. PParsePass*() functions: pass specific characters

// Parse a single char within string portion sequence. 
//
// Passed:
// - *a_sequence: as passed 
// - n_isCharFunction: (NULL pointer if not used) 
// - c_char: only significant if n_isCharFunction == NULL 
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParsePassSingleChar(struct P_STRING *a_sequence, IS_CHAR_FUNCTION n_isCharFunction, char c_char,
  struct P_STRING* na_lexeme) ;

// Parse a string portion sequence according to chars scanned by ScanPString() function. 
//
// Passed:
// - *a_sequence: as passed to (and updated by) ScanPString*() function
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
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParsePassChars(struct P_STRING* a_sequence, char b_regularScan, char b_passCharsTill,
  IS_CHAR_FUNCTION n_isCharFunction, char c_char, struct P_STRING* na_lexeme) ;

// Parse a string portion sequence : pass all white spaces... 
//
// Passed:
// - *a_sequence: 
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka white spaces) 
//
// Ret:
// - RETURNED
static inline int m_PParsePassSpaces(struct P_STRING* a_sequence, struct P_STRING* na_lexeme) {
  return PParsePassChars(a_sequence,b_REGULAR_SCAN, b_PASS_CHARS_WHILE,isspace,(char)UNDEFINED,
    na_lexeme);
} // m_PParsePassSpaces


// 3. PParseMatch*() functions: pass matching tokens 

// Parse a string portion sequence: match token 
//
// Passed:
// - *a_sequence: as passed 
// - p_token:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseMatch(struct P_STRING* a_sequence, const struct P_STRING p_token,
  TO_CHAR_FUNCTION n_toCharFunction, struct P_STRING*  na_lexeme) ;

// Parse a string portion sequence: match token (among candidates) 
//
// Passed:
// - *a_sequence: as passed 
// - n_toCharFunction:
// - cnavn_matchedId: Not significant if tokens ids not used; NULL pointer if not used 
// - na_lexeme: NULL if not used
// - tokensCount: >0 
// - sp_tokens: possible tokens (p-strings) 
// - nsn_ids: ids for possible tokens (-1 special value => skip that token) 
// #SEE Strings-First-Match @ c-ansi/p-string.h
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *avn_matchedEntry: (set to -1 special value if no match) entry of sub-string  which was FIRST
//   matched 
// - *cnavn_matchedId: (if used) (set to -1 special value if no match) sub-string's id which was
//   FIRST matched 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseMatchAmongR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* avn_matchedEntry, int* cnavn_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) ;


// Wrap PParseMatchAmongR() above
#define m_P_PARSE_MATCH_AMONG_C(/*struct P_STRING*/m_sequence, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*int*/avn_matchedEntry,/*int* */navn_matchedId,/*struct P_STRING* */na_lexeme,\
  /*int*/ tokensCount,/*const char* p_token0, int n_id0, */ ...) {\
  m_LOCAL_P_STRINGS_IDS(sp_localTokens,sn_localIds,tokensCount,__VA_ARGS__)\
  PParseMatchAmongR(&(m_sequence),n_toCharFunction,avn_matchedEntry,navn_matchedId,na_lexeme,\
    tokensCount, sp_localTokens,sn_localIds);\
}   

// 4. PParseTill*Match*() functions: scan till matching tokens 

// Parse a string portion sequence according to sub string (token) scanned by ScanPStringTillMatch()
// function. 
//
// Passed:
// - *a_sequence: as passed to (and updated by) ScanPString*() function
// - a_token:
// - n_toCharFunction:
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseTillMatch(struct P_STRING* a_sequence, struct P_STRING p_token,
  TO_CHAR_FUNCTION n_toCharFunction, struct P_STRING*  na_lexeme) ;

// Parse a string portion sequence according to sub strings (tokens) list scanned by
// ScanPStringTillFirstMatchR() function. 
//
// Passed:
// - *a_sequence: as passed to (and updated by) ScanPString*() function
// - n_toCharFunction:
// - navn_matchedEntry: NULL pointer if not used 
// - cnavn_matchedId: Not significant if tokens ids not used; NULL pointer if not used 
// - na_lexeme: NULL if not used
// - tokensCount: (>0) number of sub-strings 
// - sp_tokens: possible sub-strings (p-strings) 
// - nsn_ids: (NULL pointer if not used) tokens ids (-1 special value to SKIP  token)
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *navn_matchedEntry: (set to -1 special value if no match) entry for sub-string which was FIRST
//   matched 
// - *cnavn_matchedId: (if used) (set to -1 special value if no match) entry for sub-string which
//   was FIRST matched 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseTillFirstMatchR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* navn_matchedEntry, int* cnavn_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) ;

// Wraps PParseTillFirstMatchR() above: use C-strings as sub-strings in lieu of string portions.
// Note: tokens ids NOT supported
//
// Passed: 
// - p_token0: 1st token (C string) 
// - other C sub-strings 
// (Instead of:)
// - sp_tokens: possible sub-strings (p-strings) 
#define m_P_PARSE_TILL_FIRST_MATCH_C(/*struct P_STRING*/ m_sequence,\
  /*TO_CHAR_FUNCTION*/ n_toCharFunction,/*int* */navn_matchedEntry, /*struct P_STRING* */na_lexeme,\
  /*int*/ tokensCount, /*const char* p_token0, */ ...) {\
  m_LOCAL_P_STRINGS(sp_localTokens,tokensCount,__VA_ARGS__)\
  PParseTillFirstMatchR(&(m_sequence),n_toCharFunction,navn_matchedEntry,(int*)UNDEFINED,\
    na_lexeme, tokensCount, sp_localTokens, NULL);\
} 

// 5. PParse*() functions: miscellaneous parsing 

// Parse a string portion sequence according to offset (as if was "scanned" position) 
//
// Passed:
// - *a_sequence: as passed to (and updated by) ScanPString*() function
// - offset: typically , lenght of a token we want to "skip" in the sequence.
// TODO: what is exactly a valid offset  ? 
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseOffset(struct P_STRING* a_sequence,  int offset, struct P_STRING* na_lexeme) ;


// Parse a C integer value within string portion sequence. 
//
// Passed:
// - *a_sequence: as passed 
// - na_lexeme: NULL if not used
//
// Changed:
// - *a_sequence: parsed lexeme removed in the sequence 
// - *ac_value: only significant if succesfully parsed (lexeme's length > 0) 
// - *na_lexeme: (if used) parsed lexeme (aka token) 
//
// Ret:
// - RETURNED
int PParseGenericInteger(struct P_STRING* a_sequence, GENERIC_INTEGER* ac_value,
  struct P_STRING* na_lexeme) ;

#endif // __C_ANSI_P_PARSE_H_INCLUDED
