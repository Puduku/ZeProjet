// c-ansi/g-token.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI__G_TOKEN__H__INCLUDED
#define __C_ANSI__G_TOKEN__H__INCLUDED


#include "c-ansi/g-string.h"

//#include "c-ansi/testy-diggy.h"

// "g-token" == "g-string" with non-anonymous token id


typedef G_STRING_STUFF g_G_TOKEN_STUFF;
typedef G_STRINGS_HANDLE g_G_TOKENS_HANDLE;


#define ANONYMOUS_TOKEN_ID0 0

// Assign actual acolyt token-id to 'g-token'
// 
// Passed
// - stuff: 
// - tokenId: token id to assign
// - n_gTokensHandle: (when non NULL) "reference" tokens collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GTokenAssign(g_G_TOKEN_STUFF stuff, int tokenId, 
  g_G_TOKENS_HANDLE n_gTokensHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gTokensHandle,FIRST_ELEMENT0,
    VALUED_STRING__G_STRING_CONVEYANCE)
  stuff->acolyt.cen_value = tokenId;
  return RETURNED;
} // m_GTokenAssign
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


// #SEE GStringImport @ c-ansi/g-string.h <g-token>
#define g_GTokenImport GStringImport


// Import g-tokens in one shot...
//
// Passed:
// - handle:
// - mp_tokenDefinitionsArray:
// - expectedTokensNumber:
#define m_G_TOKENS_IMPORT(/*g_G_TOKENS_HANDLE*/ handle,\
  /*const struct TOKEN_DEFINITION*/ mp_tokenDefinitions /*[]*/,  /*int*/ tokensNumber) {\
  int i = 0;\
  g_G_TOKEN_STUFF gTokenStuff = (g_G_TOKEN_STUFF)UNDEFINED; \
  int entry = UNDEFINED;\
  for (; i < tokensNumber; i++) {\
    m_TRACK_IF((entry = g_GTokensFetch(handle,-1,&gTokenStuff)) < 0);\
    switch (g_GTokenImport(gTokenStuff, m_PString(mp_tokenDefinitions[i].p_litteral))) {\
    case COMPLETED__OK: \
      m_TRACK_IF(m_GTokenAssign(gTokenStuff, mp_tokenDefinitions[i].tokenId, handle) !=\
        RETURNED)\
    break; case COMPLETED__BUT: \
      m_RAISE(ANOMALY__NON_PURE_LOGICAL_G_STRING)\
    break; default: \
      m_TRACK()\
    }\
  } \
} 

// #SEE GStringsAddIndex @ c-ansi/g-string.h <g-token> <key>
#define gm_GTokensAddPlainLexicalIndex m_GStringsAddPlainLexicalIndex


// #SEE m_GStringsIndexSingleFetch @ c-ansi/g-string.h <g-token>
#define gm_GTokensIndexSingleFetch m_GStringsIndexSingleFetch

// #SEE GStringsDestroyInstance @ c-ansi/g-string.h
#define g_GTokensDestroyInstance GStringsDestroyInstance

// #SEE GStringsFreeze @ c-ansi/g-string.h <g-token>
#define g_GTokensFreeze GStringsFreeze

#endif // __C_ANSI__G_TOKEN__H__INCLUDED
