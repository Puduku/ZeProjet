// flint/images.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023
//
// Purpose: Displaying images of (simple) 'enum' data types  
// =======

// Manage double inclusions : conventions with __FLINT_IMAGES_H_INCLUDED__
// - undefined or 0: not yet included, OR 1st phase of inclusion (re-)triggered
// - 1: 1st phase of inclusion completed
// - 2: 2nd phase of inclusion (re-)triggered <= to be set by external modules
// - 3: 2nd phase of inclusion completed

#ifndef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ 0
#endif

#if __FLINT_IMAGES_H_INCLUDED__ == 0

#undef m_ENUM_IMAGE_FUNCTION__OUTSET
#undef m_ENUM_IMAGE_FUNCTION__BODY_CASE
#undef m_ENUM_IMAGE_FUNCTION__BUTT
#undef m_DEFINE_UNARY_ENUM
#undef m_DEFINE_BINARY_ENUM
#undef m_DEFINE_TERNARY_ENUM
#undef m_DEFINE_QUATERNARY_ENUM
#undef m_ENUM_CASE
#undef m_ENUM_CASE_VAL
#undef m_DEFINE_ENUM_BEGIN
#undef m_DEFINE_ENUM_END

//
// Image functions : THE "KITCHEN"...
// ---------------

// Enum types image functions:


// Passed:
// - m_enumImageInlineFunction:
#define m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumImageInlineFunction) \
static inline const char* m_enumImageInlineFunction(int value) {\
  const char *p_image = "?";\
  switch (value) {


//
// Passed:
// - m_case:
#define m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case) \
  case m_case:\
    p_image = #m_case ;\
    break ;


//
// Passed:
// - enumValue:
#define m_ENUM_IMAGE_FUNCTION__BUTT() \
  default:\
    p_image = "?" ;\
  } \
  return p_image;\
}


// Image functions (for enums...) : THE "MENU"...
// ---------------

// #REF Enum-type-image-function
// Generated enum image inline functions are declared as: 
// static inline const char* <image function name> (int value)
 
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
m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumImageInlineFunction)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
m_ENUM_IMAGE_FUNCTION__BUTT()

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
m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumImageInlineFunction)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
m_ENUM_IMAGE_FUNCTION__BUTT()

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
m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumImageInlineFunction)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case3)\
m_ENUM_IMAGE_FUNCTION__BUTT()

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
m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumImageInlineFunction)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case1)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case2)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case3)\
m_ENUM_IMAGE_FUNCTION__BODY_CASE(m_case4)\
m_ENUM_IMAGE_FUNCTION__BUTT()

// Simple macros to define GENERIC enum types with their image function...

// To start "imaged" enum declaration 
//
// Passed:
// - m_enumImageInlineFunction:
#define m_DEFINE_ENUM_BEGIN(m_enumImageInlineFunction) enum {\

// "imaged" enum declaration : case name 
//
// Passed:
// - m_case:
#define m_ENUM_CASE(m_case) m_case,

// "imaged" enum declaration : case name with value 
//
// Passed:
// - m_case:
// - m_val:
#define m_ENUM_CASE_VAL(m_case, m_val) m_case = m_val,

// To finish "imaged" enum declaration 
#define m_DEFINE_ENUM_END() }

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ 1

#elif __FLINT_IMAGES_H_INCLUDED__ == 2

#undef m_DEFINE_ENUM_BEGIN
#undef m_ENUM_CASE
#undef m_ENUM_CASE_VAL
#undef m_DEFINE_ENUM_END

// Usage: see above
#define m_DEFINE_ENUM_BEGIN(m_enumImageInlineFunction) m_ENUM_IMAGE_FUNCTION__OUTSET(\
  m_enumImageInlineFunction) 

// Usage: see above
#define m_ENUM_CASE(m_case) \
  case m_case:\
    p_image = #m_case ;\
    break ;

// Usage: see above
#define m_ENUM_CASE_VAL(m_case, m_val) \
  case m_case:\
    p_image = #m_case ;\
    break ;

// Usage: see above
#define m_DEFINE_ENUM_END() m_ENUM_IMAGE_FUNCTION__BUTT()

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ 3

#endif // __FLINT_IMAGES_H_INCLUDED__


