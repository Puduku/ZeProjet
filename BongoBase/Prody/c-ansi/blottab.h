// c-ansi/blottab.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2024

#ifndef __C_ANSI_BLOTTAB_H_INCLUDED__
#define __C_ANSI_BLOTTAB_H_INCLUDED__

/////////////////////////////
#include "c-ansi/blottab.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/blotex.h"
#include "c-ansi/g-string.h"

// ===> GENUINE blottabs <===

struct BLOTTAB ; // the GENUINE blottab...
typedef struct BLOTTAB* BLOTTAB_HANDLE;


// #REF BLOTTAB_INDEX_FLAG
#define STR__BLOTTAB_INDEX_FLAG 0x01
#define INT__BLOTTAB_INDEX_FLAG 0x02

// Note: blot tables are based on g-string set collections. 
//
// Passed:
// - *azh_handle: "un-initialized" handle
//
// Changed
// - *azh_handle: initialized instance handle
//
// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexFlags);


// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int BlottabDestroyInstance(void *xhr_handle) ;

// Create NEW GENUINE blot table and add it in blotex executor's blot tables.
// 
// Passed:
// - handle:
// - blottabName: NON EXISTING blot table's name 
// - fieldsNumber:
// - s_names:
// - s_blottabIndexFlags: #SEE BLOTTAB_INDEX_FLAG
//
// Changed:
// - *a_blottabHandle: created blottab's handle 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int l_BlotexlibExecutorCreateGenuineBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottabName, int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexFlags,
  BLOTTAB_HANDLE *a_blottabHandle) ;


// #SEE l_BLOTEXLIB_EXECUTOR_COMPUTE_L_VALUE_BLOTTAB_OPS_FUNCTION@c-ansi/blotex.h
// => See also: BlotexlibExecutorFactoryCreateInstance() 
int l_BlotexlibExecutorComputeLValueGenuineBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle, 
  struct P_STRING *a_sequence, struct P_STRING blottabName, 
  struct BLOTTAB_FIELD_REFERENCE *ac_blottabFieldReference, G_STRING_STUFF nc_abandonmentInfo) ;

// #SEE l_BLOTEXLIB_EXECUTOR_COMPUTE_L_VALUE_BLOTTAB_OPS_FUNCTION@c-ansi/blotex.h
// => See also: BlotexlibExecutorFactoryCreateInstance() 
int l_BlotexlibExecutorComputeRValueGenuineBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blottabName, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) ;


// #SEE UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION@c-ansi/blotex.h
int UpdateBlottabCurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottabFieldReference,
 struct BLOTEX_VALUE blotexValue) ;


#endif // __BLOTTAB_C_ANSI_H_INCLUDED__

