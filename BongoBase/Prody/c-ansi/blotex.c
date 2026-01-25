// blotex.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

#include "c-ansi/blotcode.h"
#include "c-ansi/blotex.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/named-object.h"
#include "c-ansi/stderr.h"

#include "c-ansi/testy-diggy.h"

#include <limits.h>

// blotex library executor factory: 
// ================================

struct BLOTTAB_IMPLEMENTATION {
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
  UPDATE_BLOTTAB_SPOT_FUNCTION updateBlottabSpotFunction;
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction ;
}; 

struct BLOTEXLIB_EXECUTOR_FACTORY {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)
  int blottabImplementationsNumber; 
  struct BLOTTAB_IMPLEMENTATION blottabImplementations[
    MAX_BLOTTAB_IMPLEMENTIONS_NUMBER]; 
}; 

// Public function; see .h
int BlotexlibExecutorFactoryRegisterBlottabImplementation(
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE handle,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction,
  UPDATE_BLOTTAB_SPOT_FUNCTION updateBlottabSpotFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()
  int i = handle->blottabImplementationsNumber++; 
  m_ASSERT(i < MAX_BLOTTAB_IMPLEMENTIONS_NUMBER)
  handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction =
    l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
  handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction =
    l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
  handle->blottabImplementations[i].updateBlottabSpotFunction =
    updateBlottabSpotFunction ;
  handle->blottabImplementations[i].blottabDestroyInstanceFunction =
  blottabDestroyInstanceFunction;

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryRegisterBlottabImplementation
 
// Public function; see .h
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueGenuineBlottabSetOpFunction,
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueGenuineBlottabOpsFunction,
  UPDATE_BLOTTAB_SPOT_FUNCTION updateGenuineBlottabSpotFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION genuineBlottabDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,*azh_handle)
  (*azh_handle)->blottabImplementationsNumber = 0; 
  m_ASSERT(BlotexlibExecutorFactoryRegisterBlottabImplementation(*azh_handle,
    l_blotexlibExecutorParseAndComputeLValueGenuineBlottabSetOpFunction,
    l_blotexlibExecutorParseAndComputeRValueGenuineBlottabOpsFunction,
    updateGenuineBlottabSpotFunction, genuineBlottabDestroyInstanceFunction) ==
    GENUINE_BLOTTABS_LABEL0) 
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateInstance

// Public function; see .h
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryDestroyInstance

struct BLOTTAB_EXECUTOR_IMPLEMENTATION {
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
  UPDATE_BLOTTAB_SPOT_FUNCTION updateBlottabSpotFunction;
  NAMED_OBJECTS_HANDLE h_blottabsHandle ; 
} ;

struct BLOTEXLIB_EXECUTOR {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE)
  NAMED_OBJECTS_HANDLE h_blotregsHandle ; 
  G_STRINGS_HANDLE h_workingGStringsHandle ;
  int blottabExecutorImplementationsNumber; 
  struct BLOTTAB_EXECUTOR_IMPLEMENTATION blottabExecutorImplementations[
    MAX_BLOTTAB_IMPLEMENTIONS_NUMBER]; 
} ;  

// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION@c-ansi/blotcode.h <blotex>
static int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()

  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE p_handle = (BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)pr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,p_handle)

  m_MALLOC_INSTANCE(*(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)
  BLOTEXLIB_EXECUTOR_HANDLE productHandle = *azhr_blotlibExecutorHandle;

  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,productHandle)

  m_TRACK_IF(NamedObjectsCreateInstance(&(productHandle->h_blotregsHandle), 10,
    BlotregCreateInstance,BlotregDestroyInstance) != RETURNED)

  productHandle->blottabExecutorImplementationsNumber = p_handle->blottabImplementationsNumber;
  int i = 0; for (; i < p_handle->blottabImplementationsNumber; i++) { 
    m_TRACK_IF(NamedObjectsCreateInstance(
      &(productHandle->blottabExecutorImplementations[i].h_blottabsHandle), 10,
     (NAMED_OBJECT_CREATE_INSTANCE_FUNCTION)NULL,
     p_handle->blottabImplementations[i].blottabDestroyInstanceFunction) != RETURNED)
    productHandle->blottabExecutorImplementations[i].
      l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction =
      p_handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
    productHandle->blottabExecutorImplementations[i].
      l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction =
      p_handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
    productHandle->blottabExecutorImplementations[i].updateBlottabSpotFunction =
      p_handle->blottabImplementations[i].updateBlottabSpotFunction;
  } // for

  m_TRACK_IF(G_STRINGS_CREATE_INSTANCE(&productHandle->h_workingGStringsHandle,5) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateProductInstance


// Interface for other blotlib executors:
// ======================================

// Public function; see .h
int l_BlotcodeExecutorGetBlotexlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int blotexlibEntry,
  BLOTEXLIB_EXECUTOR_HANDLE *a_blotexlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  void *r_blotexlibExecutorHandle = (void *)UNDEFINED;
  int result = BlotcodeExecutorGetBlotlibExecutorHandle(handle,blotexlibEntry,
    &r_blotexlibExecutorHandle);
  switch (result) {
  case RESULT__FOUND:
    *a_blotexlibExecutorHandle = (BLOTEXLIB_EXECUTOR_HANDLE)r_blotexlibExecutorHandle;
    m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,*a_blotexlibExecutorHandle)
  break; case RESULT__NOT_FOUND:
    m_RAISE(ANOMALY__VALUE__D,result)
  break; default:
    m_TRACK()
  } // switch 

  m_DIGGY_RETURN(RETURNED)
} // l_BlotcodeExecutorGetBlotexlibExecutorHandle

#if 0
static int DumpBlotregs(G_STRINGS_HANDLE blotregsHandle, const char *p_legend) {
m_DIGGY_BOLLARD() ////////////////////////// DEBUG TRACES:
static struct G_STRING *na_gStrings = (struct G_STRING*)UNDEFINED;
m_TRACK_IF(GStringsFetch(blotregsHandle,0, &na_gStrings) < 0)
int count = GStringsGetCount(blotregsHandle,NULL);
m_DIGGY_INFO("ALL Blotregs %s",p_legend)
m_TRACK_IF(count < 0)
m_DIGGY_VAR_D(count)
int i=0; for (; i < count ; i++){
 m_DIGGY_VAR_D(i)
 m_DIGGY_VAR_P_STRING(na_gStrings[i].cv_pString)
 m_DIGGY_VAR_P(na_gStrings[i].acolyt.cnhr_handle)
 m_DIGGY_VAR_P(&(na_gStrings[i].cv_pString))
}
m_DIGGY_RETURN(RETURNED)

} // DumpBlotregs
#endif

// Public function; see .h
int BlotexlibExecutorGetBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blotregName, g_BLOTREG_HANDLE *ac_blotregHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blotregName)
//DumpBlotregs(handle->h_blotregsHandle,"GET blotreg BEFORE SEARCH");

  int result = NamedObjectsGetNamedObject(handle->h_blotregsHandle, blotregName,
    (void **)ac_blotregHandle); 
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(*ac_blotregHandle != NULL)
switch(g_BlotregVerifyIndexes(*ac_blotregHandle)) {
case COMPLETED__OK:
break; case COMPLETED__BUT:
  m_RAISE(ANOMALY__CORRUPTED_INDEXES)
break; default:
  m_TRACK()
} // switch
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch
//DumpBlotregs(handle->h_blotregsHandle."GET blotreg AFTER SEARCH (MUST BE SAME AS BEFORE SEARCH")

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlotreg

// Public function; see .h
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blotregName, g_BLOTREG_HANDLE *na_blotregHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blotregName)
  int completed = UNDEFINED;
  g_NAMED_OBJECT_STUFF t_namedBlotregStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED; 

  switch (completed = NamedObjectsAddNamedObject(handle->h_blotregsHandle,blotregName,
    (void*)NULL, &t_namedBlotregStuff,UNDEFINED/*TODO: ag virer ?*/)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
  break; default: m_TRACK() } // switch 

  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;
  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg


// Interface for blottab implementations:
// ======================================

// Blottabs:

// Public function; see .h
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabsLabel,
  struct P_STRING blottabName, void* *acr_blottabHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(blottabsLabel < handle->blottabExecutorImplementationsNumber)

  int result = NamedObjectsGetNamedObject(
    handle->blottabExecutorImplementations[blottabsLabel].h_blottabsHandle,blottabName,
    acr_blottabHandle);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(*acr_blottabHandle != NULL);
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlottab

// Public function; see .h
int BlotexlibExecutorAddBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabsLabel,
  struct P_STRING blottabName, void* hr_blottabHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(blottabsLabel < handle->blottabExecutorImplementationsNumber)
m_DIGGY_VAR_P_STRING(blottabName)
  
  NAMED_OBJECTS_HANDLE blottabsHandle = 
    handle->blottabExecutorImplementations[blottabsLabel].h_blottabsHandle;
m_DIGGY_VAR_P(blottabsHandle)
  g_NAMED_OBJECT_STUFF t_namedBlottabStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED;
  switch (NamedObjectsAddNamedObject(blottabsHandle,blottabName,hr_blottabHandle,
    &t_namedBlottabStuff,UNDEFINED/*TODO: ag virer???*/)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorAddBlottab


// Blot expressions parsing:

// Public function: see .h
int BlotexlibExecutorSetBlotexValue(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct BLOTEX_VALUE *a_blotexValue,  int asValue, gen_BLOTVAL c_blotval,
  const struct P_STRING* cap_str, char cb_fugaciousStr) {
  m_DIGGY_BOLLARD()

  m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,a_blotexValue,asValue, c_blotval, cap_str,
    cb_fugaciousStr) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorSetBlotexValue

// Public function: see .h
int BlotexlibExecutorConcatenateStrexValue(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct BLOTEX_VALUE *a_strexValue1, struct P_STRING p_str2) {
  m_DIGGY_BOLLARD()

  m_TRACK_IF(ConcatenateStrexValue(handle->h_workingGStringsHandle,a_strexValue1,p_str2) !=
    RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorConcatenateStrexValue

// Parse and compute blotreg request. 
//
// Passed:
// - handle:
// - *a_sequence: expect <blotreg request>  
// - blotregHandle: register for request 
//
// Changed:
// - *a_sequence: after parsing 
// - blotregHandle: request initiated in register  
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorParseAndComputeBlotregRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, g_BLOTREG_HANDLE blotregHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERION criteria5[5] ;  
  struct P_STRING subSequence; 

  m_CHECK_ABANDON(DelimitBlotregRequest(a_sequence,&subSequence,nc_abandonmentInfo))
  struct BLOTEX_VALUE blotexValue = UNDEFINED_BLOTEX_VALUE ; 
  struct G_KEY gKey = {UNDEFINED};
  do {
    int as = UNDEFINED;  
    int blotregIndexLabel = UNDEFINED; 
    int indexSeekFlags = UNDEFINED;
    
    m_CHECK_ABANDON(ParseBlotregRequestAtom(&subSequence,&as,&indexSeekFlags,nc_abandonmentInfo))

    if (indexSeekFlags != INDEX_SEEK_FLAGS__ANY) m_CHECK_ABANDON(
      BlotexlibExecutorParseAndComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo))

m_DIGGY_VAR_INDEX_SEEK_FLAGS(indexSeekFlags)
    m_CHECK_ABANDON(AsBlotregIndex(&subSequence,as,indexSeekFlags != INDEX_SEEK_FLAGS__ANY?
      &blotexValue: NULL,&blotregIndexLabel,&gKey, nc_abandonmentInfo))
    
    int criteriaOpFlags = UNDEFINED;
    m_TRACK_IF(ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterion_GKeys(blotregIndexLabel,
      indexSeekFlags,&gKey, criteriaOpFlags);
    m_PParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch (g_BlotregIndexRequestR(blotregHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeBlotregRequest


// Parse and compute r-value blotreg operations:
// expect <int blotreg ops> | <str blotreg ops>
//
// Passed:
// - handle: 
// - *a_sequence: before parsing
// - blotregName: register name
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blotexValue: only significant if "success" ; value corresponding to blotreg ops
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorParseAndComputeRValueBlotregOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blotregName, g_BLOTREG_HANDLE n_blotregHandle,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<int blotreg ops> | <str blotreg ops>") 

  if (n_blotregHandle == NULL) m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 

  m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexValue, AS__VALUE_INT,TRUE__BLOTVAL0,
    (struct P_STRING*)UNDEFINED,(char)UNDEFINED) != RETURNED) // actually UNDEFINED 
  int n_indexFetchFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);

  PParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op select>...
    m_CHECK_ABANDON(m_BlotexlibExecutorParseAndComputeBlotregRequest(handle,a_sequence,n_blotregHandle,
      nc_abandonmentInfo)) 
  } // if
  PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op reset>...
    n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
  } // if
  PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op next>...
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
    m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
  } // if

  int n_as = -1; // No blotreg read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op read int> | <blotreg op read str> (R-value)
    // <blotreg ref op set int> | <blotreg ref op set str> (L-value)... 
    m_TRACK_IF(ParseAs(!b_L_VALUE,a_sequence,&n_as) != RETURNED)
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (n_indexFetchFlags < 0) {
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexValue, AS__VALUE_INT,TRUE__BLOTVAL0,
      (struct P_STRING*)UNDEFINED,(char)UNDEFINED) != RETURNED)
  } else {
    g_BLOTVAR_STUFF nt_blotvarStuff = (g_BLOTVAR_STUFF)UNDEFINED;
    int c_entry = UNDEFINED;

m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags) 
    switch (g_BlotregIndexFetch(n_blotregHandle,NULL,n_indexFetchFlags,
      &nt_blotvarStuff, &c_entry)) {
    case RESULT__NOT_FOUND:
m_ASSERT(nt_blotvarStuff == NULL)
    case RESULT__FOUND:
      m_TRACK_IF(BlotvarReadOpAsBlotexValue(nt_blotvarStuff,n_as,c_entry,
        handle->h_workingGStringsHandle, ac_blotexValue) != RETURNED)
    break; default: m_TRACK()
    } // switch
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorParseAndComputeRValueBlotregOps

// Complete blotvar reference parsing and compute blotex atom value.
//
// Passed:
// - handle:
// - *a_sequence: after parsing of register name <entity> 
// - blotregName: register name
//
// Changed:
// - *a_sequence:   
// - ac_blotexAtomValue: not significant in case of abandon; value corresponding to blotvar
//   reference
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorParseAndComputeBlotexAtomBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blotregName, g_BLOTREG_HANDLE n_blotregHandle,
  struct BLOTEX_VALUE *ac_blotexAtomValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  struct BLOTVAR_REFERENCE c_blotvarReference = UNDEFINED_BLOTVAR_REFERENCE ; 
  g_BLOTVAR_STUFF nt_blotvarStuff = (g_BLOTVAR_STUFF)UNDEFINED;
  int vn_entry = UNDEFINED; 
  m_PREPARE_ABANDON(a_sequence,
    "<blotvar as int> | <blotvar entry> | <blotvar id> | <blotvar as str> | <blotvar name>") 

  if (n_blotregHandle == NULL) m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE)
  m_CHECK_ABANDON(ParseAndComputeSimpleBlotvarReference(a_sequence, n_blotregHandle, &c_blotvarReference,
    nc_abandonmentInfo))
  m_TRACK_IF(FetchBlotvar(&c_blotvarReference, b_R_VALUE,&nt_blotvarStuff, &vn_entry) != RETURNED)
  if (nt_blotvarStuff == NULL) m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE) 
  m_PParsePassSpaces(a_sequence,NULL); // TODO: ag virer ??????

  int as = UNDEFINED;
  m_TRACK_IF(ParseAs(b_R_VALUE,a_sequence,&as) != RETURNED)
  switch (as) {
  case AS__VALUE_INT: // <blotvar as int>
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexAtomValue,AS__VALUE_INT,
      nt_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value,(const struct P_STRING*) UNDEFINED,
      (char)UNDEFINED) != RETURNED)
  break; case AS__ID: // <blotvar id>
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexAtomValue,AS__VALUE_INT,
      nt_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].acolyt.cen_value,(const struct P_STRING*) UNDEFINED,
      (char)UNDEFINED) != RETURNED)
  break; case AS__ENTRY: // <blotvar entry> (r-value) 
    m_ASSERT(vn_entry >= 0)
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexAtomValue,AS__VALUE_INT,vn_entry,
      (const struct P_STRING*) UNDEFINED, (char)UNDEFINED) != RETURNED)
  break; case AS__VALUE_STR: // <blotvar strex> 
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexAtomValue,AS__VALUE_STR,UNDEFINED,
      &nt_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].cv_pString,b_FUGACIOUS_STR) !=
      RETURNED) // TODO: really FUGACIOUS????
  break; case AS__NAME:  // <blotvar name> 
    m_TRACK_IF(SetBlotexValue(handle->h_workingGStringsHandle,ac_blotexAtomValue,AS__VALUE_STR,UNDEFINED,
      &nt_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].cv_pString,b_FUGACIOUS_STR) !=
      RETURNED) // TODO: really FUGACIOUS????
  break; default:
    m_RAISE(ANOMALY__VALUE__D,as)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeBlotexAtomBlotvar

// Parse blotreg or blottab/blottab2/blottab3... name and retrieve corresponding
// handle if exists... 
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_blottabsLabel: + >= 0: blottabX GENUINE_BLOTTABS_LABAEL0,...
//   + -1 special value : blotreg
// - *a_name:
// - *acnr_blottabHandle: (when blottabX) NULL is not existing
// - *acn_blotregHandle: (when blotreg) NULL is not existing
//
// Ret: 
// - RETURNED: Ok,
// - -1: unexpected problem
static int BlotexlibExecutorParseAndComputeBlotregOrBlottabXName(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *an_blottabsLabel, struct P_STRING *a_name,
  void ** acnr_blottabHandle, g_BLOTREG_HANDLE *acn_blotregHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P(a_name)

  ParseBlottabsLabel(a_sequence, an_blottabsLabel); 
  // Parse <entity> corresponding to blottab name:
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,a_name); 
m_DIGGY_VAR_P_STRING(*a_name)
m_DIGGY_VAR_D(*an_blottabsLabel)
  if (*an_blottabsLabel >= 0) {
    switch (BlotexlibExecutorGetBlottab(handle,*an_blottabsLabel,*a_name,
      acnr_blottabHandle)) {
    case RESULT__FOUND:
      m_ASSERT(*acnr_blottabHandle != NULL)
    break; case RESULT__NOT_FOUND:
      *acnr_blottabHandle = NULL;
    break; default:
      m_TRACK()
    } // switch
  } else {
    switch (BlotexlibExecutorGetBlotreg(handle,*a_name,
      acn_blotregHandle)) {
    case RESULT__FOUND:
      m_ASSERT(*acn_blotregHandle != NULL)
    break; case RESULT__NOT_FOUND:
      *acn_blotregHandle = NULL;
    break; default:
      m_TRACK()
    } // switch
  } // if

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorParseAndComputeBlotregOrBlottabXName

// Probe "blotex atom" : expect either <intex atom> or <strex atom>...
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexAtomValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorProbeBlotexAtom(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexAtomValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  struct P_STRING lexeme = UNDEFINED_P_STRING;

  m_PREPARE_ABANDON(a_sequence, "<intex atom> | <strex atom>") 
  m_PParsePassSpaces(a_sequence,NULL);
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 

  int n_int1Op = -1; // a priori
  m_TRACK_IF(ParseInt1Op(a_sequence, &n_int1Op) != RETURNED) 

m_DIGGY_VAR_P_STRING(*a_sequence)
  char b_intConstant = (char)UNDEFINED;
  m_TRACK_IF(ParseIntConstant(a_sequence, handle->h_workingGStringsHandle,
    &b_intConstant, ac_blotexAtomValue) != RETURNED)
  if (!b_intConstant) {
    char cb_strConstant = (char)UNDEFINED;
    m_CHECK_ABANDON(ParseStrConstant(a_sequence,handle->h_workingGStringsHandle, &cb_strConstant,
      ac_blotexAtomValue,nc_abandonmentInfo))
    if (!cb_strConstant) {
      PParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { 
        m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeBlotex(handle,a_sequence,ac_blotexAtomValue,
          nc_abandonmentInfo))
        PParsePassSingleChar(a_sequence,NULL,')',&lexeme);
        if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
        // '(' <blotex> ')'
      } else { // 
        struct P_STRING name = UNDEFINED_P_STRING;
        int n_blottabsLabel = UNDEFINED;
        void* cnr_blottabHandle = (void*)UNDEFINED;
        g_BLOTREG_HANDLE cn_blotregHandle = (void*)UNDEFINED;
        m_TRACK_IF(BlotexlibExecutorParseAndComputeBlotregOrBlottabXName(handle,a_sequence, &n_blottabsLabel,
          &name, &cnr_blottabHandle,&cn_blotregHandle) != RETURNED) 
m_DIGGY_VAR_P(cn_blotregHandle)
        PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
        if (!b_EMPTY_P_STRING(lexeme)) { 
          if (n_blottabsLabel >= 0) { // <int blottabX ops> | <str blottabX ops> ...
            m_CHECK_ABANDON(handle->blottabExecutorImplementations[n_blottabsLabel].
              l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction(handle,b_OPS,a_sequence,
              n_blottabsLabel,name,cnr_blottabHandle,ac_blotexAtomValue,nc_abandonmentInfo))
          } else { // <int blotreg> | <str blotreg> ...
            m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeRValueBlotregOps(handle,a_sequence,name,
              cn_blotregHandle, ac_blotexAtomValue, nc_abandonmentInfo))
          } // if
        } else {  
          if (n_blottabsLabel >= 0) { 
            // <int blottabX spot> | <str blottabX spot> ... 
            m_CHECK_ABANDON(handle->blottabExecutorImplementations[n_blottabsLabel].
              l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction(handle,b_SPOT,a_sequence,
              n_blottabsLabel,name,cnr_blottabHandle, ac_blotexAtomValue,nc_abandonmentInfo))
          } else { 
m_DIGGY_VAR_P(cn_blotregHandle)
            m_CHECK_ABANDON(m_BlotexlibExecutorParseAndComputeBlotexAtomBlotvar(handle,a_sequence,name,
              cn_blotregHandle,ac_blotexAtomValue, nc_abandonmentInfo))
            // ( <blotvar as int> | <blotvar as str> | <blotvar name> ) 
          } // if
        } // if
      } // if 
    } // if 
  } // if 

  if (n_int1Op >= 0) {
    if (ac_blotexAtomValue->asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    switch (n_int1Op) {
    break; case NOT__INT_1OP:
      if (ac_blotexAtomValue->select.c_blotval == TRUE__BLOTVAL0)
        ac_blotexAtomValue->select.c_blotval = FALSE__BLOTVAL;
      else ac_blotexAtomValue->select.c_blotval = TRUE__BLOTVAL0;
    break; case PLUS__INT_1OP:
    break; case MINUS__INT_1OP:
      ac_blotexAtomValue->select.c_blotval = -(ac_blotexAtomValue->select.c_blotval);
    break; default:
      m_RAISE(ANOMALY__VALUE__D,n_int1Op)
    } // switch
  } // if 

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorProbeBlotexAtom

// Parse <intex term>  
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - b_initialIntexAtomValue: TRUE => initial intex term value is significant and
//   corresponds to passed intex atom value
// - *a_intexTermValue: initial intex term value when significant
//
// Changed:
// - *a_sequence: after parsing 
// - a_intexTermValue: only updated if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorParseAndComputeIntexTerm(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, char b_initialIntexAtomValue, struct BLOTEX_VALUE *a_intexTermValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  m_ASSERT((!b_initialIntexAtomValue) || (a_intexTermValue->asValue == AS__VALUE_INT))
  m_PREPARE_ABANDON(a_sequence, "<intex term>") 
  m_PParsePassSpaces(a_sequence,NULL);
  int n_compOp = UNDEFINED;

  if (!b_initialIntexAtomValue) {
    m_CHECK_ABANDON(BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,a_intexTermValue,
      nc_abandonmentInfo))
    if (a_intexTermValue->asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
  } // if

  struct BLOTEX_VALUE intexAtomValue ;
  while (b_TRUE) {
    m_TRACK_IF(ParseCompOp(FACT_OP__COMP_OP_EXTENSION,a_sequence,
      &n_compOp) != RETURNED)
    if (n_compOp == -1) break;
    m_CHECK_ABANDON(BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&intexAtomValue,
      nc_abandonmentInfo));
    if (intexAtomValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    switch (n_compOp) {
    case EQUAL__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval ==
        intexAtomValue.select.c_blotval);
    break; case LESS__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval < 
        intexAtomValue.select.c_blotval);
    break; case LESS_EQUAL__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval <=
        intexAtomValue.select.c_blotval);
    break; case GREATER__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval > 
        intexAtomValue.select.c_blotval);
    break; case GREATER_EQUAL__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval >=
        intexAtomValue.select.c_blotval);
    break; case NOT_EQUAL__COMP_OP:
      a_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(a_intexTermValue->select.c_blotval !=
        intexAtomValue.select.c_blotval);
    break; case MULTIPLY__FACT_OP__COMP_OP:
      a_intexTermValue->select.c_blotval = a_intexTermValue->select.c_blotval *
        intexAtomValue.select.c_blotval;
    break; case DIVIDE__FACT_OP__COMP_OP:
      a_intexTermValue->select.c_blotval = a_intexTermValue->select.c_blotval /
        intexAtomValue.select.c_blotval;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,n_compOp);
    } // switch
  } // while

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorParseAndComputeIntexTerm

// Compute full intex ; that is
// Parse rest of <intex> after (already parsed) 1st <intex atom> and if such rest exist compute it.
// Otherwise, full intex simply corresponds to intex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - *a_intexValue: 1st intex atom value
//
// Changed:
// - *a_sequence: after parsing 
// - a_intexValue: only updated if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex> NOT intex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorParseAndComputeFullIntex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *a_intexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_intexValue->asValue == AS__VALUE_INT)
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_termOp = UNDEFINED;

  m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeIntexTerm(handle,a_sequence,b_TRUE, a_intexValue,
    nc_abandonmentInfo))
  m_ASSERT(a_intexValue->asValue == AS__VALUE_INT)
    
  struct BLOTEX_VALUE intexTermValue = UNDEFINED_BLOTEX_VALUE ;
  while (b_TRUE) {
    m_TRACK_IF(ParseTermOp(a_sequence,&n_termOp) != RETURNED)
    if (n_termOp == -1) break;
    m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeIntexTerm(handle,a_sequence,b_FALSE0, &intexTermValue,
      nc_abandonmentInfo))
    m_ASSERT(intexTermValue.asValue == AS__VALUE_INT)
    switch (n_termOp) {
    case ADD__TERM_OP:
      a_intexValue->select.c_blotval += intexTermValue.select.c_blotval;
    break; case SUBTRACT__TERM_OP:
      a_intexValue->select.c_blotval -= intexTermValue.select.c_blotval;
    break; case AND__TERM_OP:
      a_intexValue->select.c_blotval = a_intexValue->select.c_blotval && 
        intexTermValue.select.c_blotval;
    break; case OR__TERM_OP:
      a_intexValue->select.c_blotval = a_intexValue->select.c_blotval || 
        intexTermValue.select.c_blotval;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,n_termOp) 
    } // switch
  } // while 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeFullIntex 

// Compute full strex ; that is
// Parse rest of <strex> after (already parsed) 1st <strex atom> and if such rest exist compute it.
// Otherwise, full strex simply corresponds to strex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - *a_strexValue: 1st strex atom value parsed so far
//
// Changed:
// - *a_sequence: after parsing 
// - *a_strexValue: only updated if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex atom> NOT strex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorParseAndComputeFullStrex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *a_strexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_strexValue->asValue == AS__VALUE_STR)
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<strex>") 

  struct BLOTEX_VALUE strexAtomValue = UNDEFINED_BLOTEX_VALUE; 
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  while (b_TRUE) {
    PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) break;
  
    m_CHECK_ABANDON(BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&strexAtomValue,
      nc_abandonmentInfo))
    if (strexAtomValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
    m_TRACK_IF(ConcatenateStrexValue(handle->h_workingGStringsHandle,
      a_strexValue, strexAtomValue.select.c_strex.v_str) != RETURNED)
  } // while 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeFullStrex

// Public function; see .h
int BlotexlibExecutorParseAndComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)

  m_CHECK_ABANDON(BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,ac_blotexValue,
    nc_abandonmentInfo))

  m_PParsePassSpaces(a_sequence,NULL);
  switch (ac_blotexValue->asValue) {
  case AS__VALUE_STR: // <strex>
    m_CHECK_ABANDON(m_BlotexlibExecutorParseAndComputeFullStrex(handle,a_sequence,ac_blotexValue,
      nc_abandonmentInfo))
  break; case AS__VALUE_INT: // <intex>
    m_CHECK_ABANDON(m_BlotexlibExecutorParseAndComputeFullIntex(handle,a_sequence,ac_blotexValue,
      nc_abandonmentInfo))
  break; default: m_RAISE(ANOMALY__VALUE__D,ac_blotexValue->asValue) } // switch 

m_DIGGY_VAR_D(ac_blotexValue->asValue)
  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorParseAndComputeBlotex 

// Probe blotex reference (usable as 'l-value') ; only blotex reference is established. 
//
// Passed:
// - handle:
// - *a_sequence: expect <int blotex ref> | <str blotex ref> 
//
// Changed:
// - *a_sequence: after parsing 
// - *an_fieldReferenceBlottabsLabel: only significant if success / blottab (field) ref
//   + -1 special value: blotvar reference 
//   + >= 0: blottab reference; corresponding blottabs label
// - *acc_blotvarReference: only significant if success, NOT blottab (field) ref case
// - *acc_blottabSpotReference: only significant if success blottab (field) ref
// - *acc_as:
// - nc_abandonmentInfo: only significant if abandon 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES: success
// - ANSWER__NO: abandon 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorProbeBlotexRef(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int* an_fieldReferenceBlottabsLabel,
  struct BLOTVAR_REFERENCE *acc_blotvarReference,
  struct BLOTTAB_SPOT_REFERENCE *acc_blottabSpotReference, int *acc_as,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  *acc_as = UNDEFINED;

  struct P_STRING name; UNDEFINED;
  void* cnr_blottabHandle = (void*)UNDEFINED;
  g_BLOTREG_HANDLE cn_blotregHandle = (g_BLOTREG_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotexlibExecutorParseAndComputeBlotregOrBlottabXName(handle,a_sequence,
    an_fieldReferenceBlottabsLabel,&name, &cnr_blottabHandle,&cn_blotregHandle) != RETURNED)
  PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { 
    if (*an_fieldReferenceBlottabsLabel >= 0) { // Parsing <int blottab ref> or <str blottab ref> ...
      m_CHECK_ABANDON(handle->blottabExecutorImplementations[*an_fieldReferenceBlottabsLabel].
        l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction(handle,b_OPS,a_sequence,name,
        cnr_blottabHandle,acc_blottabSpotReference, nc_abandonmentInfo))
    } else { // Parsing <int blotreg ref> or <str blotreg ref> ...
      m_CHECK_ABANDON(ParseAndComputeLValueBlotregOps(a_sequence,name,
        cn_blotregHandle,acc_blotvarReference, acc_as, nc_abandonmentInfo))
    } // if

  } else {
    if (*an_fieldReferenceBlottabsLabel >= 0) {
      m_CHECK_ABANDON(handle->blottabExecutorImplementations[*an_fieldReferenceBlottabsLabel].
        l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction(handle,b_SPOT,a_sequence,name,
        cnr_blottabHandle,acc_blottabSpotReference, nc_abandonmentInfo))
    } else {
      // Parse <blotvar>:
      if (cn_blotregHandle == NULL) {
        switch (BlotexlibExecutorCreateBlotreg(handle,name, &cn_blotregHandle)) {
        case COMPLETED__OK:
        break; case COMPLETED__BUT:
          m_RAISE(ANOMALY__UNEXPECTED_CASE)
        break; default:
          m_TRACK()
        } // switch
      } // if 
      m_CHECK_ABANDON(ParseAndComputeSimpleBlotvarReference(a_sequence, cn_blotregHandle,
        acc_blotvarReference, nc_abandonmentInfo))
      m_PParsePassSpaces(a_sequence,NULL);
      // Expect <blotvar as int> | <blotvar id> | <blotvar as str> | <blotvar name>  
      m_CHECK_ABANDON(ParseAs(b_L_VALUE,a_sequence, acc_as))
    } // if
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorProbeBlotexRef

// Execute Eval() blot function
//
// Passed
// - handle: 
// - arguments
//
// Changed:
// - c_surrogate:
// - ac_blotval:
// - nc_abandonmentInfo:
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorExecuteCFunctionEval(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING arguments, G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(GStringsClear(handle->h_workingGStringsHandle,b_LIGHT_CLEAR) < 0)
  char b_lValueReference = b_FALSE0; // NO 'L-value' reference a priori
  int n_fieldReferenceBlottabsLabel = UNDEFINED;
  struct BLOTVAR_REFERENCE cc_lValueBlotvarReference = UNDEFINED_BLOTVAR_REFERENCE ; 
  struct BLOTTAB_SPOT_REFERENCE cc_lValueBlottabSpotReference;
  int cc_as = UNDEFINED; // only significant with assignation of single blotvar/blotreg

  m_PREPARE_ABANDON(&arguments, "Eval") 
  m_PParsePassSpaces(&arguments,NULL);
  { struct P_STRING subSequence = UNDEFINED_P_STRING;
    PParseTillMatch(&arguments,m_PString(":="),NULL, &subSequence); // TODO: improve; i.e: "joker" sequences???...
    if (b_EMPTY_P_STRING(arguments)) arguments = subSequence; // No assignation
    else { // Assignation 
      b_lValueReference = b_TRUE;
      m_CHECK_ABANDON(m_BlotexlibExecutorProbeBlotexRef(handle,&subSequence, &n_fieldReferenceBlottabsLabel,
        &cc_lValueBlotvarReference, &cc_lValueBlottabSpotReference, &cc_as, nc_abandonmentInfo))
      // <blotvar>
      m_PParsePassSpaces(&subSequence,NULL);
      if (!b_EMPTY_P_STRING(subSequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      PParseOffset(&arguments,2,NULL);
      m_PParsePassSpaces(&arguments,NULL);
    } // if
  } // subSequence

  struct BLOTEX_VALUE c_blotexValue = UNDEFINED_BLOTEX_VALUE; 
  m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeBlotex(handle,&arguments,&c_blotexValue,
    nc_abandonmentInfo))

  m_PParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  if (b_lValueReference) { // Assignation
    if (/*cb_lValueBlottabSpotReference*/ n_fieldReferenceBlottabsLabel >= 0) { // blottab assignation
      switch (cc_lValueBlottabSpotReference.asValue) {
      case AS__VALUE_INT: // [ '#' ] 
        if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      break; case AS__VALUE_STR: // '$'
        if (c_blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      break; default: m_RAISE(ANOMALY__VALUE__D,cc_lValueBlottabSpotReference.asValue)
      } // switch 
      switch (handle->blottabExecutorImplementations[n_fieldReferenceBlottabsLabel].
        updateBlottabSpotFunction(cc_lValueBlottabSpotReference, c_blotexValue)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        m_ABANDON(NOT_EXISTING_L_VALUE__ABANDONMENT_CAUSE)
      break; default:
        m_TRACK() 
      } // switch
    } else {
      g_BLOTVAR_STUFF cnt_blotvarStuff = (g_BLOTVAR_STUFF)UNDEFINED;
      m_TRACK_IF(FetchBlotvar(&cc_lValueBlotvarReference, b_L_VALUE,&cnt_blotvarStuff,NULL) !=
        RETURNED)
      if (cnt_blotvarStuff == NULL) m_ABANDON(NOT_EXISTING_L_VALUE__ABANDONMENT_CAUSE)
      switch (cc_as) {
      case AS__VALUE_INT: // [ '#' ]
        if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
        cnt_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
      break; case AS__ID: // '!' 
  m_RAISE(ANOMALY__NOT_AVAILABLE)
      break; case AS__VALUE_STR: // '$'
        if (c_blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
        m_TRACK_IF(GStringCopy(cnt_blotvarStuff+g_BLOTVAR_VALUE_ELEMENT,0,
          c_blotexValue.select.c_strex.v_str) < 0)
      break; case AS__NAME:  // '!$'
  m_RAISE(ANOMALY__NOT_AVAILABLE)
      break; default: m_RAISE(ANOMALY__VALUE__D,cc_as) } // switch
    } // if
  } // if

  switch (c_blotexValue.asValue) {
  case AS__VALUE_STR:
    *ac_blotval = TRUE__BLOTVAL0;
m_DIGGY_VAR_GEN(*ac_blotval,ld)
  break; case AS__VALUE_INT:
    *ac_blotval = c_blotexValue.select.c_blotval;
m_DIGGY_VAR_GEN(*ac_blotval,ld)
  break; default: m_RAISE(ANOMALY__VALUE__D,c_blotexValue.asValue) } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorExecuteCFunctionEval

// Execute OutputF() blot function
//
// Passed
// - handle: 
// - arguments
//
// Changed:
// - c_surrogate:
// - ac_blotval:
// - nc_abandonmentInfo:
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorExecuteCFunctionOutputF(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING arguments, G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(GStringsClear(handle->h_workingGStringsHandle,b_LIGHT_CLEAR) < 0)
  int n_format = -1;
  m_CHECK_ABANDON(ParseFormat(&arguments,&n_format,nc_abandonmentInfo))
     
  m_PREPARE_ABANDON(&arguments, "OutputF") 
  m_PParsePassSpaces(&arguments,NULL);
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  PParsePassSingleChar(&arguments,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PParsePassSpaces(&arguments,NULL);
  struct BLOTEX_VALUE c_blotexValue = UNDEFINED_BLOTEX_VALUE;
  m_CHECK_ABANDON(BlotexlibExecutorParseAndComputeBlotex(handle,&arguments,&c_blotexValue,
    nc_abandonmentInfo))

  m_PParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  switch (n_format) {
  case D__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%d", c_blotexValue.select.c_blotval);
  break; case LS__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,FMT_P_STRING,m_P_STRING_2_FMT_ARGS(
      c_blotexValue.select.c_strex.v_str));
  break; case LX__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%x",c_blotexValue.select.c_blotval);
  break; case UX__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%X", c_blotexValue.select.c_blotval);
  break; case LE__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case BE__FORMAT:
    if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; default:
    m_RAISE(ANOMALY__VALUE__D,n_format)
  } // switch

  *ac_blotval = TRUE__BLOTVAL0;
  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorExecuteCFunctionOutputF

enum {
      EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 

// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION 
static int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  *ac_blotval = TRUE__BLOTVAL0; // a priori

m_DIGGY_VAR_P_STRING(ap_blotfunc->call.referral)
m_DIGGY_VAR_P_STRING(ap_blotfunc->call.arguments)

  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
    m_CHECK_ABANDON(m_BlotexlibExecutorExecuteCFunctionEval(handle,ap_blotfunc->call.arguments,
      c_surrogate, ac_blotval, nc_abandonmentInfo))
  break; case OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
    m_CHECK_ABANDON(m_BlotexlibExecutorExecuteCFunctionOutputF(handle,ap_blotfunc->call.arguments,
      c_surrogate, ac_blotval, nc_abandonmentInfo))
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotfunc->entry.localBlotfuncNameEntry)
  } // switch
  
  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorExecuteCFunction


// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <blotex>
static int BlotexlibExecutorDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE xh_handle = (BLOTEXLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,xh_handle)

  m_TRACK_IF(NamedObjectsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)

  int i =0; for (; i < xh_handle->blottabExecutorImplementationsNumber; i++) { 
    m_TRACK_IF(NamedObjectsDestroyInstance(
      xh_handle->blottabExecutorImplementations[i].h_blottabsHandle) != RETURNED)
  } // for

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_workingGStringsHandle) != RETURNED)

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorDestroyInstance

// Public function; see .h
int l_BlotcodeLinkBlotexlib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE fp_blotexlibExecutorFactoryHandle) {
  m_DIGGY_BOLLARD()

  int entry = BlotcodeLinkBlotlib(ep_handle, nfp_blotlibPrefix,
    BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER, localBlotfuncNames, 
    BlotexlibExecutorFactoryCreateProductInstance, fp_blotexlibExecutorFactoryHandle,
    BlotexlibExecutorExecuteCFunction, BlotexlibExecutorDestroyInstance) ;
  m_TRACK_IF(entry < 0)

  m_DIGGY_RETURN(entry)
} // l_BlotcodeLinkBlotexlib
