// c-ansi/p-parse.c, version 1.93 (ANSI)
// (c) Puduku - 2023


#include "c-ansi/p-parse.h"
#include "c-ansi/p-string.h"

// Public function: see .h
int ParseSequence(struct P_STRING *a_sequence, const char* scanPtr,
  struct P_STRING* na_lexeme) {
  m_TRACK_IF(scanPtr == NULL) // TODO: ag virer
  if ((na_lexeme) != NULL) {
    na_lexeme->string = a_sequence->string;
    na_lexeme->stop = scanPtr;
  }\
  a_sequence->string = scanPtr ;

  return RETURNED;
} // ParseSequence


// Public function: see .h
int ParsePassSingleChar(struct P_STRING *a_sequence, IS_CHAR_FUNCTION n_isCharFunction, char c_char,
  struct P_STRING* na_lexeme) {
  const char *scanPtr = a_sequence->string;
  if (scanPtr < a_sequence->stop) {
    if (n_isCharFunction != NULL) {
      if (((IS_CHAR_FUNCTION)n_isCharFunction)(*scanPtr)) scanPtr++;
    } else {
      if (*scanPtr == (c_char)) scanPtr++;
    } // if
  } // if
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParsePassSingleChar

// Public function: see .h
int ParsePassChars(struct P_STRING* a_sequence, char b_regularScan, char b_passCharsTill,
  IS_CHAR_FUNCTION n_isCharFunction, char c_char, struct P_STRING* na_lexeme) {
  const char *scanPtr = ScanPString(*a_sequence,b_regularScan,b_passCharsTill, n_isCharFunction,
    c_char);
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParsePassChars


// Public function: see .h
int ParseMatch(struct P_STRING* a_sequence, const struct P_STRING p_token,
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
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseMatch


// Public function: see .h
int ParseMatchAmongR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* ac_matchedEntry, int* cnac_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) {
  const char *scanPtr = a_sequence->string;
  switch (ComparePStringsAmongR(*a_sequence,NULL,n_toCharFunction,b_SUB_STRING_2,
    ac_matchedEntry, cnac_matchedId, tokensCount, sp_tokens, nsn_ids)) {
  case LESS_THAN__COMPARISON :
  case GREATER_THAN__COMPARISON :
  break; case EQUAL_TO__COMPARISON :
    scanPtr += m_PStringLength((sp_tokens)[*(ac_matchedEntry)]);
  break; default:
    m_TRACK()
  } // switch 
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseMatchAmongR

// Public function: see .h
int ParseTillMatch(struct P_STRING* a_sequence, struct P_STRING p_token,
  TO_CHAR_FUNCTION n_toCharFunction, struct P_STRING*  na_lexeme) {
  const char *scanPtr = ScanPStringTillMatch(*a_sequence,p_token, n_toCharFunction);
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseTillMatch

// Public function: see .h
int ParseTillFirstMatchR(struct P_STRING* a_sequence, TO_CHAR_FUNCTION n_toCharFunction,
  int* nac_matchedEntry, int* cnac_matchedId, struct P_STRING* na_lexeme, int tokensCount,
  struct P_STRING*  sp_tokens, int*  nsn_ids) {
  const char *scanPtr = ScanPStringTillFirstMatchR(*a_sequence,n_toCharFunction,
    nac_matchedEntry,cnac_matchedId,tokensCount,sp_tokens,nsn_ids);
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseTillFirstMatchR


// Public function: see .h
int ParseOffset(struct P_STRING* a_sequence,  int offset, struct P_STRING* na_lexeme) {
  const char *scanPtr = a_sequence->string + offset;
  m_ASSERT(scanPtr >= a_sequence->string)
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseOffset


// Public function: see .h
int ParseGenericInteger(struct P_STRING* a_sequence, GENERIC_INTEGER* ac_value,
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
  return ParseSequence(a_sequence,scanPtr, na_lexeme);
} // ParseGenericInteger

