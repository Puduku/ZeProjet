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
// TODO: name conveyance TOKEN__G_STRING_CONVEYANCE 

// Manipulate g-string as 'valued string'
// 
// Passed
// - stuff:
// - acolytValue:
// - n_gStringsHandle:
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsValuedString(G_STRING_STUFF stuff, GENERIC_INTEGER en_value, 
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,VALUED_STRING__G_STRING_CONVEYANCE)
  stuff->acolyt.cen_value = en_value;
  return RETURNED;
} // m_GStringAsValuedString


#endif // __C_ANSI_G_PARAM_H_INCLUDED
