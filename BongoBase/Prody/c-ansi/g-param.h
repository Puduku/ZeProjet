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

typedef G_STRINGS_HANDLE g_G_PARAMS_HANDLE;

typedef g_G_STRING_SET_STUFF g_G_PARAM_STUFF;
//typedef G_STRING_STUFF g_G_PARAM_NAME_STUFF;
//typedef G_STRING_STUFF g_G_PARAM_VALUE_STUFF;

#define GET_G_PARAM_NAME_STUFF(u_gParamStuff) ((u_gParamStuff)+G_PARAM_NAME_ELEMENT)
#define GET_G_PARAM_VALUE_STUFF(u_gParamStuff) ((u_gParamStuff)+G_PARAM_VALUE_ELEMENT)

// Assign actual token id to 'g-param's name'
// 
// Passed
// - stuff: 
// - tokenId: id to assign
// - n_gParamsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GParamAssignName(g_G_PARAM_STUFF stuff, int tokenId, 
  g_G_PARAMS_HANDLE n_gParamsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gParamsHandle,G_PARAM_NAME_ELEMENT,
    VALUED_STRING__G_STRING_CONVEYANCE)
  stuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value = tokenId;
  return RETURNED;
} // m_GParamAssignName

// Assign actual integer value to 'g-param's value'
// 
// Passed
// - stuff:
// - en_value: integer value to assign
// - n_gParamsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GParamAssignValue(g_G_PARAM_STUFF stuff, GENERIC_INTEGER en_value, 
  g_G_PARAMS_HANDLE n_gParamsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gParamsHandle,G_PARAM_VALUE_ELEMENT,
    VALUED_STRING__G_STRING_CONVEYANCE)
  stuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = en_value;
  return RETURNED;
} // m_GParamAssignValue


// #SEE GStringsCreateInstance@c-ansi/g-string.h  <named-object>
int l_GParamsCreateInstance(g_G_PARAMS_HANDLE *azh_handle,int expectedItemsNumber);



#endif // __C_ANSI_G_PARAM_H_INCLUDED
