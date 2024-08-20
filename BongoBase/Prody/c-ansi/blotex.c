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

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlotregDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  G_STRINGS_HANDLE xh_handle = (G_STRINGS_HANDLE) xhr_handle;

  m_TRACK_IF(GStringsDestroyInstance(xh_handle) < 0)  

  m_DIGGY_RETURN(RETURNED) 
} // BlotregDestroyInstance 

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlottabDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  G_STRINGS_HANDLE xh_handle = (G_STRINGS_HANDLE) xhr_handle;

  m_TRACK_IF(GStringsDestroyInstance(xh_handle) < 0)  

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
int BlotcodeExecutorGetBlotexlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int blotexlibEntry,
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
} // BlotcodeExecutorGetBlotexlibExecutorHandle

// Blotregs: 

// Public function; see .h
int BlotexlibExecutorGetBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blotregName, G_STRINGS_HANDLE *ac_blotregHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__P_STRING(gKey,blotregName)
  int result = m_GStringsIndexSingleFetch(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlotregStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blotregHandle = ct_namedBlotregStuff->acolyt.cnhr_handle) != NULL)

static struct G_STRING *na_gStrings = NULL;
if (na_gStrings == NULL) m_TRACK_IF(GStringsFetch(*ac_blotregHandle,0, &na_gStrings) < 0)
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

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__P_STRING(gKey,blotregName)
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
        ACOLYT_TOKEN_ID__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
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

      t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
    } // h_blotregHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg

// Blottabs: 

// Public function; see .h
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottabName, G_STRINGS_HANDLE *ac_blottabHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__P_STRING(gKey,blottabName)
  int result = m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlottabStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blottabHandle = ct_namedBlottabStuff->acolyt.cnhr_handle) != NULL)
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlottab

// Public function; see .h
int BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING blottabName, G_STRINGS_HANDLE *na_blottabHandle) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__P_STRING(gKey,blottabName)
  int result = m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlottabStuff->acolyt.cnhr_handle != NULL)
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    { G_STRINGS_HANDLE h_blottabHandle = (G_STRINGS_HANDLE)UNDEFINED ;
      m_TRACK_IF(G_PARAMS_CREATE_INSTANCE(&h_blottabHandle,BATEAU__EXPECTED_ITEMS_NUMBER) !=
        RETURNED) 
      t_namedBlottabStuff->acolyt.cnhr_handle = h_blottabHandle;
    } // h_blottabHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blottabHandle != NULL) *na_blottabHandle = t_namedBlottabStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlottab


#define SYNTAX_ERROR__ABANDONMENT_CAUSE "Syntax error"
#define VALUE_ERROR__ABANDONMENT_CAUSE "Value error"
#define EXPECT_STREX__ABANDONMENT_CAUSE "Expect strex value"
#define EXPECT_INTEX__ABANDONMENT_CAUSE "Expect strex value"
#define UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_CAUSE "Unknown blotreg"
#define INVALID_FORMAT__ABANDONMENT_CAUSE "Invalid format"

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
  NAME__BLOTVAR_REFERENCE, // '.' <entity name>
  ENTRY__BLOTVAR_REFERENCE, // '[' <intex> ']'
  TOKEN_ID__BLOTVAR_REFERENCE, // '{' <intex> '}'
};

struct BLOTVAR_REFERENCE {
  G_STRINGS_HANDLE blotregHandle; 
  int blotvarReference;
  union {
    struct P_STRING c_name; // Only significant with NAME__BLOTVAR_REFERENCE
    int c_entry; // Only significant with ENTRY__BLOTVAR_REFERENCE
    int c_tokenId; // Only significant with TOKEN_ID__BLOTVAR_REFERENCE
  } select;
} ;


// IS_CHAR_FUNCTION:
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 


#define b_STREX b_TRUE


#define b_L_VALUE b_TRUE
#define b_R_VALUE b_FALSE0 


// Retrieve <blotvar> 
//
// Passed:
// - handle:
// - b_lValue: TRUE => create blotreg if not exist 
// - *a_blotvarSequence: before parsing 
//
// Changed:
// - *a_blotvarSequence: after parsing 
// - anp_entityName:
// - ac_blotvarReference: only significant if <blotvar> successfully parsed 
// - nc_abandonmentInfo: only significant if <blotvar> not succesfully parsed 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorRetrieveBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  struct P_STRING *a_blotvarSequence, const struct P_STRING *anp_entityName,
  struct BLOTVAR_REFERENCE *ac_blotvarReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_blotvarSequence) 
  ac_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_blotvarSequence, "<blotvar>") 

  m_PARSE_PASS_SPACES(*a_blotvarSequence,NULL) 
  if (anp_entityName == NULL) m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,
    b_PASS_CHARS_WHILE,IsEntityNameChar, UNDEFINED,&lexeme) 
  else lexeme = *anp_entityName;

  // Retrieve blotreg:
  if (b_lValue) {
    switch (BlotexlibExecutorCreateBlotreg(handle,lexeme,&ac_blotvarReference->blotregHandle)) {
    case COMPLETED__OK:
    case COMPLETED__BUT:
    break; default:
      m_TRACK()
    } // switch
  } else {
    switch (BlotexlibExecutorGetBlotreg(handle,lexeme,&ac_blotvarReference->blotregHandle)) {
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
      m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
    break; default:
      m_TRACK()
    } // switch
  } // if

  // ac_blotvarReference->blotregHandle established 
  // Retrieve blotvar reference:
  m_PARSE_OFFSET(*a_blotvarSequence,1,&lexeme)
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  GENERIC_INTEGER genericInteger = UNDEFINED;
  switch (lexeme.string[0]) {
  case '.' : // '.' <blotvar name>
    ac_blotvarReference->blotvarReference = NAME__BLOTVAR_REFERENCE;
    m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
      UNDEFINED,&lexeme)
    ac_blotvarReference->select.c_name = lexeme;
  break; case '[' : // '[' <intex> ']'
    ac_blotvarReference->blotvarReference = ENTRY__BLOTVAR_REFERENCE;
    m_PARSE_GENERIC_INTEGER(*a_blotvarSequence,genericInteger,&lexeme)
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // TODO: support empty or -1 entry when l-value (for smart fetch) 
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->select.c_entry = genericInteger;
    m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,']',&lexeme)
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; case '{' : // '{' <intex> '}' 
    ac_blotvarReference->blotvarReference = TOKEN_ID__BLOTVAR_REFERENCE;
    m_PARSE_GENERIC_INTEGER(*a_blotvarSequence,genericInteger,&lexeme)
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->select.c_tokenId = genericInteger;
    m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,'}',&lexeme)
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; default:
    m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  } // switch

  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorRetrieveBlotvar 


// Fetch blotvar 
//
// Passed:
// - handle:
// - b_lValue: TRUE => create blot if does not exist
// - ap_blotvarReference: 
// - acvnt_blotvarStuff:
// - nacvn_entry
// 
// Changed:
// - *acvnt_blotvarStuff: if R-value and not found, set to NULL
// - *nacvn_entry: if R-value and not found, set to -1  
//
// Ret: Found ? 
// - RESULT__FOUND:
// - RESULT__NOT_FOUND: blotvar created if L-value
// - -1: unexpected problem
static int BlotexlibExecutorFetchBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  const struct BLOTVAR_REFERENCE *ap_blotvarReference, G_STRING_SET_STUFF *acvnt_blotvarStuff,
  int *nacvn_entry) {
  m_DIGGY_BOLLARD_S()
//m_DIGGY_VAR_P_STRING(

  int ret =  UNDEFINED;
  int indexFetchFlags = INDEX_FETCH_FLAGS__SEEK_ONLY; 
  if (b_lValue) indexFetchFlags = INDEX_FETCH_FLAGS__FETCH; 
  *acvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (ap_blotvarReference->blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case TOKEN_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
m_DIGGY_VAR_P_STRING(ap_blotvarReference->select.c_name)
        m_ASSIGN_G_KEY__P_STRING(gKey,ap_blotvarReference->select.c_name)
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        m_ASSIGN_G_KEY__ACOLYT_TOKEN_ID(gKey,ap_blotvarReference->select.c_tokenId)
      } // if
      switch (ret = m_GStringsIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,indexLabel,
        INDEX_SEEK_FLAGS__EQUAL, &gKey, indexFetchFlags, acvnt_blotvarStuff, nacvn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
m_DIGGY_VAR_D(*nacvn_entry)
        if (b_lValue) { 
          m_ASSERT(*acvnt_blotvarStuff != NULL)
          if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE)
            m_TRACK_IF(GStringCopy((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,0,
              &(ap_blotvarReference->select.c_name)) < 0)
          else { // TOKEN_ID__BLOTVAR_REFERENCE 
            m_TRACK_IF(m_GStringAsGParamName((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,
              ap_blotvarReference->select.c_tokenId, ap_blotvarReference->blotregHandle) < 0)
          } // if
        } // if
      break; default:
        m_TRACK()
      } // switch
    } // gKey 
  break; case ENTRY__BLOTVAR_REFERENCE:
    ret = GStringsFetch(ap_blotvarReference->blotregHandle,ap_blotvarReference->select.c_entry,
      acvnt_blotvarStuff);
    m_TRACK_IF(ret < 0)
    m_ASSERT(ret == ap_blotvarReference->select.c_entry)
    ret = (acvnt_blotvarStuff == NULL? RESULT__NOT_FOUND: RESULT__NOT_FOUND);
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->blotvarReference)  
  } // switch

  m_DIGGY_RETURN(ret)
} // BlotexlibExecutorFetchBlotvar


struct BLOTEX_VALUE {
  char b_strex;
  union {
    gen_BLOTVAL c_blotval;
    struct P_STRING c_str;
  } select ;
} ;


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

// IS_CHAR_FUNCTION:
static int IsNameOrEntryBlotvarTerminalSymbol(int c) {
  return (c == '$' || c == '#');
} // IsNameOrEntryBlotvarTerminalSymbol 

// Parse blotvar reference as value of blotex atom
// expect: <blotvar> | <blotvar entry> | <blotvar id> | <blotvar strex> | <blotvar name>
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - entityName:
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" ; value corresponding to 
//   blotvar reference  
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlotexAtomBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING entityName, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  struct BLOTVAR_REFERENCE c_blotvarReference; // UNDEFINED 
  G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  int cvn_entry = UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED
  m_PREPARE_ABANDON(a_sequence, "(blotvar reference)") 

  ac_blotexValue->b_strex = b_FALSE0; // a priori
  // Parse <blotvar>:
  switch (BlotexlibExecutorRetrieveBlotvar(handle, b_R_VALUE, a_sequence, &entityName,
    &c_blotvarReference, nc_abandonmentInfo)) {
  case ANSWER__YES: // Parsed successfully    
    switch (BlotexlibExecutorFetchBlotvar(handle, b_R_VALUE,&c_blotvarReference,
      &cvnt_blotvarStuff,&cvn_entry)){
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
m_DIGGY_VAR_D(cvn_entry)
      m_ASSERT(cvn_entry == -1)
      m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE) 
    break; default:
      m_TRACK()
    } // switch
  break; case ANSWER__NO: // Parsed KO
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'!',&lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotvar id> | <blotvar entry> | <blotvar name>
    m_PARSE_PASS_SINGLE_CHAR(*a_sequence,IsNameOrEntryBlotvarTerminalSymbol,(char)UNDEFINED,
      &lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) {
      switch (lexeme.string[0]) {
      case '$': // <blotvar name>
        ac_blotexValue->b_strex = b_TRUE;
        ac_blotexValue->select.c_str =
          cvnt_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString; 
      break; case '#': // <blotvar entry>
        m_ASSERT(cvn_entry >= 0)
        ac_blotexValue->select.c_blotval = cvn_entry;
      break; default:
        m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
      } // switch
    } else { // <blotvar id>
      ac_blotexValue->select.c_blotval =
        cvnt_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value;
    } // if
  } else { // <blotvar strex> | <blotvar> 
    m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'$',&lexeme)
    if (b_EMPTY_P_STRING(lexeme)) { // <blotvar> (integer value)
      ac_blotexValue->select.c_blotval =
        cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
    } else { // <blotvar strex>
      ac_blotexValue->b_strex = b_TRUE;
      ac_blotexValue->select.c_str =
        cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString; 
    } // if
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlotexAtomBlotvar


enum {
  AS__VALUE_INT, // [ '#' ]
  AS__ENTRY_INT, // '!#' 
  AS__VALUE_STR, // '$'
  AS__NAME_STR,  // '!$'
} ;

// Parse "as" specifier (not present : default to '#" value int) 
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *a_as: (>=0) corresponding "as" specifier 
static inline int m_BlotexlibExecutorParseAs(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *a_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  m_PARSE_MATCH_C(*a_sequence,"#",NULL,&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) *a_as = AS__VALUE_INT;
  else {
    m_PARSE_MATCH_C(*a_sequence,"!#",NULL,&lexeme) 
    if (!b_EMPTY_P_STRING(lexeme)) *a_as = AS__ENTRY_INT;
    else {
      m_PARSE_MATCH_C(*a_sequence,"$",NULL,&lexeme) 
      if (!b_EMPTY_P_STRING(lexeme)) *a_as = AS__VALUE_STR;
      else {
m_DIGGY_VAR_P_STRING(*a_sequence)
        m_PARSE_MATCH_C(*a_sequence,"!$",NULL,&lexeme) 
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
        if (!b_EMPTY_P_STRING(lexeme)) *a_as = AS__NAME_STR;
        else  *a_as = AS__VALUE_INT;
      } // if
    } // if
  } // if
  m_DIGGY_RETURN(RETURNED)
} // m_BlotexlibExecutorParseAs 


// Parse <str request comp op>  
// Notice: also suitable for <request comp op>
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - b_str : TRUE => <str request comp op> ; FALSE => <request comp op>
//
// Changed:
// - *a_sequence: after parsing 
// - *an_indexSeekFlags: corresponding comp. op. (-1 special value if not found) 
static inline int m_BlotexlibExecutorParseRequestCompOp(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, char b_str, int *an_indexSeekFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  *an_indexSeekFlags = -1; // a priori
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
m_DIGGY_VAR_P_STRING(*a_sequence)
// TODO: faire un truc moins bourrin!!!!!
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'>',&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER; 
  else { 
    m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'<',&lexeme) 
    if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS; 
    else { 
      m_PARSE_MATCH_C(*a_sequence,">=",NULL,&lexeme) 
      if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER_EQUAL;
      else { 
        m_PARSE_MATCH_C(*a_sequence,"<=",NULL,&lexeme) 
        if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS_EQUAL;
        else { 
          m_PARSE_MATCH_C(*a_sequence,"==",NULL,&lexeme) 
          if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__EQUAL;
          else { 
            m_PARSE_MATCH_C(*a_sequence,"!=",NULL,&lexeme) 
            if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__NOT_EQUAL;
            else if (b_str) { 
              m_PARSE_MATCH_C(*a_sequence,"===",NULL,&lexeme) 
              if (!b_EMPTY_P_STRING(lexeme)) *an_indexSeekFlags = INDEX_SEEK_FLAGS__LIKE;
            } // if
          } // if
        } // if
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_BlotexlibExecutorParseRequestCompOp 

// Parse <logical 2op>  
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *a_criteriaFlags: corresponding op. flags (ALL_FLAG_OFF0 special value if not found) 
static inline int m_BlotexlibExecutorParseLogical2Op(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *a_criteriaOpFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PARSE_MATCH_C(*a_sequence,"and",NULL,&lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__AND;
  else { 
    m_PARSE_MATCH_C(*a_sequence,"or",NULL,&lexeme) 
    if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__OR;
    else *a_criteriaOpFlags = ALL_FLAGS_OFF0;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_BlotexlibExecuto@rParseLogical2Op

static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) ;

// Parse blotreg operations as value of blotex atom
// expect: <blotreg request> 
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - blotregHandle: 
//
// Changed:
// - *a_sequence: after parsing 
// - blotregHandle: 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlotregRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, G_STRINGS_HANDLE blotregHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blotreg request>") 
  struct P_STRING subSequence; 
  m_PARSE_TILL_MATCH_C(*a_sequence,":?",NULL, &subSequence)
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PARSE_OFFSET(*a_sequence,2,NULL)
  m_PRECISE_ABANDON(&subSequence, "<blotreg request atom>") 
  int criteriaNumber = 0;
  int as = UNDEFINED;  
  struct BLOTEX_VALUE blotexValue; // UNDEFINED
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  int indexSeekFlags = UNDEFINED;
  int criteriaOpFlags = UNDEFINED;
  struct G_KEY gKey; // UNDEFINED
  do {
    // <int blotreg request atom> | <str blotreg request atom>
    m_TRACK_IF(m_BlotexlibExecutorParseAs(handle,&subSequence,&as) != RETURNED)
m_DIGGY_VAR_D(as)
m_DIGGY_VAR_P_STRING(subSequence)
    m_TRACK_IF(m_BlotexlibExecutorParseRequestCompOp(handle,&subSequence,as != AS__VALUE_INT,
      &indexSeekFlags) != RETURNED)
m_DIGGY_VAR_INDEX_SEEK_FLAGS(indexSeekFlags)
    if (indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

    int blotregIndexLabel = NAME__BLOTREG_INDEX_LABEL; // a priori
    switch(BlotexlibExecutorComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo)) {
    case ANSWER__YES:
      switch (as) {
      case AS__VALUE_INT: // [ '#' ]
      case AS__ENTRY_INT: // '!#' 
        if (blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
        m_RAISE_IF(as == AS__ENTRY_INT, ANOMALY__NOT_AVAILABLE)
        m_ASSIGN_G_KEY__ACOLYT_VALUE(gKey,blotexValue.select.c_blotval) 
        blotregIndexLabel = INT_VALUE__BLOTREG_INDEX_LABEL; 
      break; case AS__VALUE_STR: // '$'
        blotregIndexLabel = STR_VALUE__BLOTREG_INDEX_LABEL; 
      case AS__NAME_STR:  // '!$'
m_DIGGY_VAR_P_STRING(blotexValue.select.c_str)
        if (!blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
        m_ASSIGN_G_KEY__P_STRING(gKey,blotexValue.select.c_str)
      break; default:
        m_RAISE(ANOMALY__VALUE__D,as)
      } // switch
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    
    m_TRACK_IF(m_BlotexlibExecutorParseLogical2Op(handle, &subSequence,
      &criteriaOpFlags) != RETURNED)

    m_ASSIGN_G_REQUEST_CRITERIUM__G_KEYS(criteria5[criteriaNumber++],blotregIndexLabel,
      indexSeekFlags,&gKey, criteriaOpFlags)
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch (GStringsIndexRequestR(blotregHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlotregRequest


// Parse blotreg operations as value of blotex atom
// expect: <blotreg ops int> | <blotreg ops str> 
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - entityName: blotreg entity name
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" ; value corresponding to 
//   blotreg ops 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeBlotregOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING entityName, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<blotex int ops> | <blotreg str ops>") 

  G_STRINGS_HANDLE blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  switch (BlotexlibExecutorGetBlotreg(handle,entityName,&blotregHandle)) {
  case RESULT__FOUND:
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  ac_blotexValue->b_strex = b_FALSE0; // a priori 
  ac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  int n_indexFetchFlags = -1; // a priori TODO: unsigned ?
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,':',&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op select>
    // Parse <blotreg request>
    switch (m_BlotexlibExecutorComputeBlotregRequest(handle,a_sequence,blotregHandle,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
  } // if
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'^',&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op reset>
    n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
  } // if
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'+',&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op next>
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
    m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
  } // if
  int n_as = -1; 
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'=',&lexeme) 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op read int> or <blotreg op read str>
    m_TRACK_IF(m_BlotexlibExecutorParseAs(handle,a_sequence,&n_as) != RETURNED)
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (n_indexFetchFlags < 0) {
    ac_blotexValue->b_strex = b_FALSE0; 
    ac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
  } else {
    G_STRING_SET_STUFF ct_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
    int c_entry = UNDEFINED;

m_DIGGY_INFO("Before GStringsIndexFetch()...")
m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags)
    switch (GStringsIndexFetch(blotregHandle,NULL,n_indexFetchFlags,
      &ct_blotvarStuff, &c_entry)) {
    case RESULT__FOUND:
m_DIGGY_VAR_D(n_as)
      switch (n_as) {
      case AS__VALUE_INT:
        ac_blotexValue->b_strex = b_FALSE0; 
        ac_blotexValue->select.c_blotval = ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
      break; case AS__ENTRY_INT:
        ac_blotexValue->b_strex = b_FALSE0; 
        ac_blotexValue->select.c_blotval = c_entry; 
      break; case AS__VALUE_STR:
m_DIGGY_VAR_P_STRING(ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString)
m_DIGGY_VAR_D((int)ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value)
m_DIGGY_VAR_P_STRING(ct_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString)
m_DIGGY_VAR_D((int)ct_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value)
        ac_blotexValue->b_strex = b_TRUE; 
        ac_blotexValue->select.c_str = ct_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString ; 
      break; case AS__NAME_STR:
        ac_blotexValue->b_strex = b_TRUE; 
        ac_blotexValue->select.c_str = ct_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString ; 
      break; default: 
        m_TRACK()
      } // switch

    break; case RESULT__NOT_FOUND:
      if ((ac_blotexValue->b_strex = (n_as == AS__VALUE_STR || n_as == AS__NAME_STR)))
        m_ASSIGN_EMPTY_P_STRING(ac_blotexValue->select.c_str)
      else ac_blotexValue->select.c_blotval = FALSE__BLOTVAL; 
    break; default:
      m_TRACK()
    } // switch
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // m_BlotexlibExecutorComputeBlotregOps


// Parse <fact> | <strex term>  
// That is, parse ( '(' <intex> | <strex> ')' ) | <intex atom> | <strex atom>
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
  struct P_STRING *a_sequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence, "<fact> | <strex term>") 
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 

  int n_int1Op = -1; // No <int 1op> a priori
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,IsInt1Op,(char)UNDEFINED,&lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT_1OP;
    break; case '+' : n_int1Op = PLUS__INT_1OP;
    break; case '-' : n_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    m_PARSE_PASS_SPACES(*a_sequence,NULL)
  } // if

m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PARSE_GENERIC_INTEGER(*a_sequence,ac_blotexValue->select.c_blotval,&lexeme)
m_DIGGY_VAR_LD(ac_blotexValue->select.c_blotval)
m_DIGGY_VAR_P_STRING(lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) ac_blotexValue->b_strex = b_FALSE0; // <int constant>
  else { 
    m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'"',&lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) { // <str constant> 
      ac_blotexValue->b_strex = b_TRUE;
      m_PARSE_PASS_CHARS(*a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"',
        &(ac_blotexValue->select.c_str))
      if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      m_PARSE_OFFSET(*a_sequence,1,NULL)
    } else { 
      m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'(',&lexeme)
      if (!b_EMPTY_P_STRING(lexeme)) { // '(' ... 
        switch(BlotexlibExecutorComputeBlotex(handle,a_sequence,ac_blotexValue,
          nc_abandonmentInfo)) {
        case ANSWER__YES:
          m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,')',&lexeme)
          if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      } else {
        struct P_STRING entityNameLexeme; // UNDEFINED
        m_PARSE_PASS_CHARS(*a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
          UNDEFINED,&entityNameLexeme) // <entity name>
        m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'?',&lexeme)
        if (!b_EMPTY_P_STRING(lexeme)) { // '?' terminal symbol 
          // expect: <blotreg ops int> | <blotreg ops str> 
          switch (m_BlotexlibExecutorComputeBlotregOps(handle,a_sequence,entityNameLexeme,
            ac_blotexValue, nc_abandonmentInfo)) {
          case ANSWER__YES:
          break; case ANSWER__NO:
            m_DIGGY_RETURN(ANSWER__NO)
          break; default: 
            m_TRACK()
          } // switch
        } else { // '?' not found
          // expect: <blotvar> | <blotvar entry> | <blotvar id> | <blotvar strex> | <blotvar name>
          switch (m_BlotexlibExecutorComputeBlotexAtomBlotvar(handle,a_sequence,entityNameLexeme,
            ac_blotexValue, nc_abandonmentInfo)) {
          case ANSWER__YES:
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
    if (ac_blotexValue->b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
    switch (n_int1Op) {
    break; case NOT__INT_1OP:
      if (ac_blotexValue->select.c_blotval == TRUE__BLOTVAL0)
        ac_blotexValue->select.c_blotval = FALSE__BLOTVAL;
      else ac_blotexValue->select.c_blotval = TRUE__BLOTVAL0;
    break; case PLUS__INT_1OP:
    break; case MINUS__INT_1OP:
      ac_blotexValue->select.c_blotval = -(ac_blotexValue->select.c_blotval);
    break; default:
      m_RAISE(ANOMALY__VALUE__D,n_int1Op)
    } // switch
  } // if 

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexlibExecutorProbeBlotexAtom

// IS_CHAR_FUNCTION:/
static int IsInt2OpChar(int c) {
  return (c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '!' || c == '>' || c ==
    '<');
} // IsInt2OpChar

// Terminal symbols (of <int 2op> terminal symbol)
enum {
            ADD__INT_2OP,
       SUBTRACT__INT_2OP,
       MULTIPLY__INT_2OP,
         DIVIDE__INT_2OP,
            AND__INT_2OP,
             OR__INT_2OP,
          EQUAL__INT_2OP,
           LESS__INT_2OP,
     LESS_EQUAL__INT_2OP,
        GREATER__INT_2OP,
  GREATER_EQUAL__INT_2OP,
      NOT_EQUAL__INT_2OP,
} ;

// Parse <int 2op> if present
//
// Passed:
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_int2Op:
//   + -1: special value: <int 2op> NOT present
//   + >=0 : corresponding int 2op 
static inline int m_BlotexlibExecutorParseInt2Op(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *an_int2Op) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  struct P_STRING initialSequence = *a_sequence;

  *an_int2Op = -1;
  m_PARSE_PASS_CHARS(*a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsInt2OpChar,UNDEFINED,
    &lexeme) 
m_DIGGY_VAR_P_STRING(lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) { 
      switch (lexeme.string[0]) {
      case '+': 
        if (m_PStringLength(&lexeme) == 1) *an_int2Op = ADD__INT_2OP; // TODO: FOIREUX !!!! "++" sera rejeted !!!!!!!!!!!!!!!
      break; case '-':
        if (m_PStringLength(&lexeme) == 1) *an_int2Op = SUBTRACT__INT_2OP;
      break; case '*':
        if (m_PStringLength(&lexeme) == 1) *an_int2Op = MULTIPLY__INT_2OP;
      break; case '/':
        if (m_PStringLength(&lexeme) == 1) *an_int2Op = DIVIDE__INT_2OP;
      break; case '&':
        switch (m_PStringLength(&lexeme)) {
        case 2: 
          switch (lexeme.string[1]) {
          case '&': *an_int2Op = AND__INT_2OP;
          break; 
          } // switch  
        break; 
        } // switch  
      break; case '!':
        switch (m_PStringLength(&lexeme)) {
        case 2: 
          switch (lexeme.string[1]) {
          case '!': *an_int2Op = OR__INT_2OP;
          break; case '=': *an_int2Op = NOT_EQUAL__INT_2OP;
          break; 
          } // switch  
        break; 
        } // switch  
      break; case '>':
        switch (m_PStringLength(&lexeme)) {
        case 1: *an_int2Op = GREATER__INT_2OP; 
        break; case 2: 
          if (lexeme.string[1] == '=') *an_int2Op = GREATER_EQUAL__INT_2OP;
        break; 
        } // switch  
      break; case '<':
        switch (m_PStringLength(&lexeme)) {
        case 1: *an_int2Op = LESS__INT_2OP; 
        break; case 2: 
          if (lexeme.string[1] == '=') *an_int2Op = LESS_EQUAL__INT_2OP;
        break; 
        } // switch  
      break; case '=':
        if (m_PStringLength(&lexeme) == 1) *an_int2Op = EQUAL__INT_2OP;
      break; 
        m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
      } // switch
    } // if  
m_DIGGY_VAR_D(*an_int2Op)
  
  if (*an_int2Op == -1) *a_sequence = initialSequence;
  m_DIGGY_RETURN(RETURNED)
} // m_BlotexlibExecutorParseInt2Op

// Compute full intex ; that is
// Parse residue of <intex> after 1st <intex atom> and if such residue exist compute it.
// Otherwise, full intex simply corresponds to intex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - atomBlotexValue: 1st intex atom
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex> NOT intex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeFullIntex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE atomBlotexValue,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<intex>") 
  int n_int2Op = UNDEFINED;
  *ac_blotexValue = atomBlotexValue;
  while (b_TRUE) {
    m_TRACK_IF(m_BlotexlibExecutorParseInt2Op(handle,a_sequence,&n_int2Op) != RETURNED)
m_DIGGY_VAR_D(n_int2Op)
    if (n_int2Op == -1) break;
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&atomBlotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
m_DIGGY_VAR_D(atomBlotexValue.b_strex)
      if (atomBlotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      switch (n_int2Op) {
      case ADD__INT_2OP:
        ac_blotexValue->select.c_blotval += atomBlotexValue.select.c_blotval;
      break; case SUBTRACT__INT_2OP:
        ac_blotexValue->select.c_blotval -= atomBlotexValue.select.c_blotval;
      break; case MULTIPLY__INT_2OP:
        ac_blotexValue->select.c_blotval *= atomBlotexValue.select.c_blotval;
      break; case DIVIDE__INT_2OP:
        ac_blotexValue->select.c_blotval = ac_blotexValue->select.c_blotval /
          atomBlotexValue.select.c_blotval;
      break; case AND__INT_2OP:
        ac_blotexValue->select.c_blotval = ac_blotexValue->select.c_blotval && 
          atomBlotexValue.select.c_blotval;
      break; case OR__INT_2OP:
        ac_blotexValue->select.c_blotval = ac_blotexValue->select.c_blotval || 
          atomBlotexValue.select.c_blotval;
      break; case EQUAL__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval ==
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; case LESS__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval <
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; case LESS_EQUAL__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval <=
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; case GREATER__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval >
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; case GREATER_EQUAL__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval >=
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; case NOT_EQUAL__INT_2OP:
        ac_blotexValue->select.c_blotval = (ac_blotexValue->select.c_blotval !=
          atomBlotexValue.select.c_blotval? TRUE__BLOTVAL0: FALSE__BLOTVAL);
      break; default:
        m_RAISE(ANOMALY__VALUE__D,n_int2Op) 
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
// Parse residue of <strex> after 1st <strex atom> and if such residue exist compute it.
// Otherwise, full strex simply corresponds to strex atom.
//
// Passed:
// - handle:
// - *a_sequence: before parsing
// - atomBlotexValue: 1st intex atom
//
// Changed:
// - *a_sequence: after parsing 
// - ac_blotexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "processing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'value' error (next <blotex> NOT intex) ; abandon processing 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorComputeFullStrex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct BLOTEX_VALUE atomBlotexValue,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<strex>") 

  *ac_blotexValue = atomBlotexValue;
  struct P_STRING lexeme; // UNDEFINED
  while (b_TRUE) {
    m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'+',&lexeme) // Try <str term op> ...
    if (b_EMPTY_P_STRING(lexeme)) break;
  
    switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&atomBlotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
      if (!atomBlotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
      m_TRACK_IF(GStringsFetch(handle->h_workingGStringsHandle,-1, &t_workingGStringStuff) < 0)
      m_TRACK_IF(GStringCopy(t_workingGStringStuff,0, &ac_blotexValue->select.c_str) < 0)
      m_TRACK_IF(GStringCopy(t_workingGStringStuff,-1, &atomBlotexValue.select.c_str) < 0)
      ac_blotexValue->select.c_str = t_workingGStringStuff->cv_pString;
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

  struct BLOTEX_VALUE atomBlotexValue; // UNDEFINED

  switch (BlotexlibExecutorProbeBlotexAtom(handle,a_sequence,&atomBlotexValue,
    nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  if (atomBlotexValue.b_strex) { // <strex>
    switch (m_BlotexlibExecutorComputeFullStrex(handle,a_sequence,atomBlotexValue,ac_blotexValue,
      nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch

  } else { // <intex>
    switch (m_BlotexlibExecutorComputeFullIntex(handle,a_sequence,atomBlotexValue,ac_blotexValue,
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


enum {
      EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 

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
// - handle:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *avn_int2Op:
//   + -1 special value: when abandonned (invalid format) 
//   + >=0 : corresponding format 
// 
// Ret:
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'invalid format' error; abandon processing 
static inline int m_BlotexlibExecutorParseFormat(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, int *avn_format, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED

  *avn_format = -1;
  m_PREPARE_ABANDON(a_sequence, "<format>") 
  m_PARSE_PASS_SPACES(*a_sequence,NULL)
  m_PARSE_PASS_SINGLE_CHAR(*a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PARSE_PASS_CHARS(*a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsFormatSpecifierChar,UNDEFINED,
    &lexeme)
  int length = m_PStringLength(&lexeme);
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
} // m_BlotexlibExecutorParseFormat

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
  char b_blotvarReference = b_FALSE0; // a priori
  struct BLOTVAR_REFERENCE c_blotvarReference; // UNDEFINED 
  char cb_strex = (char)UNDEFINED; 

  m_PREPARE_ABANDON(&arguments, "Eval") 
  m_PARSE_PASS_SPACES(arguments,NULL)
  { struct P_STRING blotvarSequence; // UNDEFINED
    m_PARSE_TILL_MATCH_C(arguments,":=",NULL, &blotvarSequence)
    if (b_EMPTY_P_STRING(arguments)) { // NO ':=' 
      arguments = blotvarSequence;
    } else { // <blotvar> [ '$' ] ':=' <blotex>
      b_blotvarReference = b_TRUE;
      switch (BlotexlibExecutorRetrieveBlotvar(handle,b_L_VALUE,&blotvarSequence,NULL,
        &c_blotvarReference, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
      m_PARSE_PASS_SPACES(blotvarSequence,NULL)
      struct P_STRING lexeme; // UNDEFINED
      m_PARSE_PASS_SINGLE_CHAR(blotvarSequence,NULL,'$',&lexeme);
      cb_strex = (m_PStringLength(&lexeme) == 1); 
      m_PARSE_OFFSET(arguments,2,NULL)
      m_PARSE_PASS_SPACES(arguments,NULL)
    } // if
  } // blotvarSequence

  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PARSE_PASS_SPACES(arguments,NULL)
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

  if (b_blotvarReference) {
    G_STRING_SET_STUFF t_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
    switch (BlotexlibExecutorFetchBlotvar(handle, b_L_VALUE,&c_blotvarReference,
      &t_blotvarStuff,NULL)) {
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
      // TODO: cca va foirer avec ENTRY__BLOTVAR_REFERENCE
      m_ASSERT(t_blotvarStuff != NULL)
    break; default:
      m_TRACK() 
    } // switch

m_DIGGY_VAR_P_STRING(t_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString)
m_DIGGY_VAR_D(t_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.c_tokenId)
m_DIGGY_VAR_P_STRING(t_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString)
m_DIGGY_VAR_D(t_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.c_tokenId)
    if (cb_strex) {
      if (! c_blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
      m_TRACK_IF(GStringCopy(t_blotvarStuff+G_PARAM_VALUE_ELEMENT,0,
        &c_blotexValue.select.c_str) < 0)
    } else {
      if (c_blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
      t_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
    } // if
m_DIGGY_INFO("====== AFTER =======")
m_DIGGY_VAR_P_STRING(t_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString)
m_DIGGY_VAR_D(t_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.c_tokenId)
m_DIGGY_VAR_P_STRING(t_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString)
m_DIGGY_VAR_D(t_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.c_tokenId)
m_DIGGY_VAR_P(&(t_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_pString))
m_DIGGY_VAR_P(&(t_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_pString))
  } // if

  if (c_blotexValue.b_strex) {
    *ac_blotval = TRUE__BLOTVAL0;
m_DIGGY_VAR_LD(*ac_blotval)
  } else {
    *ac_blotval = c_blotexValue.select.c_blotval;
m_DIGGY_VAR_LD(*ac_blotval)
  } // if

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
  switch(m_BlotexlibExecutorParseFormat(handle,&arguments,&n_format,nc_abandonmentInfo)) {
  case ANSWER__YES:
    break;
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch
  m_ASSERT(n_format >= 0)
     
  m_PREPARE_ABANDON(&arguments, "OutputF") 
  m_PARSE_PASS_SPACES(arguments,NULL)
  struct P_STRING lexeme; // UNDEFINED
  m_PARSE_PASS_SINGLE_CHAR(arguments,NULL,',',&lexeme)
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PARSE_PASS_SPACES(arguments,NULL)
  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PARSE_PASS_SPACES(arguments,NULL)
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

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
static int BlotexlibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE xh_handle = (BLOTEXLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,xh_handle)

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_workingGStringsHandle) != RETURNED)

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorDestroyInstance


// Public function; see .h
int BlotcodeLinkBlotexlib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE fp_blotexlibExecutorFactoryHandle) {
  m_DIGGY_BOLLARD()

  int entry = BlotcodeLinkBlotlib(ep_handle, nfp_blotlibPrefix,
    BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER, localBlotfuncNames, 
    BlotexlibExecutorFactoryCreateProductInstance, fp_blotexlibExecutorFactoryHandle,
    BlotexlibExecutorExecuteCFunction, BlotexlibExecutorDestroyInstance) ;
  m_TRACK_IF(entry < 0)

  m_DIGGY_RETURN(entry)
} // BlotcodeLinkBlotexlib

