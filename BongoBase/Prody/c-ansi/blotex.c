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
} ;  
typedef struct BLOTEXLIB_EXECUTOR *BLOTEXLIB_EXECUTOR_HANDLE;

// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <blotex>
// Public function; see .h
int BlotexlibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE p_handle = (BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)pr_handle;
  m_CHECK_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE,p_handle)

  m_MALLOC_INSTANCE(*(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle) {
  m_ASSIGN_MAGIC_FIELD(BLOTEXLIB_EXECUTOR_HANDLE,
    *(BLOTEXLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryCreateProductInstance
  

// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION <blotex>
// Public function; see .h
int BlotexlibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE handle = (BLOTEXLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_VALUE(BLOTEXLIB_EXECUTOR_HANDLE,handle)
  int answer = ANSWER__YES; // a priori
  *ac_blotval = TRUE__BLOTVAL0; // a priori
  m_DIGGY_RETURN(answer)
} // BlotexlibExecutorExecuteCFunction

// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <blotex>
// Public function; see .h
int BlotexlibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  BLOTEXLIB_EXECUTOR_HANDLE xh_handle = (BLOTEXLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_VALUE(BLOTEXLIB_EXECUTOR_HANDLE,xh_handle)
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorDestroyInstance

int BlotexlibExecutorFactoryDestroyInstance(BLOTEXLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // BlotexlibExecutorFactoryDestroyInstance


// Interface with other blotlibs
// =============================

// Allow exchange of variable values between blotex lib and other blot libraries. 

// Check handle retrieved by BlotcodeExecutorGetBlotlibExecutorHandle() function.
//
// Passed:
// r_handle: see BlotcodeExecutorGetBlotlibExecutorHandle()
//
// Returned :
// - RETURNED: OK
// - -1: unexpected problem 
int BlotexlibExecutorCheckHandle(void *r_handle) ;

// 1. blotvars interface


#define GLOBAL_BLOTREG_NAME GOOD_OLD_EMPTY_C_STRING


// Ret:
// - RESULT__FOUND: OK
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorGetBlotreg (BLOTEXLIB_EXECUTOR_HANDLE handle,
  const char* p_blotregName, *G_STRINGS_HANDLE *ac_blotregHandle) ;



// Ret:
// - COMPLETED__OK: OK
// - COMPLETED__BUT: the register alread exists 
// - -1: unexpected problem; anomaly is raised
int BlotexlibExecutorCreateBlotreg(BLOTEXLIB_EXECUTOR_HANDLE handle,
  const char* p_blotregName, *G_STRINGS_HANDLE *cac_blotregHandle);

#define BLOTVAR_NAME_ELEMENT G_PARAM_NAME_ELEMENT
#define BLOTVAR_VALUE_ELEMENT G_PARAM_VALUE_ELEMENT 

