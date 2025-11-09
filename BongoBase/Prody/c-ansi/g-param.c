// c-ansi/g-param.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023


#include "c-ansi/g-param.h" 


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

