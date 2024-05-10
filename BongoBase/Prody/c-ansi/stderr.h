// c-ansi/stderr.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004

#ifndef __C_ANSI_STDERR_H_INCLUDED
#define __C_ANSI_STDERR_H_INCLUDED

/////////////////////////////
#include "c-ansi/stderr.topo"
/////////////////////////////
#include "c-ansi/stderr-kitchen.h"
#include <stdlib.h>

// TODO: protect against crash

// RAISE family macros
//--------------------

// Default anomaly tracking value - do NOT change it
#define DEFAULT_TRACKING_VALUE (int)-1

// #REF CURRENT_TRACKING_VALUE Current anomaly tracking value - redefine it when needed...  
#define CURRENT_TRACKING_VALUE DEFAULT_TRACKING_VALUE 

// Raise anomaly and return CURRENT_TRACKING_VALUE "anomaly's tracking value".
//
// Passed:
// - p_anomaly: #SEE p_anomaly@c-ansi/stderr-kitchen.h
#define m_RAISE(/*const char* */ p_anomaly, ...) m_RAISE_V(CURRENT_TRACKING_VALUE, p_anomaly,\
   ##__VA_ARGS__)


// Raise anomaly in case of "unexpected condition"
//
// Passed :
// - ub_unexpectedCondition: "once evaluated" boolean expression testing the abnormal condition
// - p_anomaly: #SEE p_anomaly@c-ansi/stderr-kitchen.h
#define m_RAISE_IF(ub_unexpectedCondition,/*const char* */ p_anomaly, ...) m_RAISE_IF_V(\
  CURRENT_TRACKING_VALUE, ub_unexpectedCondition, p_anomaly, ##__VA_ARGS__)


// Raise anomaly in case of "unexpected condition" ; use that "verbatim" unexpected condition
// as anomaly...
//
// Passed :
// - ub_unexpectedCondition: "once evaluated" boolean expression testing the abnormal condition
#define m_RAISE_VERBATIM_IF(ub_unexpectedCondition) m_RAISE_VERBATIM_IF_V(CURRENT_TRACKING_VALUE,\
  ub_unexpectedCondition)


// Verify an "assert" ; raise corresponding anomaly if not respected...
// 
// Passed :
// - ub_assertion:
#define m_ASSERT(ub_assertion) m_RAISE_VERBATIM_IF(!(ub_assertion))


// "Last resort" actions...

// May be redefined as needed...
#define m_FATAL_ACTION() exit(-1);

// Raise anomaly and undertake "fatal action" allowing to interrupt execution WITHOUT
// returning from the function call...
// Use this macro only in case anomaly tracking is not possible (i.e "technical routines" and
// "pure" functions).
//
// Passed:
// - p_anomaly: #SEE p_anomaly@c-ansi/stderr-kitchen.h
#define m_RAISE_FATAL(/*const char* */ p_anomaly, ...) {\
  STDERR_RAISE(p_anomaly, ##__VA_ARGS__);\
  m_FATAL_ACTION() \
}


// Check whether a "forgotten" anomaly is not correctly tracked.
// Typically, you invoke this macro in process's main function...
// In order to avoid catching a tracked anomaly, a delay is observed before returning from
// current function...
//
// Passed:
// - delay: delay in seconds 
#define m_RAISE_IF_UNTRACKED(/*int*/delay) if (b_Anomaly()) {\
  STDERR_RAISE("UNTRACKED anomaly (?)...");\
  sleep(delay);\
  m_RAISE("UNTRACKED anomaly !");\
}

// Common anomalies...

// If your anomaly detection is based on some simple value check, these formats are at your
// disposal...
#define ANOMALY__VALUE__D		"=%d"
#define ANOMALY__VALUE__P		"=%p"

#define ANOMALY__SHOULD_NOT_BE_HERE "Should not be here..."
#define ANOMALY__NOT_AVAILABLE "Not available"
#define ANOMALY__UNEXPECTED_CASE "Unexpected case"


// TRACK family macros
// --------------------

// Track an anomaly with CURRENT_TRACKING_VALUE tracking value.
#define m_TRACK() \
m_TRACK_V(CURRENT_TRACKING_VALUE)

// Track anomaly in case of "unexpected condition"
//
// Passed :
// - ub_unexpectedCondition: "once evaluated" boolean expression;
#define m_TRACK_IF(ub_unexpectedCondition) m_TRACK_IF_V(\
  CURRENT_TRACKING_VALUE, ub_unexpectedCondition)

// "Last resort" action...
  
// Track an anomaly and undertake some "fatal action" allowing to interrupt execution WITHOUT
// returning from the function call...
// Designed for NON "status-returning" functions (i.e "technical routines" and "pure" functions).
//
// Passed:
// - m_fatalAction: e.g. exit(-1);
#define m_TRACK_AND(m_fatalAction) {\
  STDERR_TRACK();\
  m_fatalAction \
}

// PERROR family macros
// ---------------------

// raise "perror" and return CURRENT_TRACKING_VALUE tracking value.
//
// Passed :
// - str: point out the failing system call
#define m_PERROR(/*const char* */ str) m_PERROR_V(CURRENT_TRACKING_VALUE, str)


// raise "perror" in case of "unexpected condition" and return CURRENT_TRACKING_VALUE tracking value.
//
// Passed :
// - ub_unexpectedCondition: "once evaluated" boolean expression;
// - str: point out failing system call: (malloc(), read() etc.),...
#define m_PERROR_IF(ub_unexpectedCondition,/*const char* */ str) m_PERROR_IF_V(\
  CURRENT_TRACKING_VALUE, ub_unexpectedCondition, str)


// raise "perror" in case of "unexpected condition" and return CURRENT_TRACKING_VALUE tracking value.
// That "unexpected condition" is passed verbatim to perror (supposed to contain name of the
// system call that incurred the "error").
//
// Passed :
// - ub_unexpectedCondition: "once evaluated" boolean expression;
#define m_PERROR_VERBATIM_IF(ub_unexpectedCondition) m_PERROR_VERBATIM_IF_V(\
  CURRENT_TRACKING_VALUE, ub_unexpectedCondition)



#endif
