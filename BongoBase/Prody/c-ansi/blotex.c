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
// Public function; see .h
int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
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
  const char* p_blotregName, G_STRINGS_HANDLE *na_blotregHandle) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(-1)
} // BlotexlibExecutorCreateBlotreg

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 

// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotex>
// Public function; see .h
int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  int answer = ANSWER__YES; // a priori
  *ac_blotval = TRUE__BLOTVAL0; // a priori
  m_DIGGY_RETURN(answer)
} // BlotexlibExecutorExecuteCFunction

// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <blotex>
// Public function; see .h
int BlotexlibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE xh_handle = (BLOTEXLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,xh_handle)

  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorDestroyInstance

int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryDestroyInstance


