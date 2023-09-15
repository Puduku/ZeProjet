// c-ansi/g-token.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI__G_TOKEN__H__INCLUDED
#define __C_ANSI__G_TOKEN__H__INCLUDED


#include "c-ansi/g-string.h"

//#include "c-ansi/testy-diggy.h"

// "g-token" == "g-string" with non-anonymous token id


struct TOKEN_DEFINITION {
  const char *p_litteral;
  int tokenId;
} ; 


// #REF NAMED_OBJECTS_CREATE_INSTANCE <named-object>
// #SEE G_STRINGS_CREATE_INSTANCE <named-object>
// Note: TOKEN__G_STRING_CONVEYANCE => token id acolyt 
#define /*int*/ G_TOKENS_CREATE_INSTANCE(/*G_STRINGS_HANDLE*/ azh_handle,\
  /*int*/ expectedItemsNumber)  GStringsCreateInstance(azh_handle, expectedItemsNumber, 1,\
  TOKEN__G_STRING_CONVEYANCE, (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED)



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
    switch (m_GStringCImport(gStringStuff, mp_tokenDefinitions[i].p_litteral)) {\
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
