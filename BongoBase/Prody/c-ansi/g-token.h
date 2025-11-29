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

// Assign actual acolyt token-id to 'g-token'
// 
// Passed
// - stuff: 
// - tokenId: token id to assign
// - n_gTokensHandle: (when non NULL) "reference" tokens collection
// 
// Ret:
// - COMPLETED__OK: 
// - COMPLETED__BUT: copy instead of simple import 
// - -1: unexpected problem; anomaly is raised...
int GTokenAssign(g_G_TOKEN_STUFF stuff, const char*p_litteral, int tokenId, 
  g_G_TOKENS_HANDLE n_gTokensHandle) ;


struct TOKEN_DEFINITION {
  const char *p_litteral;
  int tokenId;
} ; 


// #SEE G_STRINGS_CREATE_INSTANCE <named-object>
// Note: TOKEN__G_STRING_CONVEYANCE => token id acolyt 
static inline int ml_GTokensCreateInstance(g_G_TOKENS_HANDLE *azh_handle, int expectedItemsNumber) {
  m_DIGGY_BOLLARD_S()
  m_TRACK_IF(GStringsCreateInstance(azh_handle,expectedItemsNumber,1,
    VALUED_STRING__G_STRING_CONVEYANCE,(const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // ml_GTokensCreateInstance


// #SEE GStringsFetch @ c-ansi/g-string.h <g-token>
#define g_GTokensFetch GStringsFetch



//#define m_G_TOKENS_IMPORT(/*g_G_TOKENS_HANDLE*/ handle,\
// Import g-tokens in one shot...
//
// Passed:
// - handle:
// - mp_tokenDefinitionsArray:
// - expectedTokensNumber:
int l_GTokensImport(g_G_TOKENS_HANDLE handle, const struct TOKEN_DEFINITION* sp_tokenDefinitions,
  int tokensNumber) ;

// #SEE GStringsAddIndex @ c-ansi/g-string.h <g-token> <key>
#define gm_GTokensAddPlainLexicalIndex m_GStringsAddPlainLexicalIndex

// #SEE m_GStringsIndexSingleFetch @ c-ansi/g-string.h <g-token>
#define gm_GTokensIndexSingleFetch m_GStringsIndexSingleFetch

// #SEE GStringsDestroyInstance @ c-ansi/g-string.h
#define g_GTokensDestroyInstance GStringsDestroyInstance

// #SEE GStringsFreeze @ c-ansi/g-string.h <g-token>
#define g_GTokensFreeze GStringsFreeze

#endif // __C_ANSI__G_TOKEN__H__INCLUDED
