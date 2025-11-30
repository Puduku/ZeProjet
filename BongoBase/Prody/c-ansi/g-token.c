// c-ansi/g-token.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/g-token.h"

// Assign actual acolyt token-id to 'g-token'
// 
// Passed
// - stuff: 
// - pcf_litteral : litteral 
// - tokenId: token id to assign
// - n_gTokensHandle: (when non NULL) "reference" tokens collection
// 
// Ret:
// - COMPLETED__OK: 
// - COMPLETED__BUT: copy instead of simple import 
// - -1: unexpected problem; anomaly is raised...
static inline int m_GTokenAssign(g_G_TOKEN_STUFF stuff, const char*pcf_litteral, int tokenId,
  g_G_TOKENS_HANDLE n_gTokensHandle) {
  m_DIGGY_BOLLARD()
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gTokensHandle,FIRST_ELEMENT0,
    VALUED_STRING__G_STRING_CONVEYANCE)
  int completed = g_GTokenImport(stuff, m_PString(pcf_litteral));
  switch (completed) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
  break; default: m_TRACK() } // switch

  stuff->acolyt.cen_value = tokenId;
  m_DIGGY_RETURN(RETURNED)
} // m_GTokenAssign


// Public function: see .h
int l_GTokensCreateInstance(g_G_TOKENS_HANDLE *azh_handle, int expectedItemsNumber) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(GStringsCreateInstance(azh_handle,expectedItemsNumber,1,
    VALUED_STRING__G_STRING_CONVEYANCE,(const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // l_GTokensCreateInstance


// Public function: see .h
int l_GTokensImport(g_G_TOKENS_HANDLE handle, const struct TOKEN_DEFINITION* spf_tokenDefinitions,
  int tokensNumber) {
  m_DIGGY_BOLLARD()
  int i = 0;
  g_G_TOKEN_STUFF gTokenStuff = (g_G_TOKEN_STUFF)UNDEFINED; 
  int entry = UNDEFINED;
  for (; i < tokensNumber; i++) {
    m_TRACK_IF((entry = g_GTokensFetch(handle,-1,&gTokenStuff)) < 0);
    switch (m_GTokenAssign(gTokenStuff,  spf_tokenDefinitions[i].p_litteral, spf_tokenDefinitions[i].tokenId, handle)) {
    case COMPLETED__OK: 
    break; case COMPLETED__BUT: 
      m_RAISE(ANOMALY__NON_PURE_LOGICAL_G_STRING)
    break; default: 
      m_TRACK()
    } // switch
  } // for 

  m_DIGGY_RETURN(RETURNED)
} // l_GTokensImport 

