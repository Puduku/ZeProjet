// c-ansi/g-token.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/g-token.h"

// Public function: see .h
int GTokenAssign(g_G_TOKEN_STUFF stuff, const char*p_litteral, int tokenId,                 
  g_G_TOKENS_HANDLE n_gTokensHandle) {
  m_DIGGY_BOLLARD()
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gTokensHandle,FIRST_ELEMENT0,
    VALUED_STRING__G_STRING_CONVEYANCE)
  int completed = g_GTokenImport(stuff, m_PString(p_litteral));
  switch (completed) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
  break; default: m_TRACK() } // switch

  stuff->acolyt.cen_value = tokenId;
  m_DIGGY_RETURN(RETURNED)
} // GTokenAssign


// Public function: see .h
int l_GTokensImport(g_G_TOKENS_HANDLE handle, const struct TOKEN_DEFINITION* sp_tokenDefinitions,
  int tokensNumber) {
  m_DIGGY_BOLLARD()
  int i = 0;
  g_G_TOKEN_STUFF gTokenStuff = (g_G_TOKEN_STUFF)UNDEFINED; 
  int entry = UNDEFINED;
  for (; i < tokensNumber; i++) {
    m_TRACK_IF((entry = g_GTokensFetch(handle,-1,&gTokenStuff)) < 0);
    switch (GTokenAssign(gTokenStuff,  sp_tokenDefinitions[i].p_litteral, sp_tokenDefinitions[i].tokenId, handle)) {
    case COMPLETED__OK: 
    break; case COMPLETED__BUT: 
      m_RAISE(ANOMALY__NON_PURE_LOGICAL_G_STRING)
    break; default: 
      m_TRACK()
    } // switch
  } // for 

  m_DIGGY_RETURN(RETURNED)
} // l_GTokensImport 

