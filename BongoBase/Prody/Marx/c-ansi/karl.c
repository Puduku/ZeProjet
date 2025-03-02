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
  G_STRINGS_HANDLE h_blotpamsHandle ; 
  //G_STRINGS_HANDLE h_workingGStringsHandle ;
} ;


// Blotpams: 

struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE {
  // Localization 
  int offset ; // >= 0 (vis-a-vis point) 
} ;
typedef struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE *PAMPHLET_EXAMPLAR_POINT_OCCURENCE_STUFF;

struct PAMPHLET_EXAMPLAR_POINT {
  // Localization 
  int offset ; // >= 0 (vis-a-vis parent point) 
  int depth ; // >= 0 
  int totalLength ; // >= 0 (all occurences)

  // Entry in "pamphlet point descriptions"
  int descriptionEntry ;

  GREEN_COLLECTION_HANDLE h_occurencesHandle; 

  G_STRINGS_HANDLE h_subPointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
  m_DECLARE_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE)
} ;
typedef struct PAMPHLET_EXAMPLAR_POINT *PAMPHLET_EXAMPLAR_POINT_HANDLE;

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int PamphletExamplarPointDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_EXAMPLAR_POINT_HANDLE xh_handle = (PAMPHLET_EXAMPLAR_POINT_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_occurencesHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_subPointsHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarPointDestroyInstance

// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
static int PamphletExamplarPointCreateInstance(PAMPHLET_EXAMPLAR_POINT_HANDLE *azh_handle,
   int fieldsNumber) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_EXAMPLAR_POINT_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE,handle)

  handle->offset = 0;
  handle->depth = 0;
  handle->totalLength = 0;
  handle->descriptionEntry = 0;
 
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_occurencesHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    sizeof(struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE), (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL,
    (GREEN_HANDLER__COMPARE_FUNCTION)NULL,(GREEN_HANDLER__EQUATE_FUNCTION)NULL,
    (void*)UNDEFINED) != RETURNED) 

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_subPointsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    PamphletExamplarPointDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_pointsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)
  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarPointCreateInstance


//struct PAMPHLET_EXAMPLAR {
//  struct P_STRING examplar ;
//  // description entry in "pamphlets" descriptions
//  int pamphletDescriptionEntry ;
//  G_STRINGS_HANDLE *h_pointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
//} ;


////////////////////////////////
// blotpam <=> pamphlet examplar
////////////////////////////////
struct BLOTPAM {
  m_DECLARE_MAGIC_FIELD(BLOTPAM_HANDLE)
  //G_STRINGS_HANDLE h_tableHandle;
  //G_STRINGS_HANDLE hp_fieldsHandle;
  struct P_STRING examplar ;
  // description entry in "pamphlets" descriptions
  int pamphletDescriptionEntry ;
  G_STRINGS_HANDLE h_pointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
} ; 
typedef struct BLOTPAM* BLOTPAM_HANDLE;



// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
static int BlotpamCreateInstance(BLOTPAM_HANDLE *azh_handle, int dummy) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  BLOTPAM_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(BLOTPAM_HANDLE,handle)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_pointsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    PamphletExamplarPointDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_pointsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)

  m_DIGGY_RETURN(RETURNED)
} // BlotpamCreateInstance


// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int BlotpamDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  BLOTPAM_HANDLE xh_handle = (BLOTPAM_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTPAM_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_pointsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // BlotpamDestroyInstance


// #SEE BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION <karl>
static int KarllibExecutorFactoryCreateProductInstance(void *pr_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()

  KARLLIB_EXECUTOR_FACTORY_HANDLE p_handle = (KARLLIB_EXECUTOR_FACTORY_HANDLE)pr_handle;
  m_CHECK_MAGIC_FIELD(KARLLIB_EXECUTOR_FACTORY_HANDLE,p_handle)

  m_MALLOC_INSTANCE(*(KARLLIB_EXECUTOR_HANDLE*)azhr_blotlibExecutorHandle)
  KARLLIB_EXECUTOR_HANDLE productHandle = *azhr_blotlibExecutorHandle;

  m_ASSIGN_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE, productHandle)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(productHandle->h_blotpamsHandle), 10,
    BlotpamDestroyInstance) != RETURNED)
  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(productHandle->h_blotpamsHandle, NULL,NULL) ==
    INDEX_LABEL0)

//  m_TRACK_IF(G_STRINGS_CREATE_INSTANCE(&productHandle->h_workingGStringsHandle,5) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorFactoryCreateProductInstance

// Public function; see .h
int l_BlotcodeExecutorGetKarllibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle, int karllibEntry,
  KARLLIB_EXECUTOR_HANDLE *a_karllibExecutorHandle) {
  m_DIGGY_BOLLARD()
  void *r_karllibExecutorHandle = (void *)UNDEFINED;
  int result = BlotcodeExecutorGetBlotlibExecutorHandle(handle,karllibEntry,
    &r_karllibExecutorHandle);
  switch (result) {
  case RESULT__FOUND:
    *a_karllibExecutorHandle =
      (KARLLIB_EXECUTOR_HANDLE)r_karllibExecutorHandle;
    m_CHECK_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE,*a_karllibExecutorHandle)
  break; case RESULT__NOT_FOUND:
    m_RAISE(ANOMALY__VALUE__D,result)
  break; default:
    m_TRACK()
  } // switch 

  m_DIGGY_RETURN(RETURNED)
} // l_BlotcodeExecutorGetKarllibExecutorHandle


///////////////////////

// Public function; see .h
int KarllibExecutorGetBlotpam(KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blotpamName,
  G_STRINGS_HANDLE *ac_pointsHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlotpamStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blotpamName);
  int result = m_GStringsIndexSingleFetch(handle->h_blotpamsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlotpamStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    { BLOTPAM_HANDLE blotpamHandle = (BLOTPAM_HANDLE)ct_namedBlotpamStuff->acolyt.cnhr_handle;
      m_ASSERT(blotpamHandle != NULL)
      *ac_pointsHandle = blotpamHandle->h_pointsHandle;
    } // blotpamHandle
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // KarllibExecutorGetBlotpam


// Public function; see .h
int KarllibExecutorCreateBlotpam(KARLLIB_EXECUTOR_HANDLE handle, struct P_STRING blotpamName,
  G_STRINGS_HANDLE *na_pointsHandle) { 
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlotpamStuff = (G_STRING_STUFF)UNDEFINED;
  BLOTPAM_HANDLE h_blotpamHandle = (BLOTPAM_HANDLE)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blotpamName);
  int result = m_GStringsIndexSingleFetch(handle->h_blotpamsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlotpamStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT(t_namedBlotpamStuff->acolyt.cnhr_handle != NULL)
    h_blotpamHandle = (BLOTPAM_HANDLE) t_namedBlotpamStuff->acolyt.cnhr_handle;
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(BlotpamCreateInstance(&h_blotpamHandle,1969) !=
      RETURNED)
    t_namedBlotpamStuff->acolyt.cnhr_handle = h_blotpamHandle;
  break; default:
    m_TRACK()
  } // switch
  //if (na_blotpamHandle != NULL) *na_blotpamHandle = t_namedBlotpamStuff->acolyt.cnhr_handle;
  if (na_pointsHandle != NULL) *na_pointsHandle = h_blotpamHandle->h_pointsHandle;

  m_DIGGY_RETURN(completed)
} // KarllibExecutorCreateBlotpam




enum {
      EVAL__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY = 0,
  OUTPUT_F__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY,
  KARLLIB_LOCAL_BLOTFUNC_NAMES_NUMBER,
} ;

static const char* localBlotfuncNames[] = { "Eval" , "OutputF" } ; 


// #SEE BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION 
static int KarllibExecutorExecuteCFunction(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  KARLLIB_EXECUTOR_HANDLE handle = (KARLLIB_EXECUTOR_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE,handle)
  *ac_blotval = TRUE__BLOTVAL0; // a priori

m_DIGGY_VAR_P_STRING(ap_blotfunc->call.referral)
m_DIGGY_VAR_P_STRING(ap_blotfunc->call.arguments)

  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case EVAL__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
//    switch (m_BlotexlibExecutorExecuteCFunctionEval(handle,ap_blotfunc->call.arguments,
//      c_surrogate, ac_blotval, nc_abandonmentInfo)) {
//    case ANSWER__YES:
//      break;
//    break; case ANSWER__NO:
//      m_DIGGY_RETURN(ANSWER__NO)
//    break; default:
//      m_TRACK()
//    } // switch

  break; case OUTPUT_F__KARLLIB_LOCAL_BLOTFUNC_NAME_ENTRY:
//    switch (m_BlotexlibExecutorExecuteCFunctionOutputF(handle,ap_blotfunc->call.arguments,
//      c_surrogate, ac_blotval, nc_abandonmentInfo)) {
//    case ANSWER__YES:
//      break;
//    break; case ANSWER__NO:
//      m_DIGGY_RETURN(ANSWER__NO)
//    break; default:
//      m_TRACK()
//    } // switch

  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotfunc->entry.localBlotfuncNameEntry)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // KarllibExecutorExecuteCFunction

// #SEE BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION <karl>
static int KarllibExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  KARLLIB_EXECUTOR_HANDLE xh_handle = (KARLLIB_EXECUTOR_HANDLE)xhr_handle;
  m_CHECK_MAGIC_FIELD(KARLLIB_EXECUTOR_HANDLE,xh_handle)

  //m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotregsHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_blotpamsHandle) != RETURNED)

  //m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_workingGStringsHandle) != RETURNED)

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // KarllibExecutorDestroyInstance


// Public function; see .h
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

