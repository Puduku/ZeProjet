#include "c-posix/att-glue.h"
#include "c-posix/att-glue-kitchen.h"
#include "c-ansi/alloc.h"


#include "c-ansi/int-runtime-test.h"

#include "c-ansi/diggy.h"

struct ATT_GLUE {
  int dummy;
} ;


// Ret:
// - RETURNED:
// -1: anomaly is raised
static int AttGlueSignednessTests (ATT_GLUE_HANDLE handle) {
  m_CHECK_SAME_SIZE(tid_t,g_ATT_ID_unsigned_int)
  m_CHECK_UNSIGNED(g_ATT_ID_unsigned_int)


#if T_ATT_TIME_T_SIGNEDNESS_ASSUMPTION == T_ATT_SIGNED_ASSUMPTION
#define ULTIMATE_EPOCH_TIME LONG_MAX
#define PRIMEVAL_EPOCH_TIME LONG_MIN

  m_CHECK_SAME_SIGNEDNESS(time_t,gen_ATT_TIME_long)
  m_CHECK_SIGNED(time_t)

#elif T_ATT_TIME_T_SIGNEDNESS_ASSUMPTION == T_ATT_UNSIGNED_ASSUMPTION
#define ULTIMATE_EPOCH_TIME ULONG_MAX
#define PRIMEVAL_EPOCH_TIME 0

  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(handle,PRIMEVAL_ATT_TIME), 0)
  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(handle,PRIMEVAL_ATT_TIME + 1), 1)
  m_CHECK_SAME_SIZE(time_t,gen_ATT_TIME_long)
  m_CHECK_UNSIGNED(time_t)

#else
#error "unexpected T_ATT_TIME_T_SIGNEDNESS_ASSUMPTION"
#endif

  m_CHECK_SAME_SIZE(time_t,gen_ATT_TIME_long)
  m_CHECK_SAME_SIZE(gen_ATT_TIME_long,long)
  m_CHECK_SIGNED(gen_ATT_TIME_long)
  m_CHECK_EQUALITY(gen_ATT_TIME_long, ULTIMATE_ATT_TIME, LONG_MAX)
  m_CHECK_EQUALITY(gen_ATT_TIME_long, PRIMEVAL_ATT_TIME, LONG_MIN)
  m_CHECK_EQUALITY(gen_ATT_TIME_long, EPOCH_2_ATT_TIME(handle,ULTIMATE_EPOCH_TIME), ULTIMATE_ATT_TIME)
  m_CHECK_EQUALITY(gen_ATT_TIME_long, EPOCH_2_ATT_TIME(handle,PRIMEVAL_EPOCH_TIME), PRIMEVAL_ATT_TIME)
  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(handle,ULTIMATE_ATT_TIME), ULTIMATE_EPOCH_TIME)
  m_CHECK_EQUALITY(time_t, ATT_2_EPOCH_TIME(handle,PRIMEVAL_ATT_TIME), PRIMEVAL_EPOCH_TIME)
  m_CHECK_COMPARISON(gen_ATT_TIME_long, 0, <, ULTIMATE_ATT_TIME)
  m_CHECK_COMPARISON(gen_ATT_TIME_long, PRIMEVAL_ATT_TIME, <, ULTIMATE_ATT_TIME)
  m_CHECK_COMPARISON(gen_ATT_TIME_long, PRIMEVAL_ATT_TIME, <, 0)
  m_CHECK_COMPARISON(gen_ATT_TIME_long, EPOCH_2_ATT_TIME(handle,PRIMEVAL_EPOCH_TIME), <, EPOCH_2_ATT_TIME(handle,ULTIMATE_EPOCH_TIME))
  m_CHECK_COMPARISON(gen_ATT_TIME_long, EPOCH_2_ATT_TIME(handle,0), <, EPOCH_2_ATT_TIME(handle,ULTIMATE_EPOCH_TIME))
  m_CHECK_COMPARISON(time_t, PRIMEVAL_EPOCH_TIME, <, ULTIMATE_EPOCH_TIME)
  m_CHECK_COMPARISON(time_t, ATT_2_EPOCH_TIME(handle,PRIMEVAL_ATT_TIME), <, ULTIMATE_EPOCH_TIME)

  return RETURNED;
} // AttGlueSignednessTests


// Public function: see .h
int AttGlueCreateInstance (ATT_GLUE_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()

   
  m_MALLOC_INSTANCE(*azh_handle)

  (*azh_handle)->dummy = UNDEFINED;

  m_TRACK_IF(AttGlueSignednessTests(*azh_handle) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // AttGlueCreateInstance


// Public function: see .h
int AttGlueDestroyInstance(ATT_GLUE_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // AttGlueDestroyInstance
