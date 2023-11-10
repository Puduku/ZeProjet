// flint/types.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: elementary definitions 
// =======

// #SEE double-inclusion-4-generic-imaged-enums @ flint/images.h <__FLINT_TYPES_H_INCLUDED__>


#ifndef __FLINT_TYPES_H_INCLUDED__
#define __FLINT_TYPES_H_INCLUDED__ 0
#endif

#if __FLINT_TYPES_H_INCLUDED__ == 0 || __FLINT_TYPES_H_INCLUDED__ == 2 

#if __FLINT_TYPES_H_INCLUDED__ == 0
#include "flint/ct-asserts.h"
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


// 2. Generic integers
// ------------------- 

// Use that int type to transport safely common integers via "void *"
// TODO: platform dependent ??
// TODO: redondant avec intptr_t  
typedef long GENERIC_INTEGER ;

m_CT_ASSERT(sizeof(GENERIC_INTEGER)==sizeof(void *))


// 3. Safe casts
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


// 4. Basic enums
// -------------- 


#endif // __FLINT_TYPES_H_INCLUDED__ == 0

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ __FLINT_TYPES_H_INCLUDED__
#include "flint/images.h"

// Programs' exit status
// TODO: use GENERIC m_DEFINE_TERNARY_ENUM() instead
m_DEFINE_ENUM_CASE_BEGIN(ExitStatusImage)
  m_ENUM_CASE_VAL(SUCCESS__EXECUTIVE__EXIT_STATUS, EXIT_SUCCESS)
  m_ENUM_CASE_VAL(FAILURE__EXECUTIVE__EXIT_STATUS, EXIT_FAILURE)
  m_ENUM_CASE_VAL(NON_EXECUTIVE__EXIT_STATUS, 100)
  // exit status in [0-99] means effective execution ; 0 always means "success"...
  // exit status 100 means exit WITHOUT execution  
  // (bad option(s) arguments(s), simple help request, etc.) 
m_DEFINE_ENUM_CASE_END();


// Manage double inclusion: update inclusion state:
#if __FLINT_TYPES_H_INCLUDED__ == 0
#undef __FLINT_TYPES_H_INCLUDED__
// Manage double inclusion: provoke 2nd inclusion:
#define __FLINT_TYPES_H_INCLUDED__ 2
#include "flint/types.h"
#else 
#undef __FLINT_TYPES_H_INCLUDED__
#define __FLINT_TYPES_H_INCLUDED__ 3
#endif

#endif // __FLINT_TYPES_H_INCLUDED__ == 0 || __FLINT_TYPES_H_INCLUDED__ == 2

