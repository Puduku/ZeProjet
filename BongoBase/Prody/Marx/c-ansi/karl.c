// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/blotcode.h"
#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/named-object.h"
#include "c-ansi/engels.h"
#include "c-ansi/stderr.h"

#include "c-ansi/testy-diggy.h"

#include <limits.h>

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int Blottab2DestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD()
  xh_handle = (PAMPHLET_EXAMPLAR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTTAB_HANDLE,xh_handle)
  m_TRACK_IF(PamphletExamplarDestroyInstance(xh_handle) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // Blottab2DestroyInstance

// Public function; see .h
int l_BlotexlibExecutorCreateBlottab2(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottab2Name, int dummy, BLOTTAB2_HANDLE *a_blottab2Handle) {
  m_DIGGY_BOLLARD()
m_RAISE(ANOMALY__NOT_AVAILABLE)
} // l_BlotexlibExecutorCreateBlottab2


// Public function: see .h
int l_BlotexlibExecutorParseAndComputeLValueBlottab2Ops(
  BLOTEXLIB_EXECUTOR_HANDLE handle, char b_spot, struct P_STRING *a_sequence,
  struct P_STRING blottab2Name, struct BLOTTAB_FIELD_REFERENCE *ac_blottab2FieldReference,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_PREPARE_ABANDON(a_sequence,"<blottab2 ref op set int> | <blottab2 ref op set str>") \
  m_ABANDON(NOT_SUPPORTED__ABANDONMENT_CAUSE)
} // l_BlotexlibExecutorParseAndComputeLValueBlottab2Ops


// Parse and compute 'r-value' blottab2 OPERATIONS:
// expect <int blottab2 ops> | <str blottab2 ops>
int ml_BlotexlibExecutorParseAndComputeRValueBlottab2Ops(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottab2Name,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
m_RAISE(ANOMALY__NOT_AVAILABLE)
} // ml_BlotexlibExecutorParseAndComputeRValueBlottab2Ops


// Parse and compute 'r-value' blottab2 SPOT:
// expect <int blottab2 spot> | <str blottab2 spot>
int ml_BlotexlibExecutorParseAndComputeRValueBlottab2Spot(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottab2Name,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
m_RAISE(ANOMALY__NOT_AVAILABLE)
} // ml_BlotexlibExecutorParseAndComputeRValueBlottab2Spot

// Public function: see .h
int l_BlotexlibExecutorParseAndComputeRValueBlottab2Ops(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  int answer = UNDEFINED;
  if (b_spot) {
    switch (answer = ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottab2Spot(handle,a_sequence,
      blottabName,ac_blotexValue,nc_abandonmentInfo)) {   
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch 
  } else {
    switch (answer = ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottab2Ops(handle,a_sequence,
      blottabName,ac_blotexValue,nc_abandonmentInfo)) {   
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch 
  } // if

  m_DIGGY_RETURN(answer)
} // l_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps





// Public function; see .h
int UpdateBlottab2CurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottab2FieldReference,
  struct BLOTEX_VALUE blotexValue) {
  m_DIGGY_BOLLARD()
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // UpdateBlottab2CurrentBlotsetField


