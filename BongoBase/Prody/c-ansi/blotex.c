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
#include "c-ansi/c-parse.h"
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

struct BLOTEXLIB_EXECUTOR {
  m_DECLARE_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE)
  G_STRINGS_HANDLE h_blotregsHandle ; 
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
  m_ASSIGN_G_KEY__STRING_PORTION(gKey,blotregName)
  int result = G_STRINGS_INDEX_FETCH(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_FETCH__SEEK_ONLY, INDEX_SEEK__KEY,  &ct_namedBlotregStuff, &gKey);
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
#define PSEUDO_ID__BLOTREG_INDEX_LABEL 1


// Public function; see .h
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotregName, G_STRINGS_HANDLE *na_blotregHandle) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__STRING_PORTION(gKey,blotregName)
  int result = G_STRINGS_INDEX_FETCH(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_FETCH__FETCH, INDEX_SEEK__KEY,  &t_namedBlotregStuff, &gKey);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlotregStuff->acolyt.cnhr_handle != NULL)
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    { G_STRINGS_HANDLE h_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED ;
      m_TRACK_IF(GStringsCreateInstance(&h_blotregHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
        VALUED_STRING__G_STRING_CONVEYANCE,G_PARAM_CARDINALITY,
        (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED) 
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
        STRING_PORTION__G_KEYS_COMPARISON,NULL,NULL,
        (STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == NAME__BLOTREG_INDEX_LABEL)
      m_ASSERT(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
        ACOLYT_VALUE__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
        (STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == PSEUDO_ID__BLOTREG_INDEX_LABEL)
      t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
    } // h_blotregHandle 
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(completed)
} // BlotexlibExecutorCreateBlotreg

enum {
      EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER = OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 


#define ANOMALY__PARSING_FAILED "Parsing FAILED!"

#define SYNTAX_ERROR__ABANDONMENT_INFO "Syntax error"
#define VALUE_ERROR__ABANDONMENT_INFO "Value error"
#define UNKNOWN_BLOTVAR__ABANDONMENT_INFO "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_INFO "Unknown blotreg"

// Passed:
// - ... : abandonment info format + optional variable arguments
//
// Changed (Implicit) variables:
// - answer:
// - nc_abandonmentInfo
#define m_ABANDON(...) {\
  answer = ANSWER__NO;\
  m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,0, __VA_ARGS__) < 0)\
}

#define b_STREX b_TRUE

enum {
  NAME__BLOTVAR_REFERENCE, // '.' <entity name>
  ENTRY__BLOTVAR_REFERENCE, // '[' <intex> ']'
  PSEUDO_ID__BLOTVAR_REFERENCE, // '{' <intex> '}'
};

union BLOTVAR_REFERENCE {
  struct STRING_PORTION c_name; // Only significant with NAME__BLOTVAR_REFERENCE
  int c_entry; // Only significant with ENTRY__BLOTVAR_REFERENCE
  gen_BLOTVAL c_pseudoId; // Only significant with PSEUDO_ID__BLOTVAR_REFERENCE
} ; 

//
struct BLOTVAR {
  G_STRINGS_HANDLE blotregHandle; 
  int blotvarReference;
  union BLOTVAR_REFERENCE select; 
} ;


// IS_CHAR_FUNCTION:
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsInt1Op

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
// - ac_blotvar: only significant if <blotvar> successfully parsed 
// - nc_abandonmentInfo: only significant if <blotvar> not succesfully parsed 
//
// Ret: <blotvar> succesfully parsed ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorRetrieveBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  struct STRING_PORTION *a_blotvarSequence, struct BLOTVAR *ac_blotvar,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
 
  int answer = ANSWER__YES; // good syntax a priori
  ac_blotvar->blotregHandle = (G_STRINGS_HANDLE)UNDEFINED; 
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

  if (answer == ANSWER__YES) { // lexeme corresponds to <entity name>
    // Retrieve blotreg:
    if (b_lValue) {
      switch (BlotexlibExecutorCreateBlotreg(handle,lexeme,&ac_blotvar->blotregHandle)) {
      case COMPLETED__OK:
      case COMPLETED__BUT:
      break; default:
        m_TRACK()
      } // switch
    } else {
      switch (BlotexlibExecutorGetBlotreg(handle,lexeme,&ac_blotvar->blotregHandle)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_INFO) 
      break; default:
        m_TRACK()
      } // switch
    } // if
  } // if

  if (answer == ANSWER__YES) { // So far, so good...
    // ac_blotvar->blotregHandle established 
    // Retrieve blotvar reference:
    struct STRING_PORTION charLexeme; // UNDEFINED
    m_PARSE_OFFSET(*a_blotvarSequence,1,&charLexeme)
    GENERIC_INTEGER genericInteger = UNDEFINED;
    switch (charLexeme.string[0]) {
    case '.' : // '.' <blotvar name>
      ac_blotvar->blotvarReference = NAME__BLOTVAR_REFERENCE;
      m_PARSE_PASS_CHARS(*a_blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
        UNDEFINED,&lexeme)
      ac_blotvar->select.c_name = lexeme;
    break; case '[' : // '[' <intex> ']'
      ac_blotvar->blotvarReference = ENTRY__BLOTVAR_REFERENCE;
      m_PARSE_GENERIC_INTEGER(*a_blotvarSequence,answer,genericInteger,NULL)
      if (answer == ANSWER__NO) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
      // TODO: support empty or -1 entry when l-value (for smart fetch) 
      else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
        VALUE_ERROR__ABANDONMENT_INFO)
      else ac_blotvar->select.c_entry = genericInteger;
      m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,']',&charLexeme)
      if (b_EMPTY_STRING_PORTION(charLexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
    break; case '{' : // '{' <intex> '}' 
      ac_blotvar->blotvarReference = PSEUDO_ID__BLOTVAR_REFERENCE;
      m_PARSE_GENERIC_INTEGER(*a_blotvarSequence,answer,ac_blotvar->select.c_pseudoId,NULL)
      if (answer == ANSWER__NO) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
      m_PARSE_PASS_SINGLE_CHAR(*a_blotvarSequence,NULL,'}',&charLexeme)
      if (b_EMPTY_STRING_PORTION(charLexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,charLexeme.string[0])
    } // switch
  } // if

  m_DIGGY_RETURN(answer) ;
} // BlotexlibExecutorRetrieveBlotvar 


// Ret: Found ? 
// - RESULT__FOUND:
// - RESULT__NOT_FOUND:
// - -1: unexpected problem
static int BlotexlibExecutorFetchBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_lValue,
  const struct BLOTVAR *ap_blotvar, G_STRING_SET_STUFF *acvnt_blotvarStuff) {
  m_DIGGY_BOLLARD_S()

  int ret =  UNDEFINED;
  int indexFetch = INDEX_FETCH__SEEK_ONLY; 
  if (b_lValue) indexFetch = INDEX_FETCH__FETCH; 
  *acvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (ap_blotvar->blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case PSEUDO_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvar->blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
        m_ASSIGN_G_KEY__STRING_PORTION(gKey,ap_blotvar->select.c_name)
      } else {
        indexLabel = PSEUDO_ID__BLOTREG_INDEX_LABEL;
        m_ASSIGN_G_KEY__ACOLYT_VALUE(gKey,ap_blotvar->select.c_pseudoId)
      } // if
      switch (ret = GStringsIndexFetch(ap_blotvar->blotregHandle,NULL,indexLabel,
        INDEX_FETCH__FETCH, INDEX_SEEK__KEY, acvnt_blotvarStuff, NULL,&gKey)) { 
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        m_RAISE_VERBATIM_IF(b_lValue)
      break; default:
        m_TRACK()
      } // switch
    } // gKey 
  break; case ENTRY__BLOTVAR_REFERENCE:
    ret = GStringsFetch(ap_blotvar->blotregHandle,ap_blotvar->select.c_entry,
      acvnt_blotvarStuff);
    m_TRACK_IF(ret < 0)
    m_ASSERT(ret == ap_blotvar->select.c_entry)
    ret = (acvnt_blotvarStuff == NULL? RESULT__NOT_FOUND: RESULT__NOT_FOUND);
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,ap_blotvar->blotvarReference)  
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

// Non-terminal symbols:
enum {
 INT_CONSTANT__NTID,
 STR_CONSTANT__NTID,
  ENTITY_NAME__NTID,
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


// <intex> | <strex> 
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
  struct STRING_PORTION blotexSequence, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()

  int answer = ANSWER__YES; // good syntax a priori
  struct STRING_PORTION lexeme; // UNDEFINED
  int n_int1Op = -1; // No int 1op a priori...
  m_PARSE_PASS_SPACES(blotexSequence,NULL)
  m_PARSE_PASS_SINGLE_CHAR(blotexSequence,IsInt1Op,UNDEFINED,&lexeme)
  if (!b_EMPTY_STRING_PORTION(lexeme)) {
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT_1OP;
    break; case '+' : n_int1Op = PLUS__INT_1OP;
    break; case '-' : n_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,lexeme.string[0])
    } // switch
  } // if

  m_PARSE_PASS_SPACES(blotexSequence,NULL)

  m_PARSE_GENERIC_INTEGER(blotexSequence,answer,ac_blotexValue->select.c_blotval,&lexeme)
  if (b_EMPTY_STRING_PORTION(lexeme)) { // Not <int constant>
    m_PARSE_PASS_SINGLE_CHAR(blotexSequence,NULL,'"',&lexeme) // try <str constant>...
    if (!b_EMPTY_STRING_PORTION(lexeme)) { // '"' token 
      m_PARSE_PASS_CHARS(blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"',
        &ac_blotexValue->select.c_str)
      if (b_EMPTY_STRING_PORTION(blotexSequence)) {
        m_ABANDON(SYNTAX_ERROR__ABANDONMENT_INFO)
      } else {
        m_PARSE_OFFSET(blotexSequence,1,NULL)
      } // if
    } else { // Not <str constant> 
      m_PARSE_PASS_CHARS(blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
        UNDEFINED,&lexeme) // try <entity name>...
      m_PARSE_PASS_SINGLE_CHAR(blotexSequence,NULL,'?',&lexeme)
      if (!b_EMPTY_STRING_PORTION(lexeme)) { // '?' found
        // <entity name> '?' => <int blotreg op> | <str blotreg read op> 
      } else { // '?' not found
        // <blotvar> | <blotvar entry> | <blotvar id> | <blotvar strex> | <blotvar name>
        struct BLOTVAR c_blotvar; // UNDEFINED 
        G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
        switch (answer = BlotexlibExecutorRetrieveBlotvar(handle, b_R_VALUE, &blotexSequence,
          &c_blotvar, nc_abandonmentInfo)) {
        case ANSWER__YES: // Parsed successfully    
          switch (BlotexlibExecutorFetchBlotvar(handle, b_R_VALUE,&c_blotvar,&cvnt_blotvarStuff)){
          case RESULT__FOUND:
            ac_blotexValue->select.c_blotval = cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value;
          break; case RESULT__NOT_FOUND:
            m_ABANDON(UNKNOWN_BLOTVAR__ABANDONMENT_INFO) 
          break; default:
            m_TRACK()
          } // switch
        break; case ANSWER__NO: // Parsed KO
        break; default:
          m_TRACK()
        } // switch
      } // if
    } // if 
  } // if 

  m_DIGGY_RETURN(answer) ;
} // BlotexlibExecutorComputeBlotex 


// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotex>
static int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  int answer = ANSWER__YES; // a priori
  *ac_blotval = TRUE__BLOTVAL0; // a priori

  struct STRING_PORTION sequence = ap_blotfunc->call.arguments; 
  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
    m_PARSE_PASS_SPACES(sequence,NULL)
    { struct STRING_PORTION blotvarSequence; // UNDEFINED
      m_PARSE_TILL_MATCH_C(sequence,":=",NULL, &blotvarSequence)
      struct BLOTVAR c_blotvar; // UNDEFINED 
      if (b_EMPTY_STRING_PORTION(sequence)) { // NO ':=' 
        sequence = blotvarSequence;
      } else { // <blotvar> [ '$' ] ':=' <blotex>
        answer = BlotexlibExecutorRetrieveBlotvar(handle,b_L_VALUE,&blotvarSequence,&c_blotvar,
          nc_abandonmentInfo);
        m_TRACK_IF(answer < 0)
        m_PARSE_PASS_SPACES(blotvarSequence,NULL)
        struct STRING_PORTION lexeme; // UNDEFINED
        m_PARSE_PASS_SINGLE_CHAR(blotvarSequence,NULL,'$',&lexeme);
        m_PARSE_OFFSET(sequence,2,NULL)
      } // if
      if (answer == ANSWER__YES) {
        struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
        answer = BlotexlibExecutorComputeBlotex(handle,sequence,&c_blotexValue,nc_abandonmentInfo);
        m_TRACK_IF(answer < 0)
        G_STRING_SET_STUFF cvnt_blotvarStuff = (G_STRING_SET_STUFF)UNDEFINED;
        switch (BlotexlibExecutorFetchBlotvar(handle, b_L_VALUE,&c_blotvar,&cvnt_blotvarStuff)){
        case RESULT__FOUND:
          cvnt_blotvarStuff[G_PARAM_VALUE_ELEMENT].acolyt.cen_value = c_blotexValue.select.c_blotval;
        break; case RESULT__NOT_FOUND:
        break; default:
          m_TRACK() 
        } // switch
      } // if 
    } // blotvarSequence 
  break; case OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,ap_blotfunc->entry.localBlotfuncNameEntry)
  } // switch
  
  m_DIGGY_RETURN(answer)
} // BlotexlibExecutorExecuteCFunction


// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <blotex>
static int BlotexlibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE xh_handle = (BLOTEXLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,xh_handle)

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)
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


// Public function; see .h
int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryDestroyInstance
