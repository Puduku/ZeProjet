// c-ansi/magic.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// PURPOSE: magic number to control @ runtime consistency of virtual data types - that cannot
// be verified @ compile time...

#ifndef __C_ANSI_MAGIC_H_INCLUDED__
#define __C_ANSI_MAGIC_H_INCLUDED__

#include "c-ansi/stderr.h"
#include "c-ansi/magic-kitchen.h"


  
// Magic FIELD support
// -------------------
// "Magic field" support is meant for "virtualized" struct-based data types.
// => The name of (public) handle type is used to calculate the magic value.


// Declare the magic field in actual (private) structure
//
// Passed:
// - m_HandleType: (public) name of handle type
#define m_DECLARE_MAGIC_FIELD(m_handleType) \
  g_MAGIC_VALUE_unsigned_long m_MAGIC_FIELD_NAME(m_handleType) ;


// Assign "magic field" reference value.
// Call this macro when initializing the actual structure instance...
//
// Passed:
// - m_HandleType: (public) name of handle type
// - handle: struct-based object handle (i.e struct pointer)
#define m_ASSIGN_MAGIC_FIELD(m_handleType, handle) \
  MAGIC_FIELD(m_handleType,handle) = K_GET_MAGIC(m_handleType) ;


// Check that "magic" number found in some (virtual) handle corresponds to "magic" reference value ;
// raise anomaly if requisite.
// #SEE DEFAULT_TRACKING_VALUE@c-ansi/stderr.h
//
// Passed:
// - m_HandleType: (public) name of handle type
// - handle: struct-based object handle (i.e struct pointer)
// TODO: proteger contre crash si pointeur est foireux...
#define m_CHECK_MAGIC_FIELD(m_handleType, handle) \
  m_CHECK_MAGIC_VALUE(m_handleType,MAGIC_FIELD(m_handleType,handle))


#endif // __C_ANSI_MAGIC_H_INCLUDED__
