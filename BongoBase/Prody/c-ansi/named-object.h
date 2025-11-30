// c-ansi/named-object.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __C_ANSI_NAMED_OBJECT_H_INCLUDED
#define __C_ANSI_NAMED_OBJECT_H_INCLUDED

#include "c-ansi/g-string.h" 
#include "c-ansi/testy-diggy.h"
#include <stdarg.h>

struct NAMED_OBJECTS ;
typedef struct NAMED_OBJECTS* NAMED_OBJECTS_HANDLE ;

typedef G_STRING_STUFF g_NAMED_OBJECT_STUFF ;


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
//typedef int (*NAMED_OBJECT_CREATE_INSTANCE_FUNCTION)(void **azhr_handle, struct P_STRING name, void *r_arguments);
typedef int (*NAMED_OBJECT_CREATE_INSTANCE_FUNCTION)(void **azhr_handle, struct P_STRING name, va_list arguments);

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
int NamedObjectsGetCount(NAMED_OBJECTS_HANDLE cp_handle,
  g_NAMED_OBJECT_STUFF *navnt_namedObjectStuff);

// Add new named object to colllection...
//
// Passed:
// - handle: named objects collection
// - namedObjectName:
// - nhr_namedObjectHandle: (when != NULL) new named object instance to add in the collection; its
//   handle
// - *ccr_arguments: (only significant if new named object instance is not provided) arguments for
//   named object instance creation function  (when significant)
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
//  void *nhr_namedObjectHandle, void *ccr_arguments, g_NAMED_OBJECT_STUFF *at_namedObjectStuff); 
  void *nhr_namedObjectHandle, g_NAMED_OBJECT_STUFF *at_namedObjectStuff, ...); 
  

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
