// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/blotcode.h"
#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/named-object.h"
#include "c-ansi/stderr.h"

#include "c-ansi/testy-diggy.h"

#include <limits.h>

struct KARLLIB_EXECUTOR_FACTORY {
  m_DECLARE_MAGIC_FIELD(KARLLIB_EXECUTOR_FACTORY_HANDLE)
  int blotexlibLabel;
} ; 

 
// Public function; see .h
int KarllibExecutorFactoryCreateInstance(KARLLIB_EXECUTOR_FACTORY_HANDLE *azh_handle, int blotexlibLabel) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(KARLLIB_EXECUTOR_FACTORY_HANDLE,*azh_handle)
  (*azh_handle)->blotexlibLabel = blotexlibLabel;
  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorFactoryCreateInstance


// Public function; see .h
int KarllibExecutorFactoryDestroyInstance(KARLLIB_EXECUTOR_FACTORY_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorFactoryDestroyInstance


struct KARLLIB_EXECUTOR {
  m_DECLARE_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE)
  G_STRINGS_HANDLE h_pamphletExamplarsHandle ; // NAMED_OBJECTS
  //G_STRINGS_HANDLE h_blotregsHandle ; 
  //G_STRINGS_HANDLE h_blottabsHandle ; 
  //G_STRINGS_HANDLE h_workingGStringsHandle ;
} ;


///////////////////////

struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE {
  // Localization 
  int offset ; // >= 0 (vis-a-vis point) 
} ;

struct PAMPHLET_EXAMPLAR_POINT {
  // Localization 
  int offset ; // >= 0 (vis-a-vis parent point) 
  int depth ; // >= 0 
  int totalLength ; // >= 0 (all occurences)

  // Entry in "pamphlet point descriptions"
  int descriptionEntry ;

  GREEN_COLLECTION_HANDLE *h_occurences; 

  G_STRINGS_HANDLE *h_subPointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
} ;


struct PAMPHLET_EXAMPLAR {
  struct P_STRING examplar ;

  // description entry in "pamphlets" descriptions
  int pamphletDescriptionEntry ;

  G_STRINGS_HANDLE *h_pointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
} ;

// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <blotex>
static int KarllibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorFactoryCreateProductInstance


// ==> NAMED OBEJCT

///////////////////////

enum {
      PIPI__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
      CACA__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  KARLLIB_LOCAL_BLOTFUNC_NAMES_NUMBER,
} ;

static const char* localBlotfuncNames[] = { "Pipi" , "Caca" } ; 


// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION 
static int KarllibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(ANSWER__YES)
} // KarllibExecutorExecuteCFunction

// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <karl>
static int KarllibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  KARLLIB_EXECUTOR_HANDLE xh_handle = (KARLLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE,xh_handle)

  //m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)

  //m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_workingGStringsHandle) != RETURNED)

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorDestroyInstance


// See .h
int l_BlotcodeLinkKarllib(BLOTCODE_HANDLE ep_handle, const char* nfp_blotlibPrefix,
  KARLLIB_EXECUTOR_FACTORY_HANDLE fp_karllibExecutorFactoryHandle) {
  m_DIGGY_BOLLARD()

  int entry = BlotcodeLinkBlotlib(ep_handle, nfp_blotlibPrefix,
    KARLLIB_LOCAL_BLOTFUNC_NAMES_NUMBER, localBlotfuncNames, 
    KarllibExecutorFactoryCreateProductInstance, fp_karllibExecutorFactoryHandle,
    KarllibExecutorExecuteCFunction, KarllibExecutorDestroyInstance) ;
  m_TRACK_IF(entry < 0)

  m_DIGGY_RETURN(entry)
} // l_BlotcodeLinkKarllib

