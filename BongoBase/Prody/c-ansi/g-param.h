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


// #SEE GStringCopy # c-ansi/g-string.h <g-param-name>
static inline int m_GParamNameCopy(g_G_PARAM_STUFF stuff, int n_offset, struct P_STRING pString) {
  return GStringCopy((stuff)+G_PARAM_NAME_ELEMENT,n_offset, pString);
} // m_GParamNameCopy


// Assign actual acolyt token id to 'g-param's name'
// 
// Passed
// - stuff: 
// - *nap_name: NULL special pointer => NOT provided
// - tokenId: id to assign
// - n_gParamsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
int GParamNameAssign(g_G_PARAM_STUFF stuff, struct P_STRING *nap_pString, int tokenId,
  g_G_PARAMS_HANDLE n_gParamsHandle) ;

// Assign actual acolyt integer value to 'g-param's value'
// 
// Passed
// - stuff:
// - en_value: integer value to assign
// - n_gParamsHandle: (when non NULL) "reference" g-params collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
int GParamValueAssign(g_G_PARAM_STUFF stuff, GENERIC_INTEGER en_value, 
  g_G_PARAMS_HANDLE n_gParamsHandle) ;

// #SEE GStringsCreateInstance@c-ansi/g-string.h  <named-object>
int l_GParamsCreateInstance(g_G_PARAMS_HANDLE *azh_handle,int expectedItemsNumber);

// #SEE GStringsAddIndex @ c-ansi/g-string.h <g-param>
#define g_GParamsAddIndex GStringsAddIndex

// #SEE GStringsFetch @ c-ansi/g-string.h <g-param>
#define g_GParamsFetch GStringsFetch

// #SEE GStringsGetCount @ c-ansi/g-string.h <g-param>
#define g_GParamsGetCount GStringsGetCount  

// #SEE GStringsIndexFetch @ c-ansi/g-string.h <g-param>
#define g_GParamsIndexFetch GStringsIndexFetch

// #SEE GStringsIndexRequestR @ c-ansi/g-string.h <g-param>
#define g_GParamsIndexRequestR GStringsIndexRequestR

// #SEE m_GStringsIndexSingleFetch @ c-ansi/g-string.h <g-param> 
#define gm_GParamsIndexSingleFetch m_GStringsIndexSingleFetch 

// #SEE GStringsVerifyIndexes @ c-ansi/g-string.h 
#define g_GParamsVerifyIndexes GStringsVerifyIndexes

// #SEE GStringsDestroyInstance @ c-ansi/g-string.h <g-param>
#define g_GParamsDestroyInstance GStringsDestroyInstance


#endif // __C_ANSI_G_PARAM_H_INCLUDED
