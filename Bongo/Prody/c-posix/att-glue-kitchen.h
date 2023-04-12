// c-posix/att-glue-kitchen.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose:
// --------
// - att-glue module's "kitchen" ; definition of platform dependent macros...

#ifndef __C_POSIX__ATT_GLUE_KITCHEN__H__INCLUDED__
#define __C_POSIX__ATT_GLUE_KITCHEN__H__INCLUDED__

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <limits.h>

#include "c-ansi/att.h"
#include "flint/tid.h"

// Possible signedness assumptions:
// - the type is signed:
#define ATT_GLUE_SIGNED_ASSUMPTION 1
// - the type is not signed:
#define ATT_GLUE_UNSIGNED_ASSUMPTION 2


// 1. tid_t assumptions
// --------------------

// assumption : sizeof(tid_t) == sizeof(unsigned int)

#define K_TID_2_ATT_ID(p_handle, tid) (tid)

#define K_ATT_ID_2_TID(p_handle, id) (id)


// 2. time_t assumptions
// ---------------------

// This assumption may be platform dependent:
#define ATT_GLUE_TIME_T_SIGNEDNESS_ASSUMPTION ATT_GLUE_SIGNED_ASSUMPTION

#if ATT_GLUE_TIME_T_SIGNEDNESS_ASSUMPTION == ATT_GLUE_SIGNED_ASSUMPTION
// assumption : both time_t and ATT_TIME correspond to "long" type

#define K_ATT_2_EPOCH_TIME(p_handle, attTime)   (attTime)

#define K_EPOCH_2_ATT_TIME(p_handle, epochTime) (epochTime)


#elif ATT_GLUE_TIME_T_SIGNEDNESS_ASSUMPTION == ATT_GLUE_UNSIGNED_ASSUMPTION
// assumption : 
// - time_t corresponds to "unsigned long" type
// - ATT_TIME corresponds to "long" type

#define K_ATT_2_EPOCH_TIME(p_handle, attTime)   ( ((unsigned long)(attTime)) + LONG_MAX + 1 )

#define K_EPOCH_2_ATT_TIME(p_handle, epochTime) ( (long)((epochTime) - LONG_MAX - 1) )


#else
#error "unexpected ATT_GLUE_TIME_T_SIGNEDNESS_ASSUMPTION"
#endif

#endif // __C_POSIX__ATT_GLUE_KITCHEN__H__INCLUDED__
