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
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction ;
} ; 

 
// Public function; see .h
int BlotexlibExecutorFactoryCreateInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE *azh_handle,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION blottabDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,*azh_handle)
  (*azh_handle)->blottabDestroyInstanceFunction = blottabDestroyInstanceFunction;
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

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blottabsHandle), 10,
    p_handle->blottabDestroyInstanceFunction) != RETURNED)
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
int BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE *ac_blottabHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blottabName);

  int result = m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlottabStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blottabHandle = (BLOTTAB_HANDLE)ct_namedBlottabStuff->acolyt.cnhr_handle) != NULL);
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlottab

// Public function; see .h
int BlotexlibExecutorAddBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE h_blottabHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blottabName)
m_DIGGY_VAR_P(handle->h_blottabsHandle)
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blottabName);
  switch (m_GStringsIndexSingleFetch(handle->h_blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL)) {
  case RESULT__FOUND:
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(GStringCopy(t_namedBlottabStuff,0,blottabName) < 0)
    m_TRACK_IF(m_GStringAsNamedObject(t_namedBlottabStuff,h_blottabHandle,
      handle->h_blottabsHandle) != RETURNED)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorAddBlottab


// Blot expressions parsing:


// IS_CHAR_FUNCTION:
int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 


#define b_STREX b_TRUE


#define b_L_VALUE b_TRUE
#define b_R_VALUE b_FALSE0 


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
  ac_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence,"<blotvar>") 

  m_PParsePassSpaces(a_sequence,NULL); 
  // Parse <entity> corresponding to register name if asked:
  if (nap_blotregName == NULL) PParsePassChars(a_sequence,b_REGULAR_SCAN,
    b_PASS_CHARS_WHILE,IsEntityNameChar, (char)UNDEFINED,&lexeme); 
  else lexeme = *nap_blotregName;

  // Retrieve blotreg corresponding to register name:
  if ((b_lValue)) {
    switch (BlotexlibExecutorCreateBlotreg(handle,lexeme,
      &ac_blotvarReference->blotregHandle)) {
    case COMPLETED__OK:
    case COMPLETED__BUT:
    break; default:
      m_TRACK()
    } // switch
  } else {
    switch (BlotexlibExecutorGetBlotreg(handle,lexeme,
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
      switch (result = m_GStringsIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,
        indexLabel,INDEX_SEEK_FLAGS__EQUAL,&gKey,b_lValue?  INDEX_FETCH_FLAGS__FETCH:
        INDEX_FETCH_FLAGS__SEEK_ONLY, acvnt_blotvarStuff, nacvn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        if (b_lValue) { 
          m_ASSERT(*acvnt_blotvarStuff != NULL)
          if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE)
            m_TRACK_IF(GStringCopy((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,0,
              ap_blotvarReference->c_select.c_name) < 0)
          else { // TOKEN_ID__BLOTVAR_REFERENCE 
            m_TRACK_IF(m_GStringAsGParamName((*acvnt_blotvarStuff)+G_PARAM_NAME_ELEMENT,
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
      ap_blotvarReference->c_select.c_entry, acvnt_blotvarStuff);
    m_TRACK_IF(ret < 0)
m_ASSERT(ret == ap_blotvarReference->c_select.c_entry)
    result = (acvnt_blotvarStuff == NULL? RESULT__NOT_FOUND: RESULT__NOT_FOUND);
  break; case CURRENT__BLOTVAR_REFERENCE:
m_ASSERT(b_lValue)
    switch (result = GStringsIndexFetch(ap_blotvarReference->blotregHandle,NULL,
      INDEX_FETCH_FLAGS__CURRENT, acvnt_blotvarStuff, nacvn_entry)){
    case RESULT__FOUND:
    break; case RESULT__NOT_FOUND:
    break; default: m_TRACK() 
    } // switch   
 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->blotvarReference)  
  } // switch

  m_DIGGY_RETURN(result)
} // FetchBlotvar

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
  m_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,a_as,&lexeme, 5,
    AS__R_VALUE__ENTRY__XX,b_lValue? -1:AS__R_VALUE__ENTRY,  AS__NAME__XX,AS__NAME,
    AS__VALUE_INT__X,AS__VALUE_INT,  AS__ID__X,AS__ID,  AS__VALUE_STR__X,AS__VALUE_STR)
  if (*a_as == -1) *a_as = AS__VALUE_INT;
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*a_as)
  m_DIGGY_RETURN(RETURNED)
} // ParseAs 

// See .h 
int ParseAsValue(struct P_STRING *a_sequence, int *an_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_as,&lexeme, 2,
    AS__VALUE_INT__X,AS__VALUE_INT, AS__VALUE_STR__X,AS__VALUE_STR)
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*an_as)
  m_DIGGY_RETURN(RETURNED)
} // ParseAsValue 


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
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_compOp,&lexeme, 9,
    "===", compOpExtension == STR__COMP_OP_EXTENSION? LIKE__STR__COMP_OP: -1,
    "!=",NOT_EQUAL__COMP_OP, ">=",GREATER_EQUAL__COMP_OP, "<=",LESS_EQUAL__COMP_OP,
    "==",EQUAL__COMP_OP, "<",LESS__COMP_OP, ">",GREATER__COMP_OP,
    "*",compOpExtension == FACT_OP__COMP_OP_EXTENSION? MULTIPLY__FACT_OP__COMP_OP: -1,
    "/",compOpExtension == FACT_OP__COMP_OP_EXTENSION? DIVIDE__FACT_OP__COMP_OP: -1)
  m_DIGGY_RETURN(RETURNED)
} // ParseCompOp


// 
int ParseRequestCompOp(struct P_STRING *a_sequence, char b_str,
  int *an_indexSeekFlags) {
  m_DIGGY_BOLLARD()
  *an_indexSeekFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);
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
} // ParseRequestCompOp 

//
int ParseLogical2Op(struct P_STRING *a_sequence,
  int *a_criteriaOpFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PParsePassSpaces(a_sequence,NULL);
  PParseMatch(a_sequence,m_PString("and"),NULL,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__AND;
  else { 
    PParseMatch(a_sequence,m_PString("or"),NULL,&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__OR;
    else *a_criteriaOpFlags = ALL_FLAGS_OFF0;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ParseLogical2Op


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

  PParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);
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
    PParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
m_DIGGY_VAR_P_STRING(lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) n_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
    else {  // select with actual criterium
m_DIGGY_VAR_P_STRING(lexeme)
      m_TRACK_IF(ParseRequestCompOp(&subSequence,as != AS__VALUE_INT,
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
    m_TRACK_IF(ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)
m_DIGGY_VAR_P_STRING(subSequence)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterium_GKeys(blotregIndexLabel,
      n_indexSeekFlags,&gKey, criteriaOpFlags);
    m_PParsePassSpaces(&subSequence,NULL);
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
// - as l-value of blotvar reference => expect <blotreg ref op set int> | <blotreg ref op set str> 
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

  m_PREPARE_ABANDON(a_sequence, b_lValue? "<blotreg ref op set int> | <blotreg ref op set str>":
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
    cac_blotvarReference->blotregHandle = blotregHandle; 
    cac_blotvarReference->blotvarReference = CURRENT__BLOTVAR_REFERENCE;
    *cac_as = UNDEFINED; // For the moment 
  } else {
    cac_blotexValue->b_strex = b_FALSE0; // a priori 
    cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  } // if
  int n_indexFetchFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);

  if (!b_lValue) {
    PParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
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
    PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op reset>...
      n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
    } // if
    PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op next>...
      if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
      m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
    } // if
  } // if
  int n_as = -1; // No blotreg read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
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
  m_PParsePassSpaces(a_sequence,NULL); // TODO: ag virer ??????

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
  PParseGenericInteger(a_sequence,&(ac_blotexAtomValue->select.c_blotval),&lexeme);
m_DIGGY_VAR_GEN(ac_blotexAtomValue->select.c_blotval,ld)
m_DIGGY_VAR_P_STRING(lexeme)
  if (!b_EMPTY_P_STRING(lexeme)) ac_blotexAtomValue->b_strex = b_FALSE0; // <int constant>
  else { 
    PParsePassSingleChar(a_sequence,NULL,'"',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <str constant> ... 
      ac_blotexAtomValue->b_strex = b_TRUE;
      PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"',
        &(ac_blotexAtomValue->select.c_str));
      if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
      PParseOffset(a_sequence,1,NULL);
    } else { 
      PParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { 
        switch(BlotexlibExecutorComputeBlotex(handle,a_sequence,ac_blotexAtomValue,
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
        struct P_STRING name; // UNDEFINED
        PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
        char b_blottab = (!b_EMPTY_P_STRING(lexeme)); 
        PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
          (char)UNDEFINED,&name); // <entity>
        PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
        if (!b_EMPTY_P_STRING(lexeme)) { 
          if (b_blottab) { // <int blottab> | <str blottab> ...
            switch (l_BlotexlibExecutorComputeBlottabOps(handle,b_R_VALUE,a_sequence,name,
              ac_blotexAtomValue,(struct BLOTTAB_FIELD_REFERENCE *)UNDEFINED,(int *)UNDEFINED,
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
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&c_matchedEntry,an_termOp,&lexeme, 4,
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
  m_PParsePassSpaces(a_sequence,NULL);
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
    PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
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

//
int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
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

  m_PParsePassSpaces(a_sequence,NULL);
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


// Probe blotex reference (usable as 'l-value') ; only blotex reference is established. 
//
// Passed:
// - handle:
// - *a_sequence: expect <int blotex ref> | <str blotex ref> 
//
// Changed:
// - *a_sequence: after parsing 
// - *acb_blottabFieldReference: only significant if success blottab (field) ref  ?
// - *acc_blotvarReference: only significant if success, NOT blottab (field) ref case
// - *acc_blottabFieldReference: only significant if success, blottab (field) ref case
// - *ac_as: only significant if success
// - nc_abandonmentInfo: only significant if abandon 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES: success
// - ANSWER__NO: abandon 
// - -1: unexpected problem
static inline int m_BlotexlibExecutorProbeBlotexRef(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, char *acb_blottabFieldReference, struct BLOTVAR_REFERENCE *acc_blotvarReference,
  struct BLOTTAB_FIELD_REFERENCE *acc_blottabFieldReference, int *ac_as,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence) 
  acc_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct P_STRING lexeme; // UNDEFINED

  m_PREPARE_ABANDON(a_sequence,"<int blotex ref> | <str blotex ref>")

  struct P_STRING name; // UNDEFINED
  PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
  *acb_blottabFieldReference = (!b_EMPTY_P_STRING(lexeme)); 
  // Parse <entity> corresponding to blotreg or blottab name:
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&name); 
  PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { 
    if (*acb_blottabFieldReference) { // Parsing <int blottab ref> or <str blottab ref> ...
      switch (l_BlotexlibExecutorComputeBlottabOps(handle,b_L_VALUE,a_sequence,name,
        (struct BLOTEX_VALUE *)UNDEFINED, acc_blottabFieldReference, ac_as, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } else { // Parsing <int blotreg ref> or <str blotreg ref> ...
      switch (BlotexlibExecutorComputeBlotregOps(handle,b_L_VALUE,a_sequence,name,
        (struct BLOTEX_VALUE *)UNDEFINED, acc_blotvarReference, ac_as, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if

  } else {
    if (*acb_blottabFieldReference) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // Parse <blotvar>:
    switch (BlotexlibExecutorParseSimpleBlotvar(handle,b_L_VALUE,a_sequence,&name,
      acc_blotvarReference, nc_abandonmentInfo)) {
    case ANSWER__YES: 
      m_PParsePassSpaces(a_sequence,NULL);
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
  char cb_lValueBlottabFieldReference = (char)UNDEFINED;
  struct BLOTVAR_REFERENCE cc_lValueBlotvarReference; // UNDEFINED 
  struct BLOTTAB_FIELD_REFERENCE cc_lValueBlottabFieldReference;
  int c_as = UNDEFINED;

  m_PREPARE_ABANDON(&arguments, "Eval") 
  m_PParsePassSpaces(&arguments,NULL);
  { struct P_STRING subSequence; // UNDEFINED
    PParseTillMatch(&arguments,m_PString(":="),NULL, &subSequence);
    if (b_EMPTY_P_STRING(arguments)) arguments = subSequence; // No assignation
    else { // Assignation 
      b_lValueReference = b_TRUE;
      switch (m_BlotexlibExecutorProbeBlotexRef(handle,&subSequence, &cb_lValueBlottabFieldReference,
        &cc_lValueBlotvarReference, &cc_lValueBlottabFieldReference, &c_as, nc_abandonmentInfo)) {
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

  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default:
    m_TRACK()
  } // switch

  m_PParsePassSpaces(&arguments,NULL);
  if (!b_EMPTY_P_STRING(arguments)) m_ABANDON(NOT_PARSABLE__ABANDONMENT_CAUSE)

  if (b_lValueReference) {
    if (cb_lValueBlottabFieldReference) {
      switch (UpdateBlottabField(&cc_lValueBlottabFieldReference, c_as, c_blotexValue)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
      break; default:
        m_TRACK() 
      } // switch
    } else {
      G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
      switch (FetchBlotvar(&cc_lValueBlotvarReference, b_L_VALUE,&cvnt_blotvarStuff,NULL)) {
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
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassSingleChar(a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsFormatSpecifierChar,(char)UNDEFINED,
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
  m_PParsePassSpaces(&arguments,NULL);
  struct P_STRING lexeme; // UNDEFINED
  PParsePassSingleChar(&arguments,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PParsePassSpaces(&arguments,NULL);
  struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
  switch (BlotexlibExecutorComputeBlotex(handle,&arguments,&c_blotexValue,nc_abandonmentInfo)) {
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
