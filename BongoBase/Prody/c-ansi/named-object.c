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

