// c-ansi/named-object.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include "c-ansi/named-object.h" 
#include "c-ansi/testy-diggy.h"


struct NAMED_OBJECTS {
  G_STRINGS_HANDLE h_gStringsHandle;
  NAMED_OBJECT_CREATE_INSTANCE_FUNCTION n_namedObjectCreateInstanceFunction;
} ; // 
  


// Public function: see .h
int NamedObjectsCreateInstance(NAMED_OBJECTS_HANDLE *azh_handle, int expectedItemsNumber,
  NAMED_OBJECT_CREATE_INSTANCE_FUNCTION n_namedObjectCreateInstanceFunction,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION namedObjectDestroyInstanceFunction){
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  NAMED_OBJECTS_HANDLE handle = *azh_handle;
  m_TRACK_IF(GStringsCreateInstance(&handle->h_gStringsHandle, expectedItemsNumber,1,
    NAMED_OBJECT__G_STRING_CONVEYANCE, (const int *)UNDEFINED,namedObjectDestroyInstanceFunction)
    != RETURNED)
  m_ASSERT(m_GStringsAddPlainLexicalIndex(handle->h_gStringsHandle, NULL,NULL) == INDEX_LABEL0)
  handle->n_namedObjectCreateInstanceFunction = n_namedObjectCreateInstanceFunction;
  m_DIGGY_RETURN(RETURNED)
} // NamedObjectsCreateInstance

// Public function: see .h
int NamedObjectsGetCount(NAMED_OBJECTS_HANDLE cp_handle, g_NAMED_OBJECT_STUFF *navnt_namedObjectStuff) {
  m_DIGGY_BOLLARD()
  int count = GStringsGetCount(cp_handle->h_gStringsHandle,navnt_namedObjectStuff);
  m_TRACK_IF(count < 0)
  m_DIGGY_RETURN(count)
} // NamedObjectsGetCount


// Public function: see .h
int NamedObjectsAddNamedObject(NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName, 
  void *cr_arguments, g_NAMED_OBJECT_STUFF *at_namedObjectStuff){ 
  m_DIGGY_BOLLARD()
  int completed = COMPLETED__OK; // a priori
  struct G_KEY gKey = m_GKey_PString(namedObjectName);
  switch (m_GStringsIndexSingleFetch(handle->h_gStringsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,at_namedObjectStuff,NULL)) {
  case RESULT__FOUND:
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(GStringCopy(*at_namedObjectStuff,0,namedObjectName) < 0) 
    if (handle->n_namedObjectCreateInstanceFunction != NULL) {
      void *hr_namedObjectHandle = (void*)UNDEFINED;
      m_TRACK_IF(handle->n_namedObjectCreateInstanceFunction(&hr_namedObjectHandle,namedObjectName,
        cr_arguments) != RETURNED)
      m_TRACK_IF(m_NamedObjectAssign(*at_namedObjectStuff,hr_namedObjectHandle,
        handle->h_gStringsHandle) != RETURNED)
    } // if
  break; default: m_TRACK() } // switch
  m_DIGGY_RETURN(completed)
} // NamedObjectsAddNamedObject

// Public function: see .h
int NamedObjectsGetNamedObject(NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName,
  void **acvnr_namedObjectHandle){ 
  m_DIGGY_BOLLARD()
  g_NAMED_OBJECT_STUFF ct_namedObjectStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(namedObjectName);
  int result = m_GStringsIndexSingleFetch(handle->h_gStringsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedObjectStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    *acvnr_namedObjectHandle = ct_namedObjectStuff->acolyt.cnhr_handle;
  break; case RESULT__NOT_FOUND:
    *acvnr_namedObjectHandle = NULL;
  break; default: m_TRACK() } // switch
  m_DIGGY_RETURN(result)
} // NamedObjectsAddNamedObject

// Public function: see .h
int NamedObjectsDestroyInstance(NAMED_OBJECTS_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_gStringsHandle) != RETURNED)
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // NamedObjectsDestroyInstance
