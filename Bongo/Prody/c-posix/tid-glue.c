#include "c-posix/tid-glue.h"

#include "c-ansi/int-runtime-test.h"
#include "c-ansi/green.h"
#include "c-ansi/alloc.h"

#include <unistd.h>
#define _GNU_SOURCE
#include <unistd.h>

#include "c-ansi/diggy.h"

// Possible platform assumptions:
#define GEEK_RULED__PLATFORM_ASSUMPTION 1
#define SANE__PLATFORM_ASSUMPTION 2


// This assumption may be platform dependent:
#define PLATFORM_ASSUMPTION	GEEK_RULED__PLATFORM_ASSUMPTION



struct PTHREAD_VS_TID {
  pthread_t pthread;
  tid_t tid;
} ;

typedef struct PTHREAD_VS_TID *PTHREAD_VS_TID_STUFF;

struct TID_GLUE {
  char b_portable;
  GREEN_COLLECTION_HANDLE ch_pthreadVsTidCollectionHandle;
} ;


// #see GREEN_ITEM_HANDLER__KEYS_COMPARE_FUNCTION @ c-ansi/green.h
static int TidGlueKeysCompare (void *cpr_handle, char b_frozen, int indexLabel, int keyRank,
  char *pr_aGreenItemStuff, char *npr_bGreenItemStuff, void *cpr_bKeys) {
  m_ASSERT(keyRank == 0)

  tid_t aKeyTid = ((PTHREAD_VS_TID_STUFF) pr_aGreenItemStuff)->tid;
  tid_t bKeyTid = (tid_t)(GENERIC_INTEGER)cpr_bKeys;
  if (npr_bGreenItemStuff != NULL) {
    bKeyTid = ((PTHREAD_VS_TID_STUFF) npr_bGreenItemStuff)->tid;
  } // if

  return GET_COMPARISON(aKeyTid,bKeyTid);
} // TidGlueKeysCompare


#if PLATFORM_ASSUMPTION == GEEK_RULED__PLATFORM_ASSUMPTION
#elif PLATFORM_ASSUMPTION == SANE__PLATFORM_ASSUMPTION

// Ret:
// - RETURNED:
// -1: anomaly is raised
static int SignednessTests (void) {
  m_CHECK_SAME_SIZE(tid_t,g_ATT_ID_unsigned_int)
  m_CHECK_UNSIGNED(g_ATT_ID_unsigned_int)

// TODO: faire les BONS tests (pthread_t vs. tid_t)... 
  m_CHECK_SAME_SIGNEDNESS(time_t,gen_ATT_TIME_long)
  m_CHECK_SIGNED(time_t)

  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(PRIMEVAL_ATT_TIME), 0)
  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(PRIMEVAL_ATT_TIME + 1), 1)
  m_CHECK_SAME_SIZE(time_t,gen_ATT_TIME_long)
  m_CHECK_UNSIGNED(time_t)

  m_CHECK_SAME_SIZE(time_t,gen_ATT_TIME_long)
  m_CHECK_SAME_SIZE(gen_ATT_TIME_long,long)
  m_CHECK_SIGNED(gen_ATT_TIME_long)

  m_CHECK_UNSIGNED(pthread_t)

  return RETURNED;
} // SignednessTests

#else
#error "unexpected PLATFORM_ASSUMPTION"
#endif


// Public function: see .h
int TidGlueCreateInstance(TID_GLUE_HANDLE *azh_handle, int b_portable) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)

  if (((*azh_handle)->b_portable = b_portable)) {
    m_TRACK_IF(GreenCollectionCreateInstance(&(*azh_handle)->ch_pthreadVsTidCollectionHandle,
      BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct PTHREAD_VS_TID), NULL, TidGlueKeysCompare,
      (void *)UNDEFINED) != RETURNED)
    m_ASSERT(GreenCollectionAddIndex((*azh_handle)->ch_pthreadVsTidCollectionHandle,1) == 0)
  } else {
#if PLATFORM_ASSUMPTION == GEEK_RULED__PLATFORM_ASSUMPTION
    m_RAISE(ANOMALY__NOT_AVAILABLE)
#elif PLATFORM_ASSUMPTION == SANE__PLATFORM_ASSUMPTION

    m_TRACK_IF(SignednessTests() != RETURNED)
#else
#error "unexpected PLATFORM_ASSUMPTION"
#endif
  } // if

  m_DIGGY_RETURN(RETURNED)
} // TidGlueCreateInstance


// Public function: see .h
int TidGlueSelfPthread2Tid (TID_GLUE_HANDLE handle, tid_t *a_tid) {
  m_DIGGY_BOLLARD()
  pthread_t pthread = pthread_self();  

  if (handle->b_portable) {
    PTHREAD_VS_TID_STUFF t_pthreadVsTidStuff = (PTHREAD_VS_TID_STUFF) UNDEFINED;

    *a_tid = gettid() ;
    switch (GreenCollectionIndexFetch(handle->ch_pthreadVsTidCollectionHandle, NULL, 0,
      INDEX_FETCH__FETCH,INDEX_SEEK__KEY, (char **)&t_pthreadVsTidStuff, NULL,
      (void *)(GENERIC_INTEGER)*a_tid)) {
    case RESULT__FOUND:
    case RESULT__NOT_FOUND:
      t_pthreadVsTidStuff->pthread = pthread ;
      t_pthreadVsTidStuff->tid = *a_tid;
    break; default:
      m_TRACK()
    } // switch
  } else {
#if PLATFORM_ASSUMPTION == GEEK_RULED__PLATFORM_ASSUMPTION
    m_RAISE(ANOMALY__NOT_AVAILABLE)
#elif PLATFORM_ASSUMPTION == SANE__PLATFORM_ASSUMPTION
    *a_tid = pthread;
#else
#error "unexpected PLATFORM_ASSUMPTION"
#endif
  } // if

  m_DIGGY_RETURN(RETURNED)
} // TidGlueSelfPthread2Tid


// Public function: see .h
int TidGlueTid2Pthread (TID_GLUE_HANDLE handle, tid_t tid, pthread_t *ac_pthread) {
  m_DIGGY_BOLLARD()
  int result = RESULT__FOUND;

  if (handle->b_portable) {
    PTHREAD_VS_TID_STUFF t_pthreadVsTidStuff = (PTHREAD_VS_TID_STUFF) UNDEFINED;
    result = GreenCollectionIndexFetch(handle->ch_pthreadVsTidCollectionHandle, NULL, 0,
      INDEX_FETCH__READ_ONLY,INDEX_SEEK__KEY, (char**)&t_pthreadVsTidStuff, NULL,
      (void *)(GENERIC_INTEGER)tid);
    switch (result) {
    case RESULT__FOUND:
      *ac_pthread = t_pthreadVsTidStuff->pthread;
    break; case RESULT__NOT_FOUND:
    break; default:
      m_TRACK()
    } // switch
  } else {
#if PLATFORM_ASSUMPTION == GEEK_RULED__PLATFORM_ASSUMPTION
    m_RAISE(ANOMALY__NOT_AVAILABLE)
#elif PLATFORM_ASSUMPTION == SANE__PLATFORM_ASSUMPTION
    *ac_pthread = tid;
#else
#error "unexpected PLATFORM_ASSUMPTION"
#endif
  } // if

  m_DIGGY_RETURN(result)
} // TidGlueTid2Pthread


// Public function: see .h
int TidGlueDestroyInstance(TID_GLUE_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  if (xh_handle->b_portable) {
    m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->ch_pthreadVsTidCollectionHandle) !=
      RETURNED)
  } // if

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // TidGlueDestroyInstance
