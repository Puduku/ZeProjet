// c-ansi/g-param.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_G_PARAM_H_INCLUDED
#define __C_ANSI_G_PARAM_H_INCLUDED

#include "c-ansi/g-string.h" 


#define G_PARAM_NAME_ELEMENT    0
#define G_PARAM_VALUE_ELEMENT   1
#define G_PARAM_CARDINALITY 2

// name AND value conveyance : VALUED_STRING__G_STRING_CONVEYANCE

const int ps_gParamGStringConveyances[G_PARAM_CARDINALITY] = { 
   TOKEN__G_STRING_CONVEYANCE, 
   VALUED_STRING__G_STRING_CONVEYANCE, 
} ; 

// #REF G_PARAMS_CREATE_INSTANCE <named-object>
// #SEE GStringsCreateInstance@c-ansi/g-string.h  <g-string>
#define /*int*/ G_PARAMS_CREATE_INSTANCE(/*G_STRINGS_HANDLE*/azh_handle,/*int*/expectedItemsNumber)\
  GStringsCreateInstance(azh_handle, expectedItemsNumber,G_PARAM_CARDINALITY,-1,\
  ps_gParamGStringConveyances,(NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED)

// Manipulate g-string as 'g-param's value'
// 
// Passed
// - stuff: g-string as g-param's value
// - en_value: integer value to assign
// - n_gStringsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsGParamValue(G_STRING_STUFF stuff, GENERIC_INTEGER en_value, 
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,G_PARAM_VALUE_ELEMENT,
   VALUED_STRING__G_STRING_CONVEYANCE)
  stuff->acolyt.cen_value = en_value;
  return RETURNED;
} // m_GStringAsGParamValue


// Manipulate g-string as 'g-param's name'
// 
// Passed
// - stuff: g-string as g-param's name 
// - tokenId: id to assign
// - n_gStringsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsGParamName(G_STRING_STUFF stuff, int tokenId, 
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,G_PARAM_NAME_ELEMENT,
   TOKEN__G_STRING_CONVEYANCE)
  stuff->acolyt.c_tokenId = tokenId;
  return RETURNED;
} // m_GStringAsGParamValue


#endif // __C_ANSI_G_PARAM_H_INCLUDED
