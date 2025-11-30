// c-ansi/g-param.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023


#include "c-ansi/g-param.h" 

// Public function: see .h
int GParamNameAssign(g_G_PARAM_STUFF stuff, struct P_STRING *nap_pString, int tokenId, 
  g_G_PARAMS_HANDLE n_gParamsHandle) {
  m_DIGGY_BOLLARD()
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gParamsHandle,G_PARAM_NAME_ELEMENT,
    VALUED_STRING__G_STRING_CONVEYANCE)
  if (nap_pString != NULL) m_TRACK_IF(GStringCopy((stuff)+G_PARAM_NAME_ELEMENT,0, *nap_pString) < 0)
  stuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value = tokenId;
  m_DIGGY_RETURN(RETURNED);
} // GParamNameAssign

int GParamValueAssign(g_G_PARAM_STUFF stuff, GENERIC_INTEGER en_value,                
  g_G_PARAMS_HANDLE n_gParamsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gParamsHandle,G_PARAM_VALUE_ELEMENT,
    VALUED_STRING__G_STRING_CONVEYANCE)
  stuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = en_value;
  return RETURNED;
} // GParamValueAssign

// Public function: see .h
int l_GParamsCreateInstance(g_G_PARAMS_HANDLE *azh_handle,int expectedItemsNumber) {
  m_DIGGY_BOLLARD()
  static const int ps_gParamGStringConveyances[G_PARAM_CARDINALITY] = { 
     VALUED_STRING__G_STRING_CONVEYANCE, // "Token id." 
     VALUED_STRING__G_STRING_CONVEYANCE, 
  } ; 

  m_TRACK_IF(GStringsCreateInstance(azh_handle, expectedItemsNumber,G_PARAM_CARDINALITY,-1,
    ps_gParamGStringConveyances,(NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_DIGGY_RETURN(RETURNED);
} // l_GParamsCreateInstance

