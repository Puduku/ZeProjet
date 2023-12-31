// blotex.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

//
// 
// NOTE 2: blotex is a "context free" blot functions library (do not impose specific BLOT_CONTEXT
// structure definition)


#include "c-ansi/blotcode.h"
#include "c-ansi/blotex.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/named-object.h"
#include "c-ansi/stderr.h"

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
  G_STRING_STUFF h_working1GStringStuff ;
  G_STRING_STUFF h_working2GStringStuff ;
  G_STRING_STUFF availableGStringStuff ;
} ;  

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlotregDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  G_STRINGS_HANDLE xh_handle = (G_STRINGS_HANDLE) xhr_handle;

  m_TRACK_IF(GStringsDestroyInstance(xh_handle) < 0)  

  m_DIGGY_RETURN(RETURNED) 
} // BlotregDestroyInstance 

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

  m_ASSERT(G_STRINGS_ADD_INDEX(productHandle->h_blotregsHandle, STRING_PORTION__G_KEYS_COMPARISON,
    NULL,NULL,(STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) != INDEX_LABEL0)

  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&productHandle->h_working1GStringStuff) != RETURNED)
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&productHandle->h_working2GStringStuff) != RETURNED)
  productHandle->availableGStringStuff = productHandle->h_working1GStringStuff;

  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateProductInstance
  

// Public function; see .h
int BlotexlibExecutorCheckHandle(void *r_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorCheckHandle


// Public function; see .h
int BlotexlibExecutorGetBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotregName, G_STRINGS_HANDLE *ac_blotregHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__STRING_PORTION(&gKey,0,blotregName)
  int result = m_GStringsIndexSingleFetch(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlotregStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blotregHandle = ct_namedBlotregStuff->acolyt.cnhr_handle) != NULL)
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorGetBlotreg


#define NAME__BLOTREG_INDEX_LABEL      0
#define TOKEN_ID__BLOTREG_INDEX_LABEL 1

// Public function; see .h
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotregName, G_STRINGS_HANDLE *na_blotregHandle) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__STRING_PORTION(&gKey,0,blotregName)
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
        STRING_PORTION__G_KEYS_COMPARISON,NULL,NULL,
        (STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == NAME__BLOTREG_INDEX_LABEL)
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
        ACOLYT_VALUE__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
        (STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == TOKEN_ID__BLOTREG_INDEX_LABEL)
      t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
    } // h_blotregHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg


#define SYNTAX_ERROR__ABANDONMENT_INFO "Syntax error"
#define VALUE_ERROR__ABANDONMENT_INFO "Value error"
#define UNKNOWN_BLOTVAR__ABANDONMENT_INFO "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_INFO "Unknown blotreg"

// Passed:
// - ... : abandonment info format + optional variable arguments
//
// Changed (Implicit) variables:
// - nc_abandonmentInfo
#define m_ABANDON(...) {\
  m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,0, __VA_ARGS__) < 0)\
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
    struct STRING_PORTION c_name; // Only significant with NAME__BLOTVAR_REFERENCE
    int c_entry; // Only significant with ENTRY__BLOTVAR_REFERENCE
    int c_tokenId; // Only significant with TOKEN_ID__BLOTVAR_REFERENCE
  } select;
} ;


// IS_CHAR_FUNCTION:
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsInt1Op


#define b_STREX b_TRUE


#define b_L_VALUE b_TRUE
#define b_R_VALUE b_FALSE0 


// <blotvar> 
//
// Passed:
// - handle:
// - b_lValue: TRUE => create blotreg if not exist 
// - *a_blotvarSequence: before parsing 
//
// Changed:
// - *a_blotvarSequence: after parsing 
// - ac_blotvarReference: only significant if <blotvar> successfully parsed 
// - nc_abandonmentInfo: only significant if <blotvar> not succesfully parsed 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorRetrieveBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  struct STRING_PORTION *a_blotvarSequence, struct BLOTVAR_REFERENCE *ac_blotvarReference,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
 
  ac_blotvarReference->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  struct STRING_PORTION lexeme;

  m_PARSE_PASS_SPACES(*a_blotvarSequence,NULL) 
  m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'.',&lexeme)
  if (b_EMPTY_STRING_PORTION(*a_blotvarSequence)) { // '.' not found
    *a_blotvarSequence = lexeme; 
    m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'[',&lexeme)
    if (b_EMPTY_STRING_PORTION(*a_blotvarSequence)) { // '[' not found
      *a_blotvarSequence = lexeme; 
      m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'{',&lexeme)
      if (b_EMPTY_STRING_PORTION(*a_blotvarSequence)) { // '{' not found
        m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO) // Could not parse <blotvar>
      } // if
    } // if
  } // if

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
      m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_INFO) 
    break; default:
      m_TRACK()
    } // switch
  } // if

  // ac_blotvarReference->blotregHandle established 
  // Retrieve blotvar reference:
  m_PARSE_OFFSET(*a_blotvarSequence,1,&lexeme)
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
    if (b_EMPTY_STRING_PORTION(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
    // TODO: support empty or -1 entry when l-value (for smart fetch) 
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_INFO)
    else ac_blotvarReference->select.c_entry = genericInteger;
    m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,']',&lexeme)
    if (b_EMPTY_STRING_PORTION(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
  break; case '{' : // '{' <intex> '}' 
    ac_blotvarReference->blotvarReference = TOKEN_ID__BLOTVAR_REFERENCE;
    m_PARSE_GENERIC_INTEGER(*a_blotvarSequence,genericInteger,&lexeme)
    if (b_EMPTY_STRING_PORTION(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_INFO)
    else ac_blotvarReference->select.c_tokenId = genericInteger;
    m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,'}',&lexeme)
    if (b_EMPTY_STRING_PORTION(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
  break; default:
    m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
  } // switch

  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorRetrieveBlotvar 


// Ret: Found ? 
// - RESULT__FOUND:
// - RESULT__NOT_FOUND:
// - -1: unexpected problem
static int BlotexlibExecutorFetchBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  const struct BLOTVAR_REFERENCE *ap_blotvarReference, G_STRING_SET_STUFF *acvnt_blotvarStuff,
  int *nacvn_entry) {
  m_DIGGY_BOLLARD_S()

  int ret =  UNDEFINED;
  int indexFetch = INDEX_FETCH_FLAGS__SEEK_ONLY; 
  if (b_lValue) indexFetch = INDEX_FETCH_FLAGS__FETCH; 
  *acvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (ap_blotvarReference->blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case TOKEN_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
        m_ASSIGN_G_KEY__STRING_PORTION(&gKey,0,ap_blotvarReference->select.c_name)
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        m_ASSIGN_G_KEY__ACOLYT_TOKEN_ID(&gKey,0,ap_blotvarReference->select.c_tokenId)
      } // if
      switch (ret = m_GStringsIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,indexLabel,
        INDEX_SEEK_FLAGS__EQUAL, &gKey, INDEX_FETCH_FLAGS__FETCH, acvnt_blotvarStuff, nacvn_entry)) { 
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        m_RAISE_VERBATIM_IF(b_lValue)
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
    struct STRING_PORTION c_str;
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

// IS_CHAR_FUNCTION:
static int IsInt2OpChar(int c) {
  return (c == '+' || c == '-' || c == '*' || c == '/' || c == '&' || c == '!');
} // IsInt2OpChar

// Terminal symbols (of <int 2op> terminal symbol)
enum {
  ADD__INT_2OP,
  SUBTRACT__INT_2OP,
  MULTIPLY__INT_2OP,
  DIVIDE__INT_2OP,
  AND__INT_2OP,
  OR__INT_2OP,
} ;

// IS_CHAR_FUNCTION:
static int IsIntBlotregOpPrefix(int c) {
  return (c == ':' || c == '^' || c == '+' ||  c == '=');
} // IsIntBlotregOpPrefix

// Prefix terminal symbols (of <int blotreg op details> non terminal symbol)
enum {
  INIT__INT_BLOTREG_OP_PREFIX,
  RESET__INT_BLOTREG_OP_PREFIX,
  FETCH__INT_BLOTREG_OP_PREFIX,
  READ__INT_BLOTREG_OP_PREFIX,
} ;

// Parse <intex> | <strex> 
//
// Passed:
// - handle:
// - blotexSequence: 
//
// Changed:
// - ac_blotexValue: only significant if "computed successfully" 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Computed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'syntax' 'not found' error; abandon processing 
// - -1: unexpected problem
static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION *a_blotexSequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  struct STRING_PORTION lexeme; // UNDEFINED
  struct BLOTEX_VALUE atomBlotexValue; // UNDEFINED
  int n_int1Op = -1; // No <int 1op> a priori
  m_PARSE_PASS_SPACES(*a_blotexSequence,NULL)
  if (b_EMPTY_STRING_PORTION(*a_blotexSequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO) 
  m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,IsInt1Op,UNDEFINED,&lexeme)
  if (!b_EMPTY_STRING_PORTION(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT_1OP;
    break; case '+' : n_int1Op = PLUS__INT_1OP;
    break; case '-' : n_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    m_PARSE_PASS_SPACES(*a_blotexSequence,NULL)
  } // if

  // Try <int constant>...
  m_PARSE_GENERIC_INTEGER(*a_blotexSequence,atomBlotexValue.select.c_blotval,&lexeme)
  if (b_EMPTY_STRING_PORTION(lexeme)) { // Not <int constant>
    m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,NULL,'"',&lexeme) // try <str constant>...
    if ((atomBlotexValue.b_strex = !b_EMPTY_STRING_PORTION(lexeme))) { // <str constant> 
      m_PARSE_PASS_CHARS(*a_blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"',
        &atomBlotexValue.select.c_str)
      if (b_EMPTY_STRING_PORTION(*a_blotexSequence)) {
        m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
      } else {
        m_PARSE_OFFSET(*a_blotexSequence,1,NULL)
      } // if
    } else { // Not <str constant> 
      struct STRING_PORTION nonConstantCasesBlotexSequence = *a_blotexSequence;
      // Pass <entity name> :
      struct STRING_PORTION entityNameLexeme; // UNDEFINED
      m_PARSE_PASS_CHARS(*a_blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
        UNDEFINED,&entityNameLexeme) 
      m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,NULL,'?',&lexeme)
      if (!b_EMPTY_STRING_PORTION(lexeme)) { // '?' terminal symbol 
        // <entity name> '?' => expect: <int blotreg op> | <str blotreg read op> 
        G_STRINGS_HANDLE blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
        switch (BlotexlibExecutorGetBlotreg(handle,entityNameLexeme,&blotregHandle)) {
        case RESULT__FOUND:
        break; case RESULT__NOT_FOUND:
          m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_INFO) 
        break; default:
          m_TRACK()
        } // switch
        m_PARSE_MATCH_C(*a_blotexSequence,"=$",NULL, &lexeme) // Try <str blotreg read op> ...
        if ((atomBlotexValue.b_strex = !b_EMPTY_STRING_PORTION(lexeme))) { // <str blotreg read op> 
        } else { // <int blotreg op>
          m_PARSE_PASS_SPACES(*a_blotexSequence,NULL) // Try <int blotreg op details> ...
          m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,IsIntBlotregOpPrefix,UNDEFINED,&lexeme)
          if (b_EMPTY_STRING_PORTION(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO) 
          int intBlotregOpPrefix = UNDEFINED;
          switch (lexeme.string[0]) {
          case ':' : // <blotreg init>
            intBlotregOpPrefix = INIT__INT_BLOTREG_OP_PREFIX;
          break; case '^' : // <blotreg reset>
            intBlotregOpPrefix = RESET__INT_BLOTREG_OP_PREFIX;
          break; case '+' : // <blotreg fetch>
            intBlotregOpPrefix = FETCH__INT_BLOTREG_OP_PREFIX;
          break; case '=' : // <blotreg read int>
            intBlotregOpPrefix = READ__INT_BLOTREG_OP_PREFIX;
          break; default:
            m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
          } // switch
        } // if
      } else { // '?' not found
        // expect: <blotvar> | <blotvar entry> | <blotvar id> | <blotvar strex> | <blotvar name>
        *a_blotexSequence = nonConstantCasesBlotexSequence ;
        struct BLOTVAR_REFERENCE c_blotvarReference; // UNDEFINED 
        G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
        int cvn_entry = UNDEFINED; 
        // Parse <blotvar>:
        switch (BlotexlibExecutorRetrieveBlotvar(handle, b_R_VALUE, a_blotexSequence,
          &c_blotvarReference, nc_abandonmentInfo)) {
        case ANSWER__YES: // Parsed successfully    
          switch (BlotexlibExecutorFetchBlotvar(handle, b_R_VALUE,&c_blotvarReference,
            &cvnt_blotvarStuff,&cvn_entry)){
          case RESULT__FOUND:
          break; case RESULT__NOT_FOUND:
            m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_INFO) 
          break; default:
            m_TRACK()
          } // switch
        break; case ANSWER__NO: // Parsed KO
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
        m_PARSE_PASS_SPACES(*a_blotexSequence,NULL)
        m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,NULL,'!',&lexeme)
        if (!b_EMPTY_STRING_PORTION(lexeme)) { // <blotvar id> | <blotvar entry> | <blotvar name>
          m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,IsNameOrEntryBlotvarTerminalSymbol,UNDEFINED,
            &lexeme)
          if (!b_EMPTY_STRING_PORTION(lexeme)) {
            switch (lexeme.string[0]) {
            case '$': // <blotvar name>
              atomBlotexValue.b_strex = b_TRUE;
              atomBlotexValue.select.c_str =
                cvnt_blotvarStuff[G_PARAM_NAME_ELEMENT].cv_stringPortion; 
            break; case '#': // <blotvar entry>
              m_ASSERT(cvn_entry >= 0)
              atomBlotexValue.select.c_blotval = cvn_entry;
            break; default:
              m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
            } // switch
          } else { // <blotvar id>
            atomBlotexValue.select.c_blotval =
              cvnt_blotvarStuff[G_PARAM_NAME_ELEMENT].acolyt.cen_value;
          } // if
        } else { // <blotvar strex> | <blotvar> 
          m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,NULL,'$',&lexeme)
          if (b_EMPTY_STRING_PORTION(lexeme)) { // <blotvar> (integer value)
            atomBlotexValue.select.c_blotval =
              cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
          } else { // <blotvar strex>
            atomBlotexValue.select.c_str =
              cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].cv_stringPortion; 
          } // if
        } // if
      } // if
    } // if 
  } // if 

  m_PARSE_PASS_SPACES(*a_blotexSequence,NULL)
  if (atomBlotexValue.b_strex) { // strex 
    m_PARSE_PASS_SINGLE_CHAR(*a_blotexSequence,NULL,'+',&lexeme) // Try <str 2op> ...
    if (b_EMPTY_STRING_PORTION(lexeme)) *ac_blotexValue = atomBlotexValue;
    else { 
      switch (BlotexlibExecutorComputeBlotex(handle,a_blotexSequence,ac_blotexValue,
        nc_abandonmentInfo)) {
      case ANSWER__YES:
        if (!ac_blotexValue->b_strex) m_ABANDON(VALUE_ERROR__ABANDONMENT_INFO)
        m_TRACK_IF(GStringCopy(handle->availableGStringStuff,0, &atomBlotexValue.select.c_str) < 0)
        m_TRACK_IF(GStringCopy(handle->availableGStringStuff,-1, &ac_blotexValue->select.c_str) < 0)
        ac_blotexValue->select.c_str = handle->availableGStringStuff->cv_stringPortion;
        handle->availableGStringStuff = (handle->availableGStringStuff ==
          handle->h_working1GStringStuff? handle->h_working2GStringStuff:
          handle->h_working1GStringStuff);
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if 

  } else { // intex
    m_PARSE_PASS_CHARS(*a_blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsInt2OpChar,UNDEFINED,
      &lexeme) // Try <int 2op> ...
    if (b_EMPTY_STRING_PORTION(lexeme)) *ac_blotexValue = atomBlotexValue;
    else { 
      int int2Op = UNDEFINED;
      switch (lexeme.string[0]) {
      case '+': 
        if (m_StringPortionLength(&lexeme) != 1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        int2Op = ADD__INT_2OP;
      break; case '-':
        if (m_StringPortionLength(&lexeme) != 1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        int2Op = SUBTRACT__INT_2OP;
      break; case '*':
        if (m_StringPortionLength(&lexeme) != 1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        int2Op = MULTIPLY__INT_2OP;
      break; case '/':
        if (m_StringPortionLength(&lexeme) != 1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        int2Op = DIVIDE__INT_2OP;
      break; case '&':
        switch (m_CompareWithCString(&lexeme,"&&")) {
        case LESS_THAN__COMPARISON : 
        case GREATER_THAN__COMPARISON:
          m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        break; case EQUAL_TO__COMPARISON : 
          int2Op = AND__INT_2OP;
        break; default:
          m_TRACK()
        } // switch  
      break; case '!':
        switch (m_CompareWithCString(&lexeme,"!!")) {
        case LESS_THAN__COMPARISON : 
        case GREATER_THAN__COMPARISON:
          m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
        break; case EQUAL_TO__COMPARISON : 
          int2Op = OR__INT_2OP;
        break; default:
          m_TRACK()
        } // switch  
      break; default:
        m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
      } // switch
      
      switch (BlotexlibExecutorComputeBlotex(handle,a_blotexSequence,ac_blotexValue,
        nc_abandonmentInfo)){
      case ANSWER__YES:
        if (!ac_blotexValue->b_strex) m_ABANDON(VALUE_ERROR__ABANDONMENT_INFO)
        switch (int2Op) {
        case ADD__INT_2OP:
          ac_blotexValue->select.c_blotval += atomBlotexValue.select.c_blotval;
        break; case  SUBTRACT__INT_2OP:
          ac_blotexValue->select.c_blotval = atomBlotexValue.select.c_blotval -
            ac_blotexValue->select.c_blotval;
        break; case  MULTIPLY__INT_2OP:
          ac_blotexValue->select.c_blotval *= atomBlotexValue.select.c_blotval;
        break; case  DIVIDE__INT_2OP:
          ac_blotexValue->select.c_blotval = atomBlotexValue.select.c_blotval /
            ac_blotexValue->select.c_blotval;
        break; case AND__INT_2OP:
          ac_blotexValue->select.c_blotval = atomBlotexValue.select.c_blotval && 
            ac_blotexValue->select.c_blotval;
        break; case OR__INT_2OP:
          ac_blotexValue->select.c_blotval = atomBlotexValue.select.c_blotval || 
            ac_blotexValue->select.c_blotval;
        break; default:
          m_RAISE(ANOMALY__VALUE__D,int2Op) 
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if 
  } // if 

  m_DIGGY_RETURN(ANSWER__YES) ;
} // BlotexlibExecutorComputeBlotex 


enum {
      EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER = OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 

// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotex>
static int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  *ac_blotval = TRUE__BLOTVAL0; // a priori

  struct STRING_PORTION sequence = ap_blotfunc->call.arguments; 
  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
    m_PARSE_PASS_SPACES(sequence,NULL)
    { char b_blotvarReference = b_FALSE0; // a priori
      struct BLOTVAR_REFERENCE c_blotvarReference; // UNDEFINED 
      char cb_strex = (char)UNDEFINED; 
      { struct STRING_PORTION blotvarSequence; // UNDEFINED
        m_PARSE_TILL_MATCH_C(sequence,":=",NULL, &blotvarSequence)
        if (b_EMPTY_STRING_PORTION(sequence)) { // NO ':=' 
          sequence = blotvarSequence;
        } else { // <blotvar> [ '$' ] ':=' <blotex>
          b_blotvarReference = b_TRUE;
          switch (BlotexlibExecutorRetrieveBlotvar(handle,b_L_VALUE,&blotvarSequence,
            &c_blotvarReference, nc_abandonmentInfo)) {
          case ANSWER__YES:
          break; case ANSWER__NO:
            m_DIGGY_RETURN(ANSWER__NO)
          break; default:
            m_TRACK()
          } // switch
          m_PARSE_PASS_SPACES(blotvarSequence,NULL)
          struct STRING_PORTION lexeme; // UNDEFINED
          m_PARSE_PASS_SINGLE_CHAR(blotvarSequence,NULL,'$',&lexeme);
          cb_strex = (m_StringPortionLength(&lexeme) == 1); 
          m_PARSE_OFFSET(sequence,2,NULL)
          m_PARSE_PASS_SPACES(sequence,NULL)
        } // if
      } // blotvarSequence

      struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
      switch (BlotexlibExecutorComputeBlotex(handle,&sequence,&c_blotexValue,nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch

      m_PARSE_PASS_SPACES(sequence,NULL)
      if (!b_EMPTY_STRING_PORTION(sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)

      if (b_blotvarReference) {
        G_STRING_SET_STUFF t_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
        switch (BlotexlibExecutorFetchBlotvar(handle, b_L_VALUE,&c_blotvarReference,
          &t_blotvarStuff,NULL)) {
        case RESULT__FOUND:
        break; case RESULT__NOT_FOUND:
          m_RAISE(ANOMALY__UNEXPECTED_CASE)
        break; default:
          m_TRACK() 
        } // switch

        if (cb_strex) {
          if (! c_blotexValue.b_strex) m_ABANDON(VALUE_ERROR__ABANDONMENT_INFO)
          m_TRACK_IF(GStringCopy(t_blotvarStuff+G_PARAM_VALUE_ELEMENT,0,&c_blotexValue.select.c_str))
        } else {
          if (c_blotexValue.b_strex) m_ABANDON(VALUE_ERROR__ABANDONMENT_INFO)
          t_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
        } // if
      } // if

      if (c_blotexValue.b_strex) {
        *ac_blotval = TRUE__BLOTVAL0;
      } else {
        *ac_blotval = c_blotexValue.select.c_blotval;
      } // if
    } // b_blotvarReference 

  break; case OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
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

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(xh_handle->h_working1GStringStuff) != RETURNED)
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(xh_handle->h_working2GStringStuff) != RETURNED)

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

