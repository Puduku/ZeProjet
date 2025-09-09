// c-ansi/blottab.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2024

#ifndef __C_ANSI_BLOTTAB_H_INCLUDED__
#define __C_ANSI_BLOTTAB_H_INCLUDED__

/////////////////////////////
#include "c-ansi/blottab.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"

//#undef __C_ANSI_BLOTEX_H_INCLUDED__

#include "c-ansi/blotex.h"
#include "c-ansi/g-string.h"


// Blottabs: 

// #REF BLOTTAB_INDEX_FLAG
#define STR__BLOTTAB_INDEX_FLAG 0x01
#define INT__BLOTTAB_INDEX_FLAG 0x02

// Note: blot tables are based on g-string set collections. 
//
// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexFlags);


// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int BlottabDestroyInstance(void *xhr_handle) ;

// Create NEW blot table and add it in blotex executor's blot tables.
// 
// Passed:
// - handle:
// - blottabName: NON EXISTING blot table's name 
// - fieldsNumber:
// - s_names:
// - s_blottabIndexFlags: #SEE BLOTTAB_INDEX_FLAG
// - na_tableHandle: NULL special pointer: not used
// - nap_fieldsHandle: NULL special pointer: not used
//
// Changed:
// - *a_blottabHandle: created blottab's handle 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int l_BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexFlags,
  BLOTTAB_HANDLE *a_blottabHandle) ;

// #SEE l_BlotexlibExecutorComputeBlottabOps@c-ansi/blotex.h
int l_BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName, 
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTTAB_FIELD_REFERENCE *cac_blottabFieldReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) ;

// #SEE UpdateCurrentBlotsetField @c-ansi/blotex.h
int UpdateCurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottabFieldReference, int as,
 struct BLOTEX_VALUE blotexValue) ;


#endif // __BLOTTAB_C_ANSI_H_INCLUDED__
