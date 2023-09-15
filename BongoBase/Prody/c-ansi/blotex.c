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
#include "c-ansi/g-string.h"
#include "c-ansi/stderr.h"


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
  break; case RESULT__NOT_FOUND: {
    G_STRINGS_HANDLE h_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED ;
    m_TRACK_IF(GStringsCreateInstance(&h_blotregHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
      VALUED_STRING__G_STRING_CONVEYANCE,G_PARAM_CARDINALITY,
      (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED) 
    m_TRACK_IF(GStringsAddIndex(h_blotregHandle,1,G_PARAM_NAME_ELEMENT,
      STRING_PORTION__G_KEYS_COMPARISON,NULL,NULL,
      (STRING_PORTION_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) != INDEX_LABEL0)
    t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
  } // RESULT__NOT_FOUND
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


#define b_STREX b_TRUE

struct BLOTVAR {
  G_STRINGS_HANDLE n_blotregHandle; // NULL special value => 'not identified' 
  G_STRING_SET_STUFF cnt_blotvarStuff; // Only significant if blotreg is identified:
    // NULL special value => not existing
  int cc_entry; // Only significant if blotreg is identified and item exists
  char b_strex;
} ;


#define b_CREATE b_TRUE

// <blotvar> [ '$' ]
//
// Passed:
// - handle:
// - b_create: TRUE => create if not exist 
// - blotvarSequence: 
//
// Changed:
// - ac_blotvar: possibly 'not found' 
//
// Ret: Good syntax ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorRetrieveBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle, char b_create,
  struct STRING_PORTION blotvarSequence, struct BLOTVAR *ac_blotvar) {
  m_DIGGY_BOLLARD_S()
 
  int answer = ANSWER__YES; // good syntax a priori
  ac_blotvar->n_blotregHandle = NULL; // not found a priori
  struct STRING_PORTION lexeme;

  m_PARSE_PASS_CHARS(blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'.',&lexeme)
  if (b_EMPTY_STRING_PORTION(blotvarSequence)) { // '.' not found
    blotvarSequence = lexeme; 
    m_PARSE_PASS_CHARS(blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'[',&lexeme)
    if (b_EMPTY_STRING_PORTION(blotvarSequence)) { // '[' not found
      blotvarSequence = lexeme; 
      m_PARSE_PASS_CHARS(blotvarSequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'{',&lexeme)
      if (b_EMPTY_STRING_PORTION(blotvarSequence)) { // '{' not found
        answer = ANSWER__NO; 
      } // if
    } // if
  } // if

  if (answer == ANSWER__YES) {
    // Retrieve blotreg:
    if (b_create) {
      switch (BlotexlibExecutorCreateBlotreg(handle,lexeme,&ac_blotvar->n_blotregHandle)) {
      case COMPLETED__OK:
      case COMPLETED__BUT:
      break; default:
        m_TRACK()
      } // switch
    } else {
      switch (BlotexlibExecutorGetBlotreg(handle,lexeme,&ac_blotvar->n_blotregHandle)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        ac_blotvar->n_blotregHandle = NULL; // not found
      break; default:
        m_TRACK()
      } // switch
    } // if

    if (ac_blotvar->n_blotregHandle != NULL) { // blotreg identified
      struct STRING_PORTION charLexeme; // UNDEFINED
      m_PARSE_OFFSET(blotvarSequence,1,&charLexeme)
      m_PARSE_PASS_CHARS(blotvarSequence,b_REVERTED_SCAN,b_PASS_CHARS_TILL,NULL,'$',&lexeme)
      ac_blotvar->b_strex = b_EMPTY_STRING_PORTION(blotvarSequence);
      switch (charLexeme.string[0]) {
      case '.' : // '.' <blotvar name>
        { struct G_KEY gKey ;
          m_ASSIGN_G_KEY__STRING_PORTION(gKey,lexeme)
          switch (GStringsIndexFetch(ac_blotvar->n_blotregHandle,NULL,INDEX_LABEL0,
            INDEX_FETCH__SEEK_ONLY, INDEX_SEEK__KEY,&ac_blotvar->cnt_blotvarStuff,
            &ac_blotvar->cc_entry,&gKey)) { 
          case RESULT__FOUND:
          break; case RESULT__NOT_FOUND:
            ac_blotvar->n_blotregHandle = NULL;  
          break; default:
            m_TRACK()
          } // switch
        } // gKey 
      break; case '[' : // '[' <intex> ']'
      break; case '{' : // '{' <intex> '}' 
      break; default:
        m_RAISE(ANOMALY__VALUE__FMT_D,charLexeme.string[0])
      } // switch
    } // if
  } // if

  m_DIGGY_RETURN(answer) ;
} // BlotexlibExecutorRetrieveBlotvar 

struct BLOTEX_VALUE {
  char b_strex;
} ;

// IS_CHAR_FUNCTION:
static int IsInt1Op (int c) {
  return (c == '+' || c == '-' || c == '!');
} // IsInt1Op

enum {
  NOT__INT1_OP,
  PLUS__INT1_OP,
  MINUS__INT1_OP,
} ;

// <intex> | <strex> 
//
// Passed:
// - handle:
// - blotvarSequence: 
//
// Changed:
// - ac_blotexValue: possibly 'not found' 
//
// Ret: Good syntax ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotexSequence, struct BLOTEX_VALUE *ac_blotexValue) {
  m_DIGGY_BOLLARD_S()

  int answer = ANSWER__YES; // good syntax a priori
  struct STRING_PORTION lexeme; // UNDEFINED
  int n_int1Op = -1; // No int 1op a priori...
  m_PARSE_PASS_SPACES(blotexSequence,NULL)
  m_PARSE_PASS_CHARS(blotexSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsInt1Op,UNDEFINED,&lexeme)
  switch (m_StringPortionLength(&lexeme)) {
  case 0: // NO int 1op
  break; case 1: // int 1op
    switch (lexeme.string[0]) {
    case '!' : n_int1Op = NOT__INT1_OP;
    break; case '+' : n_int1Op = PLUS__INT1_OP;
    break; case '-' : n_int1Op = MINUS__INT1_OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,lexeme.string[0])
    } // switch
  break; default: // multiple int 1op
    answer = ANSWER__NO; // Syntax error...
  } // switch
  
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
  case EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY: {
    struct STRING_PORTION blotvarSequence; // UNDEFINED
    m_PARSE_TILL_MATCH_C(sequence,":=",NULL, &blotvarSequence)
    if (b_EMPTY_STRING_PORTION(sequence)) { // NO ':=' 
      sequence = blotvarSequence;
    } else { // <blotvar> [ '$' ] ':=' <blotex>
      struct STRING_PORTION blotvarLexeme; // UNDEFINED
      m_PARSE_STRIP_SPACES(blotvarSequence,&blotvarLexeme)
      struct BLOTVAR c_blotvar; // UNDEFINED 
      answer = BlotexlibExecutorRetrieveBlotvar(handle,b_CREATE,blotvarLexeme,&c_blotvar);
      m_TRACK_IF(answer < 0)
      m_PARSE_OFFSET(sequence,2,NULL)
    } // if
    if (answer == ANSWER__YES) {
      struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
      answer = BlotexlibExecutorComputeBlotex(handle,sequence,&c_blotexValue);
      m_TRACK_IF(answer < 0)
    } // if
  } break; case OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
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
