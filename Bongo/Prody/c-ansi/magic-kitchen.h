// c-ansi/magic-kitchen.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// PURPOSE: magic number to control @ runtime consistency of virtual data types - that cannot be
// verified @ compile time - SCULLERY

#ifndef __C_ANSI_MAGIC_KITCHEN_H_INCLUDED__
#define __C_ANSI_MAGIC_KITCHEN_H_INCLUDED__

#include "c-ansi/stderr.h"


typedef unsigned long g_MAGIC_VALUE_unsigned_long ;


// Calculate simple hash value for C-string. We use such hash as "magic number"...
//
// Passed:
// - p_str: '\0' terminated (C-)string
//
// Returned:
// - hash value
g_MAGIC_VALUE_unsigned_long SimpleHash (const unsigned char *p_str) ;


// Calculate "magic" (hash) number for some type name
// 
// Passed:
// - m_type: the type name
//
// Returned
// - The corresponding  magic value 
#define K_GET_MAGIC(m_type) SimpleHash((const unsigned char *) #m_type)


// Check that passed magic value is that corresponding to actual type ;
// raise anomaly if requisite.
// #SEE DEFAULT_TRACKING_VALUE @ c-ansi/stderr.h
//
// Passed:
// - m_type: the actual type name
// - magicValue: present (existent) magic value
#define m_CHECK_MAGIC_VALUE(m_type,  /*g_MAGIC_VALUE_unsigned_long*/magicValue) \
  if (magicValue != K_GET_MAGIC(m_type)) {\
    m_RAISE("Bad magic field for " #m_type " (actual:%lx - expected:%lx)",\
      magicValue,K_GET_MAGIC(m_type))\
  }


// Magic field support:
// --------------------

// Set up the name (in structure) of "magic field".
//
// Passed:
// - m_HandleType: (public) name of handle type
#define m_MAGIC_FIELD_NAME(m_handleType) magic ## m_handleType


// Design "magic field" corresponding to some handle type.
//
// Passed:
// - m_HandleType: (public) name of handle type
// - handle: struct-based object handle (i.e struct pointer)
#define MAGIC_FIELD(m_handleType, handle) \
  ((m_handleType)(handle))->m_MAGIC_FIELD_NAME(m_handleType)


#endif // __C_ANSI_MAGIC_KITCHEN_H_INCLUDED__
