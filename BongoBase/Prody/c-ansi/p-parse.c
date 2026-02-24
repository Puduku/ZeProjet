// c-ansi/p-parse.c, version 1.93 (ANSI)
// (c) Puduku - 2023


#include "c-ansi/p-parse.h"
#include "c-ansi/p-string.h"

// Public function: see .h
int o_PParseSequence(struct P_STRING *a_sequence, const char* scanPtr,
  struct P_STRING* na_lexeme) {
  if ((na_lexeme) != NULL) {
    na_lexeme->string = a_sequence->string;
    na_lexeme->stop = scanPtr;
  }\
  a_sequence->string = scanPtr ;

  return RETURNED;
} // o_PParseSequence


// Public function: see .h
int o_PParsePassSingleChar(struct P_STRING *a_sequence, IS_CHAR_FUNCTION n_isCharFunction, char c_char,
  struct P_STRING* na_lexeme) {
  const char *scanPtr = a_sequence->string;
  if (scanPtr < a_sequence->stop) {
    if (n_isCharFunction != NULL) {
      if (((IS_CHAR_FUNCTION)n_isCharFunction)(*scanPtr)) scanPtr++;
    } else {
      if (*scanPtr == (c_char)) scanPtr++;
    } // if
  } // if
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // o_PParsePassSingleChar

// Public function: see .h
int o_PParsePassChars(struct P_STRING* a_sequence, char b_regularScan, char b_passCharsTill,
  IS_CHAR_FUNCTION n_isCharFunction, char c_char, struct P_STRING* na_lexeme) {
  const char *scanPtr = ScanPString(*a_sequence,b_regularScan,b_passCharsTill, n_isCharFunction,
    c_char);
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // o_PParsePassChars


// Public function: see .h
int PParseMatch(struct P_STRING* a_sequence, const struct P_STRING p_token,
  TO_CHAR_FUNCTION n_toCharFunction, struct P_STRING*  na_lexeme) {
  const char *scanPtr = a_sequence->string;
  int length = m_PStringLength(p_token);
  switch (ComparePStrings(*a_sequence,p_token,NULL,n_toCharFunction,b_SUB_STRING_2)) {
  case LESS_THAN__COMPARISON :
  case GREATER_THAN__COMPARISON :
  break; case EQUAL_TO__COMPARISON :
    scanPtr += length;
  break; default:
    m_TRACK()
  } // switch 
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseMatch


// Public function: see .h
int PParseMatchAmongR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* avn_matchedEntry, int* cnavn_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) {
  const char *scanPtr = a_sequence->string;
  switch (ComparePStringsAmongR(*a_sequence,NULL,n_toCharFunction,b_SUB_STRING_2,
    avn_matchedEntry, cnavn_matchedId, tokensCount, sp_tokens, nsn_ids)) {
  case LESS_THAN__COMPARISON :
  case GREATER_THAN__COMPARISON :
  break; case EQUAL_TO__COMPARISON :
    scanPtr += m_PStringLength((sp_tokens)[*(avn_matchedEntry)]);
  break; default:
    m_TRACK()
  } // switch 
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseMatchAmongR

// Public function: see .h
int PParseTillMatch(struct P_STRING* a_sequence, struct P_STRING p_token,
  TO_CHAR_FUNCTION n_toCharFunction, struct P_STRING*  na_lexeme) {
  const char *scanPtr = o_ScanPStringTillMatch(*a_sequence,p_token, n_toCharFunction);
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseTillMatch

// Public function: see .h
int PParseTillFirstMatchR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* navn_matchedEntry, int* cnavn_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) {
  const char *scanPtr = o_ScanPStringTillFirstMatchR(*a_sequence,n_toCharFunction,
    navn_matchedEntry,cnavn_matchedId,tokensCount,sp_tokens,nsn_ids);
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseTillFirstMatchR


// Public function: see .h
int PParseOffset(struct P_STRING* a_sequence,  int offset, struct P_STRING* na_lexeme) {
  const char *scanPtr = a_sequence->string + offset;
  m_ASSERT(scanPtr >= a_sequence->string)
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseOffset


// Public function: see .h
int PParseGenericInteger(struct P_STRING* a_sequence, GENERIC_INTEGER* ac_value,
  struct P_STRING* na_lexeme) {
  const char *scanPtr = a_sequence->string;
  int c_parsedLength;
  switch (ReadGenericIntegerPString(*a_sequence, ac_value,&c_parsedLength)) {
  case ANSWER__YES:
    scanPtr += c_parsedLength;
  break; case ANSWER__NO:
  break; default:
    m_TRACK()
  } // switch
  return o_PParseSequence(a_sequence,scanPtr, na_lexeme);
} // PParseGenericInteger

