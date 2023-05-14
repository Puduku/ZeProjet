// flint/images.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: Displaying images of (simple) data
// =======

#ifndef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__

//
// Image functions : THE "KITCHEN"...
// ---------------

// Enum types image functions:

// Canvas to build enum's image function
// void <EnumImageFunction> (int <enumValue>) {
//  m_ENUM_IMAGE_FUNCTION__OUTSET(<enumValue>)
//  m_ENUM_IMAGE_FUNCTION__BODY_CASE(<ENUM CASE 1>)
//  m_ENUM_IMAGE_FUNCTION__BODY_CASE(<ENUM CASE 2>)
//  . . .
//  m_ENUM_IMAGE_FUNCTION__BUTT(<enumValue>)
// }


// Passed:
// - enumValue:
#define m_ENUM_IMAGE_FUNCTION__OUTSET(m_enumValue) \
{\
  const char *p_image = "?";\
  switch (m_enumValue) {


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


#endif //  __FLINT_IMAGES_H_INCLUDED__

