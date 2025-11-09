// c-ansi/named-object.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_NAMED_OBJECT_H_INCLUDED
#define __C_ANSI_NAMED_OBJECT_H_INCLUDED

#include "c-ansi/g-string.h" 
#include "c-ansi/testy-diggy.h"

typedef G_STRINGS_HANDLE g_NAMED_OBJECTS_HANDLE ;

typedef G_STRING_STUFF g_NAMED_OBJECT_STUFF ;

// => The function also adds plain lexical index (INDEX_LABEL0) 
// #SEE GStringsCreateInstance@c-ansi/g-string.h  <named-object>
// Reminder: NAMED_OBJECT__G_STRING_CONVEYANCE => object handle acolyt 
int l_NamedObjectsCreateInstance(g_NAMED_OBJECTS_HANDLE *azh_handle,int expectedItemsNumber,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION namedObjectDestroyInstanceFunction);


// #SEE GStringsDestroyInstance @ c-ansi/g-string.h <named-object>
static inline int m_NamedObjectsDestroyInstance(g_NAMED_OBJECTS_HANDLE xh_handle) {
  m_DIGGY_BOLLARD_S()
  m_TRACK_IF(GStringsDestroyInstance(xh_handle) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // m_NamedObjectsDestroyInstance  


// Manipulate g-string as 'named object'
// 
// Passed
// - stuff: stuff of (supposed 'named object') g-string
// - nhr_handle: object's (head) handle
// - n_gStringsHandle: (when non NULL) "reference" named objects collection
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsNamedObject(G_STRING_STUFF stuff, void *nhr_handle,
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_DIGGY_BOLLARD_S()
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,FIRST_ELEMENT0,
    NAMED_OBJECT__G_STRING_CONVEYANCE)
  stuff->acolyt.cnhr_handle = NULL;
  stuff->acolyt.cnhr_handle = nhr_handle;
  m_DIGGY_RETURN(RETURNED)
} // m_GStringAsNamedObject 

#endif // __C_ANSI_NAMED_OBJECT_H_INCLUDED
