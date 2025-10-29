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

// PIVOT_MODEL:

struct PIVOT_MODEL {
  m_DECLARE_MAGIC_FIELD(PIVOT_MODEL_HANDLE)
  struct P_STRING v_name; // that of as "named object"
  int n_minSize;
  int n_maxSize;
  int rawMatterFlags;
  int refinedMatter; // must be compatible with raw matter flags
} ;
typedef struct PIVOT_MODEL *PIVOT_MODEL_HANDLE;

//
static int PivotModelCreateInstance(PIVOT_MODEL_HANDLE *azh_handle, struct P_STRING name) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  PIVOT_MODEL_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PIVOT_MODEL_HANDLE,handle)
  handle->v_name = name;
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
 
// TRACT_OR_PIVOT_MODEL:

#define b_PIVOT b_TRUE
#define b_TRACT !b_PIVOT 

// ULTRA green type:
struct TRACT_OR_PIVOT_MODEL {
  m_DECLARE_MAGIC_FIELD(TRACT_OR_PIVOT_MODEL_HANDLE)
  int minOccurencesNumber;
  int maxOccurencesNumber;
  char b_pivot ;
  union {
    int c_pivotModelEntry ; // Only significant with "pivot"
    int c_tractModelEntry; // Only significant if "not" pivot (aka aggregate)...
  } select;
} ;
typedef struct TRACT_OR_PIVOT_MODEL *TRACT_OR_PIVOT_MODEL_STUFF;

//
static int TractOrPivotModelInitialize(TRACT_OR_PIVOT_MODEL_STUFF stuff, int minOccurencesNumber,
  int maxOccurencesNumber, char b_pivot, int c_pivotModelEntry, int c_tractModelEntry) {
  m_DIGGY_BOLLARD_S()
  stuff->minOccurencesNumber = minOccurencesNumber;
  stuff->maxOccurencesNumber = maxOccurencesNumber;
  stuff->b_pivot = b_pivot; 
  stuff->select.c_pivotModelEntry = c_pivotModelEntry; 
  stuff->select.c_tractModelEntry = c_tractModelEntry;
  m_DIGGY_RETURN(RETURNED)
} // TractOrPivotModelInitialize


// TRACT_MODEL:

struct TRACT_MODEL {
  m_DECLARE_MAGIC_FIELD(TRACT_MODEL_HANDLE)
  struct P_STRING v_name; // that of as "named object"
  GREEN_COLLECTION_HANDLE h_tractOrPivotModelsHandle;
} ;
typedef struct TRACT_MODEL *TRACT_MODEL_HANDLE;

//
static int TractModelCreateInstance(TRACT_MODEL_HANDLE *azh_handle,
  int expectedTractOrPivotModelsNumber) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  TRACT_MODEL_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(TRACT_MODEL_HANDLE,handle)
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_tractOrPivotModelsHandle,
    expectedTractOrPivotModelsNumber, sizeof(struct TRACT_OR_PIVOT_MODEL),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  m_DIGGY_RETURN(RETURNED)
} // TractModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int TractModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  TRACT_MODEL_HANDLE xh_handle = (TRACT_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(TRACT_MODEL_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_tractOrPivotModelsHandle) != RETURNED) 

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // TractModelDestroyInstance

// PAMPHLET_MODEL:

struct PAMPHLET_MODEL {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE)
  struct P_STRING v_name; // that of as "named object"
  GREEN_COLLECTION_HANDLE h_tractOrPivotModelsHandle;
} ;
typedef struct PAMPHLET_MODEL *PAMPHLET_MODEL_HANDLE;

//
static int PamphletModelCreateInstance(PAMPHLET_MODEL_HANDLE *azh_handle,
  int expectedTractOrPivotModelsNumber) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_MODEL_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE,handle)
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_tractOrPivotModelsHandle,
    expectedTractOrPivotModelsNumber, sizeof(struct TRACT_OR_PIVOT_MODEL),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  m_DIGGY_RETURN(RETURNED)
} // PamphletModelCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int PamphletModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_MODEL_HANDLE xh_handle = (PAMPHLET_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_tractOrPivotModelsHandle) != RETURNED) 

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

// === TRACT_OR_PIVOT =====

// ULTRA GREEN ITEM:
struct TRACT_OR_PIVOT {
  int occurence; // 0 for 1st occurence (in tract)... 
  int occurencesNumber; // (in tract) 
  // Localization  
  int offset ; // >= 0 (in tract) 
  int length ; // >= 0 
  // 
  char vb_pivot ; // following tract or pivot model...
  union {
    int c_pivotModelEntry;
    struct { 
      int totalChildrenNumber; // total number of items to skip for next tract of pivot
      int tractEntry; 
    } c_tract;
  } select ;
} ;
typedef struct TRACT_OR_PIVOT *TRACT_OR_PIVOT_STUFF;

// === TRACTs =====
// See GREEN_ITEM @ c-ansi/green.h
struct TRACT {
  int tractModelEntry;
  GREEN_COLLECTION_HANDLE nh_tractOrPivotsHandle ; 
} ;
typedef struct TRACT *TRACT_STUFF;

// Passed:
// - stuff: 
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
static int TractInitialize(TRACT_STUFF stuff, int expectedItemsNumber) {
  m_DIGGY_BOLLARD_S()

  if (stuff->nh_tractOrPivotsHandle == NULL) {
    m_TRACK_IF(GreenCollectionCreateInstance(&stuff->nh_tractOrPivotsHandle,
      expectedItemsNumber, sizeof(struct TRACT_OR_PIVOT),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  } // if

  m_DIGGY_RETURN(RETURNED)
} // TractInitialize

// #SEE GREEN_HANDLER__DISENGAGE_FUNCTION @ c-ansi/green.h 
static int DisengageTract(void *r_handle, char *r_greenItemStuff) {
  m_DIGGY_BOLLARD_S()

  TRACT_STUFF tractStuff = (TRACT_STUFF) r_greenItemStuff;
  if (tractStuff->nh_tractOrPivotsHandle != NULL) {
    m_TRACK_IF(GreenCollectionDestroyInstance(tractStuff->nh_tractOrPivotsHandle) != RETURNED)
    tractStuff->nh_tractOrPivotsHandle = NULL;
  }
  
  m_DIGGY_RETURN(RETURNED)
} // DisengageTract


// ======== PAMPHLETs ======

struct PAMPHLET {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_HANDLE)
  ENGELS_HANDLE engelsHandle;
  int n_pamphletModelEntry ; // -1 special value => pamphlet is NOT defined 
  struct P_STRING ci_examplar ; // Only significant if pamphlet is defined
  GREEN_COLLECTION_HANDLE hcv_tractOrPivotsHandle ; // Only significant if pamphlet is defined ; 
    // tracts or pivots then correspond to the "examplar" 
} ; 

// Public function; see .h
int PamphletCreateInstance(PAMPHLET_HANDLE *azh_handle, ENGELS_HANDLE f_engelsHandle) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_HANDLE,handle)

  handle->engelsHandle = f_engelsHandle;
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->hcv_tractOrPivotsHandle,
    BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct TRACT_OR_PIVOT),
    (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
    (GREEN_HANDLER__EQUATE_FUNCTION)NULL,(void*)UNDEFINED) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // PamphletCreateInstance


// Public function; see .h
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  PAMPHLET_HANDLE xh_handle = (PAMPHLET_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->hcv_tractOrPivotsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletExamplarDestroyInstance

