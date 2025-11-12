// c-ansi/named-object.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include "c-ansi/named-object.h" 
#include "c-ansi/testy-diggy.h"


// Public function: see .h
int l_NamedObjectsCreateInstance(g_NAMED_OBJECTS_HANDLE *azh_handle, int expectedItemsNumber,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION namedObjectDestroyInstanceFunction){
  m_DIGGY_BOLLARD()
  m_TRACK_IF(GStringsCreateInstance(azh_handle, expectedItemsNumber,1, NAMED_OBJECT__G_STRING_CONVEYANCE,
    (const int *)UNDEFINED,namedObjectDestroyInstanceFunction) != RETURNED)
  m_ASSERT(m_GStringsAddPlainLexicalIndex(*azh_handle, NULL,NULL) == INDEX_LABEL0)
  m_DIGGY_RETURN(RETURNED)
} // l_NamedObjectsCreateInstance

// Public function: see .h
int l_NamedObjectsAddNamedObject(g_NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName,
  g_NAMED_OBJECT_STUFF *at_namedObjectStuff){ 
  m_DIGGY_BOLLARD()
  int completed = COMPLETED__OK; // a priori
  struct G_KEY gKey = m_GKey_PString(namedObjectName);
  switch (m_GStringsIndexSingleFetch(handle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,at_namedObjectStuff,NULL)) {
  case RESULT__FOUND:
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(GStringCopy(*at_namedObjectStuff,0,namedObjectName) < 0) 
  break; default: m_TRACK() } // switch
  m_DIGGY_RETURN(completed)
} // l_NamedObjectsAddNamedObject

// Public function: see .h
int l_NamedObjectsGetNamedObject(g_NAMED_OBJECTS_HANDLE handle, struct P_STRING namedObjectName,
  void **acvnr_namedObjectHandle){ 
  m_DIGGY_BOLLARD()
  g_NAMED_OBJECT_STUFF ct_namedObjectStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(namedObjectName);
  int result = m_GStringsIndexSingleFetch(handle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedObjectStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    *acvnr_namedObjectHandle = ct_namedObjectStuff->acolyt.cnhr_handle;
  break; case RESULT__NOT_FOUND:
    *acvnr_namedObjectHandle = NULL;
  break; default: m_TRACK() } // switch
  m_DIGGY_RETURN(result)
} // l_NamedObjectsAddNamedObject

