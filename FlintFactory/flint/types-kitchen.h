// flint/types-kitchen.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: elementary definitions (kitchen) 
// =======

#ifndef __FLINT_TYPES_KITCHEN_H_INCLUDED__
#define __FLINT_TYPES_KITCHEN_H_INCLUDED__



// Adapted from http://www.pixelbeat.org/programming/gcc/static_assert.html
#define m_K_CONCAT(m_a, m_b) m_a##m_b

//
// - line: assertion's line number
// - u_assertion: (once evaluated) assertion to verify 
#define m_K_CT_ASSERT(line,u_assertion) enum { m_K_CONCAT(k_assert_, line) = 1/(!!(u_assertion)) } ;


// Image functions
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


#endif //  __FLINT_TYPES_KITCHEN_H_INCLUDED__

