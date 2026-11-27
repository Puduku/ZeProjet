// c-ansi/g-index.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/g-index.h"
#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-ansi/c-stack.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"

#ifdef DIGGY
int diggyGIndexDumpMax = 0; // TODO: ag virer...
#endif


// G-INDEX ("proto" object)
// -------

// Index's virtual function to compare two items.
//
// Passed:
// - r_handle:
// - indexLabel:
// - aEntry: entry for "A"
// - n_bEntry: >= 0 => entry for "B"
// - cr_bGKeys: raw key(s) for "B" when entry for "B" not provided
//
// Ret: 
// - >=0: "comparison" between :"A" and "B" with that key component... 
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
// - -1: unexpected problem; anomaly is raised
typedef int (*ENTRY_COMPARE_FUNCTION) (void *r_handle, int indexLabel, int aEntry, int n_bEntry,
  void *cr_bGKeys) ;

struct G_INDEX {
  int *hsc_array; // - Physical size given by global instance's itemPhysicalCount
                  // - 'count' first items are significant
// TODO: all indexes use same count / entryCompareFunction / r_entryCompareFunctionHandle
// TODO: let's say it's a kind of cache ???
  int count; // >= 0
  ENTRY_COMPARE_FUNCTION entryCompareFunction;
  void* r_entryCompareFunctionHandle;
  int indexLabel;
};

// Passed:
// - az_me:
// - itemPhysicalCount:
// - entryCompareFunction:
// - r_entryCompareFunctionHandle:
// - indexLabel:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexInit(struct G_INDEX* az_me, int itemPhysicalCount,
  ENTRY_COMPARE_FUNCTION entryCompareFunction, void* r_entryCompareFunctionHandle,
  int indexLabel) { m_DIGGY_BOLLARD_S()
  m_MALLOC_ARRAY(az_me->hsc_array,itemPhysicalCount)
  az_me->count = 0;
  az_me->entryCompareFunction = entryCompareFunction;
  az_me->r_entryCompareFunctionHandle = r_entryCompareFunctionHandle;
  az_me->indexLabel = indexLabel;
  m_DIGGY_RETURN(RETURNED) 
} // m_GIndexInit


// Passed:
// - a_me:
// - indexEntry:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexRemove(struct G_INDEX* a_me, int indexEntry) { m_DIGGY_BOLLARD_S()
  memmove(a_me->hsc_array + indexEntry,  a_me->hsc_array + indexEntry + 1,
    sizeof(int) * ((a_me->count -= 1) - indexEntry));
  m_DIGGY_RETURN(RETURNED) 
} // om_GIndexRemove

// Passed:
// - a_me:
// - newIndexEntry:
// - newEntry:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexAdd(struct G_INDEX* a_me, int newIndexEntry, int newEntry) {
  m_DIGGY_BOLLARD_S()
  memmove(a_me->hsc_array + newIndexEntry + 1,  a_me->hsc_array + newIndexEntry,
    sizeof(int) * (a_me->count++ - newIndexEntry));
  (a_me)->hsc_array[newIndexEntry] = newEntry;
  m_DIGGY_RETURN(RETURNED) 
} // om_GIndexAdd


// Passed:
// - a_me:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexClear(struct G_INDEX* a_me) { m_DIGGY_BOLLARD_S()
  (a_me)->count = 0 ;
  m_DIGGY_RETURN(RETURNED) 
} // om_GIndexClear

// Passed:
// - a_me:
//
// Ret:
// number of "active" index entries
static int o_GIndexGetCount(struct G_INDEX* a_me) { m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN((a_me)->count)
} // o_GIndexGetCount

// Passed:
// - a_me:
// - indexEntry:
//
// Ret:
// number of "active" index entries
static int o_GIndexGetItemEntry(struct G_INDEX* a_me, int indexEntry) { m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN((a_me)->hsc_array[indexEntry])
} // o_GIndexGetItemEntry


// Passed:
// - a_me:
// - newItemPhysicalCount:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexResize(struct G_INDEX* a_me, int newItemPhysicalCount) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY((a_me)->hsc_array, newItemPhysicalCount)
  m_DIGGY_RETURN(RETURNED) 
} // m_GIndexResize

// Compare two items of the index
//
// Passed:
// - a_me:
// - aIndexEntry: entry in index for "A"
// - n_bEntry: >= 0 => entry in index for "B"
// - cr_bGKeys: raw key(s) for "B" when entry for "B" not provided
//
// Ret:
// - >= 0: "A - B" comparison between key of index' entry and passed key
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
// - -1: anomaly is raised
static int GIndexCompare(struct G_INDEX* a_me, int aIndexEntry, int n_bEntry, void* cr_bGKeys) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;
  int aEntry = a_me->hsc_array[aIndexEntry];
m_DIGGY_VAR_P(cr_bGKeys)
    m_TRACK_IF((comparison = a_me->entryCompareFunction(a_me->r_entryCompareFunctionHandle,
      a_me->indexLabel, aEntry, n_bEntry,cr_bGKeys)) < 0)
  if (comparison == EQUAL_TO__COMPARISON && n_bEntry != -1)  comparison = 
    DIFFERENCE_2_COMPARISON(aEntry - n_bEntry) ;
  m_DIGGY_RETURN(comparison)
} // GIndexCompare

// Binary search of "B" in index
//
// Passed:
// - a_me: index to use
// - n_bEntry: >= 0 => entry in index for "B"
// - cr_bGKeys: raw key(s) for "B" when entry for "B" not provided
// - cac_indexEntries: only significant if entry in index is specified for "B". 
//
// Modified:
// - *an_indexEntry: >=0 => found entry in index
//   -1 => "B" is not referenced in index ("Not found")
// - *a_top: "best" top index entry established so far ; when "Not found", corresponds to the
//   closest entry in the index that is on top of "B"
// - *cac_indexEntries: (when used) only significant if found entry in index; delimit ALL "B"
//   entries 
//
// Ret:
// - RETURNED
// - -1: anomaly is raised
static int GIndexBSearch(struct G_INDEX *a_me, int n_bEntry, void *cr_bGKeys, int *an_indexEntry,
  int *a_top, struct INDEX_ENTRY_BLOCK *cac_indexEntries) { m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;
  int i = UNDEFINED;

  int bottom = -1, // Best index entry "before"
    top = a_me->count; // Best index entry "after"

  *an_indexEntry = -1;
  while (top - bottom > 1) {
    i =  (top + bottom) >> 1 ;
m_DIGGY_VAR_P(cr_bGKeys)
    m_TRACK_IF((comparison = GIndexCompare(a_me, i,n_bEntry,cr_bGKeys)) < 0)
    switch (comparison) {
    case GREATER_THAN__COMPARISON:
      top = i ;
    break; case LESS_THAN__COMPARISON:
      bottom = i ;
    break; case EQUAL_TO__COMPARISON:
      *an_indexEntry = i ;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,comparison)
    } // switch
    if (*an_indexEntry >= 0) break;
  } // while
  *a_top = top;
  if (*an_indexEntry >= 0 && n_bEntry < 0) {
    for (i = *an_indexEntry-1 ; i >= 0 ; i--) {
      m_TRACK_IF((comparison = GIndexCompare(a_me, i,-1,cr_bGKeys)) < 0)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == LESS_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    cac_indexEntries->first = i+1;
    for (i = *an_indexEntry+1 ; i < a_me->count ; i++) {
      m_TRACK_IF((comparison = GIndexCompare(a_me, i,-1,cr_bGKeys)) < 0)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == GREATER_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    cac_indexEntries->last = i-1;
  } //if     
  m_DIGGY_RETURN(RETURNED)
} // GIndexBSearch


// Passed:
// -a_me:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: index is corrupted
// - -1: anomaly is raised
static int GIndexVerify(struct G_INDEX *a_me) { m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;

#ifdef DIGGY
if (diggyGIndexDumpMax > 0) {
  m_DIGGY_INFO("Dump: a_me->count=%d (indexLabel=%d)",a_me->count,a_me->indexLabel)
  int i = 0 ; for (; i < diggyGIndexDumpMax && i < a_me->count; i++) {
    m_DIGGY_INFO("  a_me->hsc_array[%d]=%d",i,a_me->hsc_array[i])
  } // for
} // if 
#endif

  int j = 1; for (; j < a_me->count; j++) {
    m_TRACK_IF((comparison = GIndexCompare(a_me, j, a_me->hsc_array[j-1],
      (void *)(GENERIC_INTEGER) UNDEFINED)) < 0)
    if (comparison != GREATER_THAN__COMPARISON) {
      m_ASSERT(comparison != EQUAL_TO__COMPARISON)
      break;
    } // if
  } // for

  if (j < a_me->count) {
m_DIGGY_VAR_D(j)
    m_DIGGY_RETURN(COMPLETED__BUT)
  } // if

  m_DIGGY_RETURN(COMPLETED__OK)
} // GIndexVerify

// Passed:
// - a_me:
// - entry:
// - expectedHits:
//
// Returned
// - COMPLETED__OK: number of "hits" (aka number of times the entry is present in the index)
//                  is as expected
// - COMPLETED__BUT: number of "hits" NOT as expected
// - -1: unexpected problem ; anomaly is raised
static int GIndexVerifyEntry(struct G_INDEX *a_me, int entry, int expectedHits) {
  m_DIGGY_BOLLARD_S()

  int completed = COMPLETED__OK;
  
  int hits = 0;
  int *s_indexArrayPtr = a_me->hsc_array;
  int i = 0; for (; i < a_me->count ; i++, s_indexArrayPtr++) {
    if (*s_indexArrayPtr == entry) hits++ ;
  } // for

  if (hits != expectedHits) {
m_DIGGY_INFO("hits=%d expectedHits=%d",hits,expectedHits)
    completed = COMPLETED__BUT;
  } // if
  m_DIGGY_RETURN(completed)
} // GIndexVerifyEntry 


// Passed:
// - ax_me:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexFree(struct G_INDEX* ax_me) { m_DIGGY_BOLLARD_S()
  free(ax_me->hsc_array);
  m_DIGGY_RETURN(RETURNED)
} // om_GIndexFree


// G-INDEXES (REAL object) 
// ---------

struct G_INDEXES {
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction;
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction;
  void *r_entryRawFunctionsHandle;
  int indexCount ; // when ENABLED ; >= 0
  int *vnhs_gKeyCounts; // NULL when indexCount == 0
  struct G_INDEX *vnhs_indexes ; // NULL when indexCount == 0
};

// Public function: see .h
int GIndexesCreateInstance(G_INDEXES_HANDLE *azh_handle,
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction,
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction, void* r_entryRawFunctionsHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  G_INDEXES_HANDLE handle = *azh_handle;
  handle->entryRawCompareFunction = entryRawCompareFunction;
  handle->entryRawEquateFunction = entryRawEquateFunction;
  handle->r_entryRawFunctionsHandle = r_entryRawFunctionsHandle;
  handle->indexCount = 0;
  handle->vnhs_gKeyCounts = NULL;
  handle->vnhs_indexes = NULL;
  m_DIGGY_RETURN(RETURNED)
} // GIndexesCreateInstance

// See ENTRY_COMPARE_FUNCTION
static int GIndexesEntryCompare (void *r_handle, int indexLabel, int aEntry, int n_bEntry,
  void *cr_bGKeys) { m_DIGGY_BOLLARD_S()
  G_INDEXES_HANDLE handle = (G_INDEXES_HANDLE)r_handle;  
  int comparison = UNDEFINED;
  m_ASSERT(indexLabel < handle->indexCount) 
  int j = 0; for ( ; j < handle->vnhs_gKeyCounts[indexLabel] ; j++) {
m_DIGGY_VAR_P(cr_bGKeys)
    m_TRACK_IF((comparison = handle->entryRawCompareFunction(
      handle->r_entryRawFunctionsHandle, indexLabel, j, aEntry, n_bEntry,cr_bGKeys)) < 0)
    if (comparison != EQUAL_TO__COMPARISON) break ;
  } // for
  m_DIGGY_RETURN(comparison)
} // GIndexesEntryCompare

// Public function; see .h
int GIndexesAddIndex(G_INDEXES_HANDLE handle, int itemPhysicalCount, int gKeyCount) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY(handle->vnhs_indexes,handle->indexCount+1)
  m_REALLOC_ARRAY(handle->vnhs_gKeyCounts,handle->indexCount+1)
  struct G_INDEX *a_index = handle->vnhs_indexes + handle->indexCount ;

  handle->vnhs_gKeyCounts[handle->indexCount] = gKeyCount;
  m_TRACK_IF(m_GIndexInit(a_index, itemPhysicalCount, GIndexesEntryCompare, handle,
    handle->indexCount) != RETURNED)
  m_DIGGY_RETURN(handle->indexCount++)
} // GIndexesAddIndex

int o_GIndexesGetIndexCount(G_INDEXES_HANDLE handle) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(handle->indexCount)  
} // o_GIndexesGetIndexCount


// Public function; see .h
int GIndexesResize (G_INDEXES_HANDLE handle, int newItemPhysicalCount) { m_DIGGY_BOLLARD_S()
  int i = 0;
  struct G_INDEX *cs_index = handle->vnhs_indexes;
  for ( ; i < handle->indexCount ; i++, cs_index++) {
    m_TRACK_IF(m_GIndexResize(cs_index, newItemPhysicalCount) != RETURNED)
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GIndexesResize


// Public function; see .h
int GIndexesAdd(G_INDEXES_HANDLE handle, int entry) { m_DIGGY_BOLLARD()
  int n_indexEntry = UNDEFINED, top = UNDEFINED;
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes;
  for ( ; i < handle->indexCount ; i ++, s_index++) {
    m_TRACK_IF(GIndexBSearch(s_index, entry, (void*)UNDEFINED, &n_indexEntry, &top,
      (struct INDEX_ENTRY_BLOCK*)UNDEFINED) != RETURNED)
    if (n_indexEntry == -1) {
      om_GIndexAdd(s_index,top,entry);
    } // if
  } // for
  m_DIGGY_RETURN(RETURNED)
} // GIndexesAdd

// Public function; see .h
int GIndexesRemove(G_INDEXES_HANDLE handle, int entry) { m_DIGGY_BOLLARD()
  int n_indexEntry = UNDEFINED, top = UNDEFINED;
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes;
  for ( ; i < handle->indexCount ; i ++, s_index++) {
    m_TRACK_IF(GIndexBSearch(s_index, entry,(void *)UNDEFINED,&n_indexEntry,&top,
      (struct INDEX_ENTRY_BLOCK*)UNDEFINED) != RETURNED)
    if (n_indexEntry >= 0) {
      om_GIndexRemove(s_index,n_indexEntry);
    } // if
  } // for
  m_DIGGY_RETURN(RETURNED)
 } // GIndexesRemove

// Public function; see .h
int o_GIndexesGetItemCount(G_INDEXES_HANDLE handle) { m_DIGGY_BOLLARD()
m_ASSERT(handle->vnhs_indexes != NULL)
  m_DIGGY_RETURN(o_GIndexGetCount(handle->vnhs_indexes + 0)) // same count for all indexes
} // o_GIndexesGetItemCount


// Public function; see .h
int GIndexesBSearch(G_INDEXES_HANDLE handle, int indexLabel, int n_bEntry, void *cr_bGKeys, 
  int *an_indexEntry, int *a_top, struct INDEX_ENTRY_BLOCK *cac_indexEntries) { m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(GIndexBSearch(handle->vnhs_indexes + indexLabel,n_bEntry,cr_bGKeys,
    an_indexEntry, a_top, cac_indexEntries))
} // GIndexesBSearch

// Public function; see .h
int o_GIndexesGetItemEntry(G_INDEXES_HANDLE handle, int indexLabel, int indexEntry) { 
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(o_GIndexGetItemEntry(handle->vnhs_indexes + indexLabel,indexEntry))
} // o_GIndexesGetItemEntry

// Adequate an item with a key.
//
// Passed:
// - handle:
// - indexLabel:
// - aEntry: entry for "A"
// - r_bGKeys: raw key(s) for "B" 
//
// Ret: adequation between "A" (entry) and "B" (key) ?
// - ANSWER__YES: 
// - ANSWER__NO: 
// - -1: unexpected problem; anomaly is raised
static inline int m_GIndexesEntryEquate(G_INDEXES_HANDLE handle, int indexLabel, int aEntry,
  void *r_bGKeys) { m_DIGGY_BOLLARD_S()
  int answer = UNDEFINED;
  m_ASSERT(indexLabel < handle->indexCount) 
  int j = 0; for ( ; j < handle->vnhs_gKeyCounts[indexLabel] ; j++) {
    m_TRACK_IF((answer = handle->entryRawEquateFunction(handle->r_entryRawFunctionsHandle,
      indexLabel, j, aEntry, r_bGKeys)) < 0)
    if (answer != ANSWER__YES) break ;
  } // for
  
  m_DIGGY_RETURN(answer)
} // m_GIndexesEntryEquate

// Public function; see .h
int GIndexesSeekEntryEquate(G_INDEXES_HANDLE handle, int indexLabel, int aEntry,
  unsigned int indexSeekFlags, void *r_bGKeys) { m_DIGGY_BOLLARD()
  if (indexSeekFlags == INDEX_SEEK_FLAGS__ANY) return ANSWER__YES;

  int answer = ANSWER__NO; // a priori
  if (indexSeekFlags != INDEX_SEEK_FLAGS__LIKE) {
    int comparison = GIndexesEntryCompare(handle, indexLabel, aEntry, -1, r_bGKeys);
    m_TRACK_IF(comparison < 0)
    switch (indexSeekFlags) {
    case INDEX_SEEK_FLAGS__EQUAL:
      if (comparison == EQUAL_TO__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__LESS:
      if (comparison == LESS_THAN__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__LESS_EQUAL:
      if (comparison == LESS_THAN__COMPARISON || comparison == EQUAL_TO__COMPARISON) answer =
        ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__GREATER: 
      if (comparison == GREATER_THAN__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__GREATER_EQUAL:
      if (comparison == GREATER_THAN__COMPARISON || comparison == EQUAL_TO__COMPARISON) answer =
        ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__NOT_EQUAL:
      if (comparison != EQUAL_TO__COMPARISON) answer = ANSWER__YES;
    break; default: 
      m_RAISE(ANOMALY__VALUE__D,indexSeekFlags)
    } // switch
  } else { // INDEX_SEEK_FLAGS__LIKE
    m_TRACK_IF((answer = m_GIndexesEntryEquate(handle,indexLabel, aEntry, r_bGKeys)) < 0)
  } // if 
  m_DIGGY_RETURN(answer)
} // GIndexesSeekEntryEquate


// Public function; see .h
int GIndexesVerifyEnabled (G_INDEXES_HANDLE handle) { m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN(handle->indexCount > 0?  ANSWER__YES: ANSWER__NO) 
} // GIndexesVerifyEnabled

// Public function; see .h
int GIndexesVerify (G_INDEXES_HANDLE handle) { m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes ;

  for ( ; i < handle->indexCount ; i++, s_index++) {
m_DIGGY_VAR_D(i)
    switch (GIndexVerify(s_index)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT:
      completed = COMPLETED__BUT;
    break; default:
      m_TRACK()
    } // switch
    if (completed == COMPLETED__BUT) break;
  } // for

  m_DIGGY_RETURN(completed)
} // GIndexesVerify

// Public function; see .h
int GIndexesVerifyCount (G_INDEXES_HANDLE handle,int *ac_commonCount) { m_DIGGY_BOLLARD_S()
  m_ASSERT(handle->indexCount > 0)

  *ac_commonCount = UNDEFINED; 
  int n_lastCount = -1;
  int completed = COMPLETED__OK; // a priori
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  int i = 0;
  for ( ; i < handle->indexCount ; i++, s_index++) {
    int thatCount = o_GIndexGetCount(s_index);
    if (n_lastCount != -1 && thatCount != n_lastCount) {
m_DIGGY_INFO("thatCount=%d n_lastCount=%d",thatCount,n_lastCount)
      completed = COMPLETED__BUT;
      break;
    } // if 
    n_lastCount = thatCount ;
  } // for
  m_ASSERT(n_lastCount >= 0)
  if (completed == COMPLETED__OK) *ac_commonCount = n_lastCount;
 
  m_DIGGY_RETURN(completed)
} // GIndexesVerifyCount

// Public function; see .h
int GIndexesVerifyEntry (G_INDEXES_HANDLE handle, int entry, int expectedHits) { m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  for ( ; i < handle->indexCount ; i++, s_index++) {
    switch (GIndexVerifyEntry(s_index, entry, expectedHits)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT:
      completed = COMPLETED__BUT;
    break; default:
      m_TRACK()
    } // switch
    if (completed == COMPLETED__BUT) break;
  } // for

  m_DIGGY_RETURN(completed)
} // GIndexesVerifyEntry 


// Public function; see .h
int GIndexesClear (G_INDEXES_HANDLE handle) { m_DIGGY_BOLLARD_S()
  int i = 0 ;
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  for ( ; i < handle->indexCount ; i++, s_index++) {
    om_GIndexClear(s_index);
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GIndexesClear


// Public function; see .h
int GIndexesDestroyInstance(G_INDEXES_HANDLE xh_handle) { m_DIGGY_BOLLARD()
  int i = 0;
  struct G_INDEX *cs_index = xh_handle->vnhs_indexes;
  for ( ; i < xh_handle->indexCount ; i++, cs_index++) {
    om_GIndexFree(cs_index);
  } // for
  if (xh_handle->indexCount > 0) {
    free(xh_handle->vnhs_indexes);
    free(xh_handle->vnhs_gKeyCounts);
  } // if
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // GIndexesDestroyInstance
