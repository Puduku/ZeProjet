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

// blotex library executor 
// =======================

#define MAX_BLOTTAB_IMPLEMENTIONS_NUMBER 3

struct BLOTTAB_IMPLEMENTATION {
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_L_VALUE_BLOTTAB_SET_OP_FUNCTION
  l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
  l_BLOTEXLIB_EXECUTOR_PARSE_AND_COMPUTE_R_VALUE_BLOTTAB_OPS_FUNCTION
  l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
  UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION updateBlottabCurrentBlotsetFieldFunction;
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
  UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION updateBlottabCurrentBlotsetFieldFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()
  int i = handle->blottabImplementationsNumber++; 
  m_ASSERT(i < MAX_BLOTTAB_IMPLEMENTIONS_NUMBER)
  handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction =
    l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
  handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction =
    l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
  handle->blottabImplementations[i].updateBlottabCurrentBlotsetFieldFunction =
    updateBlottabCurrentBlotsetFieldFunction ;
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
  UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION updateGenuineBlottabCurrentBlotsetFieldFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION genuineBlottabDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,*azh_handle)
  (*azh_handle)->blottabImplementationsNumber = 0; 
  m_ASSERT(BlotexlibExecutorFactoryRegisterBlottabImplementation(*azh_handle,
    l_blotexlibExecutorParseAndComputeLValueGenuineBlottabSetOpFunction,
    l_blotexlibExecutorParseAndComputeRValueGenuineBlottabOpsFunction,
    updateGenuineBlottabCurrentBlotsetFieldFunction, genuineBlottabDestroyInstanceFunction) ==
    GENUINE_BLOTTAB_LABEL0) 
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
  UPDATE_BLOTTAB_CURRENT_BLOTSET_FIELD_FUNCTION updateBlottabCurrentBlotsetFieldFunction;
  G_STRINGS_HANDLE h_blottabsHandle ; 
} ;

struct BLOTEXLIB_EXECUTOR {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE)
  G_STRINGS_HANDLE h_blotregsHandle ; 
  G_STRINGS_HANDLE h_workingGStringsHandle ;
  int blottabExecutorImplementationsNumber; 
  struct BLOTTAB_EXECUTOR_IMPLEMENTATION blottabExecutorImplementations[
    MAX_BLOTTAB_IMPLEMENTIONS_NUMBER]; 
} ;  


// Blotregs: 

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlotregDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  G_STRINGS_HANDLE xh_handle = (G_STRINGS_HANDLE) xhr_handle;

  m_TRACK_IF(GStringsDestroyInstance(xh_handle) < 0)  

  m_DIGGY_RETURN(RETURNED) 
} // BlotregDestroyInstance 


// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION@c-ansi/blotcode.h <blotex>
static int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()

  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE p_handle = (BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)pr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,p_handle)

  m_MALLOC_INSTANCE(*(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)
  BLOTEXLIB_EXECUTOR_HANDLE productHandle = *azhr_blotlibExecutorHandle;

  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,productHandle)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blotregsHandle), 10,
    BlotregDestroyInstance) != RETURNED)
  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(productHandle->h_blotregsHandle, NULL,NULL) ==
    INDEX_LABEL0)

  productHandle->blottabExecutorImplementationsNumber = p_handle->blottabImplementationsNumber;
  int i = 0; for (; i < p_handle->blottabImplementationsNumber; i++) { 
    m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(
      &(productHandle->blottabExecutorImplementations[i].h_blottabsHandle), 10,
      p_handle->blottabImplementations[i].blottabDestroyInstanceFunction) != RETURNED)
    m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(
      productHandle->blottabExecutorImplementations[i].h_blottabsHandle, NULL,NULL) ==
      INDEX_LABEL0)
    productHandle->blottabExecutorImplementations[i].
      l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction =
      p_handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction;
    productHandle->blottabExecutorImplementations[i].
      l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction =
      p_handle->blottabImplementations[i].l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction;
    productHandle->blottabExecutorImplementations[i].updateBlottabCurrentBlotsetFieldFunction =
      p_handle->blottabImplementations[i].updateBlottabCurrentBlotsetFieldFunction;
  } // for

  m_TRACK_IF(G_STRINGS_CREATE_INSTANCE(&productHandle->h_workingGStringsHandle,5) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateProductInstance
  

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
  struct P_STRING blotregName, G_STRINGS_HANDLE *ac_blotregHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blotregName)
//DumpBlotregs(handle->h_blotregsHandle,"GET blotreg BEFORE SEARCH");
  G_STRING_STUFF ct_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey = m_GKey_PString(blotregName);
  int result = m_GStringsIndexSingleFetch(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlotregStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blotregHandle = ct_namedBlotregStuff->acolyt.cnhr_handle) != NULL)
switch(GStringsVerifyIndexes(*ac_blotregHandle)) {
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

#define NAME__BLOTREG_INDEX_LABEL      0
#define TOKEN_ID__BLOTREG_INDEX_LABEL  1
#define INT_VALUE__BLOTREG_INDEX_LABEL 2
#define STR_VALUE__BLOTREG_INDEX_LABEL 3

// Public function; see .h
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blotregName, G_STRINGS_HANDLE *na_blotregHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blotregName)
  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey = m_GKey_PString(blotregName);
  int result = m_GStringsIndexSingleFetch(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlotregStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlotregStuff->acolyt.cnhr_handle != NULL)
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    { G_STRINGS_HANDLE h_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED ;
      m_TRACK_IF(m_GParamsCreateInstance(&h_blotregHandle,BATEAU__EXPECTED_ITEMS_NUMBER) !=
        RETURNED) 
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
        P_STRING__G_KEYS_COMPARISON,NULL,NULL,
        (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
        NAME__BLOTREG_INDEX_LABEL)
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
        ACOLYT_VALUE__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
        (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
        TOKEN_ID__BLOTREG_INDEX_LABEL)
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_VALUE_ELEMENT,
        ACOLYT_VALUE__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
        (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
        INT_VALUE__BLOTREG_INDEX_LABEL)
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_VALUE_ELEMENT,
        P_STRING__G_KEYS_COMPARISON,NULL,NULL,
        (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
        STR_VALUE__BLOTREG_INDEX_LABEL)

      m_TRACK_IF(GStringCopy(t_namedBlotregStuff,0,blotregName) < 0)
      m_TRACK_IF(m_GStringAsNamedObject(t_namedBlotregStuff,h_blotregHandle,
        handle->h_blotregsHandle) != RETURNED)
    } // h_blotregHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg


// Blottabs:



// Public function; see .h
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabLabel,
  struct P_STRING blottabName, void* *acr_blottabHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(blottabLabel < handle->blottabExecutorImplementationsNumber)

  G_STRING_STUFF ct_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blottabName);

  int result = m_GStringsIndexSingleFetch(
    handle->blottabExecutorImplementations[blottabLabel].h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlottabStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*acr_blottabHandle = ct_namedBlottabStuff->acolyt.cnhr_handle) != NULL);
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlottab

// Public function; see .h
int BlotexlibExecutorAddBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, int blottabLabel,
  struct P_STRING blottabName, void* hr_blottabHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(blottabLabel < handle->blottabExecutorImplementationsNumber)
m_DIGGY_VAR_P_STRING(blottabName)
  
  G_STRINGS_HANDLE blottabsHandle = 
    handle->blottabExecutorImplementations[blottabLabel].h_blottabsHandle;
m_DIGGY_VAR_P(blottabsHandle)
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blottabName);
  switch (m_GStringsIndexSingleFetch(blottabsHandle,NULL,INDEX_LABEL0,INDEX_SEEK_FLAGS__EQUAL,&gKey,
    INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL)) {
  case RESULT__FOUND:
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(GStringCopy(t_namedBlottabStuff,0,blottabName) < 0)
    m_TRACK_IF(m_GStringAsNamedObject(t_namedBlottabStuff,hr_blottabHandle,
      blottabsHandle) != RETURNED)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorAddBlottab


// Blot expressions parsing:


// specific blotvar reference (of a register) 
enum {
  NAME__BLOTVAR_REFERENCE, // blotvar identified as '.' <entity> 
  ENTRY__BLOTVAR_REFERENCE, // blotvar identified as '[' <intex> ']'
  TOKEN_ID__BLOTVAR_REFERENCE, // blotvar identified as {' <intex> '}'
  CURRENT__BLOTVAR_REFERENCE // Current blotvar '?=' 
};

struct BLOTVAR_REFERENCE {
  G_STRINGS_HANDLE blotregHandle;
  int blotvarReference; 
  union {
    struct P_STRING c_name; // Only significant with NAME__BLOTVAR_REFERENCE
    int c_entry; // Only significant with ENTRY__BLOTVAR_REFERENCE
    int c_tokenId; // Only significant with TOKEN_ID__BLOTVAR_REFERENCE
  } c_select; // NOT significant with CURRENT__BLOTVAR_REFERENCE
} ;

#define UNDEFINED_BLOTVAR_REFERENCE { (G_STRINGS_HANDLE) UNDEFINED }

// Complete parsing of "simple" blotvar reference.
//
// Passed:
// - handle:
// - b_lValue: TRUE => create blotreg if not exist 
// - *a_sequence: expected: [ last part of ] <blotvar> 
// - blotregName: 
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotvarReference: only significant if success
// - nc_abandonmentInfo: only significant if abandon 
//
// Ret: "simple" blotvar successfully parsed ? 
// - ANSWER__YES: success
// - ANSWER__NO: abandon 
// - -1: unexpected problem
static int BlotexlibExecutorParseAndComputeSimpleBlotvarReference(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, const struct P_STRING blotregName,
  struct BLOTVAR_REFERENCE *ac_blotvarReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  ac_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  m_PREPARE_ABANDON(a_sequence,"<blotvar>") 

  m_PParsePassSpaces(a_sequence,NULL); 

  // Retrieve blotreg corresponding to register name:
  if ((b_lValue)) {
    switch (BlotexlibExecutorCreateBlotreg(handle,blotregName,
      &ac_blotvarReference->blotregHandle)) {
    case COMPLETED__OK:
    case COMPLETED__BUT:
    break; default:
      m_TRACK()
    } // switch
  } else {
    switch (BlotexlibExecutorGetBlotreg(handle,blotregName,
      &ac_blotvarReference->blotregHandle)) {
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
      m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
    break; default:
      m_TRACK()
    } // switch
  } // if

  // Retrieve blotvar reference:
  PParseOffset(a_sequence,1,&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  GENERIC_INTEGER genericInteger = UNDEFINED;
  switch (lexeme.string[0]) {
  case '.' : // '.' <entity> : 
    ac_blotvarReference->blotvarReference = NAME__BLOTVAR_REFERENCE;
    PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
      (char)UNDEFINED,&lexeme);
    ac_blotvarReference->c_select.c_name = lexeme;
  break; case '[' : // '[' <intex> ']' :
    ac_blotvarReference->blotvarReference = ENTRY__BLOTVAR_REFERENCE;
    PParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // TODO: support empty or -1 entry when l-value (for smart fetch) 
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_entry = genericInteger;
    PParsePassSingleChar(a_sequence,NULL,']',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; case '{' : // '{' <intex> '}' : 
    ac_blotvarReference->blotvarReference = TOKEN_ID__BLOTVAR_REFERENCE;
    PParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_tokenId = genericInteger;
    PParsePassSingleChar(a_sequence,NULL,'}',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; default:
    m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  } // switch

  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorParseAndComputeSimpleBlotvarReference 


// Fetch actual blotvar corresponding to blotvar reference 
//
// Passed:
// - ap_blotvarReference: blotvar reference 
// - cb_lValue: ONLY significant with NAME__BLOTVAR_REFERENCE / TOKEN_ID__BLOTVAR_REFERENCE; 
//   r-value=>do not create if not found ; l-value=>do create if not found
// - navn_entry: NULL address if not used
// 
// Changed:
// - *ant_blotvarStuff: if not found and seek only, set to NULL
// - *navn_entry: if not found  and seek only, set to -1  
//
// Ret: 
// - RETURNED: Ok
// - -1: unexpected problem
static int FetchBlotvar(const struct BLOTVAR_REFERENCE *ap_blotvarReference, char cb_lValue,
  G_STRING_SET_STUFF *ant_blotvarStuff, int *navn_entry) {
  m_DIGGY_BOLLARD_S()

  int ret = UNDEFINED;
  *ant_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (ap_blotvarReference->blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case TOKEN_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
m_DIGGY_VAR_P_STRING(ap_blotvarReference->c_select.c_name)
        gKey = m_GKey_PString(ap_blotvarReference->c_select.c_name);
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        gKey = m_GKey_AcolytValue(ap_blotvarReference->c_select.c_tokenId);
      } // if
      switch (m_GStringsIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,indexLabel,
        INDEX_SEEK_FLAGS__EQUAL,&gKey,cb_lValue?  INDEX_FETCH_FLAGS__FETCH:
        INDEX_FETCH_FLAGS__SEEK_ONLY, ant_blotvarStuff, navn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        if (cb_lValue) { 
          m_ASSERT(*ant_blotvarStuff != NULL)
          if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE)
            m_TRACK_IF(GStringCopy((*ant_blotvarStuff)+G_PARAM_NAME_ELEMENT,0,
              ap_blotvarReference->c_select.c_name) < 0)
          else { // TOKEN_ID__BLOTVAR_REFERENCE 
            m_TRACK_IF(m_GStringAsGParamName((*ant_blotvarStuff)+G_PARAM_NAME_ELEMENT,
              ap_blotvarReference->c_select.c_tokenId, ap_blotvarReference->blotregHandle)
              < 0)
          } // if
        } // if
      break; default:
        m_TRACK()
      } // switch
    } // gKey 
  break; case ENTRY__BLOTVAR_REFERENCE:
    ret = GStringsFetch(ap_blotvarReference->blotregHandle,
      ap_blotvarReference->c_select.c_entry, ant_blotvarStuff);
    m_TRACK_IF(ret < 0)
m_ASSERT(ret == ap_blotvarReference->c_select.c_entry)
  break; case CURRENT__BLOTVAR_REFERENCE:
    switch (GStringsIndexFetch(ap_blotvarReference->blotregHandle,NULL,
      INDEX_FETCH_FLAGS__CURRENT, ant_blotvarStuff, navn_entry)){
    case RESULT__FOUND:
m_ASSERT(*ant_blotvarStuff != NULL)
    break; case RESULT__NOT_FOUND:
m_ASSERT(*ant_blotvarStuff == NULL)
    break; default: m_TRACK() 
    } // switch   
 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->blotvarReference)  
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // FetchBlotvar


// Public function: see .h
int BlotexlibExecutorSetBlotexValue(BLOTEXLIB_EXECUTOR_HANDLE handle, int asValue,
  gen_BLOTVAL c_blotval, const struct P_STRING* cap_str, char cb_fugaciousStr,
  struct BLOTEX_VALUE *a_blotexValue) {
  m_DIGGY_BOLLARD()
  switch ((a_blotexValue->asValue = asValue)) {
  case AS__VALUE_STR:
    { G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
      m_TRACK_IF((a_blotexValue->select.c_strex.workingGStringEntry =
        GStringsFetch(handle->h_workingGStringsHandle,-1, &t_workingGStringStuff)) < 0)
      if (cb_fugaciousStr) {
        m_TRACK_IF(GStringCopy(t_workingGStringStuff,0, *cap_str) < 0)
      } else {
        switch(GStringImport(t_workingGStringStuff, *cap_str)) {
        case COMPLETED__OK:
        break; case COMPLETED__BUT: 
        break; default:
          m_TRACK()
        } // switch
      } // if
      a_blotexValue->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
    } // t_workingGStringStuff
  break; case AS__VALUE_INT:
    a_blotexValue->select.c_blotval = c_blotval;
  break; default: m_RAISE(ANOMALY__VALUE__D,asValue) } // switch  
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorSetBlotexValue

// Public function: see .h
int BlotexlibExecutorConcatenateStrexValue(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct BLOTEX_VALUE *a_strexValue1, struct P_STRING p_str2) {
  m_DIGGY_BOLLARD()
  m_ASSERT(a_strexValue1->asValue == AS__VALUE_STR)
  G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF(GStringsFetch(handle->h_workingGStringsHandle,
    a_strexValue1->select.c_strex.workingGStringEntry, &t_workingGStringStuff) !=
    a_strexValue1->select.c_strex.workingGStringEntry)
  m_TRACK_IF(GStringCopy(t_workingGStringStuff,-1, p_str2) < 0)
  a_strexValue1->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
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
  struct P_STRING *a_sequence, G_STRINGS_HANDLE blotregHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blotreg request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme;
  struct P_STRING subSequence; 

  PParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blotreg request atom>") 
  struct BLOTEX_VALUE blotexValue = UNDEFINED_BLOTEX_VALUE ; 
  struct G_KEY gKey = {UNDEFINED};
  do {
    int as = UNDEFINED;  
    int blotregIndexLabel = UNDEFINED; 
    // <blotreg request atom int> | <blotreg request atom str> ...
    m_TRACK_IF(ParseAs(b_R_VALUE,&subSequence,&as) != RETURNED)
    switch (as) {
    case AS__VALUE_INT: // [ '#' ]
      blotregIndexLabel = INT_VALUE__BLOTREG_INDEX_LABEL; 
    break; case AS__ENTRY: // '!#' (r-value) 
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__ID: // '!' 
      blotregIndexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL; 
    break; case AS__VALUE_STR: // '$'
      blotregIndexLabel = STR_VALUE__BLOTREG_INDEX_LABEL; 
    break; case AS__NAME:  // '!$'
      blotregIndexLabel = NAME__BLOTREG_INDEX_LABEL;
    break; default: m_RAISE(ANOMALY__VALUE__D,as)
    } // switch
    
    int n_indexSeekFlags = UNDEFINED;
    PParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) n_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
    else {  // select with actual criterium
      m_TRACK_IF(ParseRequestCompOp(&subSequence,as != AS__VALUE_INT,
        &n_indexSeekFlags) != RETURNED)
      if (n_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

      switch(BlotexlibExecutorParseAndComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo)) {
      case ANSWER__YES:
        switch (as) {
        case AS__VALUE_INT: // [ '#' ]
          if (blotexValue.asValue == AS__VALUE_STR) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__ENTRY: // '!#' (r-value) 
          if (blotexValue.asValue == AS__VALUE_STR) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
        break; case AS__ID: // '!' 
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__VALUE_STR: // '$'
          if (blotexValue.asValue == AS__VALUE_INT) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
          gKey = m_GKey_PString(blotexValue.select.c_strex.v_str);
        break; case AS__NAME:  // '!$'
          if (blotexValue.asValue == AS__VALUE_INT) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
          gKey = m_GKey_PString(blotexValue.select.c_strex.v_str);
        break; default: m_RAISE(ANOMALY__VALUE__D,as)
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default: m_TRACK()
      } // switch
    } // if
m_DIGGY_VAR_INDEX_SEEK_FLAGS(n_indexSeekFlags)
    
    int criteriaOpFlags = UNDEFINED;
    m_TRACK_IF(ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterium_GKeys(blotregIndexLabel,
      n_indexSeekFlags,&gKey, criteriaOpFlags);
    m_PParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch (GStringsIndexRequestR(blotregHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeBlotregRequest

// Parse and compute blotreg operations (l-values) :
// Expect <blotreg ref op set int> | <blotreg ref op set str> 
//
// Passed:
// - handle: 
// - *a_sequence: before parsing
// - blotregName: register name
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blotvarReference: only significant if "success" ; corresponding blotvar reference
// - *ac_as: only significant if "success" 
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorParseAndComputeLValueBlotregOps(BLOTEXLIB_EXECUTOR_HANDLE handle, 
  struct P_STRING *a_sequence, struct P_STRING blotregName, 
  struct BLOTVAR_REFERENCE *ac_blotvarReference, int *ac_as, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<blotreg ref op set int> | <blotreg ref op set str>")

  G_STRINGS_HANDLE blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  switch (BlotexlibExecutorGetBlotreg(handle,blotregName,&blotregHandle)) {
  case RESULT__FOUND:
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  ac_blotvarReference->blotregHandle = blotregHandle; 
  ac_blotvarReference->blotvarReference = CURRENT__BLOTVAR_REFERENCE;
  *ac_as = UNDEFINED; // For the moment 

  int n_indexFetchFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);

  int n_as = -1; // No blotreg read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg ref op set int> | <blotreg ref op set str>... 
    m_TRACK_IF(ParseAs(b_L_VALUE,a_sequence,&n_as) != RETURNED)
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
    if (n_as == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *ac_as = n_as ;

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorParseAndComputeLValueBlotregOps

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
  struct P_STRING *a_sequence, struct P_STRING blotregName, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<int blotreg ops> | <str blotreg ops>") 

  G_STRINGS_HANDLE blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  switch (BlotexlibExecutorGetBlotreg(handle,blotregName,&blotregHandle)) {
  case RESULT__FOUND:
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
    (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED) // actually UNDEFINED 
  int n_indexFetchFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);

  PParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op select>...
    switch (m_BlotexlibExecutorParseAndComputeBlotregRequest(handle,a_sequence,blotregHandle,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
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
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
      (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
  } else {
    G_STRING_SET_STUFF ct_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
    int c_entry = UNDEFINED;

m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags) 
    switch (GStringsIndexFetch(blotregHandle,NULL,n_indexFetchFlags,
      &ct_blotvarStuff, &c_entry)) {
    case RESULT__FOUND:
      switch (n_as) {
      case -1: 
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
          (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
      break; case AS__VALUE_INT:
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,
          ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value,
          (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
      break; case AS__ENTRY: // (r-value)
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,c_entry,
          (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
      break; case AS__ID:
m_RAISE(ANOMALY__NOT_AVAILABLE)
      break; case AS__VALUE_STR:
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
          &ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString,b_FUGACIOUS_STR,ac_blotexValue) !=
          RETURNED) // TODO: really FUGACIOUS????
      break; case AS__NAME:
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
          &ct_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString,b_FUGACIOUS_STR,ac_blotexValue) !=
          RETURNED) // TODO: really FUGACIOUS????
      break; default: 
        m_TRACK()
      } // switch

    break; case RESULT__NOT_FOUND:
      if (n_as == AS__VALUE_STR || n_as == AS__NAME) m_TRACK_IF(BlotexlibExecutorSetBlotexValue(
        handle,AS__VALUE_STR,UNDEFINED, ap_aTrivialEmptyPString,!b_FUGACIOUS_STR,
        ac_blotexValue) != RETURNED)
      else m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,FALSE__BLOTVAL,
        (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
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
  struct P_STRING *a_sequence, struct P_STRING blotregName, struct BLOTEX_VALUE *ac_blotexAtomValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  struct BLOTVAR_REFERENCE c_blotvarReference = UNDEFINED_BLOTVAR_REFERENCE ; 
  G_STRING_SET_STUFF nt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  int vn_entry = UNDEFINED; 
  m_PREPARE_ABANDON(a_sequence,
    "<blotvar as int> | <blotvar entry> | <blotvar id> | <blotvar as str> | <blotvar name>") 

  switch (BlotexlibExecutorParseAndComputeSimpleBlotvarReference(handle, b_R_VALUE, a_sequence, blotregName,
    &c_blotvarReference, nc_abandonmentInfo)) {
  case ANSWER__YES: 
    m_TRACK_IF(FetchBlotvar(&c_blotvarReference, b_R_VALUE,&nt_blotvarStuff, &vn_entry) != RETURNED)
    if (nt_blotvarStuff == NULL) m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE) 
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_PParsePassSpaces(a_sequence,NULL); // TODO: ag virer ??????

  int as = UNDEFINED;
  m_TRACK_IF(ParseAs(b_R_VALUE,a_sequence,&as) != RETURNED)
  switch (as) {
  case AS__VALUE_INT: // <blotvar as int>
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_INT,
      nt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value,(const struct P_STRING*) UNDEFINED,
      (char)UNDEFINED, ac_blotexAtomValue) != RETURNED)
  break; case AS__ID: // <blotvar id>
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_INT,
      nt_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value,(const struct P_STRING*) UNDEFINED,
      (char)UNDEFINED, ac_blotexAtomValue) != RETURNED)
  break; case AS__ENTRY: // <blotvar entry> (r-value) 
    m_ASSERT(vn_entry >= 0)
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_INT,vn_entry,
      (const struct P_STRING*) UNDEFINED, (char)UNDEFINED, ac_blotexAtomValue) != RETURNED)
  break; case AS__VALUE_STR: // <blotvar strex> 
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
      &nt_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString,b_FUGACIOUS_STR, ac_blotexAtomValue) !=
      RETURNED) // TODO: really FUGACIOUS????
  break; case AS__NAME:  // <blotvar name> 
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
      &nt_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString,b_FUGACIOUS_STR, ac_blotexAtomValue) !=
      RETURNED) // TODO: really FUGACIOUS????
  break; default:
    m_RAISE(ANOMALY__VALUE__D,as)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeBlotexAtomBlotvar


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
  PParsePassSingleChar(a_sequence,IsInt1Op,(char)UNDEFINED,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT_1OP;
    break; case '+' : n_int1Op = PLUS__INT_1OP;
    break; case '-' : n_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    m_PParsePassSpaces(a_sequence,NULL);
  } // if

m_DIGGY_VAR_P_STRING(*a_sequence)
  gen_BLOTVAL c_blotval = UNDEFINED; 
  PParseGenericInteger(a_sequence,&c_blotval,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_INT,
    c_blotval, (const struct P_STRING*) UNDEFINED, (char)UNDEFINED, ac_blotexAtomValue) != RETURNED)
  else { 
    PParsePassSingleChar(a_sequence,NULL,'"',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <str constant> ... 
      PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"', &lexeme);
      if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      PParseOffset(a_sequence,1,NULL);
      m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,&lexeme,
        !b_FUGACIOUS_STR, ac_blotexAtomValue) != RETURNED) // TODO: FUGACIOUS???
    } else { 
      PParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { 
        switch(BlotexlibExecutorParseAndComputeBlotex(handle,a_sequence,ac_blotexAtomValue,
          nc_abandonmentInfo)) {
        case ANSWER__YES:
          PParsePassSingleChar(a_sequence,NULL,')',&lexeme);
          if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
          // '(' <blotex> ')'
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      } else { // 
        struct P_STRING name = UNDEFINED_P_STRING;
        int n_blottabLabel = UNDEFINED;
        ParseBlottabLabel(a_sequence, &n_blottabLabel);
        PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
          (char)UNDEFINED,&name); // <entity>
        PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
        if (!b_EMPTY_P_STRING(lexeme)) { 
          if (n_blottabLabel >= 0) { // <int blottabX ops> | <str blottabX ops> ...
            switch (handle->blottabExecutorImplementations[n_blottabLabel].
              l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction(handle,b_OPS,a_sequence,
              name,ac_blotexAtomValue,nc_abandonmentInfo)) {
            case ANSWER__YES:
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
          } else { // <int blotreg> | <str blotreg> ...
            switch (BlotexlibExecutorParseAndComputeRValueBlotregOps(handle,a_sequence,name, ac_blotexAtomValue,
              nc_abandonmentInfo)) {
            case ANSWER__YES:
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
          } // if
        } else {
          if (n_blottabLabel >= 0) { 
            // <int blottabX spot> | <str blottabX spot> ... 
            switch (handle->blottabExecutorImplementations[n_blottabLabel].
              l_blotexlibExecutorParseAndComputeRValueBlottabOpsFunction(handle,b_SPOT,a_sequence,
                name, ac_blotexAtomValue,nc_abandonmentInfo)) {
            case ANSWER__YES:
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
          } else { 
            switch (m_BlotexlibExecutorParseAndComputeBlotexAtomBlotvar(handle,a_sequence,name,
              ac_blotexAtomValue, nc_abandonmentInfo)) {
            case ANSWER__YES: // ( <blotvar as int> | <blotvar as str> | <blotvar name> ) 
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
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
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,a_intexTermValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (a_intexTermValue->asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
  } // if

  struct BLOTEX_VALUE intexAtomValue ;
  while (b_TRUE) {
    m_TRACK_IF(ParseCompOp(FACT_OP__COMP_OP_EXTENSION,a_sequence,
      &n_compOp) != RETURNED)
    if (n_compOp == -1) break;
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&intexAtomValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
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
     
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
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

  switch (BlotexlibExecutorParseAndComputeIntexTerm(handle,a_sequence,b_TRUE, a_intexValue,
    nc_abandonmentInfo)) {
  case ANSWER__YES:
    m_ASSERT(a_intexValue->asValue == AS__VALUE_INT)
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
    
  struct BLOTEX_VALUE intexTermValue = UNDEFINED_BLOTEX_VALUE ;
  while (b_TRUE) {
    m_TRACK_IF(ParseTermOp(a_sequence,&n_termOp) != RETURNED)
    if (n_termOp == -1) break;
    switch (BlotexlibExecutorParseAndComputeIntexTerm(handle,a_sequence,b_FALSE0, &intexTermValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
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
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
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
  
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&strexAtomValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (strexAtomValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      m_TRACK_IF(BlotexlibExecutorConcatenateStrexValue(handle,
        a_strexValue, strexAtomValue.select.c_strex.v_str) != RETURNED)
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
       m_TRACK()
    } // switch
  } // while 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorParseAndComputeFullStrex

// Public function; see .h
int BlotexlibExecutorParseAndComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)

  switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,ac_blotexValue,
    nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PParsePassSpaces(a_sequence,NULL);
  switch (ac_blotexValue->asValue) {
  case AS__VALUE_STR: // <strex>
    switch (m_BlotexlibExecutorParseAndComputeFullStrex(handle,a_sequence,ac_blotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
  break; case AS__VALUE_INT: // <intex>
    switch (m_BlotexlibExecutorParseAndComputeFullIntex(handle,a_sequence,ac_blotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
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
//// - *acb_blottabFieldReference: only significant if success blottab (field) ref  ?
// - *an_fieldReferenceBlottabLabel: only significant if success / blottab (field) ref
// - *acc_blotvarReference: only significant if success, NOT blottab (field) ref case
// - nc_abandonmentInfo: only significant if abandon 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES: success
// - ANSWER__NO: abandon 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorProbeBlotexRef(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int* an_fieldReferenceBlottabLabel,
  struct BLOTVAR_REFERENCE *acc_blotvarReference,
  struct BLOTTAB_FIELD_REFERENCE *acc_blottabFieldReference, int *acc_as,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  acc_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  *acc_as = UNDEFINED;

  struct P_STRING name; UNDEFINED;
  ParseBlottabLabel(a_sequence, an_fieldReferenceBlottabLabel); 
  // Parse <entity> corresponding to blotreg or blottab name:
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&name); 
  PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { 
    if (*an_fieldReferenceBlottabLabel >= 0) { // Parsing <int blottab ref> or <str blottab ref> ...
      switch (handle->blottabExecutorImplementations[*an_fieldReferenceBlottabLabel].
        l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction(handle,b_OPS,a_sequence,name,
        acc_blottabFieldReference, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } else { // Parsing <int blotreg ref> or <str blotreg ref> ...
      switch (BlotexlibExecutorParseAndComputeLValueBlotregOps(handle,a_sequence,name,acc_blotvarReference,
        acc_as, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if

  } else {
    if (*an_fieldReferenceBlottabLabel >= 0) {
      switch (handle->blottabExecutorImplementations[*an_fieldReferenceBlottabLabel].
        l_blotexlibExecutorParseAndComputeLValueBlottabSetOpFunction(handle,b_SPOT,a_sequence,name,
        acc_blottabFieldReference, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } else {
      // Parse <blotvar>:
      switch (BlotexlibExecutorParseAndComputeSimpleBlotvarReference(handle,b_L_VALUE,a_sequence,name,
        acc_blotvarReference, nc_abandonmentInfo)) {
      case ANSWER__YES: 
        m_PParsePassSpaces(a_sequence,NULL);
        // Expect <blotvar as int> | <blotvar id> | <blotvar as str> | <blotvar name>  
        switch (ParseAs(b_L_VALUE,a_sequence, acc_as)) {
        case ANSWER__YES:
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
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
  int n_fieldReferenceBlottabLabel = UNDEFINED;
  struct BLOTVAR_REFERENCE cc_lValueBlotvarReference = UNDEFINED_BLOTVAR_REFERENCE ; 
  struct BLOTTAB_FIELD_REFERENCE cc_lValueBlottabFieldReference;
  int cc_as = UNDEFINED; // only significant with assignation of single blotvar/blotreg

  m_PREPARE_ABANDON(&arguments, "Eval") 
  m_PParsePassSpaces(&arguments,NULL);
  { struct P_STRING subSequence = UNDEFINED_P_STRING;
    PParseTillMatch(&arguments,m_PString(":="),NULL, &subSequence); // TODO: improve; i.e: "joker" sequences???...
    if (b_EMPTY_P_STRING(arguments)) arguments = subSequence; // No assignation
    else { // Assignation 
      b_lValueReference = b_TRUE;
      switch (m_BlotexlibExecutorProbeBlotexRef(handle,&subSequence, &n_fieldReferenceBlottabLabel,
        &cc_lValueBlotvarReference, &cc_lValueBlottabFieldReference, &cc_as, nc_abandonmentInfo)) {
      case ANSWER__YES: // <blotvar>
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
      m_PParsePassSpaces(&subSequence,NULL);
      if (!b_EMPTY_P_STRING(subSequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      PParseOffset(&arguments,2,NULL);
      m_PParsePassSpaces(&arguments,NULL);
    } // if
  } // subSequence

  struct BLOTEX_VALUE c_blotexValue = UNDEFINED_BLOTEX_VALUE; 
  switch (BlotexlibExecutorParseAndComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  if (b_lValueReference) { // Assignation
    if (/*cb_lValueBlottabFieldReference*/ n_fieldReferenceBlottabLabel >= 0) { // blottab assignation
      switch (cc_lValueBlottabFieldReference.asValue) {
      case AS__VALUE_INT: // [ '#' ] 
        if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      break; case AS__VALUE_STR: // '$'
        if (c_blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      break; default: m_RAISE(ANOMALY__VALUE__D,cc_lValueBlottabFieldReference.asValue)
      } // switch TODO: use correct blottab label:
      switch (handle->blottabExecutorImplementations[n_fieldReferenceBlottabLabel].
        updateBlottabCurrentBlotsetFieldFunction(cc_lValueBlottabFieldReference, c_blotexValue)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        m_ABANDON(NOT_EXISTING_L_VALUE__ABANDONMENT_CAUSE)
      break; default:
        m_TRACK() 
      } // switch
    } else {
      G_STRING_SET_STUFF cnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
      m_TRACK_IF(FetchBlotvar(&cc_lValueBlotvarReference, b_L_VALUE,&cnt_blotvarStuff,NULL) !=
        RETURNED)
      if (cnt_blotvarStuff == NULL) m_ABANDON(NOT_EXISTING_L_VALUE__ABANDONMENT_CAUSE)
      switch (cc_as) {
      case AS__VALUE_INT: // [ '#' ]
        if (c_blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
        cnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
      break; case AS__ID: // '!' 
  m_RAISE(ANOMALY__NOT_AVAILABLE)
      break; case AS__VALUE_STR: // '$'
        if (c_blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
        m_TRACK_IF(GStringCopy(cnt_blotvarStuff+G_PARAM_VALUE_ELEMENT,0,
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
  switch(ParseFormat(&arguments,&n_format,nc_abandonmentInfo)) {
  case ANSWER__YES:
    break;
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_ASSERT(n_format >= 0)
     
  m_PREPARE_ABANDON(&arguments, "OutputF") 
  m_PParsePassSpaces(&arguments,NULL);
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  PParsePassSingleChar(&arguments,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PParsePassSpaces(&arguments,NULL);
  struct BLOTEX_VALUE c_blotexValue = UNDEFINED_BLOTEX_VALUE;
  switch (BlotexlibExecutorParseAndComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

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
    switch (m_BlotexlibExecutorExecuteCFunctionEval(handle,ap_blotfunc->call.arguments,
      c_surrogate, ac_blotval, nc_abandonmentInfo)) {
    case ANSWER__YES:
      break;
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch

  break; case OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
    switch (m_BlotexlibExecutorExecuteCFunctionOutputF(handle,ap_blotfunc->call.arguments,
      c_surrogate, ac_blotval, nc_abandonmentInfo)) {
    case ANSWER__YES:
      break;
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch

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

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)

  int i =0; for (; i < xh_handle->blottabExecutorImplementationsNumber; i++) { 
    m_TRACK_IF(GStringsDestroyInstance(
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
