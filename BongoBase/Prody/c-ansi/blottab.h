// blottab.h, version 1.92 (ANSI)
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


// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexTypes);


// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int BlottabDestroyInstance(void *xhr_handle) ;


// Retrieve some blot table.
// 
// Passed:
// - handle:
// - blottabName: blot table's name 
//
// Changed:
// - *ac_blottabHandle: (only significant when retrieved : RESULT__FOUND) 
//
// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int l_BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE *ac_blottabHandle) ;


enum { // #REF BLOTTAB_INDEX_TYPE
  NONE__BLOTTAB_INDEX_TYPE,
  STR__BLOTTAB_INDEX_TYPE,
  INT__BLOTTAB_INDEX_TYPE,
} ;


// Create blot table.
// Note: blot tables are based on g-string set collections. 
// 
// Passed:
// - handle:
// - blottabName: blot table's name 
// - fieldsNumber:
// - s_names:
// - s_blottabIndexTypes: #SEE BLOTTAB_INDEX_TYPE 
// - na_tableHandle: NULL special pointer: not used
// - nap_fieldsHandle: NULL special pointer: not used
//
// Changed:
// - *a_blottabHandle: 
//
// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the table already exists 
// - -1: unexpected problem; anomaly is raised
int l_BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexTypes,
  BLOTTAB_HANDLE *a_blottabHandle) ;

struct BLOTTAB_REFERENCE {
  int element;
  BLOTTAB_HANDLE blottabHandle; // BLOTTAB
} ;

// ==== SEE c-ansi/blotex.h
int l_BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTTAB_REFERENCE *cac_blottabReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) ;


int UpdateBlottabField(const struct BLOTTAB_REFERENCE* ap_blottabReference, int as,
 struct BLOTEX_VALUE blotexValue) ;


#endif // __BLOTTAB_C_ANSI_H_INCLUDED__
