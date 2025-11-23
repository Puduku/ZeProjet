// c-ansi/named-object.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_NAMED_OBJECT_H_INCLUDED
#define __C_ANSI_NAMED_OBJECT_H_INCLUDED

#include "c-ansi/g-string.h" 
#include "c-ansi/testy-diggy.h"

struct NAMED_OBJECTS ;
typedef struct NAMED_OBJECTS* NAMED_OBJECTS_HANDLE ;

typedef G_STRING_STUFF g_NAMED_OBJECT_STUFF ;

// Assign actual acolyt handle to 'named object'
// 
// Passed
// - stuff: 
// - nhr_handle: named object's head handle
// - n_gStringsHandle: (when not NULL) g-strings collection which the named object belongs to
// 
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised...
static inline int m_NamedObjectAssign(g_NAMED_OBJECT_STUFF stuff, void *nhr_handle,
  G_STRINGS_HANDLE n_gStringsHandle) {
  m_DIGGY_BOLLARD_S()
  m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(n_gStringsHandle,FIRST_ELEMENT0,
    NAMED_OBJECT__G_STRING_CONVEYANCE)
  stuff->acolyt.cnhr_handle = NULL;
  stuff->acolyt.cnhr_handle = nhr_handle;
  m_DIGGY_RETURN(RETURNED)
} // m_NamedObjectAssign 


// Passed:
// - *azhr_handle: UNDEFINED handle 
// - name:
// - r_arguments
//
// Modified:
// - *azhr_handle: instance's handle
//
// Returned:
// - RETURNED: done
// - -1: unexpected problem ; anomaly is raised
typedef int (*NAMED_OBJECT_CREATE_INSTANCE_FUNCTION)(void **azhr_handle, struct P_STRING name, void *r_arguments);


// => Create collection of NAMED_OBJECT__G_STRING_CONVEYANCE g-strings 
// => The function also adds plain lexical index (INDEX_LABEL0) 
// Also passed:
// - n_namedObjectCreateInstanceFunction: != NULL => automatic creation of named objects
// - namedObjectDestroyInstanceFunction:
// #SEE GStringsCreateInstance@c-ansi/g-string.h  <named-object>
int NamedObjectsCreateInstance(NAMED_OBJECTS_HANDLE *azh_handle,int expectedItemsNumber,
  NAMED_OBJECT_CREATE_INSTANCE_FUNCTION n_namedObjectCreateInstanceFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION namedObjectDestroyInstanceFunction);

// #SEE GStringsGetCount @ c-ansi/g-string.h <named-object>
int NamedObjectsGetCount(NAMED_OBJECTS_HANDLE cp_handle, g_NAMED_OBJECT_STUFF *navnt_namedObjectStuff);

// Add new named object to colllection...
//
// Passed:
// - handle: named objects collection
// - namedObjectName:
// - hr_namedObjectHandle: new named object instance to add in the collection; its handle
// - nar_namedObjectHandle: NULL special pointer if not used
// 
// Changed:
// - handle: new named object added if not already exists in collection...
// - at_namedObjectStuff: named object of the collection ; its g-string
//
// Ret:
// - COMPLETED__OK: new (empty) named object added in the collection 
// - COMPLETED__BUT: named object with same name already exists in collection
// - -1: unexpected problem; anomaly is raised...
int NamedObjectsAddNamedObject(NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName,
  void *cr_arguments, g_NAMED_OBJECT_STUFF *at_namedObjectStuff); 
  
// Get named object of colllection...
//
// Passed:
// - handle: named objects collection
// - namedObjectName:
// 
// Changed:
// - *acvnr_namedObjectHandle: (set to NULL if not found) named object of the collection
//
// Ret:
// - RESULT__FOUND: 
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised...
int NamedObjectsGetNamedObject(NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName,
  void **acvnr_namedObjectHandle); 


// #SEE GStringsDestroyInstance @ c-ansi/g-string.h <named-object>
int NamedObjectsDestroyInstance(NAMED_OBJECTS_HANDLE xh_handle) ; 


#endif // __C_ANSI_NAMED_OBJECT_H_INCLUDED
