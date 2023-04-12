// c-ansi/stderr-kitchen.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004

#ifndef __C_ANSI_STDERR_KITCHEN_H_INCLUDED
#define __C_ANSI_STDERR_KITCHEN_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "c-ansi/types.h"



// Report (that is, raise) an anomaly in stderr
//
// Passed:
// - p_file: source file (__FILE__)
// - p_function: source function (__func__)
// - line: source line number (__LINE__)
// - p_anomaly: #REF p_anomaly
// - ...: variadic parameters ("a la" printf()) of format string
void StderrRaise(const char *p_file, const char *p_function, int line, const char *p_anomaly,...);


// Track an anomaly in stderr
//
// Passed:
// - p_file: source file (__FILE__)
// - p_function: source function (__func__)
// - line: source line number (__LINE__)
void StderrTrack(const char *p_file, const char *p_function, int line);


// Report whether an anomaly has already been raised (in the process)
//
// Returned
// - true :
// - false :
char b_Anomaly(void);

				  
// MACROS
// ------

// RAISE family macros
// --------------------

// StderrRaise() wrapper.
// Aim: populate file/function/line parameters "off-stage"
//
// Passed:
// - p_anomaly: #SEE p_anomaly
#define STDERR_RAISE(/*const char* */ p_anomaly, ...) StderrRaise(__FILE__,__func__,__LINE__,\
  p_anomaly, ##__VA_ARGS__)


// Raise anomaly and return ad hoc tracking value.
//
// Passed:
// - trackingValue : #REF trackingValue
//   the "special return value" that convey "raised anomaly" #ENDEF
// - p_anomaly: #SEE p_anomaly
#define m_RAISE_V(trackingValue, /*const char* */ p_anomaly, ...) {\
  STDERR_RAISE(p_anomaly, ##__VA_ARGS__);\
  return trackingValue;\
}


// Raise anomaly in case of "unexpected condition"
//
// Passed:
// - trackingValue : #SEE trackingValue 
// - ub_unexpectedCondition: "once evaluated" boolean expression testing the abnormal condition
// - p_anomaly: #SEE p_anomaly
#define m_RAISE_IF_V(trackingValue, ub_unexpectedCondition, /*const char* */ p_anomaly, ...) {\
  if (ub_unexpectedCondition) m_RAISE_V(trackingValue,p_anomaly, ##__VA_ARGS__) \
}

// Raise anomaly  in case of "unexpected condition" ; use verbatim that "unexpected condition"
// as anomaly description...
//
// Passed:
// - trackingValue : #SEE trackingValue 
// - ub_unexpectedCondition: "once evaluated" boolean expression testing the abnormal condition
#define m_RAISE_VERBATIM_IF_V(trackingValue, ub_unexpectedCondition) m_RAISE_IF_V(trackingValue,\
  ub_unexpectedCondition, # ub_unexpectedCondition)



// TRACK family macros
// --------------------

// StderrTrack() wrapper.
// Aim: populate file/function/line parameters "offstage"
#define STDERR_TRACK() \
StderrTrack(__FILE__,__func__,__LINE__)


// Track an anomaly with ad hoc tracking value.
// - trackingValue : #SEE trackingValue 
#define m_TRACK_V(trackingValue) {\
  STDERR_TRACK();\
  return trackingValue;\
}

//
// - trackingValue : #SEE trackingValue 
// - ub_unexpectedCondition: "once evaluated" boolean expression;
#define m_TRACK_IF_V(trackingValue, ub_unexpectedCondition) {\
  if (ub_unexpectedCondition) m_TRACK_V(trackingValue) \
}

// PERROR family macros
// ---------------------

// raise "perror" and return "ad hoc" tracking value.
//
// Passed :
// - trackingValue : #SEE trackingValue 
// - str: point out failing system call..
#define m_PERROR_V(trackingValue, /*const char* */ str) {\
  perror(str) ;\
  m_TRACK_V(trackingValue) \
}

// raise "perror" in case of "unexpected condition" and return "ad hoc" tracking value.
//
// Passed :
// - trackingValue : #SEE trackingValue 
// - ub_unexpectedCondition: "once evaluated" boolean expression;
// - str: point out failing system call..
#define m_PERROR_IF_V(trackingValue, ub_unexpectedCondition, /*const char* */ str) {\
  if (ub_unexpectedCondition) m_PERROR_V(trackingValue,str) \
}


// raise "perror" in case of "unexpected condition", use that "unexpected condition" as legend,
// and return "ad hoc" tracking value.
//
// Passed :
// - trackingValue : #SEE trackingValue 
// - ub_unexpectedCondition: "once evaluated" boolean expression;
#define m_PERROR_VERBATIM_IF_V(trackingValue, ub_unexpectedCondition) {\
  if (ub_unexpectedCondition) m_PERROR_V(trackingValue, # ub_unexpectedCondition) \
}

#endif
