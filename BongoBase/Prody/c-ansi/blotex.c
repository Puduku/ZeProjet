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

struct BLOTEXLIB_EXECUTOR_FACTORY {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)
} ; 

 
// Public function; see .h
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,*azh_handle)
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateInstance


// Public function; see .h
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryDestroyInstance


struct BLOTEXLIB_EXECUTOR {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE)
  G_STRINGS_HANDLE h_blotregsHandle ; 
  G_STRINGS_HANDLE h_blottabsHandle ; 
  G_STRINGS_HANDLE h_workingGStringsHandle ;
} ;  


// Blotregs: 

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlotregDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  G_STRINGS_HANDLE xh_handle = (G_STRINGS_HANDLE) xhr_handle;

  m_TRACK_IF(GStringsDestroyInstance(xh_handle) < 0)  

  m_DIGGY_RETURN(RETURNED) 
} // BlotregDestroyInstance 

// Blottabs: 

struct BLOTTAB {
  m_DECLARE_MAGIC_FIELD(BLOTTAB_HANDLE)
  G_STRINGS_HANDLE h_tableHandle;
  G_STRINGS_HANDLE hp_fieldsHandle;
} ; 
typedef struct BLOTTAB* BLOTTAB_HANDLE;


// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
static int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexTypes) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  BLOTTAB_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(BLOTTAB_HANDLE,handle)

  m_TRACK_IF(GStringsCreateInstance(&handle->h_tableHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
    fieldsNumber, VALUED_STRING__G_STRING_CONVEYANCE, (const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION) UNDEFINED) != RETURNED)

  m_TRACK_IF(GStringsCreateInstance(&handle->hp_fieldsHandle,10,2,
    VALUED_STRING__G_STRING_CONVEYANCE, (const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_ASSERT(GStringsAddIndex(handle->hp_fieldsHandle,1,
    0,P_STRING__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)NULL,(TO_CHAR_FUNCTION)NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == INDEX_LABEL0)

  G_STRING_SET_STUFF fieldStuff = (G_STRING_SET_STUFF)UNDEFINED; 
  int c_indexLabel = UNDEFINED;
  int i = 0; for (; i < fieldsNumber; i++) {
    if (s_blottabIndexTypes[i] != NONE__BLOTTAB_INDEX_TYPE) m_TRACK_IF((c_indexLabel =
      GStringsAddIndex(handle->h_tableHandle,1,i,s_blottabIndexTypes[i] == STR__BLOTTAB_INDEX_TYPE?
      P_STRING__G_KEYS_COMPARISON: ACOLYT_VALUE__G_KEYS_COMPARISON ,NULL,NULL,
      (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED)) < 0)
    m_TRACK_IF(GStringsFetch(handle->hp_fieldsHandle, -1, &fieldStuff) != i)
    m_ASSERT(fieldStuff != NULL)
    m_TRACK_IF(GStringCopy(fieldStuff,0, s_names[i]) < 0) 
    fieldStuff->acolyt.cen_value = s_blottabIndexTypes[i] ; 
    fieldStuff[1].acolyt.cen_value = c_indexLabel ;
  } // for

  m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)

  m_DIGGY_RETURN(RETURNED)
} // BlottabCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlottabDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  BLOTTAB_HANDLE xh_handle = (BLOTTAB_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTTAB_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_tableHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->hp_fieldsHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // BlottabDestroyInstance


// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <blotex>
static int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE p_handle = (BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)pr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,p_handle)

  m_MALLOC_INSTANCE(*(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,
    *(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)

  BLOTEXLIB_EXECUTOR_HANDLE productHandle = *azhr_blotlibExecutorHandle;

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blotregsHandle), 10,
    BlotregDestroyInstance) != RETURNED)
  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(productHandle->h_blotregsHandle, NULL,NULL) ==
    INDEX_LABEL0)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blottabsHandle), 10,
    BlottabDestroyInstance) != RETURNED)
  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(productHandle->h_blottabsHandle, NULL,NULL) ==
    INDEX_LABEL0)

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
    *a_blotexlibExecutorHandle =
      (BLOTEXLIB_EXECUTOR_HANDLE)r_blotexlibExecutorHandle;
    m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,*a_blotexlibExecutorHandle)
  break; case RESULT__NOT_FOUND:
    m_RAISE(ANOMALY__VALUE__D,result)
  break; default:
    m_TRACK()
  } // switch 

  m_DIGGY_RETURN(RETURNED)
} // l_BlotcodeExecutorGetBlotexlibExecutorHandle

// Blotregs: 
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
m_DIGGY_VAR_P_STRING(blotregName)
    m_ASSERT((*ac_blotregHandle = ct_namedBlotregStuff->acolyt.cnhr_handle) != NULL)
m_DIGGY_INFO("... was FOUND : *ac_blotregHandle=%p",*ac_blotregHandle)
#if 0
struct G_STRING *na_gStrings = (struct G_STRING *)UNDEFINED;
m_TRACK_IF(GStringsFetch(*ac_blotregHandle,0, &na_gStrings) < 0)
int count = GStringsGetCount(*ac_blotregHandle,NULL);
m_TRACK_IF(count < 0)
m_DIGGY_VAR_D(count)
int i=0; for (; i < count ; i++){
 m_DIGGY_VAR_D(i)
 m_DIGGY_VAR_P_STRING(na_gStrings[(i*2)+G_PARAM_NAME_ELEMENT].cv_pString)
 m_DIGGY_VAR_D(na_gStrings[(i*2)+G_PARAM_NAME_ELEMENT].acolyt.c_tokenId)
 m_DIGGY_VAR_P_STRING(na_gStrings[(i*2)+G_PARAM_VALUE_ELEMENT].cv_pString)
 m_DIGGY_VAR_D(na_gStrings[(i*2)+G_PARAM_VALUE_ELEMENT].acolyt.c_tokenId)
 m_DIGGY_VAR_P(&(na_gStrings[(i*2)+G_PARAM_NAME_ELEMENT].cv_pString))
 m_DIGGY_VAR_P(&(na_gStrings[(i*2)+G_PARAM_VALUE_ELEMENT].cv_pString))
}
m_DIGGY_VAR_P_STRING(blotregName)
#endif
switch(GStringsVerifyIndexes(*ac_blotregHandle)) {
case COMPLETED__OK:
break; case COMPLETED__BUT:
  m_RAISE(ANOMALY__CORRUPTED_INDEXES)
break; default:
  m_TRACK()
} // switch
//////////////////////////
  break; case RESULT__NOT_FOUND:
m_DIGGY_VAR_P_STRING(blotregName)
m_DIGGY_INFO("... was NOT FOUND => on abandonne... ")
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
      m_TRACK_IF(G_PARAMS_CREATE_INSTANCE(&h_blotregHandle,BATEAU__EXPECTED_ITEMS_NUMBER) !=
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
      //t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
    } // h_blotregHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg

// Blottabs: 

// Public function; see .h
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  G_STRINGS_HANDLE *ac_tableHandle, G_STRINGS_HANDLE *acp_fieldsHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey = m_GKey_PString(blottabName);
  int result = m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlottabStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    { BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE)ct_namedBlottabStuff->acolyt.cnhr_handle;
      m_ASSERT(blottabHandle != NULL)
      *ac_tableHandle = blottabHandle->h_tableHandle;
      *acp_fieldsHandle = blottabHandle->hp_fieldsHandle;
    } // blottabHandle
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlottab

// Public function; see .h
int BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexTypes,
  G_STRINGS_HANDLE *na_tableHandle, G_STRINGS_HANDLE *nap_fieldsHandle) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  BLOTTAB_HANDLE h_blottabHandle = (BLOTTAB_HANDLE)UNDEFINED; 
  struct G_KEY gKey = m_GKey_PString(blottabName);
  int result = m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlottabStuff->acolyt.cnhr_handle != NULL)
    h_blottabHandle = (BLOTTAB_HANDLE) t_namedBlottabStuff->acolyt.cnhr_handle;
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(BlottabCreateInstance(&h_blottabHandle,fieldsNumber, s_names,s_blottabIndexTypes) !=
      RETURNED)
    t_namedBlottabStuff->acolyt.cnhr_handle = h_blottabHandle;
  break; default:
    m_TRACK()
  } // switch
  //if (na_blottabHandle != NULL) *na_blottabHandle = t_namedBlottabStuff->acolyt.cnhr_handle;
  if (na_tableHandle != NULL) *na_tableHandle = h_blottabHandle->h_tableHandle;
  if (nap_fieldsHandle != NULL) *nap_fieldsHandle = 
    h_blottabHandle->hp_fieldsHandle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlottab

// Blot expressions parsing:

#define SYNTAX_ERROR__ABANDONMENT_CAUSE "Syntax error"
#define NOT_PARSABLE__ABANDONMENT_CAUSE "Not parsable"
#define VALUE_ERROR__ABANDONMENT_CAUSE "Value error"
#define EXPECT_STREX__ABANDONMENT_CAUSE "Expect strex value"
#define EXPECT_INTEX__ABANDONMENT_CAUSE "Expect strex value"
#define UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_CAUSE "Unknown blotreg"
#define INVALID_FORMAT__ABANDONMENT_CAUSE "Invalid format"
#define NOT_EXISTING__ABANDONMENT_CAUSE "Not existing l-value"
#define NO_BLOTTAB_INDEX__ABANDONMENT_CAUSE "No index in table"
#define EXPECT_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Integer index expected in table"
#define EXPECT_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE "String index expected in table"
#define UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE "Unknown blottab"
#define UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE "Unknown blottab field"
#define ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE "Blottab already exists"

// Passed:
// - a_sequence: parsed sequence address
// - p_sequenceType:  
#define m_PREPARE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  struct P_STRING *ema_sequence = (a_sequence);\
  const char *emp_sequenceType = (p_sequenceType);\
  
// Passed:
// - a_sequence: parsed sequence address 
// - p_sequenceType:  
#define m_PRECISE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  ema_sequence = (a_sequence);\
  emp_sequenceType = (p_sequenceType);\

// Passed:
// - p_cause : parsing abandonment cause format 
// - ... : parsing abandonment cause:
//  + mandatory const char* : parsing abandonment cause format
//  + cause format's optional variable arguments
// - ema_sequence : implicit variable 
// - emp_sequenceType: implicit variable
//
// Changed (Implicit) variables:
// - nc_abandonmentInfo:
#define m_ABANDON(...) {\
  if (nc_abandonmentInfo != NULL) {\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,0,"In %s [" FMT_P_STRING "] : ",\
      emp_sequenceType, m_P_STRING_2_FMT_ARGS((*ema_sequence))) < 0)\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,-1, __VA_ARGS__) < 0)\
  } \
  m_DIGGY_RETURN(ANSWER__NO)\
}

enum {
  NAME__BLOTVAR_REFERENCE, // Simple blotvar: '.' <entity> 
  ENTRY__BLOTVAR_REFERENCE, // Simple blotvar: '[' <intex> ']'
  TOKEN_ID__BLOTVAR_REFERENCE, // Simple blotvar: '{' <intex> '}'
  SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE // '?=' 
};

struct BLOTVAR_REFERENCE {
  int in_blottabElement; // -1 special value => BLOTREG
  union {
    G_STRINGS_HANDLE c_blotregHandle; // BLOTREG
    struct { 
      G_STRINGS_HANDLE tableHandle; 
      G_STRINGS_HANDLE p_fieldsHandle; 
    } c_blottab; // BLOTTAB
  } select; 
  int cv_blotvarReference; // Always SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE with BLOTTAB
  union {
    struct P_STRING c_name; // Only significant with NAME__BLOTVAR_REFERENCE
    int c_entry; // Only significant with ENTRY__BLOTVAR_REFERENCE
    int c_tokenId; // Only significant with TOKEN_ID__BLOTVAR_REFERENCE
  } c_select; // NOT significant with SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE
} ;


// IS_CHAR_FUNCTION:
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 


#define b_STREX b_TRUE


#define b_L_VALUE b_TRUE
#define b_R_VALUE b_FALSE0 


// Parse or complete parsing of "simple" blotvar ; only blotvar reference is established.
//
// Passed:
// - handle:
// - b_lValue: TRUE => create blotreg if not exist 
// - *a_sequence: expected: [ last part of ] <blotvar> 
// TODO: NULL case sert ag rien...
// - nap_blotregName: + NULL special value: register <entity> not yet parsed 
//   + != NULL => register <entity> already parsed
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
static int BlotexlibExecutorParseSimpleBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  struct P_STRING *a_sequence, const struct P_STRING *nap_blotregName,
  struct BLOTVAR_REFERENCE *ac_blotvarReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  ac_blotvarReference->in_blottabElement = -1; // BLOTREG 
  ac_blotvarReference->select.c_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence,"<blotvar>") 

  m_ParsePassSpaces(a_sequence,NULL); 
  // Parse <entity> corresponding to register name if asked:
  if (nap_blotregName == NULL) ParsePassChars(a_sequence,b_REGULAR_SCAN,
    b_PASS_CHARS_WHILE,IsEntityNameChar, (char)UNDEFINED,&lexeme); 
  else lexeme = *nap_blotregName;

  // Retrieve blotreg corresponding to register name:
  if ((/* ac_blotvarReference->cb_lValue = */b_lValue)) {
    switch (BlotexlibExecutorCreateBlotreg(handle,lexeme,
      &ac_blotvarReference->select.c_blotregHandle)) {
    case COMPLETED__OK:
    case COMPLETED__BUT:
    break; default:
      m_TRACK()
    } // switch
  } else {
    switch (BlotexlibExecutorGetBlotreg(handle,lexeme,
      &ac_blotvarReference->select.c_blotregHandle)) {
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
      m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
    break; default:
      m_TRACK()
    } // switch
  } // if

  // ac_blotvarReference->select.c_blotregHandle established 
  // Retrieve blotvar reference:
  ParseOffset(a_sequence,1,&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  GENERIC_INTEGER genericInteger = UNDEFINED;
  switch (lexeme.string[0]) {
  case '.' : // '.' <entity> : 
    ac_blotvarReference->cv_blotvarReference = NAME__BLOTVAR_REFERENCE;
    ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
      (char)UNDEFINED,&lexeme);
    ac_blotvarReference->c_select.c_name = lexeme;
  break; case '[' : // '[' <intex> ']' :
    ac_blotvarReference->cv_blotvarReference = ENTRY__BLOTVAR_REFERENCE;
    ParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // TODO: support empty or -1 entry when l-value (for smart fetch) 
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_entry = genericInteger;
    ParsePassSingleChar(a_sequence,NULL,']',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; case '{' : // '{' <intex> '}' : 
    ac_blotvarReference->cv_blotvarReference = TOKEN_ID__BLOTVAR_REFERENCE;
    ParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_tokenId = genericInteger;
    ParsePassSingleChar(a_sequence,NULL,'}',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; default:
    m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  } // switch

  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorParseSimpleBlotvar 


// Fetch actual blotvar corresponding to blotvar reference 
//
// Passed:
// - ap_blotvarReference: Either  
//  + current 'L-value' reference
//  + blotvar 'L-value' or 'R-value' reference 
// - b_lValue:
// - nacvn_entry: NULL address if not used
// 
// Changed:
// - *acvnt_blotvarStuff: if not blotvar 'L-value' reference and not found, set to NULL
// - *nacvn_entry: if not blotvar 'L-value' reference and not found, set to -1  
//
// Ret: Found ? 
// - RESULT__FOUND:
// - RESULT__NOT_FOUND: blotvar created if reference is blotvar 'L-value'
// - -1: unexpected problem
static int FetchBlotvar(const struct BLOTVAR_REFERENCE *ap_blotvarReference, char b_lValue,
  G_STRING_SET_STUFF *acvnt_blotvarStuff, int *nacvn_entry) {
  m_DIGGY_BOLLARD_S()

  int result =  UNDEFINED, ret = UNDEFINED;
  *acvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (ap_blotvarReference->cv_blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case TOKEN_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->cv_blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
m_DIGGY_VAR_P_STRING(ap_blotvarReference->c_select.c_name)
        gKey = m_GKey_PString(ap_blotvarReference->c_select.c_name);
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        gKey = m_GKey_AcolytValue(ap_blotvarReference->c_select.c_tokenId);
      } // if
      switch (result = m_GStringsIndexSingleFetch(ap_blotvarReference->select.c_blotregHandle,NULL,
        indexLabel,INDEX_SEEK_FLAGS__EQUAL,&gKey,b_lValue?  INDEX_FETCH_FLAGS__FETCH:
        INDEX_FETCH_FLAGS__SEEK_ONLY, acvnt_blotvarStuff, nacvn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        if (b_lValue) { 
          m_ASSERT(*acvnt_blotvarStuff != NULL)
          if (ap_blotvarReference->cv_blotvarReference == NAME__BLOTVAR_REFERENCE)
            m_TRACK_IF(GStringCopy((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,0,
              ap_blotvarReference->c_select.c_name) < 0)
          else { // TOKEN_ID__BLOTVAR_REFERENCE 
            m_TRACK_IF(m_GStringAsGParamName((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,
              ap_blotvarReference->c_select.c_tokenId, ap_blotvarReference->select.c_blotregHandle)
              < 0)
          } // if
        } // if
      break; default:
        m_TRACK()
      } // switch
    } // gKey 
  break; case ENTRY__BLOTVAR_REFERENCE:
    ret = GStringsFetch(ap_blotvarReference->select.c_blotregHandle,
      ap_blotvarReference->c_select.c_entry, acvnt_blotvarStuff);
    m_TRACK_IF(ret < 0)
m_ASSERT(ret == ap_blotvarReference->c_select.c_entry)
    result = (acvnt_blotvarStuff == NULL? RESULT__NOT_FOUND: RESULT__NOT_FOUND);
  break; case SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE:
    switch (result = GStringsIndexFetch(ap_blotvarReference->select.c_blotregHandle,NULL,
      INDEX_FETCH_FLAGS__CURRENT, acvnt_blotvarStuff, nacvn_entry)){
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
    break; default: m_TRACK() 
    } // switch   
 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->cv_blotvarReference)  
  } // switch

  m_DIGGY_RETURN(result)
} // FetchBlotvar


struct BLOTEX_VALUE {
  char b_strex;
  union {
    gen_BLOTVAL c_blotval;
    struct P_STRING c_str;
  } select ;
} ;

enum {
  AS__R_VALUE__ENTRY,// '!#' 
  AS__NAME,          // '!$'
  AS__VALUE_INT,     // [ '#' ]
  AS__ID,            // '!'
  AS__VALUE_STR,     // '$'
} ;

#define  AS__R_VALUE__ENTRY__XX "!#" 
#define  AS__NAME__XX           "!$"
#define  AS__VALUE_INT__X       "#" 
#define  AS__ID__X              "!"
#define  AS__VALUE_STR__X       "$"

// Parse "as" specifier (not present : default to '#" value int) 
//
// Passed:
// - *a_sequence: before parsing
// - b_lValue: TRUE => "as entry" not accepted 
//
// Changed:
// - *a_sequence: after parsing 
// - *a_as: (>=0) corresponding "as" specifier 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
static int ParseAs(char b_lValue, struct P_STRING *a_sequence, int *a_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_ParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,a_as,&lexeme, 5,
    AS__R_VALUE__ENTRY__XX,b_lValue? -1:AS__R_VALUE__ENTRY,  AS__NAME__XX,AS__NAME,
    AS__VALUE_INT__X,AS__VALUE_INT,  AS__ID__X,AS__ID,  AS__VALUE_STR__X,AS__VALUE_STR)
  if (*a_as == -1) *a_as = AS__VALUE_INT;
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*a_as)
  m_DIGGY_RETURN(RETURNED)
} // ParseAs 

// Parse "as" "value" specifier if present
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_as: (>=0) corresponding "as" "value" specifier (-1 if not present) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
static inline int m_ParseAsValue(struct P_STRING *a_sequence, int *an_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_ParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_as,&lexeme, 2,
    AS__VALUE_INT__X,AS__VALUE_INT, AS__VALUE_STR__X,AS__VALUE_STR)
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*an_as)
  m_DIGGY_RETURN(RETURNED)
} // m_ParseAsValue 


// Enumeration of <comp op> | <str comp op> | <fact op> terminal symbols 
enum {
              EQUAL__COMP_OP,
               LESS__COMP_OP,
         LESS_EQUAL__COMP_OP,
            GREATER__COMP_OP,
      GREATER_EQUAL__COMP_OP,
          NOT_EQUAL__COMP_OP,
          LIKE__STR__COMP_OP, // only <str comp op>
  MULTIPLY__FACT_OP__COMP_OP, // only <fact op>
    DIVIDE__FACT_OP__COMP_OP, // only <fact op>
} ;

enum {
       NO__COMP_OP_EXTENSION,
      STR__COMP_OP_EXTENSION,
  FACT_OP__COMP_OP_EXTENSION,
} ;

// Parse comparison operator, if present...
//
// Passed:
// - compOpExtension: + NO__COMP_OP_EXTENSION: parse <comp op>
//   + STR__COMP_OP_EXTENSION: parse <str comp op>
//   + FACT_OP__COMP_OP_EXTENSION: parse <fact op>
// - *a_sequence: expected (eventually) : <comp op> | <str comp op> | <fact op>  
//
// Changed:
// - *a_sequence: after parsing 
// - *an_compOp: + -1: special value: comparison operator NOT present
//   + >=0 : corresponding comparison operator 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
static int ParseCompOp(int compOpExtension, struct P_STRING *a_sequence, int *an_compOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  int n_matchedEntry = UNDEFINED;
  m_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_compOp,&lexeme, 9,
    "===", compOpExtension == STR__COMP_OP_EXTENSION? LIKE__STR__COMP_OP: -1,
    "!=",NOT_EQUAL__COMP_OP, ">=",GREATER_EQUAL__COMP_OP, "<=",LESS_EQUAL__COMP_OP,
    "==",EQUAL__COMP_OP, "<",LESS__COMP_OP, ">",GREATER__COMP_OP,
    "*",compOpExtension == FACT_OP__COMP_OP_EXTENSION? MULTIPLY__FACT_OP__COMP_OP: -1,
    "/",compOpExtension == FACT_OP__COMP_OP_EXTENSION? DIVIDE__FACT_OP__COMP_OP: -1)
  m_DIGGY_RETURN(RETURNED)
} // ParseCompOp


// Parse comparison operator in register/table request, if present... 
//
// Passed:
// - *a_sequence: expect <str comp op> | <comp op> 
// - b_str : TRUE => expect <str comp op> ; FALSE => expect <comp op>
//
// Changed:
// - *a_sequence: after parsing 
// - *an_indexSeekFlags: + -1 special value: comparison operator not present
//   + >= 0: actual seek flags corresponding to comparison operator ; may be NOT 
//     INDEX_SEEK_FLAG__ANY
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
static inline int m_ParseRequestCompOp(struct P_STRING *a_sequence, char b_str,
  int *an_indexSeekFlags) {
  m_DIGGY_BOLLARD()
  *an_indexSeekFlags = -1; // a priori
  m_ParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)

  int n_compOp = UNDEFINED;
  m_TRACK_IF(ParseCompOp(b_str? STR__COMP_OP_EXTENSION:
    NO__COMP_OP_EXTENSION, a_sequence, &n_compOp) != RETURNED)

  switch (n_compOp) {
  case -1: 
    *an_indexSeekFlags = -1;
  break; case GREATER__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER; 
  break; case LESS__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS; 
  break; case GREATER_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER_EQUAL;
  break; case LESS_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS_EQUAL;
  break; case EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__EQUAL;
  break; case NOT_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__NOT_EQUAL;
  break; case LIKE__STR__COMP_OP: 
    m_ASSERT(b_str)
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LIKE;
  break; default: m_RAISE(ANOMALY__VALUE__D,n_compOp)
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // m_ParseRequestCompOp 

// Parse <logical 2op>  
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *a_criteriaFlags: corresponding op. flags (ALL_FLAG_OFF0 special value if not found) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
static inline int m_ParseLogical2Op(struct P_STRING *a_sequence,
  int *a_criteriaOpFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_ParsePassSpaces(a_sequence,NULL);
  ParseMatch(a_sequence,m_PString("and"),NULL,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__AND;
  else { 
    ParseMatch(a_sequence,m_PString("or"),NULL,&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__OR;
    else *a_criteriaOpFlags = ALL_FLAGS_OFF0;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_ParseLogical2Op


// See below
static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) ;


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
static inline int m_BlotexlibExecutorComputeBlotregRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, G_STRINGS_HANDLE blotregHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blotreg request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme;
  struct P_STRING subSequence; 

  ParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blotreg request atom>") 
  struct BLOTEX_VALUE blotexValue; // UNDEFINED
  struct G_KEY gKey; // UNDEFINED
  do {
    int as = UNDEFINED;  
    int blotregIndexLabel = UNDEFINED; 
    // <blotreg request atom int> | <blotreg request atom str> ...
    m_TRACK_IF(ParseAs(b_R_VALUE,&subSequence,&as) != RETURNED)
m_DIGGY_VAR_D(as)
    switch (as) {
    case AS__VALUE_INT: // [ '#' ]
      blotregIndexLabel = INT_VALUE__BLOTREG_INDEX_LABEL; 
    break; case AS__R_VALUE__ENTRY: // '!#' 
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__ID: // '!' 
      blotregIndexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL; 
    break; case AS__VALUE_STR: // '$'
      blotregIndexLabel = STR_VALUE__BLOTREG_INDEX_LABEL; 
    break; case AS__NAME:  // '!$'
      blotregIndexLabel = NAME__BLOTREG_INDEX_LABEL;
    break; default: m_RAISE(ANOMALY__VALUE__D,as)
    } // switch
m_DIGGY_VAR_P_STRING(subSequence)
    
    int n_indexSeekFlags = UNDEFINED;
    ParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
m_DIGGY_VAR_P_STRING(lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) n_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
    else {  // select with actual criterium
m_DIGGY_VAR_P_STRING(lexeme)
      m_TRACK_IF(m_ParseRequestCompOp(&subSequence,as != AS__VALUE_INT,
        &n_indexSeekFlags) != RETURNED)
      if (n_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

      switch(BlotexlibExecutorComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo)) {
      case ANSWER__YES:
        switch (as) {
        case AS__VALUE_INT: // [ '#' ]
          if (blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__R_VALUE__ENTRY: // '!#' 
          if (blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
        break; case AS__ID: // '!' 
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__VALUE_STR: // '$'
          if (!blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
m_DIGGY_VAR_P_STRING(blotexValue.select.c_str)
          gKey = m_GKey_PString(blotexValue.select.c_str);
        break; case AS__NAME:  // '!$'
          if (!blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
m_DIGGY_VAR_P_STRING(blotexValue.select.c_str)
          gKey = m_GKey_PString(blotexValue.select.c_str);
        break; default: m_RAISE(ANOMALY__VALUE__D,as)
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default: m_TRACK()
      } // switch
    } // if
m_DIGGY_VAR_INDEX_SEEK_FLAGS(n_indexSeekFlags)
    
    int criteriaOpFlags = UNDEFINED;
    m_TRACK_IF(m_ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)
m_DIGGY_VAR_P_STRING(subSequence)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterium_GKeys(blotregIndexLabel,
      n_indexSeekFlags,&gKey, criteriaOpFlags);
    m_ParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

m_DIGGY_INFO("Before GStringsIndexRequestR(blotregHandle=%p)...",blotregHandle)
  switch (GStringsIndexRequestR(blotregHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlotregRequest

// Parse and compute blotreg operations:
// - as r-value of blotex atom => expect <int blotreg ops> | <str blotreg ops>
// or
// - as l-value of blotvar reference => expect <blotreg ref op set int> | <blotreg op read str> 
//
// Passed:
// - handle: 
// - b_lValue: true=>l-value ; false=>r-value
// - *a_sequence: before parsing
// - blotregName: register name
// - cac_blotexValue: only significant with (r-value) 
// - cac_blotvarReference: only significant with (l-value)
// - cac_as: only significant with (l-value) 
//
// Changed:
// - *a_sequence: after parsing 
// - *cac_blotexValue: only significant if "success" (r-value) ; value corresponding to blotreg ops
// - *cac_blotvarReference: only significant if "success" (l-value) ; corresponding blotvar
//   reference
// - *cac_as: only significant if "success" (l-value) 
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorComputeBlotregOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blotregName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTVAR_REFERENCE *cac_blotvarReference, int *cac_as,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, b_lValue? "<blotreg ref op set int> | <blotreg op read str>":
    "<int blotreg ops> | <str blotreg ops>") 

  G_STRINGS_HANDLE blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
m_DIGGY_VAR_P_STRING(blotregName)
  switch (BlotexlibExecutorGetBlotreg(handle,blotregName,&blotregHandle)) {
  case RESULT__FOUND:
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  if (b_lValue) {
    cac_blotvarReference->in_blottabElement = -1; // BLOTREG 
    cac_blotvarReference->select.c_blotregHandle = blotregHandle; 
    cac_blotvarReference->cv_blotvarReference = SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE;
    *cac_as = UNDEFINED;
  } else {
    cac_blotexValue->b_strex = b_FALSE0; // a priori 
    cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  } // if
  int n_indexFetchFlags = -1; // a priori
  m_ParsePassSpaces(a_sequence,NULL);

  if (!b_lValue) {
    ParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op select>...
      switch (m_BlotexlibExecutorComputeBlotregRequest(handle,a_sequence,blotregHandle,
        nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if
    ParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op reset>...
      n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
    } // if
    ParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op next>...
      if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
      m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
    } // if
  } // if
  int n_as = -1; // No blotreg read/set op a priori 
  ParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op read int> | <blotreg op read str> (R-value)
    // <blotreg ref op set int> | <blotreg ref op set str> (L-value)... 
    m_TRACK_IF(ParseAs(b_lValue,a_sequence,&n_as) != RETURNED)
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (b_lValue) {
    if (n_as == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *cac_as = n_as ;
  } else {
    if (n_indexFetchFlags < 0) {
      cac_blotexValue->b_strex = b_FALSE0; 
      cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
    } else {
      G_STRING_SET_STUFF ct_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
      int c_entry = UNDEFINED;
  
  m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags) 
      switch (GStringsIndexFetch(blotregHandle,NULL,n_indexFetchFlags,
        &ct_blotvarStuff, &c_entry)) {
      case RESULT__FOUND:
  m_DIGGY_VAR_D(n_as)
        switch (n_as) {
        case -1: 
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
        break; case AS__VALUE_INT:
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
        break; case AS__R_VALUE__ENTRY:
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = c_entry; 
        break; case AS__ID:
          cac_blotexValue->b_strex = b_FALSE0; 
  m_RAISE(ANOMALY__NOT_AVAILABLE)
        break; case AS__VALUE_STR:
  m_DIGGY_VAR_P_STRING(ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString)
  m_DIGGY_VAR_D((int)ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value)
  m_DIGGY_VAR_P_STRING(ct_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString)
  m_DIGGY_VAR_D((int)ct_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value)
          cac_blotexValue->b_strex = b_TRUE; 
          cac_blotexValue->select.c_str = ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString ; 
        break; case AS__NAME:
          cac_blotexValue->b_strex = b_TRUE; 
          cac_blotexValue->select.c_str = ct_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString ; 
        break; default: 
          m_TRACK()
        } // switch
  
      break; case RESULT__NOT_FOUND:
        if ((cac_blotexValue->b_strex = (n_as == AS__VALUE_STR || n_as == AS__NAME)))
          cac_blotexValue->select.c_str = m_PString(GOOD_OLD_EMPTY_C_STRING) ;
        else cac_blotexValue->select.c_blotval = FALSE__BLOTVAL; 
      break; default: m_TRACK()
      } // switch
    } // if
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorComputeBlotregOps

// Passed:
// - np_fieldsHandle: != NULL: seek for table...
// - cnac_tableIndexLabel: (significant when seek for table)
//  + NULL special value: seek for table element 
//  + != NULL: seek for table index label
//
// Changed:
// - *a_sequence: after parsing
// - *cnac_tableIndexLabel: (when seek for table index label; only significant if no abandon)
// - *ccac_element: (when seek for table element; only significant if no abandon) 
// - *cac_asValue: (when seek for table; only significant if no abandon) default to "as value int"
// - *cac_fieldName: (when NO seek for table; only significant if no abandon)
// - *cac_blottabIndexType: (when NO seek for table; only significant if no abandon)
// - nc_abandonmentInfo: 
//
// Ret:
// - ANSWER__YES: Ok
// - ANSWER__NO: parsing abandoned
static int ParseBlottabElement(struct P_STRING *a_sequence, G_STRINGS_HANDLE np_fieldsHandle,
  int *cnac_tableIndexLabel, int *ccac_element, int *cac_asValue, struct P_STRING *cac_fieldName,
  int *cac_blottabIndexType, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  G_STRING_SET_STUFF p_fieldStuff; // UNDEFINED
  struct P_STRING fieldName; //UNDEFINED
  int n_asValue = UNDEFINED;
 
  m_PREPARE_ABANDON(a_sequence,"<entity> [ <<as value int>> | <<as value str>> ]")
  ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&fieldName); // <entity>

  if (np_fieldsHandle != NULL) { // Seek for table
    const struct G_KEY p_fieldKey = m_GKey_PString(fieldName);
    int result = UNDEFINED;
    int entry = UNDEFINED;
    switch (result = m_GStringsIndexSingleFetch(np_fieldsHandle,(INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
      INDEX_LABEL0, INDEX_SEEK_FLAGS__EQUAL, &p_fieldKey, INDEX_FETCH_FLAGS__READ_ONLY, 
      &p_fieldStuff, &entry)) {
    case RESULT__FOUND:
      m_ASSERT(p_fieldStuff != NULL)
      m_ASSERT(entry > 0)
    break; case RESULT__NOT_FOUND:
      m_ABANDON(UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE) 
    break; default: m_TRACK()
    } // switch 
    if (cnac_tableIndexLabel != NULL) {
      if (p_fieldStuff->acolyt.cen_value == NONE__BLOTTAB_INDEX_TYPE) m_ABANDON(
        NO_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      *cnac_tableIndexLabel = p_fieldStuff[1].acolyt.cen_value ;
    } else *ccac_element = entry;
  } // if

  m_TRACK_IF(m_ParseAsValue(a_sequence, &n_asValue) != RETURNED)  
m_DIGGY_VAR_D(n_asValue)
  if (np_fieldsHandle != NULL) { // Seek for table
    if (n_asValue == -1) n_asValue = AS__VALUE_INT;
    *cac_asValue = n_asValue ;
m_DIGGY_VAR_D(n_asValue)
    if (cnac_tableIndexLabel != NULL) {
      switch (n_asValue) {
      case AS__VALUE_INT: // '#'
        if (p_fieldStuff->acolyt.cen_value != INT__BLOTTAB_INDEX_TYPE) m_ABANDON(
          EXPECT_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      break; case AS__VALUE_STR: // '$'
        if (p_fieldStuff->acolyt.cen_value != STR__BLOTTAB_INDEX_TYPE) m_ABANDON(
          EXPECT_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue)
      } // switch
    } // if
  } else {
    switch (n_asValue) {
    case -1:  *cac_blottabIndexType = NONE__BLOTTAB_INDEX_TYPE; 
    break; case AS__VALUE_INT: *cac_blottabIndexType = INT__BLOTTAB_INDEX_TYPE; 
    break; case AS__VALUE_STR: *cac_blottabIndexType = STR__BLOTTAB_INDEX_TYPE; 
    break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue)
    } // switch
    *cac_fieldName = fieldName;
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseBlottabElement 

// Parse and compute blottab request. 
//
// Passed:
// - handle:
// - *a_sequence: expect <blottab request>  
// - tableHandle: table for request 
//
// Changed:
// - *a_sequence: after parsing 
// - tableHandle: 
// - p_fieldsHandle: 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlottabRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, G_STRINGS_HANDLE tableHandle,
  G_STRINGS_HANDLE p_fieldsHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme;
  struct P_STRING subSequence; 

  ParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blottab request atom>") 
  struct BLOTEX_VALUE blotexValue; // UNDEFINED
  struct G_KEY gKey; // UNDEFINED
  do {
    int asValue = UNDEFINED;  
    int tableIndexLabel = UNDEFINED ;
    // <blottab request atom int> | <blottab request atom str> ...
    switch (ParseBlottabElement(&subSequence, p_fieldsHandle, &tableIndexLabel, (int*)UNDEFINED,
      &asValue,(struct P_STRING*)UNDEFINED,(int*)UNDEFINED,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
m_DIGGY_VAR_P_STRING(subSequence)
    
    int n_indexSeekFlags = UNDEFINED;
    ParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
m_DIGGY_VAR_P_STRING(lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) n_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
    else {  // select with actual criterium
m_DIGGY_VAR_P_STRING(lexeme)
      m_TRACK_IF(m_ParseRequestCompOp(&subSequence,asValue != AS__VALUE_INT,
        &n_indexSeekFlags) != RETURNED)
      if (n_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

      switch(BlotexlibExecutorComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo)) {
      case ANSWER__YES:
        switch (asValue) {
        case AS__VALUE_INT: // [ '#' ]
          if (blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__VALUE_STR: // '$'
          if (!blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
m_DIGGY_VAR_P_STRING(blotexValue.select.c_str)
          gKey = m_GKey_PString(blotexValue.select.c_str);
        break; default: m_RAISE(ANOMALY__VALUE__D,asValue)
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default: m_TRACK()
      } // switch
    } // if
m_DIGGY_VAR_INDEX_SEEK_FLAGS(n_indexSeekFlags)
    
    int criteriaOpFlags = UNDEFINED;
    m_TRACK_IF(m_ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)
m_DIGGY_VAR_P_STRING(subSequence)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterium_GKeys(tableIndexLabel,
      n_indexSeekFlags,&gKey, criteriaOpFlags);
    m_ParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

m_DIGGY_INFO("Before GStringsIndexRequestR(tableHandle=%p)...",tableHandle)
  switch (GStringsIndexRequestR(tableHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlottabRequest

// Parse and compute blottab creation. 
//
// Passed:
// - handle:
// - *a_sequence: expect <blottab creation>  
// - blottabName: name of NON EXISTING blottab
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_tableHandle: (only significant when success) CREATED  
// - *acp_fieldsHandle: (only significant when success) CREATED 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlottabCreation(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blottabName, G_STRINGS_HANDLE *ac_tableHandle,
  G_STRINGS_HANDLE *acp_fieldsHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab creation>") 

  struct P_STRING subSequence; 

  ParseTillMatch(a_sequence,m_PString("]?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParseOffset(a_sequence,2,NULL);

  struct P_STRING s_names10[10];
  int s_blottabIndexTypes10[10];
  int i = -1;
  do {
    m_ASSERT(++i < 10)
    switch (ParseBlottabElement(&subSequence, NULL, (int*)UNDEFINED, (int*)UNDEFINED,
       (int*)UNDEFINED, s_names10 + i, s_blottabIndexTypes10 + i,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    m_ParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch(BlotexlibExecutorCreateBlottab(handle, blottabName, i+1, s_names10, s_blottabIndexTypes10,
    ac_tableHandle, acp_fieldsHandle)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__VALUE__D,COMPLETED__BUT)
  break; default: m_TRACK() 
  } // switch 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlottabCreation

// Parse and compute blottab operations:
// - as r-value of blotex atom => expect <int blottab ops> | <str blottab ops>
// or
// - as l-value of blotvar reference => expect <blottab ref op read int> | <blottab op read str> 
//
// Passed:
// - handle: 
// - b_lValue: true=>l-value ; false=>r-value
// - *a_sequence: before parsing
// - blottabName: blot table name
// - cac_blotexValue: only significant with (r-value) 
// - cac_blotvarReference: only significant with (l-value)
// - cac_asValue: only significant with (l-value) 
//
// Changed:
// - *a_sequence: after parsing 
// - *cac_blotexValue: only significant if "success" (r-value) ; value corresponding to blottab ops
// - *cac_blotvarReference: only significant if "success" (l-value) ; corresponding blotvar
//   reference (BLOTTAB ref)
// - *cac_asValue: only significant if "success" (l-value) 
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTVAR_REFERENCE *cac_blotvarReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, b_lValue? "<blottab ref op set int> | <blottab ref op set str>":
    "<int blottab ops> | <str blottab ops>") 

  G_STRINGS_HANDLE n_tableHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  G_STRINGS_HANDLE cp_fieldsHandle = (G_STRINGS_HANDLE)UNDEFINED; 
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,blottabName,&n_tableHandle,&cp_fieldsHandle)) {
  case RESULT__FOUND:
  break; case RESULT__NOT_FOUND:
    n_tableHandle = NULL; 
  break; default:
    m_TRACK()
  } // switch

  if (!b_lValue) {
    cac_blotexValue->b_strex = b_FALSE0; // a priori 
    cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  } // if
  int n_indexFetchFlags = -1; // a priori 
  m_ParsePassSpaces(a_sequence,NULL);

  if (!b_lValue) {
    ParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op create> ...
      if (n_tableHandle != NULL) m_ABANDON(ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE)
      switch (m_BlotexlibExecutorComputeBlottabCreation(handle,a_sequence,blottabName,
        &n_tableHandle, &cp_fieldsHandle, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch

    } else {
      if (n_tableHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
      ParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op select> ...
        switch (m_BlotexlibExecutorComputeBlottabRequest(handle,a_sequence,n_tableHandle,
          cp_fieldsHandle, nc_abandonmentInfo)) {
        case ANSWER__YES:
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      } // if
      ParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op reset>
        n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
      } // if
      ParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op next>
        if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
        m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
      } // if
    } // if
  } // if
  int n_asValue = -1; // No blottab read/set op a priori 
  ParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  int c_element = UNDEFINED; // Only significant with blottab read/set op
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op read int> | <blottab op read str> (R-value)
    // <blottab ref op set int> | <blottab ref op set str> (L-value)... 
    switch (ParseBlottabElement(a_sequence, cp_fieldsHandle, NULL,&c_element, &n_asValue,
      (struct P_STRING*)UNDEFINED,(int*)UNDEFINED,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (b_lValue) {
    if (n_asValue == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    if (n_tableHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
    cac_blotvarReference->in_blottabElement = c_element;
    cac_blotvarReference->select.c_blottab.tableHandle = n_tableHandle; 
    cac_blotvarReference->select.c_blottab.p_fieldsHandle = cp_fieldsHandle; 
    cac_blotvarReference->cv_blotvarReference = SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE;
    *cac_asValue = n_asValue ;
  } else {
    if (n_indexFetchFlags < 0) {
      cac_blotexValue->b_strex = b_FALSE0; 
      cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
    } else {
      G_STRING_SET_STUFF ct_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
      int c_entry = UNDEFINED;
  
  m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags)
      m_ASSERT(n_tableHandle != NULL)
      switch (GStringsIndexFetch(n_tableHandle,NULL,n_indexFetchFlags,
        &ct_blotsetStuff, &c_entry)) {
      case RESULT__FOUND:
  m_DIGGY_VAR_D(n_asValue)
        switch (n_asValue) {
        case -1: 
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
        break; case AS__VALUE_INT:
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = ct_blotsetStuff[c_element].acolyt.cen_value;
        break; case AS__VALUE_STR:
          cac_blotexValue->b_strex = b_TRUE; 
          cac_blotexValue->select.c_str = ct_blotsetStuff[c_element].cv_pString ; 
        break; default: 
          m_TRACK()
        } // switch
  
      break; case RESULT__NOT_FOUND:
        if ((cac_blotexValue->b_strex = (n_asValue == AS__VALUE_STR)))
          cac_blotexValue->select.c_str = m_PString(GOOD_OLD_EMPTY_C_STRING) ;
        else cac_blotexValue->select.c_blotval = FALSE__BLOTVAL; 
      break; default: m_TRACK()
      } // switch
    } // if
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorComputeBlottabOps



// IS_CHAR_FUNCTION:
static int IsInt1Op(int c) {
  return (c == '+' || c == '-' || c == '!');
} // IsInt1Op

// Terminal symbols (of <int 1op> terminal symbol)
enum {
  NOT__INT_1OP,
  PLUS__INT_1OP,
  MINUS__INT_1OP,
} ;

// Complete blotvar reference parsing and compute blotex atom value.
//
// Passed:
// - handle:
// - *a_sequence: after parsing of register name <entity> 
// - blotregName: register name
//
// Changed:
// - *a_sequence:   
// - ac_blotexValue: not significant in case of abandon; value corresponding to blotvar reference
// - nc_abandonmentInfo: 
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlotexAtomBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blotregName, struct BLOTEX_VALUE *ac_blotexAtomValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  struct BLOTVAR_REFERENCE c_blotvarReference; // UNDEFINED 
  G_STRING_SET_STUFF vnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  int vn_entry = UNDEFINED; 
  m_PREPARE_ABANDON(a_sequence,
    "<blotvar as int> | <blotvar entry> | <blotvar id> | <blotvar as str> | <blotvar name>") 

  switch (BlotexlibExecutorParseSimpleBlotvar(handle, b_R_VALUE, a_sequence, &blotregName,
    &c_blotvarReference, nc_abandonmentInfo)) {
  case ANSWER__YES: 
    switch (FetchBlotvar(&c_blotvarReference, b_R_VALUE,&vnt_blotvarStuff, &vn_entry)){
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
m_DIGGY_VAR_D(vn_entry)
m_ASSERT(vn_entry == -1)
      m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE) 
    break; default:
      m_TRACK()
    } // switch
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_ParsePassSpaces(a_sequence,NULL); // TODO: ag virer ??????

  int as = UNDEFINED;
  m_TRACK_IF(ParseAs(b_R_VALUE,a_sequence,&as) != RETURNED)
  ac_blotexAtomValue->b_strex = b_FALSE0; // a priori
  switch (as) {
  case AS__VALUE_INT: // <blotvar as int>
    ac_blotexAtomValue->select.c_blotval =
      vnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
  break; case AS__ID: // <blotvar id>
    ac_blotexAtomValue->select.c_blotval =
      vnt_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value;
  break; case AS__R_VALUE__ENTRY: // <blotvar entry> 
    m_ASSERT(vn_entry >= 0)
    ac_blotexAtomValue->select.c_blotval = vn_entry;
  break; case AS__VALUE_STR: // <blotvar strex> 
    ac_blotexAtomValue->b_strex = b_TRUE;
    ac_blotexAtomValue->select.c_str =
      vnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString; 
  break; case AS__NAME:  // <blotvar name> 
    ac_blotexAtomValue->b_strex = b_TRUE;
    ac_blotexAtomValue->select.c_str =
      vnt_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString; 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,as)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlotexAtomBlotvar

// Probe "blotex atom" : expect either <intex atom> or <strex atom>...
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" 
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

  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence, "<intex atom> | <strex atom>") 
  m_ParsePassSpaces(a_sequence,NULL);
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 

  int n_int1Op = -1; // a priori
  ParsePassSingleChar(a_sequence,IsInt1Op,(char)UNDEFINED,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT_1OP;
    break; case '+' : n_int1Op = PLUS__INT_1OP;
    break; case '-' : n_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    m_ParsePassSpaces(a_sequence,NULL);
  } // if

m_DIGGY_VAR_P_STRING(*a_sequence)
  ParseGenericInteger(a_sequence,&(ac_blotexAtomValue->select.c_blotval),&lexeme);
m_DIGGY_VAR_GEN(ac_blotexAtomValue->select.c_blotval,ld)
m_DIGGY_VAR_P_STRING(lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) ac_blotexAtomValue->b_strex = b_FALSE0; // <int constant>
  else { 
    ParsePassSingleChar(a_sequence,NULL,'"',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <str constant> ... 
      ac_blotexAtomValue->b_strex = b_TRUE;
      ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"',
        &(ac_blotexAtomValue->select.c_str));
      if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      ParseOffset(a_sequence,1,NULL);
    } else { 
      ParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { 
        switch(BlotexlibExecutorComputeBlotex(handle,a_sequence,ac_blotexAtomValue,
          nc_abandonmentInfo)) {
        case ANSWER__YES:
          ParsePassSingleChar(a_sequence,NULL,')',&lexeme);
          if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
          // '(' <blotex> ')'
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      } else { // 
        struct P_STRING name; // UNDEFINED
        ParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
        char b_blottab = (!b_EMPTY_P_STRING(lexeme)); 
        ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
          (char)UNDEFINED,&name); // <entity>
        ParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
        if (!b_EMPTY_P_STRING(lexeme)) { 
          if (b_blottab) { // <int blottab> | <str blottab> ...
            switch (BlotexlibExecutorComputeBlottabOps(handle,b_R_VALUE,a_sequence,name,
              ac_blotexAtomValue,(struct BLOTVAR_REFERENCE *)UNDEFINED,(int *)UNDEFINED,
              nc_abandonmentInfo)) {
            case ANSWER__YES:
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
          } else { // <int blotreg> | <str blotreg> ...
            switch (BlotexlibExecutorComputeBlotregOps(handle,b_R_VALUE,a_sequence,name,
              ac_blotexAtomValue,(struct BLOTVAR_REFERENCE *)UNDEFINED,(int *)UNDEFINED,
              nc_abandonmentInfo)) {
            case ANSWER__YES:
            break; case ANSWER__NO:
              m_DIGGY_RETURN(ANSWER__NO)
            break; default: 
              m_TRACK()
            } // switch
          } // if
        } else {
          if (b_blottab) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
          switch (m_BlotexlibExecutorComputeBlotexAtomBlotvar(handle,a_sequence,name,
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

  if (n_int1Op >= 0) {
    if (ac_blotexAtomValue->b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
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


// Terminal symbols (of <int 2op> terminal symbol)
enum {
            ADD__TERM_OP,
       SUBTRACT__TERM_OP,
            AND__TERM_OP,
             OR__TERM_OP,
} ;

// Parse <term op> if present
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_termOp:
//   + -1: special value: <int 2op> NOT present
//   + >=0 : corresponding int 2op 
static inline int m_ParseTermOp(struct P_STRING *a_sequence, int *an_termOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  int c_matchedEntry = UNDEFINED;
  m_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&c_matchedEntry,an_termOp,&lexeme, 4,
    "&&",AND__TERM_OP, "||",OR__TERM_OP, "+",ADD__TERM_OP, "-",SUBTRACT__TERM_OP)
  // TODO: FOIREUX !!!!???? "++" sera rejeted !!!!!!????
  m_DIGGY_RETURN(RETURNED)
} // m_ParseTermOp

// Parse <intex term>  
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - n_intexAtomValue: + NULL special value : no intex atom 
//   + != NULL: initial intex atom 
//
// Changed:
// - *a_sequence: after parsing 
// - ac_intexTermValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorComputeIntexTerm(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *n_intexAtomValue, struct BLOTEX_VALUE
  *ac_intexTermValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  m_PREPARE_ABANDON(a_sequence, "<intex term>") 
  m_ParsePassSpaces(a_sequence,NULL);
  int n_compOp = UNDEFINED;
  if (n_intexAtomValue != NULL) *ac_intexTermValue = *n_intexAtomValue;
  else {
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,ac_intexTermValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (ac_intexTermValue->b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
  } // if

  while (b_TRUE) {
    m_TRACK_IF(ParseCompOp(FACT_OP__COMP_OP_EXTENSION,a_sequence,
      &n_compOp) != RETURNED)
    if (n_compOp == -1) break;
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,n_intexAtomValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (n_intexAtomValue->b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      switch (n_compOp) {
      case EQUAL__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval ==
          n_intexAtomValue->select.c_blotval);
      break; case LESS__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval < 
          n_intexAtomValue->select.c_blotval);
      break; case LESS_EQUAL__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval <=
          n_intexAtomValue->select.c_blotval);
      break; case GREATER__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval > 
          n_intexAtomValue->select.c_blotval);
      break; case GREATER_EQUAL__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval >=
          n_intexAtomValue->select.c_blotval);
      break; case NOT_EQUAL__COMP_OP:
        ac_intexTermValue->select.c_blotval = BOOLEAN_BLOTVAL(ac_intexTermValue->select.c_blotval !=
          n_intexAtomValue->select.c_blotval);
      break; case MULTIPLY__FACT_OP__COMP_OP:
        ac_intexTermValue->select.c_blotval = ac_intexTermValue->select.c_blotval *
          n_intexAtomValue->select.c_blotval;
      break; case DIVIDE__FACT_OP__COMP_OP:
        ac_intexTermValue->select.c_blotval = ac_intexTermValue->select.c_blotval /
          n_intexAtomValue->select.c_blotval;
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
} // BlotexlibExecutorComputeIntexTerm


// Compute full intex ; that is
// Parse rest of <intex> after (already parsed) 1st <intex atom> and if such rest exist compute it.
// Otherwise, full intex simply corresponds to intex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - intexAtomValue: 1st intex atom
//
// Changed:
// - *a_sequence: after parsing 
// - ac_intexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex> NOT intex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeFullIntex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE intexAtomValue,
  struct BLOTEX_VALUE *ac_intexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(!intexAtomValue.b_strex)
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_termOp = UNDEFINED;
  switch (BlotexlibExecutorComputeIntexTerm(handle,a_sequence,&intexAtomValue,
    ac_intexValue, nc_abandonmentInfo)) {
  case ANSWER__YES:
    m_ASSERT(!ac_intexValue->b_strex)
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
    
  struct BLOTEX_VALUE intexTermValue ;
  while (b_TRUE) {
    m_TRACK_IF(m_ParseTermOp(a_sequence,&n_termOp) != RETURNED)
m_DIGGY_VAR_D(n_termOp)
    if (n_termOp == -1) break;
    switch (BlotexlibExecutorComputeIntexTerm(handle,a_sequence,NULL, &intexTermValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      m_ASSERT(!intexTermValue.b_strex)
      switch (n_termOp) {
      case ADD__TERM_OP:
        ac_intexValue->select.c_blotval += intexTermValue.select.c_blotval;
      break; case SUBTRACT__TERM_OP:
        ac_intexValue->select.c_blotval -= intexTermValue.select.c_blotval;
      break; case AND__TERM_OP:
        ac_intexValue->select.c_blotval = ac_intexValue->select.c_blotval && 
          intexTermValue.select.c_blotval;
      break; case OR__TERM_OP:
        ac_intexValue->select.c_blotval = ac_intexValue->select.c_blotval || 
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
} // m_BlotexlibExecutorComputeFullIntex 

// Compute full strex ; that is
// Parse rest of <strex> after (already parsed) 1st <strex atom> and if such rest exist compute it.
// Otherwise, full strex simply corresponds to strex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - strexAtomValue: 1st intex atom
//
// Changed:
// - *a_sequence: after parsing 
// - ac_strexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex> NOT intex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeFullStrex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE strexAtomValue,
  struct BLOTEX_VALUE *ac_strexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(strexAtomValue.b_strex)
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<strex>") 

  *ac_strexValue = strexAtomValue;
  struct P_STRING lexeme; // UNDEFINED
  while (b_TRUE) {
    ParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) break;
  
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&strexAtomValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (!strexAtomValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
      m_TRACK_IF(GStringsFetch(handle->h_workingGStringsHandle,-1, &t_workingGStringStuff) < 0)
      m_TRACK_IF(GStringCopy(t_workingGStringStuff,0, ac_strexValue->select.c_str) < 0)
      m_TRACK_IF(GStringCopy(t_workingGStringStuff,-1, strexAtomValue.select.c_str) < 0)
      ac_strexValue->select.c_str = t_workingGStringStuff->cv_pString;
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
       m_TRACK()
    } // switch
  } // while 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeFullStrex

// Parse <blotex> 
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)

  struct BLOTEX_VALUE blotexAtomValue; // UNDEFINED

  switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&blotexAtomValue,
    nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_ParsePassSpaces(a_sequence,NULL);
  if (blotexAtomValue.b_strex) { // <strex>
    switch (m_BlotexlibExecutorComputeFullStrex(handle,a_sequence,blotexAtomValue,ac_blotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch

  } else { // <intex>
    switch (m_BlotexlibExecutorComputeFullIntex(handle,a_sequence,blotexAtomValue,ac_blotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
  } // if 

m_DIGGY_VAR_D(ac_blotexValue->b_strex)
  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorComputeBlotex 


// Probe blotvar reference (usable as 'l-value') ; only blotvar reference is established. 
//
// Passed:
// - handle:
// - *a_sequence: expect <int blotvar ref> | <str blotvar ref> 
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blotvarReference: only significant if success
//// - *acn_blottabElement: only significant if success:
//   + -1 special value: blotvar reference for BLOTREG 
//   + >= 0: blotvar reference for BLOTTAB; corresponding field in that table
// - *ac_as: only significant if success
// - nc_abandonmentInfo: only significant if abandon 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES: success
// - ANSWER__NO: abandon 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorProbeBlotvarRef(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTVAR_REFERENCE *ac_blotvarReference, int *ac_as,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  ac_blotvarReference->select.c_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence,"<int blotvar ref> | <str blotvar ref>")

  struct P_STRING name; // UNDEFINED
  ParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
  char b_blottabRef = (!b_EMPTY_P_STRING(lexeme)); 
  // Parse <entity> corresponding to blotreg or blottab name:
  ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&name); 
  ParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { 
    if (b_blottabRef) { // Parsing <int blottab ref> or <str blottab ref> ...
      switch (BlotexlibExecutorComputeBlottabOps(handle,b_L_VALUE,a_sequence,name,
        (struct BLOTEX_VALUE *)UNDEFINED, ac_blotvarReference, ac_as, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } else { // Parsing <int blotreg ref> or <str blotreg ref> ...
      switch (BlotexlibExecutorComputeBlotregOps(handle,b_L_VALUE,a_sequence,name,
        (struct BLOTEX_VALUE *)UNDEFINED, ac_blotvarReference, ac_as, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if

  } else {
    if (b_blottabRef) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // Parse <blotvar>:
    switch (BlotexlibExecutorParseSimpleBlotvar(handle,b_L_VALUE,a_sequence,&name,
      ac_blotvarReference, nc_abandonmentInfo)) {
    case ANSWER__YES: 
      m_ParsePassSpaces(a_sequence,NULL);
      // Expect <blotvar as int> | <blotvar id> | <blotvar as str> | <blotvar name>  
      switch (ParseAs(b_L_VALUE,a_sequence, ac_as)) {
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

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorProbeBlotvarRef

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
  char b_lValueBlotvarReference = b_FALSE0; // NO 'L-value' blotvar reference a priori
  struct BLOTVAR_REFERENCE c_lValueBlotvarReference; // UNDEFINED 
  int c_as = UNDEFINED;
  // int cn_blottabElement = UNDEFINED;

  m_PREPARE_ABANDON(&arguments, "Eval") 
  m_ParsePassSpaces(&arguments,NULL);
  { struct P_STRING subSequence; // UNDEFINED
    ParseTillMatch(&arguments,m_PString(":="),NULL, &subSequence);
    if (b_EMPTY_P_STRING(arguments)) arguments = subSequence; // No assignation
    else { // Assignation 
      b_lValueBlotvarReference = b_TRUE;
      switch (m_BlotexlibExecutorProbeBlotvarRef(handle,&subSequence,
        &c_lValueBlotvarReference, /* &cn_blottabElement,*/ &c_as, nc_abandonmentInfo)) {
      case ANSWER__YES: // <blotvar>
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
      m_ParsePassSpaces(&subSequence,NULL);
      if (!b_EMPTY_P_STRING(subSequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      ParseOffset(&arguments,2,NULL);
      m_ParsePassSpaces(&arguments,NULL);
    } // if
  } // subSequence

  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_ParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  if (b_lValueBlotvarReference) {
    G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
    switch (FetchBlotvar(&c_lValueBlotvarReference, b_L_VALUE,&cvnt_blotvarStuff,NULL)) {
    case RESULT__FOUND:
m_ASSERT(cvnt_blotvarStuff != NULL)
    break; case RESULT__NOT_FOUND:
      // TODO: cca va foirer avec ENTRY__BLOTVAR_REFERENCE
    break; default:
      m_TRACK() 
    } // switch
    if (cvnt_blotvarStuff == NULL) m_ABANDON(NOT_EXISTING__ABANDONMENT_CAUSE)
    switch (c_as) {
    case AS__VALUE_INT: // [ '#' ]
      if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
    break; case AS__ID: // '!' 
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__VALUE_STR: // '$'
      if (! c_blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      m_TRACK_IF(GStringCopy(cvnt_blotvarStuff+G_PARAM_VALUE_ELEMENT,0,
        c_blotexValue.select.c_str) < 0)
    break; case AS__NAME:  // '!$'
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; default: m_RAISE(ANOMALY__VALUE__D,c_as)
    } // switch
  } // if

  if (c_blotexValue.b_strex) {
    *ac_blotval = TRUE__BLOTVAL0;
m_DIGGY_VAR_GEN(*ac_blotval,ld)
  } else {
    *ac_blotval = c_blotexValue.select.c_blotval;
m_DIGGY_VAR_GEN(*ac_blotval,ld)
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorExecuteCFunctionEval


// IS_CHAR_FUNCTION:
static int IsFormatSpecifierChar(int c) {
  return (c == 'd' || c == 's' || c == 'x' || c == 'X' || c == 'L' || c == 'B' || c == 'E');
} // IsFormatSpecifierChar

// Terminal symbols (of <format> terminal symbol)
enum {
             D__FORMAT,
             LS__FORMAT,
             LX__FORMAT,
            UX__FORMAT,
            LE__FORMAT,
            BE__FORMAT,
} ;

// Parse <format> 
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *avn_format:
//   + -1 special value: when abandonned (invalid format) 
//   + >=0 : corresponding format 
// 
// Ret:
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'invalid format' error; abandon processing 
static inline int m_ParseFormat(struct P_STRING *a_sequence, int *avn_format,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED

  *avn_format = -1;
  m_PREPARE_ABANDON(a_sequence, "<format>") 
  m_ParsePassSpaces(a_sequence,NULL);
  ParsePassSingleChar(a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsFormatSpecifierChar,(char)UNDEFINED,
    &lexeme);
  int length = m_PStringLength(lexeme);
  switch (lexeme.string[0]) {
  case 'd': 
    if (length == 1) *avn_format = D__FORMAT;
  break; case 's':
    if (length == 1) *avn_format = LS__FORMAT;
  break; case 'x':
    if (length == 1) *avn_format = LX__FORMAT;
  break; case 'X':
    if (length == 1) *avn_format = UX__FORMAT;
  break; case 'L':
    if (length == 2 && lexeme.string[0] == 'E') *avn_format = LE__FORMAT;
  break; case 'B':
    if (length == 2 && lexeme.string[0] == 'E') *avn_format = BE__FORMAT;
  } // switch

  if (*avn_format < 0) m_ABANDON(INVALID_FORMAT__ABANDONMENT_CAUSE) 

  m_DIGGY_RETURN(ANSWER__YES)
} // m_ParseFormat

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
  switch(m_ParseFormat(&arguments,&n_format,nc_abandonmentInfo)) {
  case ANSWER__YES:
    break;
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_ASSERT(n_format >= 0)
     
  m_PREPARE_ABANDON(&arguments, "OutputF") 
  m_ParsePassSpaces(&arguments,NULL);
  struct P_STRING lexeme; // UNDEFINED
  ParsePassSingleChar(&arguments,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_ParsePassSpaces(&arguments,NULL);
  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_ParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  switch (n_format) {
  case D__FORMAT:
    if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%d", c_blotexValue.select.c_blotval);
  break; case LS__FORMAT:
    if (!c_blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,FMT_P_STRING,m_P_STRING_2_FMT_ARGS(
      c_blotexValue.select.c_str));
  break; case LX__FORMAT:
    if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%x",c_blotexValue.select.c_blotval);
  break; case UX__FORMAT:
    if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%X", c_blotexValue.select.c_blotval);
  break; case LE__FORMAT:
    if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case BE__FORMAT:
    if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
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

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blottabsHandle) != RETURNED)

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
