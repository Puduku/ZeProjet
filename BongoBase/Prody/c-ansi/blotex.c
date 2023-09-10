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
  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blotregsHandle), 10) != RETURNED)

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

  G_STRING_STUFF t_namedBlotregStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey ;
  m_ASSIGN_G_KEY__STRING_PORTION(gKey,blotregName)
  int result = G_STRINGS_INDEX_FETCH(handle->h_blotregsHandle,NULL,INDEX_LABEL0,
    INDEX_FETCH__FETCH, INDEX_SEEK__KEY,  &t_namedBlotregStuff, &gKey);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlotregStuff->acolyt.cnhr_handle != NULL)
  break; case RESULT__NOT_FOUND: {
    G_STRINGS_HANDLE h_blotregHandle = (G_STRINGS_HANDLE)UNDEFINED ;
    m_TRACK_IF(GStringsCreateInstance(&h_blotregHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
      VALUED_STRING__G_STRING_CONVEYANCE,G_PARAM_CARDINALITY,
      (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED) 
    t_namedBlotregStuff->acolyt.cnhr_handle = h_blotregHandle;
  } // RESULT__NOT_FOUND
  break; default:
    m_TRACK()
  } // switch
  if (na_blotregHandle != NULL) *na_blotregHandle = t_namedBlotregStuff->acolyt.cnhr_handle;

  m_DIGGY_RETURN(result)
} // BlotexlibExecutorCreateBlotreg

enum {
      EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  BLOTEXLIB_LOCAL_BLOTFUNC_NAMES_NUMBER = OUTPUT_F__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 


#define b_STREX b_TRUE

struct BLOTVAR {
  G_STRINGS_HANDLE n_blotregHandle; // NULL special value => 'not found' 
  int c_entry;
  char b_strex;
} ;

// Changed:
// - ac_blotvar: possibly 'not found' 
//
// Ret: Good syntax ? 
// - ANSWER__YES:
// - ANSWER__NO:
// - -1: unexpected problem
static int BlotexlibExecutorRetrieveBlotvar(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotvarLexeme, struct BLOTVAR *ac_blotvar) {
  m_DIGGY_BOLLARD_S()
 
  int answer = ANSWER__YES; // good syntax a priori
  ac_blotvar->n_blotregHandle = NULL; // not found a priori
  struct STRING_PORTION lexeme2;

  m_PARSE_PASS_CHARS(blotvarLexeme,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'.',&lexeme2)
  if (b_EMPTY_STRING_PORTION(blotvarLexeme)) { // '.' not found
    blotvarLexeme = lexeme2; 
    m_PARSE_PASS_CHARS(blotvarLexeme,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'[',&lexeme2)
    if (b_EMPTY_STRING_PORTION(blotvarLexeme)) { // '[' not found
      blotvarLexeme = lexeme2; 
      m_PARSE_PASS_CHARS(blotvarLexeme,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'{',&lexeme2)
      if (b_EMPTY_STRING_PORTION(blotvarLexeme)) { // '{' not found
        answer = ANSWER__NO; 
      } else { // found '{' 
      } // if
    } else { // found '[' 
    } // if
  } else { // found '.' 
  } // if

  m_DIGGY_RETURN(answer) ;
} // BlotexlibExecutorRetrieveBlotvar 

struct BLOTEX_VALUE {
  char b_strex;
} ;

static int BlotexlibExecutorComputeBlotex(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct STRING_PORTION blotexLexeme, struct BLOTEX_VALUE *ac_blotexValue) {
  m_DIGGY_BOLLARD_S()

  int answer = ANSWER__YES; // good syntax a priori
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

  m_ASSIGN_LOCAL_C_STRING_PORTION(token, ":=")
  int tokenLength = m_StringPortionLength(&token);
  struct STRING_PORTION sequence = ap_blotfunc->call.arguments; 
  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case EVAL__BLOTEXLIB_LOCAL_BLOTFUNC_NAME_ENTRY: {
    struct STRING_PORTION blotvarSequence; // UNDEFINED
    m_PARSE_TILL_MATCH(sequence,token,NULL, &blotvarSequence)
    if (b_EMPTY_STRING_PORTION(sequence)) { // NO ':=' 
      sequence = blotvarSequence;
    } else { // <blotvar> [ '$' ] ':=' <blotex>
      struct STRING_PORTION blotvarLexeme; // UNDEFINED
      m_PARSE_STRIP_SPACES(blotvarSequence,&blotvarLexeme)
      struct BLOTVAR c_blotvar; // UNDEFINED 
      answer = BlotexlibExecutorRetrieveBlotvar(handle,blotvarLexeme,&c_blotvar);
      m_TRACK_IF(answer < 0)
      m_PARSE_OFFSET(sequence,tokenLength,NULL)
    } // if
    if (answer == ANSWER__YES) {
      struct BLOTEX_VALUE c_blotexValue; // UNDEFINED
      answer = BlotexlibExecutorComputeBlotex(handle,sequence,&c_blotexValue);
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
