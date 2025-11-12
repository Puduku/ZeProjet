// karl.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#include "c-ansi/karl.h"
#include "c-ansi/alloc.h"
#include "c-ansi/g-string.h"
#include "c-ansi/stderr.h"
#include "c-ansi/named-object.h"
#include "c-ansi/inifile.h"

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

// I.1 PIVOT_MODEL:

// NAMED OBJECT: 
struct PIVOT_MODEL {
  m_DECLARE_MAGIC_FIELD(PIVOT_MODEL_HANDLE)
  struct P_STRING name; // that of as "named object"
  int n_minSize;
  int n_maxSize;
  int rawMatterFlags;
  int refinedMatter; // must be compatible with raw matter flags
} ;
typedef struct PIVOT_MODEL *PIVOT_MODEL_HANDLE;

// Passed:
// - *azh_handle:
// - name:
// - n_minSize:
// - n_maxSize:
// - rawMatterFlags:
// - refinedMatter:
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; 
static int PivotModelCreateInstance(PIVOT_MODEL_HANDLE *azh_handle, struct P_STRING name,
  int n_minSize, int n_maxSize, int rawMatterFlags, int refinedMatter) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  PIVOT_MODEL_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PIVOT_MODEL_HANDLE,handle)
  handle->name = name;
  handle->n_minSize = n_minSize;
  handle->n_maxSize = n_maxSize;
  handle->rawMatterFlags = rawMatterFlags;
  handle->refinedMatter = refinedMatter;
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
 
// I.2 TRACT_OR_PIVOT_MODEL:

#define b_PIVOT b_TRUE
#define b_TRACT !b_PIVOT 

// ULTRA GREEN ITEM:
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

// Passed:
// - stuff:
// - minOccurencesNumber:
// - maxOccurencesNumber:
// - b_pivot:
// - c_pivotModelEntry:
// - c_tractModelEntry:
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; 
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


// I.3 TRACT_MODEL:

// NAMED OBJECT: 
struct TRACT_MODEL {
  m_DECLARE_MAGIC_FIELD(TRACT_MODEL_HANDLE)
  struct P_STRING name; // that of as "named object"
  GREEN_COLLECTION_HANDLE h_tractOrPivotModelsHandle;
} ;
typedef struct TRACT_MODEL *TRACT_MODEL_HANDLE;

// Passed:
// - *azh_handle:
// - name:
// - expectedTractOrPivotModelsNumber:
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; 
static int TractModelCreateInstance(TRACT_MODEL_HANDLE *azh_handle, struct P_STRING name,
  int expectedTractOrPivotModelsNumber) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_INSTANCE(*azh_handle)
  TRACT_MODEL_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(TRACT_MODEL_HANDLE,handle)
  handle->name = name;
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_tractOrPivotModelsHandle,
    expectedTractOrPivotModelsNumber, sizeof(struct TRACT_OR_PIVOT_MODEL),
      (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
      (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED) != RETURNED) 
  m_DIGGY_RETURN(RETURNED)
} // TractModelCreateInstance

// Passed:
// - handle:
// - minOccurencesNumber:
// - maxOccurencesNumber:
// - b_pivot:
// - c_pivotModelEntry:
// - c_tractModelEntry:
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; 
static int TractModelAddTractOrPivotModel(TRACT_MODEL_HANDLE handle, int minOccurencesNumber,
  int maxOccurencesNumber, char b_pivot, int c_pivotModelEntry, int c_tractModelEntry) {
  int entry = UNDEFINED;
  TRACT_OR_PIVOT_MODEL_STUFF tractOrPivotModelStuff = (TRACT_OR_PIVOT_MODEL_STUFF) UNDEFINED;
  m_TRACK_IF((entry = GreenCollectionFetch(handle->h_tractOrPivotModelsHandle,-1,
    (char**)&tractOrPivotModelStuff)) < 0) 
  m_TRACK_IF(TractOrPivotModelInitialize(tractOrPivotModelStuff,minOccurencesNumber,
    maxOccurencesNumber,b_pivot,c_pivotModelEntry,c_tractModelEntry) != RETURNED) 
  m_DIGGY_RETURN(RETURNED)
} // TractModelAddTractOrPivotModel

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
static int TractModelDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  TRACT_MODEL_HANDLE xh_handle = (TRACT_MODEL_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(TRACT_MODEL_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_tractOrPivotModelsHandle) != RETURNED) 

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // TractModelDestroyInstance

// I.4 PAMPHLET_MODEL:

// NAMED OBJECT: 
struct PAMPHLET_MODEL {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_MODEL_HANDLE)
  struct P_STRING name; // that of as "named object"
  GREEN_COLLECTION_HANDLE h_tractOrPivotModelsHandle;
} ;
typedef struct PAMPHLET_MODEL *PAMPHLET_MODEL_HANDLE;

// Passed:
// - *azh_handle:
// - name:
// - expectedTractOrPivotModelsNumber:
// 
// Changed:
// - azh_handle: 
// 
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; 
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

// I.5 ENGELS:

struct ENGELS {
  g_NAMED_OBJECTS_HANDLE h_pivotModelsHandle ;
  g_NAMED_OBJECTS_HANDLE h_tractModelsHandle ;
  g_NAMED_OBJECTS_HANDLE h_pamphletModelsHandle ;
} ;

// Public function: see .h
int EngelsCreateInstance(ENGELS_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle)
  ENGELS_HANDLE handle = *azh_handle;

  m_TRACK_IF(l_NamedObjectsCreateInstance(&(handle->h_pivotModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, PivotModelDestroyInstance) != RETURNED)
  m_TRACK_IF(l_NamedObjectsCreateInstance(&(handle->h_tractModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, TractModelDestroyInstance) != RETURNED)
  m_TRACK_IF(l_NamedObjectsCreateInstance(&(handle->h_pamphletModelsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER, PamphletModelDestroyInstance) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // EngelsCreateInstance


// Public function: see .h
int EngelsAddPivotModels(ENGELS_HANDLE handle, g_G_PARAMS_HANDLE p_configHandle) {
  m_DIGGY_BOLLARD()
  int completed = COMPLETED__OK;  // a priori
  int count = GStringsGetCount(p_configHandle,NULL);
  m_TRACK_IF(count < 0)

  g_G_PARAM_STUFF t_pivotModelConfigStuff = (g_G_PARAM_STUFF)UNDEFINED;
  g_NAMED_OBJECT_STUFF t_namedPivotModelStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED;
  int entry = UNDEFINED;
  int n_minSize = UNDEFINED;
  int n_maxSize = UNDEFINED;
  int rawMatterFlags = UNDEFINED;
  int refinedMatter = UNDEFINED;
  PIVOT_MODEL_HANDLE h_pivotModelHandle = (PIVOT_MODEL_HANDLE)UNDEFINED;
  int i = 0; for (; i < count; i++) {
    m_TRACK_IF(GStringsFetch(p_configHandle, i, &t_pivotModelConfigStuff) != i)
    struct G_KEY gKey = m_GKey_PString(t_pivotModelConfigStuff[G_PARAM_NAME_ELEMENT].cv_pString);
    switch (m_GStringsIndexSingleFetch(handle->h_pivotModelsHandle,NULL,INDEX_LABEL0,
      INDEX_SEEK_FLAGS__EQUAL,&gKey,INDEX_FETCH_FLAGS__FETCH, &t_namedPivotModelStuff, &entry)) {
    case RESULT__FOUND: 
      completed = COMPLETED__BUT; 
    break; case RESULT__NOT_FOUND:
    break; default: m_TRACK() } // switch 
    
    m_TRACK_IF(PivotModelCreateInstance(&h_pivotModelHandle,
      t_pivotModelConfigStuff[G_PARAM_NAME_ELEMENT].cv_pString,n_minSize,n_maxSize,rawMatterFlags,
      refinedMatter) != RETURNED) 
    m_TRACK_IF(GStringCopy(t_namedPivotModelStuff,0,
      t_pivotModelConfigStuff[G_PARAM_NAME_ELEMENT].cv_pString) < 0)
    m_TRACK_IF(m_NamedObjectAssign(t_namedPivotModelStuff,h_pivotModelHandle,
      handle->h_pivotModelsHandle) != RETURNED)
  } // for
  
  m_DIGGY_RETURN(completed)
} // EngelsAddPivotModels

// Public function: see .h
int EngelsAddTractModel(ENGELS_HANDLE handle, struct P_STRING name,
  g_G_PARAMS_HANDLE p_configHandle) {
  m_DIGGY_BOLLARD()
  int completed = COMPLETED__OK;  // a priori
  int count = GStringsGetCount(p_configHandle,NULL);
  m_TRACK_IF(count < 0)

  int entry = UNDEFINED;
  g_G_PARAM_STUFF t_tractModelConfigStuff = (g_G_PARAM_STUFF)UNDEFINED;
  g_NAMED_OBJECT_STUFF t_namedTractModelStuff = (g_NAMED_OBJECT_STUFF)UNDEFINED;
  int expectedTractOrPivotModelsNumber = UNDEFINED;
  TRACT_MODEL_HANDLE h_tractModelHandle = (TRACT_MODEL_HANDLE)UNDEFINED;

  struct G_KEY gKey = m_GKey_PString(name);
  switch (m_GStringsIndexSingleFetch(handle->h_tractModelsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey,INDEX_FETCH_FLAGS__FETCH, &t_namedTractModelStuff, &entry)) {
  case RESULT__FOUND: 
    completed = COMPLETED__BUT; 
  break; case RESULT__NOT_FOUND:
  break; default: m_TRACK() } // switch 
    
  m_TRACK_IF(TractModelCreateInstance(&h_tractModelHandle,name,expectedTractOrPivotModelsNumber) !=
    RETURNED) 
  m_TRACK_IF(GStringCopy(t_namedTractModelStuff,0,name) < 0)
  m_TRACK_IF(m_NamedObjectAssign(t_namedTractModelStuff,h_tractModelHandle,
    handle->h_tractModelsHandle) != RETURNED)

  int i = 0; for (; i < count; i++) {
    m_TRACK_IF(GStringsFetch(p_configHandle, i, &t_tractModelConfigStuff) != i)
  } // for
  
  m_DIGGY_RETURN(completed)
} // EngelsAddTractModel


//
static int EngelsGetTrackModelsNumber(ENGELS_HANDLE handle, int trackModelEntry) {
  m_DIGGY_BOLLARD_S()
  int itemsNumber = GStringsGetCount(handle->h_tractModelsHandle,NULL);
  m_TRACK_IF(itemsNumber < 0)
  m_DIGGY_RETURN(itemsNumber)
} // EngelsGetTrackModelsNumber


// Public function: see .h
int EngelsDestroyInstance(ENGELS_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  
  m_TRACK_IF(g_NamedObjectsDestroyInstance(xh_handle->h_pivotModelsHandle) != RETURNED)
  m_TRACK_IF(g_NamedObjectsDestroyInstance(xh_handle->h_tractModelsHandle) != RETURNED)
  m_TRACK_IF(g_NamedObjectsDestroyInstance(xh_handle->h_pamphletModelsHandle) != RETURNED)
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // EngelsDestroyInstance

// === II. examplars: === 
// ======================

// II.1 TRACT_OR_PIVOT: 

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

// II.2 TRACT:

// GREEN ITEM:
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
static int TractAssign(TRACT_STUFF stuff, ENGELS_HANDLE engelsHandle, int tractModelEntry) {
  m_DIGGY_BOLLARD_S()

  int itemsNumber = EngelsGetTrackModelsNumber(engelsHandle, tractModelEntry);
  m_TRACK_IF(itemsNumber < 0) 
  m_ASSERT(itemsNumber > 0)
  stuff->tractModelEntry = tractModelEntry;
  if (stuff->nh_tractOrPivotsHandle == NULL) {
    m_TRACK_IF(GreenCollectionCreateInstance(&stuff->nh_tractOrPivotsHandle, itemsNumber,
      sizeof(struct TRACT_OR_PIVOT), (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL,
      (GREEN_HANDLER__COMPARE_FUNCTION)NULL, (GREEN_HANDLER__EQUATE_FUNCTION)NULL, (void*)UNDEFINED)
      != RETURNED) 
  } // if

  m_DIGGY_RETURN(RETURNED)
} // TractAssign

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


// II.3 PAMPHLET:

// NAMED OBJECT: 
struct PAMPHLET {
  m_DECLARE_MAGIC_FIELD(PAMPHLET_HANDLE)
  struct P_STRING name; // that of as "named object"
  ENGELS_HANDLE engelsHandle;
  int n_pamphletModelEntry ; // -1 special value => pamphlet is NOT defined 
  struct P_STRING ci_examplar ; // Only significant if pamphlet is defined
  GREEN_COLLECTION_HANDLE hcv_tractOrPivotsHandle ; // When pamphlet is defined: 
    // tracts or pivots then correspond to the "examplar" 
  GREEN_COLLECTION_HANDLE hcv_tractsHandle ; // When pamphlet is defined:
    // all tracts used in the "examplar" 
} ; 

// Public function; see .h
int PamphletCreateInstance(PAMPHLET_HANDLE *azh_handle, ENGELS_HANDLE f_engelsHandle) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle)
  PAMPHLET_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(PAMPHLET_HANDLE,handle)

  handle->engelsHandle = f_engelsHandle;
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->hcv_tractOrPivotsHandle,
    BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct TRACT_OR_PIVOT),
    (GREEN_HANDLER__DISENGAGE_FUNCTION)NULL, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
    (GREEN_HANDLER__EQUATE_FUNCTION)NULL,(void*)UNDEFINED) != RETURNED)
  m_TRACK_IF(GreenCollectionCreateInstance(&handle->hcv_tractsHandle,
    BATEAU__EXPECTED_ITEMS_NUMBER, sizeof(struct TRACT),
    DisengageTract, (GREEN_HANDLER__COMPARE_FUNCTION)NULL,
    (GREEN_HANDLER__EQUATE_FUNCTION)NULL,(void*)UNDEFINED) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // PamphletCreateInstance

// Public function; see .h
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD()

  PAMPHLET_HANDLE xh_handle = (PAMPHLET_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(PAMPHLET_HANDLE,xh_handle)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->hcv_tractOrPivotsHandle) != RETURNED)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->hcv_tractsHandle) != RETURNED)

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // PamphletDestroyInstance


