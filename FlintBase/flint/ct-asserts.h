// flint/ct-asserts.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023
//
// Purpose: Compile Time asserts...
// =======

#ifndef __FLINT_CT_ASSERTS_H_INCLUDED__
#define __FLINT_CT_ASSERTS_H_INCLUDED__

// CT asserts : THE "KITCHEN"...
// --------------------------

// Adapted from http://www.pixelbeat.org/programming/gcc/static_assert.html
#define m_K_CONCAT(m_a, m_b) m_a##m_b

//
// - line: assertion's line number
// - u_assertion: (once evaluated) assertion to verify 
#define m_K_CT_ASSERT(line,u_assertion) enum { m_K_CONCAT(k_assert_, line) = 1/(!!(u_assertion)) } ;


// CT asserts : THE "MENU"...
// -----------------------

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



#endif //  __FLINT_CT_ASSERTS_H_INCLUDED__

