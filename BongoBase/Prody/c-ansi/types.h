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


// Generic statuses (returned by "Procedures")
// -------------------------------------------

// Generic Unary status...
// This is actually the ONLY possible return status of a "true" procedure
m_DEFINE_UNARY_ENUM(ReturnedImage,
  RETURNED, )// return from procedure (tacitly "completed")

// Binary statuses...

// "Natural" dual verity values of the girls:
m_DEFINE_BINARY_ENUM(AnswerImage,
  ANSWER__YES,,// procedure completed ...and the answer is "YES !!"
  ANSWER__NO,) // procedure completed ...and the answer is "NO !!"

// "Natural" dual verity values of the guys:
m_DEFINE_BINARY_ENUM(AttemptImage,
  ATTEMPT__SUCCESSFUL,,// this try is successful
  ATTEMPT__TRY_AGAIN,) // this try was not lucky

// Dual verity values allowing the ‘glass half full’ case to be expressed. (and easily transformed
// in unary verity by European Institutions)
m_DEFINE_BINARY_ENUM(CompletedImage,
  COMPLETED__OK,, // procedure completed,
  COMPLETED__BUT,)// procedure completed, but we want to point something ; we all agree (OK) 
    // to the fact that we don't agree (BUT)...

// Universal dual verity values of the "constructivists"
m_DEFINE_BINARY_ENUM(ResultImage,
  RESULT__FOUND,,    // completed and fruitful research
  RESULT__NOT_FOUND,)// completed research, but did not come to fruition


// Ternary "comparison" status...

m_DEFINE_TERNARY_ENUM(ComparisonImage,
     LESS_THAN__COMPARISON,,
      EQUAL_TO__COMPARISON,,
  GREATER_THAN__COMPARISON,)


// Handling differences and comparisons status
// -------------------------------------------

// Simple "universal" macro to compare 2 integral values...
// Prerequisites: A and B are rigorously of same type.
// 
// Passed:
// - m_integralA: A
// - m_integralB: B
//
// Ret: "trivial" difference (int type)
// - -1: A < B
// - 0: A == B
// - +1: A > B
#define /*int*/ GET_DIFFERENCE(m_integralA,m_integralB) ((m_integralA) > (m_integralB)? 1 :\
  ((m_integralA) == (m_integral)? 0: -1))

// Get comparison status corresponding to difference
//
// Passed:
// - difference: (int type)
//
// Returned:
// - LESS_THAN__COMPARISON: difference < 0
// - EQUAL_TO__COMPARISON: difference == 0
// - GREATER_THAN__COMPARISON: difference > 0
#define DIFFERENCE_2_COMPARISON(/*int*/difference) ((difference) < 0? LESS_THAN__COMPARISON:\
  ((difference) > 0? GREATER_THAN__COMPARISON: EQUAL_TO__COMPARISON))

// Simple "universal" macro to compare 2 integral values...
// Prerequisites: A and B are rigorously of same type.
// 
// Passed:
// - m_integralA: A
// - m_integralB: B
//
// Ret: comparison status
// - LESS_THAN__COMPARISON: A < B
// - EQUAL_TO__COMPARISON: A == B
// - GREATER_THAN__COMPARISON: A > B
#define /*int*/ GET_COMPARISON(m_integralA,m_integralB) ( (m_integralA) > (m_integralB)?\
  GREATER_THAN__COMPARISON: ((m_integralA) == (m_integralB)? EQUAL_TO__COMPARISON:\
  LESS_THAN__COMPARISON) )


// Passed:
// - comparison:
//   + LESS_THAN__COMPARISON
//   + EQUAL_TO__COMPARISON
//   + GREATER_THAN__COMPARISON
//
// Changed: 
// - m_difference: trivial difference
//   + -1: LESS_THAN__COMPARISON
//   + 0: EQUAL_TO__COMPARISON
//   + 1: GREATER_THAN__COMPARISON:
#define m_COMPARISON_2_DIFFERENCE(/*int*/comparison, /*int*/m_difference)  switch (comparison) {\
case LESS_THAN__COMPARISON:\
  m_difference = -1;\
break; case EQUAL_TO__COMPARISON:\
  m_difference = 0;\
break; case GREATER_THAN__COMPARISON:\
  m_difference = 1;\
break; default:\
  m_RAISE(ANOMALY__VALUE__D,comparison)\
} 


#endif // __C_ANSI_TYPES_H_INCLUDED__

