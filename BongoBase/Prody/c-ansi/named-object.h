// c-ansi/named-object.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_NAMED_OBJECT_H_INCLUDED
#define __C_ANSI_NAMED_OBJECT_H_INCLUDED

#include "c-ansi/g-string.h" 


// #REF NAMED_OBJECTS_CREATE_INSTANCE <named-object>
// #SEE GStringsCreateInstance@c-ansi/g-string.h  <g-string>
// Note: NAMED_OBJECT__G_STRING_CONVEYANCE => object handle acolyt 
#define /*int*/ NAMED_OBJECTS_CREATE_INSTANCE(/*G_STRINGS_HANDLE*/ azh_handle,\
  /*int*/ expectedItemsNumber,\
  /*NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION*/namedObjectDestroyInstanceFunction)\
  GStringsCreateInstance(azh_handle, expectedItemsNumber,1, NAMED_OBJECT__G_STRING_CONVEYANCE,\
  (const int *)UNDEFINED,namedObjectDestroyInstanceFunction)


// Manipulate g-string as 'named object'
// 
// Passed
// - stuff:
// - nhr_handle: object's (head) handle
// - n_gStringsHandle:
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_GStringAsNamedObject(G_STRING_STUFF stuff, void *nhr_handle,
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,FIRST_ELEMENT0,
    NAMED_OBJECT__G_STRING_CONVEYANCE)
  stuff->acolyt.cnhr_handle = nhr_handle;
  return RETURNED;
} // m_GStringAsNamedObject 

#endif // __C_ANSI_NAMED_OBJECT_H_INCLUDED
