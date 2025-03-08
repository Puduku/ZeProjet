// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/g-string.h"
#include "c-ansi/stderr.h"
#include "c-ansi/named-object.h"

#include "c-ansi/testy-diggy.h"



struct PAMPHLET_POINT_DESCRIPTION {
  int size;
} ;

struct PAMPHLET_DESCRIPTION {
  G_STRINGS_HANDLE h_pointDescriptionsHandle ; 
} ;

// Public function: see .h
int EngelsCreateInstance() {
  m_DIGGY_BOLLARD()

  m_DIGGY_RETURN(RETURNED)
} // EngelsCreateInstance

// PamphletExamplars: 

struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE {
  // Localization 
  int offset ; // >= 0 (vis-a-vis point) 
} ;
typedef struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE *PAMPHLET_EXAMPLAR_POINT_OCCURENCE_STUFF;

struct PAMPHLET_EXAMPLAR_POINT {
  // Localization 
  int offset ; // >= 0 (vis-a-vis parent point) 
  int depth ; // >= 0 
  int totalLength ; // >= 0 (all occurences)

  // Entry in "pamphlet point descriptions"
  int descriptionEntry ;

  GREEN_COLLECTION_HANDLE h_occurencesHandle; 

  G_STRINGS_HANDLE h_subPointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
  m_DECLARE_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE)
} ;
typedef struct PAMPHLET_EXAMPLAR_POINT *PAMPHLET_EXAMPLAR_POINT_HANDLE;

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int PamphletExamplarPointDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_EXAMPLAR_POINT_HANDLE xh_handle = (PAMPHLET_EXAMPLAR_POINT_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_occurencesHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_subPointsHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarPointDestroyInstance

// Public function: see .h
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
static int PamphletExamplarPointCreateInstance(PAMPHLET_EXAMPLAR_POINT_HANDLE *azh_handle,
   int fieldsNumber) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_EXAMPLAR_POINT_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_EXAMPLAR_POINT_HANDLE,handle)

  handle->offset = 0;
  handle->depth = 0;
  handle->totalLength = 0;
  handle->descriptionEntry = 0;
 
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_occurencesHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    sizeof(struct PAMPHLET_EXAMPLAR_POINT_OCCURENCE), (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL,
    (GREEN_HANDLER__COMPARE_FUNCTION)NULL,(GREEN_HANDLER__EQUATE_FUNCTION)NULL,
    (void*)UNDEFINED) != RETURNED) 

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_subPointsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    PamphletExamplarPointDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_pointsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)
  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarPointCreateInstance


//struct PAMPHLET_EXAMPLAR {
//  struct P_STRING examplar ;
//  // description entry in "pamphlets" descriptions
//  int pamphletDescriptionEntry ;
//  G_STRINGS_HANDLE *h_pointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
//} ;


////////////////////////////////
// blotpam <=> pamphlet examplar
////////////////////////////////
struct PAMPHLET_EXAMPLAR {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE)
  //G_STRINGS_HANDLE h_tableHandle;
  //G_STRINGS_HANDLE hp_fieldsHandle;
  struct P_STRING examplar ;
  // description entry in "pamphlets" descriptions
  int pamphletDescriptionEntry ;
  G_STRINGS_HANDLE h_pointsHandle ; // PAMPHLET_EXAMPLAR_POINT as NAMED_OBJECT
} ; 
typedef struct PAMPHLET_EXAMPLAR* PAMPHLET_EXAMPLAR_HANDLE;



// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletExamplarCreateInstance(PAMPHLET_EXAMPLAR_HANDLE *azh_handle, int dummy) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_EXAMPLAR_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE,handle)

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&handle->h_pointsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    PamphletExamplarPointDestroyInstance) != RETURNED)
  //GStringsAddIndex(handle->h_pointsHandle);

  //m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarCreateInstance


// Public function; see .h
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletExamplarDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_EXAMPLAR_HANDLE xh_handle = (PAMPHLET_EXAMPLAR_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_EXAMPLAR_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_pointsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarDestroyInstance

