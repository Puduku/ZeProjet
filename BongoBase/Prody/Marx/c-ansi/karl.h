// karl.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#ifndef __C_ANSI_KARL_H_INCLUDED__
#define __C_ANSI_KARL_H_INCLUDED__

/////////////////////////////
#include "c-ansi/karl.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/blotex.h"
#include "c-ansi/g-string.h"
#include "c-ansi/engels.h"

// ===> "super" blottabs (aka pamphlets) <===

typedef PAMPHLET_HANDLE g_BLOTTAB2_HANDLE;


// Reminder: "super" blottabs are nothing but PAMPHLETS. 
//
// Passed:
// - *azh_handle: "un-initialized" handle
// - f_engelsHandle: pamphlet models handler...
//
// Changed:
// - *azh_handle: initilized instance handle
//
// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
static inline int m_Blottab2CreateInstance(g_BLOTTAB2_HANDLE* azh_handle, ENGELS_HANDLE f_engelsHandle) {
  m_DIGGY_BOLLARD_S()
  m_TRACK_IF(PamphletCreateInstance(azh_handle,f_engelsHandle) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // m_Blottab2CreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int Blottab2DestroyInstance(void *xhr_handle) ;

// Create NEW "super" blot table and add it in blotex executor's "super" blot tables.
// 
// Passed:
// - handle:
// - blottab2Name: NON EXISTING "super" blot table's name 
// - dummy:
//
// Changed:
// - *a_blottab2Handle: created blottab2's handle 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
int l_BlotexlibExecutorCreateBlottab2(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottab2Name, int dummy, g_BLOTTAB2_HANDLE *a_blottab2Handle) ;


// #SEE l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION@c-ansi/blotex.h
int l_BlotexlibExecutorParseAndComputeLValueBlottab2SetOp(BLOTEXLIB_EXECUTOR_HANDLE handle, 
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottab2Name, 
  struct BLOTTAB_FIELD_REFERENCE *ac_blottab2FieldReference, G_STRING_STUFF nc_abandonmentInfo) ;

// #SEE l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION@c-ansi/blotex.h
int l_BlotexlibExecutorParseAndComputeRValueBlottab2Ops(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottab2Name,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) ;


// #SEE UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION@c-ansi/blotex.h
int UpdateBlottab2CurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottab2FieldReference,
 struct BLOTEX_VALUE blotexValue) ;




#endif // __C_ANSI_KARL_H_INCLUDED__



