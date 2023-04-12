// c-ansi/types.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: defining very usual data types... 
// =======

#ifndef __C_ANSI_TYPES_H_INCLUDED__
#define __C_ANSI_TYPES_H_INCLUDED__

////////////////////////////
#include "c-ansi/types.topo"
////////////////////////////

#include "flint/types.h"
//#include "c-ansi/diggy.h"

// Simple "universal" macro to compare 2 integral values...
// Prerequisites: a and b are rigorously of same type.
#define /*int*/ GET_DIFFERENCE(m_integralA,m_integralB) ((m_integralA) > (m_integralB)? 1 :\
  ((m_integralA) == (m_integral)? 0: -1))


// "procedures" & return "statuses"

// Unary status...
// This is actually the ONLY possible return status of a "true" procedure
m_DEFINE_UNARY_ENUM(ReturnedImage,
  RETURNED, )// return from procedure (tacitly "completed")

// Binary statuses...

m_DEFINE_BINARY_ENUM(AnswerImage,
  ANSWER__YES,,// procedure completed ...and the answer is "YES !!"
  ANSWER__NO,) // procedure completed ...and the answer is "NO !!"

m_DEFINE_BINARY_ENUM(AttemptImage,
  ATTEMPT__SUCCESSFUL,,// this try is successful
  ATTEMPT__TRY_AGAIN,) // this try was not lucky


m_DEFINE_BINARY_ENUM(CompletedImage,
  COMPLETED__OK,, // procedure completed,
  COMPLETED__BUT,)// procedure completed, but we want to point something...


m_DEFINE_BINARY_ENUM(ResultImage,
  RESULT__FOUND,,    // "search" completed... result is found" 
  RESULT__NOT_FOUND,)// "search" completed... result is not found...  


// Trinary status(es)...

m_DEFINE_TERNARY_ENUM(ComparisonImage,
     LESS_THAN__COMPARISON,,
      EQUAL_TO__COMPARISON,,
  GREATER_THAN__COMPARISON,)

// Passed:
// - difference:
//
// Returned:
// - LESS_THAN__COMPARISON: difference < 0
// - EQUAL_TO__COMPARISON: difference == 0
// - GREATER_THAN__COMPARISON: difference > 0
#define DIFFERENCE_2_COMPARISON(/*int*/difference) ((difference) < 0? LESS_THAN__COMPARISON:\
  ((difference) > 0? GREATER_THAN__COMPARISON: EQUAL_TO__COMPARISON))

// Simple "universal" macro to compare 2 integral values...
// Prerequisites: a and b are rigorously of same type.
#define /*int*/ GET_COMPARISON(m_integralA,m_integralB) ( (m_integralA) > (m_integralB)?\
  GREATER_THAN__COMPARISON: ((m_integralA) == (m_integralB)? EQUAL_TO__COMPARISON:\
  LESS_THAN__COMPARISON) )


// Passed:
// - comparison:
//   + LESS_THAN__COMPARISON
//   + EQUAL_TO__COMPARISON
//   + GREATER_THAN__COMPARISON
//
// Returned:
// - -1: LESS_THAN__COMPARISON
// - 0: EQUAL_TO__COMPARISON
// - 1: GREATER_THAN__COMPARISON:
#define m_COMPARISON_2_DIFFERENCE(/*int*/comparison, /*int*/m_difference)  switch (comparison) {\
case LESS_THAN__COMPARISON:\
  m_difference = -1;\
break; case EQUAL_TO__COMPARISON:\
  m_difference = 0;\
break; case GREATER_THAN__COMPARISON:\
  m_difference = 1;\
break; default:\
  m_RAISE(ANOMALY__VALUE__FMT_D,comparison)\
} 



#endif // __C_ANSI_TYPES_H_INCLUDED__

