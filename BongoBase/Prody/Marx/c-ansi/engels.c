// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/g-string.h"
#include "c-ansi/stderr.h"
#include "c-ansi/named-object.h"

#include "c-ansi/testy-diggy.h"

// === I. models: === 
// ==================

#define        BINARY__RAW_MATTER_FLAG 0x01 // ON means "BINARY" ; OFF means "TEXT" 
#define LITTLE_ENDIAN__RAW_MATTER_FLAG 0x02 // requires "BINARY" 
#define       ISO_C99__RAW_MATTER_FLAG 0x04 // requires "TEXT" ; "scanf() ctime(), etc. support"

enum {
  LITTLE_ENDIAN_INTEGER__RAW_MATTER_FLAGS = BINARY__RAW_MATTER_FLAG & LITTLE_ENDIAN__RAW_MATTER_FLAG, 
     BIG_ENDIAN_INTEGER__RAW_MATTER_FLAGS = BINARY__RAW_MATTER_FLAG, 
           TEXT_INTEGER__RAW_MATTER_FLAGS = ISO_C99__RAW_MATTER_FLAG,
             TEXT_FLOAT__RAW_MATTER_FLAGS = ISO_C99__RAW_MATTER_FLAG,
              TEXT_DATE__RAW_MATTER_FLAGS = ISO_C99__RAW_MATTER_FLAG, // see strftime(), etc.
                   TEXT__RAW_MATTER_FLAGS = ALL_FLAGS_OFF0,
} ;

enum {
  TEXT__REFINED_MATTER, 
  INTEGER__REFINED_MATTER, 
  FLOAT__REFINED_MATTER,
  DATE__REFINED_MATTER,
} ;

// TRACT_OR_PIVOT_MODEL:

#define b_PIVOT b_TRUE
#define b_TRACT !b_PIVOT 

struct TRACT_OR_PIVOT_MODEL {
  m_DECLARE_MAGIC_FIELD(TRACT_OR_PIVOT_MODEL_HANDLE)
  char b_pivot ;
  union {
    int c_pivotModelEntry ; // Only significant with "pivot"
    int c_tractModelEntry; // Only significant if "not" pivot (aka aggregate)...
  } select;
} ;
typedef struct TRACT_OR_PIVOT_MODEL *TRACT_OR_PIVOT_MODEL_HANDLE;

//
static int TractOrPivotModelCreateInstance(TRACT_OR_PIVOT_MODEL_HANDLE *azh_handle,char b_pivot,
  int c_pivotModelEntry, int c_tractModelEntry) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(TRACT_OR_PIVOT_MODEL_HANDLE,*azh_handle)
  (*azh_handle)->b_pivot = b_pivot; 
  (*azh_handle)->select.c_pivotModelEntry = c_pivotModelEntry; 
  (*azh_handle)->select.c_tractModelEntry = c_tractModelEntry;
  m_DIGGY_RETURN(RETURNED)
} // TractModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int TractOrPivotModelDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()
  TRACT_OR_PIVOT_MODEL_HANDLE xh_handle = (TRACT_OR_PIVOT_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(TRACT_OR_PIVOT_MODEL_HANDLE,xh_handle)
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // TractOrPivotModelDestroyInstance
 
// PIVOT_MODEL:

struct PIVOT_MODEL {
  m_DECLARE_MAGIC_FIELD(PIVOT_MODEL_HANDLE)
  int n_rawSize;
  int rawMatterFlags;
  int refinedMatter; // must be compatible with raw matter flags
} ;
typedef struct PIVOT_MODEL *PIVOT_MODEL_HANDLE;

//
static int PivotModelCreateInstance(PIVOT_MODEL_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(PIVOT_MODEL_HANDLE,*azh_handle)
  m_DIGGY_RETURN(RETURNED)
} // PivotModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int PivotModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PIVOT_MODEL_HANDLE xh_handle = (PIVOT_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PIVOT_MODEL_HANDLE,xh_handle)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PivotModelDestroyInstance
 

// TRACT_MODEL:

struct TRACT_MODEL {
  m_DECLARE_MAGIC_FIELD(TRACT_MODEL_HANDLE)
  G_STRINGS_HANDLE h_tractOrPivotModelsHandle ; // managed as NAMED OBJECTs 
} ;
typedef struct TRACT_MODEL *TRACT_MODEL_HANDLE;

//
static int TractModelCreateInstance(TRACT_MODEL_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(TRACT_MODEL_HANDLE,*azh_handle)
  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&((*azh_handle)->h_tractOrPivotModelsHandle),10,
    TractOrPivotModelDestroyInstance) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // TractModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int TractModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  TRACT_MODEL_HANDLE xh_handle = (TRACT_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(TRACT_MODEL_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_tractOrPivotModelsHandle) != RETURNED) 

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // TractModelDestroyInstance

// PAMPHLET_MODEL:

struct PAMPHLET_MODEL {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE)
  G_STRINGS_HANDLE h_tractOrPivotModelsHandle ; // managed as NAMED OBJECTs  
} ;
typedef struct PAMPHLET_MODEL *PAMPHLET_MODEL_HANDLE;

//
static int PamphletModelCreateInstance(PAMPHLET_MODEL_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE,*azh_handle)
  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&((*azh_handle)->h_tractOrPivotModelsHandle),10,
    TractOrPivotModelDestroyInstance) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // PamphletModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int PamphletModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_MODEL_HANDLE xh_handle = (PAMPHLET_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_tractOrPivotModelsHandle) != RETURNED) 

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletModelDestroyInstance


// ===== ENGELS ===

struct ENGELS {
  G_STRINGS_HANDLE h_pivotModelsHandle ; // managed as NAMED OBJECTs 
  G_STRINGS_HANDLE h_tractModelsHandle ; // managed as NAMED OBJECTs 
  G_STRINGS_HANDLE h_pamphletModelsHandle ; // managed as NAMED OBJECTs 
} ;

// Public function: see .h
int EngelsCreateInstance(ENGELS_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle)
  ENGELS_HANDLE handle = *azh_handle;

  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(handle->h_pivotModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, PivotModelDestroyInstance) != RETURNED)
  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(handle->h_tractModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, TractModelDestroyInstance) != RETURNED)
  m_TRACK_IF(NAMED_OBJECTS_CREATE_INSTANCE(&(handle->h_pamphletModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, PamphletModelDestroyInstance) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // EngelsCreateInstance


// === II. examplars: === 
// ======================

// === TRACTs =====

// ULTRA GREEN ITEM:
struct TRACT_OCCURENCE {
  // Localization: 
  int offset ; // >= 0 (vis-a-vis tract) 
} ;
typedef struct TRACT_OCCURENCE *TRACT_OCCURENCE_STUFF;

// ULTRA GREEN ITEM:
struct TRACT_OR_PIVOT {
  char vb_pivot ;
  union {
    int c_pivotModelEntry;
    int c_tractModelEntry;
  } select ;
} ;
typedef struct TRACT_OR_PIVOT *TRACT_OR_PIVOT_STUFF;

// See GREEN_ITEM @ c-ansi/green.h
struct TRACT {
  int tractModelEntry;
  // Localization 
  int offset ; // >= 0 (vis-a-vis parent tract) 
  int depth ; // >= 0 
  int totalLength ; // >= 0 (all occurences)

  GREEN_COLLECTION_HANDLE nh_tractOccurencesHandle; 
  GREEN_COLLECTION_HANDLE nh_tractOrPivotsHandle ; 
} ;
typedef struct TRACT *TRACT_STUFF;

// Passed:
// - stuff: 
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
static int TractInitialize(TRACT_STUFF stuff,
   int fieldsNumber) {
  m_DIGGY_BOLLARD_S()

  if (stuff->nh_tractOccurencesHandle == NULL) {
    m_TRACK_IF(GreenCollectionCreateInstance(&stuff->nh_tractOccurencesHandle,
      BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct TRACT_OCCURENCE),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  } // if

  if (stuff->nh_tractOrPivotsHandle == NULL) {
    m_TRACK_IF(GreenCollectionCreateInstance(&stuff->nh_tractOrPivotsHandle,
      BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct TRACT_OR_PIVOT),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  } // if

  m_DIGGY_RETURN(RETURNED)
} // TractInitialize

// #SEE GREEN_HANDLER__DISENGAGE_FUNCTION @ c-ansi/green.h 
static int PamphletTractDisengage(void *r_handle, char *r_greenItemStuff) {
  m_DIGGY_BOLLARD_S()

  TRACT_STUFF tractStuff = (TRACT_STUFF) r_greenItemStuff;
  if (tractStuff->nh_tractOccurencesHandle != NULL) {
    m_TRACK_IF(GreenCollectionDestroyInstance(tractStuff->nh_tractOccurencesHandle) != RETURNED)
    tractStuff->nh_tractOccurencesHandle = NULL;
  }
  if (tractStuff->nh_tractOrPivotsHandle != NULL) {
    m_TRACK_IF(GreenCollectionDestroyInstance(tractStuff->nh_tractOrPivotsHandle) != RETURNED)
    tractStuff->nh_tractOrPivotsHandle = NULL;
  }
  
  m_DIGGY_RETURN(RETURNED)
} // PamphletTractDisengage


// ======== PAMPHLETs ======

struct PAMPHLET {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_HANDLE)
  ENGELS_HANDLE engelsHandle;
  int pamphletModelEntry ;
  struct P_STRING examplar ;
  GREEN_COLLECTION_HANDLE h_tractsHandle ; 
} ; 

// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletCreateInstance(PAMPHLET_HANDLE *azh_handle, ENGELS_HANDLE f_engelsHandle) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_HANDLE,handle)

  handle->engelsHandle = f_engelsHandle;
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_tractsHandle, BATEAU__EXPECTED_ITEMS_NUMBER,
    sizeof(struct TRACT), PamphletTractDisengage, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
    (GREEN_HANDLER__EQUATE_FUNCTION)NULL, handle) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // PamphletCreateInstance


// Public function; see .h
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_HANDLE xh_handle = (PAMPHLET_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_tractsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarDestroyInstance

