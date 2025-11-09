// c-ansi/g-token.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI__G_TOKEN__H__INCLUDED
#define __C_ANSI__G_TOKEN__H__INCLUDED


#include "c-ansi/g-string.h"

//#include "c-ansi/testy-diggy.h"

// "g-token" == "g-string" with non-anonymous token id

#define ANONYMOUS_TOKEN_ID0 0

struct TOKEN_DEFINITION {
  const char *p_litteral;
  int tokenId;
} ; 

typedef G_STRINGS_HANDLE g_G_TOKENS_HANDLE ;



// #SEE G_STRINGS_CREATE_INSTANCE <named-object>
// Note: TOKEN__G_STRING_CONVEYANCE => token id acolyt 
static inline int ml_GTokensCreateInstance(g_G_TOKENS_HANDLE *azh_handle, int expectedItemsNumber) {
  m_DIGGY_BOLLARD_S()
  m_TRACK_IF(GStringsCreateInstance(azh_handle,expectedItemsNumber,1,
    VALUED_STRING__G_STRING_CONVEYANCE,(const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // ml_GTokensCreateInstance


// Manipulate g-string as 'token'
// 
// Passed
// - stuff: stuff of (supposed 'token') g-string
// - tokenId: token id to assign
// - n_gStringsHandle: (when non NULL) "reference" tokens collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsToken(G_STRING_STUFF stuff, int tokenId, 
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,FIRST_ELEMENT0,
    VALUED_STRING__G_STRING_CONVEYANCE)
  stuff->acolyt.cen_value = tokenId;
  return RETURNED;
} // m_GStringAsToken



// Import g-tokens in one shot...
//
// Passed:
// - handle:
// - mp_tokenDefinitionsArray:
// - expectedTokensNumber:
#define m_G_TOKENS_IMPORT(/*G_STRINGS_HANDLE*/ handle,\
  /*const struct TOKEN_DEFINITION*/ mp_tokenDefinitions /*[]*/,  /*int*/ tokensNumber) {\
  int i = 0;\
  G_STRING_STUFF gStringStuff = (G_STRING_STUFF)UNDEFINED; \
  int entry = UNDEFINED;\
  for (; i < tokensNumber; i++) {\
    m_TRACK_IF((entry = GStringsFetch(handle,-1,&gStringStuff)) < 0);\
    switch (GStringImport(gStringStuff, m_PString(mp_tokenDefinitions[i].p_litteral))) {\
    case COMPLETED__OK: \
      m_TRACK_IF(m_GStringAsToken(gStringStuff, mp_tokenDefinitions[i].tokenId, handle) !=\
        RETURNED)\
    break; case COMPLETED__BUT: \
      m_RAISE(ANOMALY__NON_PURE_LOGICAL_G_STRING)\
    break; default: \
      m_TRACK()\
    }\
  } \
} 


#endif // __C_ANSI__G_TOKEN__H__INCLUDED
