// flint/types.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: elementary definitions 
// =======

#ifndef __FLINT_TYPES_H_INCLUDED__
#define __FLINT_TYPES_H_INCLUDED__


#include "flint/types-kitchen.h"
#include <stdlib.h>


// 1. Basic constants 
// ------------------

// Sentinel (integer) value for undefined variables
#define UNDEFINED 0xFADADE69

// Booleans

// Reminder: C's "technical" convention:
// Boolean value <----> Integer value 
// FALSE         <----> 0 
// TRUE          <----> != 0  

// The constant is named "FALSE0", just to emphase that 0 DOES represent "false"
// boolean value (hence, any kind of memory "zero-ization" de facto initializes boolean variables
// to "false")
#define b_FALSE0 0
#define b_TRUE  (!b_FALSE0)


// 2. Checks at "Compile time" (CT) 
// --------------------------------

// Verify assertion at "Compile Time"... 
// 
// Passed:
// - u_assertion: (once evaluated) assertion to verify 
#define m_CT_ASSERT(u_assertion) m_K_CT_ASSERT(__LINE__,u_assertion)

// See https://stackoverflow.com/questions/6280055/how-do-i-check-if-a-variable-is-of-a-certain-type-compare-two-types-in-c
// Check at compile time that some value is of a particular type.
//
// Passed: 
// - m_type: expected value type
// - p_val: some value 
#define m_CT_CHECK_VAL_TYPE(m_type,p_val) {\
  m_type em_dummyVar1; \
  typeof(p_val) em_dummyVar2; \
  (void)(&em_dummyVar1 == &em_dummyVar2); \
}


// 3. Generic integers
// ------------------- 

// Use that int type to transport safely common integers via "void *"
// TODO: platform dependent ??
// TODO: redondant avec intptr_t  
typedef long GENERIC_INTEGER ;

m_CT_ASSERT(sizeof(GENERIC_INTEGER)==sizeof(void *))


// 4. Safe casts
// -------------

// "Safe" assignation between pointers (seen as "data addresses") requiring a "cast" operation,
// due to type mismatch with 'const' qualifier. That is, this macro allows only assignation of a
// "read only" pointer into a "read and write" one, given the ONLY difference between the two
// pointers' type is the presence of 'const' qualifier for the "right" (assignee) pointer.
// If the assignation is not safe, an error (or warning, depending on compiler's settings) is raised
// at compile time...  
// 
// Passed:
// - m_lPtrType: left pointer's type (WITHOUT 'const' qualifier) 
// - p_rPtr: right (assignee, value) pointer to assign (pointed data is protected by 'const' qualifier)
//
// Created:
// - mep_lPtr: left (assigned, variable) pointer (REMOVED 'const' qualifier's protection of pointed data)
#define m_ASSIGN_LOCAL_EP_PTR__SAFE_CAST(m_lPtrType, mep_lPtr,  p_rPtr) \
m_CT_CHECK_VAL_TYPE(const m_lPtrType,p_rPtr); \
m_lPtrType mep_lPtr = (m_lPtrType) p_rPtr;


// TODO: #define EP_PTR__SAFE_CAST(m_epPtrType,  p_ptr)


// 5. Image functions (for enums...)
// ------------------

// #REF Enum-type-image-function
// static inline const char* <Enum type image function name>(int value)
 
// Simple macro to define unary enum types with their image function...
//
// Passed: 
// - m_enumImageInlineFunction: 
// - m_case1:
// - m_val1:
#define m_DEFINE_UNARY_ENUM(m_enumImageInlineFunction, m_case1, m_val1) \
enum {\
  m_case1 m_val1,\
} ;\
static inline const char* m_enumImageInlineFunction(int value) {\
  m_ENUM_IMAGE_FUNCTION__OUTSET(value)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
  m_ENUM_IMAGE_FUNCTION__BUTT()\
} 

// Simple macro to define binary enum types with their image function...
//
// Passed: 
// - m_enumImageInlineFunction:
// - m_case1:
// - m_val1:
// - m_case2:
// - m_val2:
#define m_DEFINE_BINARY_ENUM(m_enumImageInlineFunction, m_case1, m_val1, \
  m_case2, m_val2) \
enum {\
  m_case1 m_val1,\
  m_case2 m_val2,\
} ;\
static inline const char* m_enumImageInlineFunction(int value) {\
  m_ENUM_IMAGE_FUNCTION__OUTSET(value)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
  m_ENUM_IMAGE_FUNCTION__BUTT()\
} 

// Simple macro to define ternary enum types with their image function...
//
// Passed: 
// - m_enumImageInlineFunction:
// - m_case1:
// - m_val1:
// - m_case2:
// - m_val2:
// - m_case3:
// - m_val3:
#define m_DEFINE_TERNARY_ENUM(m_enumImageInlineFunction, m_case1, m_val1, \
  m_case2, m_val2, m_case3, m_val3) \
enum {\
  m_case1 m_val1,\
  m_case2 m_val2,\
  m_case3 m_val3,\
} ;\
static inline const char* m_enumImageInlineFunction(int value) {\
  m_ENUM_IMAGE_FUNCTION__OUTSET(value)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case3)\
  m_ENUM_IMAGE_FUNCTION__BUTT()\
} 

// Simple macro to define quaternary enum types with their image function...
//
// Passed: 
// - m_enumImageInlineFunction:
// - m_case1:
// - m_val1:
// - m_case2:
// - m_val2:
// - m_case3:
// - m_val3:
#define m_DEFINE_QUATERNARY_ENUM(m_enumImageInlineFunction, m_case1, m_val1, \
  m_case2, m_val2, m_case3, m_val3, m_case4, m_val4) \
enum {\
  m_case1 m_val1,\
  m_case2 m_val2,\
  m_case3 m_val3,\
  m_case4 m_val4,\
} ;\
static inline const char* m_enumImageInlineFunction(int value) {\
  m_ENUM_IMAGE_FUNCTION__OUTSET(value)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case3)\
  m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case4)\
  m_ENUM_IMAGE_FUNCTION__BUTT()\
} 


// 6. Basic enums
// -------------- 

// Programs' exit status
m_DEFINE_TERNARY_ENUM(ExitStatusImage,
  SUCCESS__EXECUTIVE__EXIT_STATUS, = EXIT_SUCCESS,
  FAILURE__EXECUTIVE__EXIT_STATUS, = EXIT_FAILURE,
  NON_EXECUTIVE__EXIT_STATUS, = 100 )
  // exit status in [0-99] means effective execution ; 0 always means "success"...
  // exit status 100 means exit WITHOUT execution  
  // (bad option(s) arguments(s), simple help request, etc.) 

#endif //  __FLINT_TYPES_H_INCLUDED__

