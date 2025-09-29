// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/g-string.h"
#include "c-ansi/stderr.h"
#include "c-ansi/named-object.h"

#include "c-ansi/testy-diggy.h"



struct PAMPHLET_SPOT_DESCRIPTION {
  int size;
} ;

struct PAMPHLET_DESCRIPTION {
  G_STRINGS_HANDLE h_spotDescriptionsHandle ; 
} ;

// Public function: see .h
int EngelsCreateInstance() {
  m_DIGGY_BOLLARD()

  m_DIGGY_RETURN(RETURNED)
} // EngelsCreateInstance

// PamphletExamplars: 

// === SPOTs =====

struct SPOT_OCCURENCE {
  // Localization 
  int offset ; // >= 0 (vis-a-vis spot) 
} ;
typedef struct SPOT_OCCURENCE *SPOT_OCCURENCE_STUFF;

struct SPOT {
  // Localization 
  int offset ; // >= 0 (vis-a-vis parent spot) 
  int depth ; // >= 0 
  int totalLength ; // >= 0 (all occurences)

  // Entry in "spot descriptions"
  int spotDescriptionEntry ;

  GREEN_COLLECTION_HANDLE h_spotOccurencesHandle; 

  G_STRINGS_HANDLE h_subSpotsHandle ; // SPOT as NAMED_OBJECT
  m_DECLARE_MAGIC_FIELD(SPOT_HANDLE)
} ;
typedef struct SPOT *SPOT_HANDLE;

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int SpotDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  SPOT_HANDLE xh_handle = (SPOT_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(SPOT_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_occurencesHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_subSpotsHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // SpotDestroyInstance

// Public function: see .h
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
static int SpotCreateInstance(SPOT_HANDLE *azh_handle,
   int fieldsNumber) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  SPOT_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(SPOT_HANDLE,handle)

  handle->offset = 0;
  handle->depth = 0;
  handle->totalLength = 0;
  handle->descriptionEntry = 0;
 
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_occurencesHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    sizeof(struct SPOT_OCCURENCE), (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL,
    (GREEN_HANDLER__COMPARE_FUNCTION)NULL,(GREEN_HANDLER__EQUATE_FUNCTION)NULL,
    (void*)UNDEFINED) != RETURNED) 

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_subSpotsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    SpotDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_spotsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)
  m_DIGGY_RETURN(RETURNED)
} // SpotCreateInstance


// ======== PAMPHLET_EXAMPLARs ======

struct PAMPHLET_EXAMPLAR {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE)
  struct P_STRING examplar ;
  // description entry in "pamphlets" descriptions
  int pamphletDescriptionEntry ;
  G_STRINGS_HANDLE h_spotsHandle ; // SPOT as NAMED_OBJECT
} ; 

// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletExamplarCreateInstance(PAMPHLET_EXAMPLAR_HANDLE *azh_handle, int dummy) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_EXAMPLAR_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE,handle)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_spotsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    SpotDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_spotsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarCreateInstance


// Public function; see .h
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletExamplarDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_EXAMPLAR_HANDLE xh_handle = (PAMPHLET_EXAMPLAR_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_spotsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarDestroyInstance

