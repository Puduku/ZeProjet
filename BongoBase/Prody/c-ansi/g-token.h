// c-ansi/g-token.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI__G_TOKEN__H__INCLUDED
#define __C_ANSI__G_TOKEN__H__INCLUDED


#include "c-ansi/g-string.h"

//#include "c-ansi/testy-diggy.h"

// "g-token" == "g-string" with non-anonymous token id


typedef G_STRING_STUFF g_G_TOKEN_STUFF;
typedef G_STRINGS_HANDLE g_G_TOKENS_HANDLE;

// #SEE GStringImport @ c-ansi/g-string.h <g-token>
#define g_GTokenImport GStringImport


#define ANONYMOUS_TOKEN_ID0 0

struct TOKEN_DEFINITION {
  const char *p_litteral;
  int tokenId;
} ; 


// #SEE G_STRINGS_CREATE_INSTANCE <named-object>
// Note: TOKEN__G_STRING_CONVEYANCE => token id acolyt 
int l_GTokensCreateInstance(g_G_TOKENS_HANDLE *azh_handle, int expectedItemsNumber) ;

// #SEE GStringsFetch @ c-ansi/g-string.h <g-token>
#define g_GTokensFetch GStringsFetch

// #SEE GStringsAddIndex @ c-ansi/g-string.h <g-token> <key>
#define gm_GTokensAddPlainLexicalIndex m_GStringsAddPlainLexicalIndex

// Import g-tokens in one shot...
// Once you finished importing g-tokens, call g_GTokensFreeze()  
//
// Passed:
// - handle:
// - spf_tokenDefinitions:
// - expectedTokensNumber:
int l_GTokensImport(g_G_TOKENS_HANDLE handle, const struct TOKEN_DEFINITION* spf_tokenDefinitions,
  int tokensNumber) ;

// #SEE m_GStringsIndexSingleFetch @ c-ansi/g-string.h <g-token>
#define gm_GTokensIndexSingleFetch m_GStringsIndexSingleFetch

// #SEE GStringsFreeze @ c-ansi/g-string.h <g-token>
#define g_GTokensFreeze GStringsFreeze

// #SEE GStringsDestroyInstance @ c-ansi/g-string.h
#define g_GTokensDestroyInstance GStringsDestroyInstance

#endif // __C_ANSI__G_TOKEN__H__INCLUDED
