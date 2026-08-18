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


// Overview: 
// "GREEN INDEX" ("proto" object)
// ^ uses
// "GREEN INDEXES" (REAL object)

char b_diggyGreenCollectionExam = b_TRUE;

/////////// 1. GREEN INDEX ("proto" object......) ////////////////////

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
  int *hsc_array; // - Physical size given by global instance's itemsPhysicalNumber
                  // - 'count' first items are significant
  int count; // >= 0
  ENTRY_COMPARE_FUNCTION entryCompareFunction;
  void* r_entryFunctionsHandle;
  int indexLabel;
};

// Passed:
// - az_me:
// - itemsPhysicalNumber:
// - m_entryCompareFunction:
// - mr_entryFunctionsHandle:
// - indexLabel:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexInit(struct G_INDEX* az_me, int itemsPhysicalNumber,
  ENTRY_COMPARE_FUNCTION entryCompareFunction, void* r_entryFunctionsHandle,
  int indexLabel) {
  m_DIGGY_BOLLARD_S()
  m_MALLOC_ARRAY(az_me->hsc_array,itemsPhysicalNumber)
  az_me->count = 0;
  az_me->entryCompareFunction = entryCompareFunction;
  az_me->r_entryFunctionsHandle = r_entryFunctionsHandle;
  az_me->indexLabel = indexLabel;
  m_DIGGY_RETURN(RETURNED) 
} // m_GIndexInit


// Passed:
// - a_me:
// - indexEntry:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexRemove(struct G_INDEX* a_me, int indexEntry) {
  m_DIGGY_BOLLARD_S()
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
static inline int om_GIndexClear(struct G_INDEX* a_me) {
  m_DIGGY_BOLLARD_S()
  (a_me)->count = 0 ;
  m_DIGGY_RETURN(RETURNED) 
} // om_GIndexClear

// Passed:
// - a_me:
//
// Ret:
// number of "active" index entries
static inline int om_GIndexGetCount(struct G_INDEX* a_me) {
  m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN((a_me)->count)
} // om_GIndexGetCount


// Passed:
// - a_me:
// - newItemsPhysicalNumber:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexResize(struct G_INDEX* a_me, int newItemsPhysicalNumber) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY((a_me)->hsc_array, newItemsPhysicalNumber)
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
    m_TRACK_IF((comparison = a_me->entryCompareFunction(a_me->r_entryFunctionsHandle,
      a_me->indexLabel, aEntry, n_bEntry,cr_bGKeys)) < 0)
  if (comparison == EQUAL_TO__COMPARISON && n_bEntry != -1)  comparison = 
    DIFFERENCE_2_COMPARISON(aEntry - n_bEntry) ;
  m_DIGGY_RETURN(comparison)
} // GIndexCompare

// Index entries "block"
struct INDEX_ENTRY_BLOCK {
  int first; // >= 0
  int last; // >= first
} ;

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
  int *a_top, struct INDEX_ENTRY_BLOCK *cac_indexEntries) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;
  int i = UNDEFINED;

  int bottom = -1, // Best index entry "before"
    top = a_me->count; // Best index entry "after"

  *an_indexEntry = -1;
  while (top - bottom > 1) {
    i =  (top + bottom) >> 1 ;

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

struct INDEX_SEQUENCE {
  // index entries "blocks":
  int indexEntryBlockCount2; // between 0 and 2 : 0 => 'disabled' ; >0 -> 'enabled' 
  struct INDEX_ENTRY_BLOCK indexEntryBlocks2[2];
  // Fields below are only significant if 'enabled' (indexEntryBlockCount2 > 0) :
  int cv_firstIndexEntry; // first index entry for ALL "blocks"  
  int cv_lastIndexEntry; // last index entry for ALL "blocks"  
  int ci_indexEntryCursor; // "current" index entry:
    // - < cv_firstIndexEntry => Ascending:"soft reset", Descending:'no more' 
    // - between [indexEntries[0].first - cv_lastIndexEntry] => 'in sequence' 
    // - cv_lastIndexEntry => Ascending:"no more", Descending:"soft reset" 
  const struct INDEX_ENTRY_BLOCK *cv_indexEntryBlockPtr; // "block" corresponding to "current" index entry 
} ;

// Public function; see .h
int o_IndexSequenceNew(char *meBuffer) {
  m_DIGGY_BOLLARD()
  struct INDEX_SEQUENCE* mePtr = (struct INDEX_SEQUENCE*) meBuffer;
  mePtr->indexEntryBlockCount2 = 0;
  m_DIGGY_RETURN(RETURNED)
} // o_IndexSequenceNew


// Public function; see .h
int o_IndexSequenceSize(void) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(sizeof(struct INDEX_SEQUENCE))
} // o_IndexSequenceSize


// Get current entry in index sequence.
//
// Passed:
// - a_me:
// - ap_indexSequence-> : current state of sequence
//
// Changed:
// - *an_entry: 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexSequenceCurrent(struct G_INDEX* a_me,
  const struct INDEX_SEQUENCE *ap_indexSequence, int *an_entry) {

  if (ap_indexSequence->indexEntryBlockCount2 > 0 && ap_indexSequence->ci_indexEntryCursor >= 
    ap_indexSequence->indexEntryBlocks2[0].first && ap_indexSequence->ci_indexEntryCursor <=
    ap_indexSequence->cv_lastIndexEntry)
    *an_entry = a_me->hsc_array[ap_indexSequence->ci_indexEntryCursor];
  else *an_entry = -1; // No current item 

  return RETURNED;  
} // m_GIndexSequenceCurrent 


// Update index sequence (NEXT) and get entry 
//
// Passed:
// - a_me:
// - b_descending: 
// - a_indexSequence-> : current state of sequence
//
// Changed:
// - a_indexSequence-> : new state of sequence
// - *an_entry: item entry to retrieve : 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexSequenceNext(struct G_INDEX* a_me, char b_descending,
  struct INDEX_SEQUENCE *a_indexSequence, int *an_entry) {
  m_DIGGY_BOLLARD_S()
  *an_entry = -1; // "disabled" / "No more" a priori
  if (a_indexSequence->indexEntryBlockCount2 > 0) { // "enabled"
    if (b_descending) {
      if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntryBlocks2[0].first) { 
        // NOT YET "no more"...
        if (a_indexSequence->ci_indexEntryCursor > a_indexSequence->cv_lastIndexEntry) {
          // "soft reset"
          a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntryBlockPtr =
            a_indexSequence->indexEntryBlocks2 + a_indexSequence->indexEntryBlockCount2 - 1)->last;
        } else if (--(a_indexSequence->ci_indexEntryCursor) <
          a_indexSequence->cv_indexEntryBlockPtr->first && a_indexSequence->cv_indexEntryBlockPtr >
          a_indexSequence->indexEntryBlocks2) {
          // Pass to next block:
          a_indexSequence->ci_indexEntryCursor = (--(a_indexSequence->cv_indexEntryBlockPtr))->last;
        } // if
        if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntryBlocks2[0].first) 
          // STILL NOT YET "no more"
          *an_entry = a_me->hsc_array[a_indexSequence->ci_indexEntryCursor];
      } // if
    } else { // Ascending...
      if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) { 
        // NOT YET "no more"...
        if (a_indexSequence->ci_indexEntryCursor < a_indexSequence->indexEntryBlocks2[0].first) {
          // "soft reset"
          a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntryBlockPtr =
            a_indexSequence->indexEntryBlocks2)->first;
        } else if (++(a_indexSequence->ci_indexEntryCursor) >
          a_indexSequence->cv_indexEntryBlockPtr->last && a_indexSequence->cv_indexEntryBlockPtr <
          a_indexSequence->indexEntryBlocks2 + a_indexSequence->indexEntryBlockCount2 - 1) {
          // Pass to next block:
          a_indexSequence->ci_indexEntryCursor = (++(a_indexSequence->cv_indexEntryBlockPtr))->first;
        } // if
        if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) 
          // STILL NOT YET "no more"
          *an_entry = a_me->hsc_array[a_indexSequence->ci_indexEntryCursor];
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_GIndexSequenceNext 

// Set NEW index sequence (according to passed "seek params").
//
// Passed:
// - a_me:
// - b_descending: indicating "direction" of "soft reset" 
// - indexSeekFlags: seek flags for sequence request ; 
//   + INDEX_SEEK_FLAGS__ANY : NOT key-based selection 
//   + other flags : key-based selection
// - cr_gKeys: search key(s) (significant when key-based selection) 
//
// Changed:
// - a_indexSequence-> : new sequence corresponding to passed "seek values"
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GIndexSequenceReset(struct G_INDEX* a_me, char b_descending,
  unsigned int indexSeekFlags, void *cr_gKeys, struct INDEX_SEQUENCE *a_indexSequence) {
  m_DIGGY_BOLLARD_S()
  a_indexSequence->indexEntryBlockCount2 = 0; // Empty ("disabled") selection a priori
  if (a_me->count > 0) { 
m_DIGGY_VAR_INDEX_SEEK_FLAGS(indexSeekFlags)

    int n_firstIndexEntry = -1; // Not set a priori
    int n_lastIndexEntry = -1; // Not set a priori
    if (b_FLAG_SET_ON(indexSeekFlags,INDEX_SEEK_FLAG__ANY)) { // NOT key-based selection
      n_firstIndexEntry = 0; 
    } else { // key-based selection
      int top = UNDEFINED;
      int n_indexEntry = UNDEFINED;
      struct INDEX_ENTRY_BLOCK c_indexEntries; // UNDEFINED
      m_TRACK_IF(GIndexBSearch(a_me, -1,cr_gKeys, &n_indexEntry,&top, &c_indexEntries) != RETURNED);
      switch (indexSeekFlags) {
      case INDEX_SEEK_FLAGS__EQUAL:
        if (n_indexEntry != -1) {
          n_firstIndexEntry = c_indexEntries.first;
          n_lastIndexEntry = c_indexEntries.last;
        } // if
      break; case INDEX_SEEK_FLAGS__GREATER_EQUAL:
        if (n_indexEntry != -1) n_firstIndexEntry = c_indexEntries.first; 
        else if (top < a_me->count) n_firstIndexEntry = top;
      break; case INDEX_SEEK_FLAGS__GREATER:
        if (n_indexEntry != -1 && c_indexEntries.last+1 < a_me->count) n_firstIndexEntry =
          c_indexEntries.last+1;
        else if (n_indexEntry == -1 && top < a_me->count) n_firstIndexEntry = top;
      break; case INDEX_SEEK_FLAGS__LESS_EQUAL: 
        if (n_indexEntry != -1) n_lastIndexEntry = n_indexEntry;
        else if (n_indexEntry == -1 && top-1 >= 0) n_lastIndexEntry = top-1;
      break; case INDEX_SEEK_FLAGS__LESS: 
        if (n_indexEntry != -1 && c_indexEntries.first-1 >= 0) n_lastIndexEntry = 
          c_indexEntries.first-1;
        else if (n_indexEntry == -1 && top-1 >= 0) n_lastIndexEntry = top-1;
      break; case INDEX_SEEK_FLAGS__NOT_EQUAL:
        if (n_indexEntry != -1) {
          if (c_indexEntries.first-1 >= 0) n_lastIndexEntry = c_indexEntries.first;
          if (c_indexEntries.last+1 < a_me->count) n_firstIndexEntry = c_indexEntries.last+1;
        } else n_firstIndexEntry = 0; 
      break; default: // INDEX_SEEK_FLAGS__LIKE case 
        m_RAISE(ANOMALY__VALUE__D,indexSeekFlags)
      } // switch
    } // if 

    if (n_firstIndexEntry >= 0 && n_lastIndexEntry >= 0 && n_firstIndexEntry > n_lastIndexEntry) {
      a_indexSequence->indexEntryBlockCount2 = 2; 
      a_indexSequence->indexEntryBlocks2[0].first = 0;
      a_indexSequence->indexEntryBlocks2[0].last = n_lastIndexEntry;
      a_indexSequence->indexEntryBlocks2[1].first = n_firstIndexEntry;
      a_indexSequence->cv_lastIndexEntry = a_indexSequence->indexEntryBlocks2[1].last =
        a_me->count-1;
    } else if (n_firstIndexEntry >= 0 || n_lastIndexEntry >= 0) {
      a_indexSequence->indexEntryBlockCount2 = 1; 
      if (n_firstIndexEntry < 0) n_firstIndexEntry = 0;
      if (n_lastIndexEntry < 0) n_lastIndexEntry = a_me->count-1;
      a_indexSequence->indexEntryBlocks2[0].first = n_firstIndexEntry;
      a_indexSequence->cv_lastIndexEntry =
        a_indexSequence->indexEntryBlocks2[0].last = n_lastIndexEntry;
    } // if
    // Prime the pump: ("soft reset")
    a_indexSequence->ci_indexEntryCursor = (b_descending? a_indexSequence->cv_lastIndexEntry
      + 1: a_indexSequence->indexEntryBlocks2[0].first - 1);
    a_indexSequence->cv_indexEntryBlockPtr = a_indexSequence->indexEntryBlocks2;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_GIndexSequenceReset


// Passed:
// -a_me:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: index is corrupted
// - -1: anomaly is raised
static int GIndexVerify(struct G_INDEX *a_me) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;

#ifdef DIGGY
  if (b_diggyGreenCollectionExam) {
    m_DIGGY_INFO("Exam: a_me->count=%d (indexLabel=%d)",a_me->count,a_me->indexLabel)
    int i = 0 ; for (; i < a_me->count; i++) {
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

  if (hits != expectedHits) completed = COMPLETED__BUT;

  m_DIGGY_RETURN(completed)
} // GIndexVerifyEntry 


// Passed:
// - ax_me:
// 
// Ret:
// - RETURNED: Ok
static inline int om_GIndexFree(struct G_INDEX* ax_me) {
  m_DIGGY_BOLLARD_S()
  free(ax_me->hsc_array);
  m_DIGGY_RETURN(RETURNED)
} // om_GIndexFree

///////////// 2. GREEN INDEXES REAL object //////////////

struct G_INDEXES {
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction;
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction;
  void *r_entryRawFunctionsHandle;
  int indexesNumber ; // when ENABLED ; >= 0
  int *vnhs_gKeyCounts; // NULL when indexesNumber == 0
  struct G_INDEX *vnhs_indexes ; // NULL when indexesNumber == 0
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
  handle->indexesNumber = 0;
  handle->vnhs_gKeyCounts = NULL;
  handle->vnhs_indexes = NULL;
  m_DIGGY_RETURN(RETURNED)
} // GIndexesCreateInstance

// ENTRY_COMPARE_FUNCTION
static int GIndexesEntryCompare (void *r_handle, int indexLabel, int aEntry, int n_bEntry,
  void *cr_bGKeys) {
  m_DIGGY_BOLLARD_S()
  G_INDEXES_HANDLE handle = (G_INDEXES_HANDLE)r_handle;  
  int comparison = UNDEFINED;
  m_ASSERT(indexLabel < handle->indexesNumber) 
  int j = 0; for ( ; j < handle->vnhs_gKeyCounts[indexLabel] ; j++) {
    m_TRACK_IF((comparison = handle->entryRawCompareFunction(
      handle->r_entryRawFunctionsHandle, indexLabel, j, aEntry, n_bEntry,cr_bGKeys)) < 0)
    if (comparison != EQUAL_TO__COMPARISON) break ;
  } // for
  m_DIGGY_RETURN(comparison)
} // GIndexesEntryCompare

// Public function; see .h
int GIndexesAddIndex(G_INDEXES_HANDLE handle, int itemsPhysicalNumber, int gKeyCount) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY(handle->vnhs_indexes,handle->indexesNumber+1)
  m_REALLOC_ARRAY(handle->vnhs_gKeyCounts,handle->indexesNumber+1)
  struct G_INDEX *a_index = handle->vnhs_indexes + handle->indexesNumber ;

  handle->vnhs_gKeyCounts[handle->indexesNumber] = gKeyCount;
  m_TRACK_IF(m_GIndexInit(a_index, itemsPhysicalNumber, GIndexesEntryCompare, handle,
    handle->indexesNumber) != RETURNED)
  m_DIGGY_RETURN(handle->indexesNumber++)
} // GIndexesAddIndex

// Public function; see .h
int GIndexesResize (G_INDEXES_HANDLE handle, int newItemsPhysicalNumber) {
  m_DIGGY_BOLLARD_S()
  int i = 0;
  struct G_INDEX *cs_index = handle->vnhs_indexes;
  for ( ; i < handle->indexesNumber ; i++, cs_index++) {
    m_TRACK_IF(m_GIndexResize(cs_index, newItemsPhysicalNumber) != RETURNED)
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GIndexesResize

// Public function; see .h
int GIndexesRemove(G_INDEXES_HANDLE handle, int entry) {
  m_DIGGY_BOLLARD()
  int n_indexEntry = UNDEFINED, top = UNDEFINED;
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes;
  for ( ; i < handle->indexesNumber ; i ++, s_index++) {
    m_TRACK_IF(GIndexBSearch(s_index, entry,(void *)UNDEFINED,&n_indexEntry,&top,
      (struct INDEX_ENTRY_BLOCK*)UNDEFINED) != RETURNED)
    if (n_indexEntry >= 0) {
      om_GIndexRemove(s_index,n_indexEntry);
    } // if
  } // for
  m_DIGGY_RETURN(RETURNED)
 } // GIndexesRemove

// Public function; see .h
int GIndexesAdd(G_INDEXES_HANDLE handle, int entry) {
  m_DIGGY_BOLLARD()
  int n_indexEntry = UNDEFINED, top = UNDEFINED;
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes;
  for ( ; i < handle->indexesNumber ; i ++, s_index++) {
    m_TRACK_IF(GIndexBSearch(s_index, entry, (void*)UNDEFINED, &n_indexEntry, &top,
      (struct INDEX_ENTRY_BLOCK*)UNDEFINED) != RETURNED)
    if (n_indexEntry == -1) {
      om_GIndexAdd(s_index,top,entry);
    } // if
  } // for
  m_DIGGY_RETURN(RETURNED)
} // GIndexesAdd


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
  void *r_bGKeys) {
  int answer = UNDEFINED;
  m_ASSERT(indexLabel < handle->indexesNumber) 
  int j = 0; for ( ; j < handle->vnhs_gKeyCounts[indexLabel] ; j++) {
    m_TRACK_IF((answer = handle->entryRawEquateFunction(handle->r_entryRawFunctionsHandle,
      indexLabel, j, aEntry, r_bGKeys)) < 0)
    if (answer != ANSWER__YES) break ;
  } // for
  
  return answer;
} // m_GIndexesEntryEquate

// Perform "equation" of entry with a key. 
//
// Passed:
// - handle:
// - indexLabel: 
// - aEntry: "A" entry 
// - indexSeekFlags: 
// - r_bGKeys: "B" keys(s) 
//
// Returned:
// - >=0: "equation" between :"A" and "B" with that key component... 
//   + ANSWER__YES : "A" item and "B" key(s) are similar 
//   + ANSWER__NO : "A" item and "B" key(s) are NOT similar 
// - -1: unexpected problem; anomaly is raised
static int GIndexesSeekEntryEquate(G_INDEXES_HANDLE handle, int indexLabel, int aEntry,
  unsigned int indexSeekFlags, void *r_bGKeys) {
  m_DIGGY_BOLLARD_S()
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

// TODO: cette macro est tregs gednedrale...
// Passed:
// - u_me: array 
// - u_countMax:
// - m_count:
// - m_item
// 
// Changed:
// - m_me:
// - m_count:
#define m_ARRAY_ADD_ITEM(u_me, /*int*/u_countMax, /*int*/m_count, m_item) { \
  m_ASSERT((m_count) < (u_countMax))\
  (u_me)[(m_count)++] = (m_item);\
}
  
// Ensure criteria op flags all set properly for actual criteria evaluation
//
// Ret:
// - COMPLETED__OK: Ok (no rectification)
// - COMPLETED__BUT: criteria op. flags needed rectification
// - -1: unexpected problem; anomaly is raised
static inline int g_GRequestCriteriaRectifyOpFlags(struct G_REQUEST_CRITERION *s_me, int meCountMax,
  int *a_meCount) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // Not rectified a priori
  int depth = 0;
  int initialCriteriaOpFlags = s_me[0].criteriaOpFlags;
  if (*a_meCount > 1) {
    s_me[0].criteriaOpFlags = CRITERIA_OP_FLAG__AND;
    // Ensure following OR op. would have precedence over that AND op. 
    if (om_CriteriaOpFlagsOpenBracketCount(s_me[1].criteriaOpFlags) == 0) om_FLAGS_SET_ON(
      s_me[1].criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)
  } else s_me[0].criteriaOpFlags = ALL_FLAGS_OFF0;
  if (s_me[0].criteriaOpFlags != initialCriteriaOpFlags) completed = COMPLETED__BUT;
  int i = 1; for (; i < *a_meCount; i++) {
    int initialCriteriaOpFlags = s_me[i].criteriaOpFlags;      
    if (b_FLAG_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__OR)) {
      om_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__AND)
      if (i+1 < *a_meCount) {
        // Ensure AND op. have precedence over that OR op. 
        if (om_CriteriaOpFlagsOpenBracketCount(s_me[i+1].criteriaOpFlags) == 0) om_FLAGS_SET_ON(
          s_me[i+1].criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)
      } // if
    } else if (b_FLAG_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__AND)) om_FLAGS_SET_OFF(
      s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__OR)
    depth += om_CriteriaOpFlagsOpenBracketCount(s_me[i].criteriaOpFlags);
    if (depth - om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags) < 0) {
      om_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,o_FLAGS3(CRITERIA_OP_FLAG__CLOSE1,
        CRITERIA_OP_FLAG__CLOSE2, CRITERIA_OP_FLAG__CLOSE3)) 
      switch (depth) {
      case 0: 
      break; case 1: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)
      break; case 2: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)
      break; default: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3) } // switch
    } // if
    m_ASSERT((depth -= om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags)) >= 0) 
    if (i == 1) {
      if (depth - om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags) > 0) {
        om_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,o_FLAGS3(CRITERIA_OP_FLAG__CLOSE1,
          CRITERIA_OP_FLAG__CLOSE2,CRITERIA_OP_FLAG__CLOSE3))
        switch (depth) {
        case 0:
        break; case 1: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)
        break; case 2: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)
        break; default: om_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3) } // switch
      } // if  
      m_ASSERT((depth -= om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags)) == 0) 
    } // if
    if (s_me[i].criteriaOpFlags != initialCriteriaOpFlags) completed = COMPLETED__BUT;
  } // for

  m_DIGGY_RETURN(completed)
} // GRequestCriteriaRectifyOpFlags

// Public function; see .h
int GRequestCriteriaAdd(struct G_REQUEST_CRITERION *s_me, int meCountMax, int *a_meCount,
  struct G_REQUEST_CRITERION criterion, char b_lastCriterion) {
  m_DIGGY_BOLLARD()

  m_ARRAY_ADD_ITEM(s_me, meCountMax, *a_meCount, criterion)

  int completed = COMPLETED__OK; // a priori
  if (b_lastCriterion) m_TRACK_IF((completed = g_GRequestCriteriaRectifyOpFlags(s_me,meCountMax,
    a_meCount)) < 0)
  m_DIGGY_RETURN(completed)
} // GRequestCriteriaAdd

// Public function; see .h
int GIndexesSequenceReset(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *ap_1stGRequestCriterion, char b_descending,
  char *indexSequenceBuffer) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(ap_1stGRequestCriterion->indexLabel < handle->indexesNumber) 
m_DIGGY_VAR_INDEX_SEEK_FLAGS(ap_1stGRequestCriterion->indexSeekFlags)
  m_TRACK_IF(m_GIndexSequenceReset(handle->vnhs_indexes + ap_1stGRequestCriterion->indexLabel,
    b_descending, ap_1stGRequestCriterion->indexSeekFlags,
    ap_1stGRequestCriterion->cr_gKeys, (struct INDEX_SEQUENCE*)indexSequenceBuffer) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // GIndexesSequenceReset

// Simple status stack used for g-criteria evaluation
// depth >= 0 
// depth == count - 1 => count always >= 1
m_STRUCT_C_STACK(GC_EVALUATOR,char)

// Re-init g-criteria "evaluator".
//
// Passed:
// - a_me: 
//
// Changed:
// - *a_me: reset for new criteria eval: depth == 0 ; 'U' 
//
// Ret:
// - RETURNED
static inline int m_GcEvaluatorReset(struct GC_EVALUATOR* a_me) {
  m_DIGGY_BOLLARD_S()
  m_C_STACK_CLEAR(*a_me)
  m_C_STACK_PUSH(*a_me,'U'); 
  m_DIGGY_RETURN(RETURNED)
} // m_GcEvaluatorReset

// Passed:
// - a_me:
//
// Ret: evualator's depth (>=0)
static inline int om_GcEvaluatorDepth(const struct GC_EVALUATOR* ap_me) {
  return ap_me->count - 1;
} // om_GcEvaluatorDepth 

// Iterate next criterion evaluation
//
// Passed:
// - *a_me: current status of g-criteria evaluator 
// - b_passed: current criterion adequation ? (true/false)
// - criteriaOpFlags: current criterion's op. flags (must have been prepared properly: see
//   g_GRequestCriteriaRectifyOpFlags() above)
//
// Changed:
// - *a_me: updated status of g-criteria evaluator 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
static inline int m_GcEvaluatorIterate(struct GC_EVALUATOR* a_me, char b_passed,
  int criteriaOpFlags) {
  m_DIGGY_BOLLARD_S()
  int bracketCount = om_CriteriaOpFlagsOpenBracketCount(criteriaOpFlags);
  int i = 0; for (; i < bracketCount; i++) {
    m_C_STACK_PUSH(*a_me,'U')
  } // for

  char status; m_C_STACK_PEEK(*a_me,status);
  switch (status) {
  case 'U':
    if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OR)) 
      m_C_STACK_POKE(*a_me,b_passed? 'V': 'O') 
    else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__AND)) 
      m_C_STACK_POKE(*a_me,b_passed? 'A': 'X') 
    else m_C_STACK_POKE(*a_me,b_passed? 'V': 'X')
  break; case 'O':
    m_C_STACK_POKE(*a_me,b_passed? 'V': 'O')
  break; case 'A':
    m_C_STACK_POKE(*a_me,b_passed? 'A': 'X')
  break; case 'V':
  break; case 'X':
  break; default: m_RAISE(ANOMALY__VALUE__D,status)
  } // switch 

  bracketCount = om_CriteriaOpFlagsCloseBracketCount(criteriaOpFlags);
  m_ASSERT(om_GcEvaluatorDepth(a_me) >= bracketCount)
  for (i = 0; i < bracketCount; i++) {
    char status;  m_C_STACK_POP(*a_me,status);
    m_ASSERT(status == 'V' || status == 'X')
    char b_passed2 = (status == 'V');
    m_C_STACK_PEEK(*a_me,status);
    switch (status) {
    case 'U':
    break; case 'O':
      m_C_STACK_POKE(*a_me,b_passed2? 'V': 'X')
    break; case 'A':
      m_C_STACK_POKE(*a_me,b_passed2? 'V': 'X')
    break; case 'V':
    break; case 'X':
    break; default: m_RAISE(ANOMALY__VALUE__D,status)
    } // switch
  } // for
  m_ASSERT(om_GcEvaluatorDepth(a_me) >= 0)

  m_DIGGY_RETURN(RETURNED)
} // m_GcEvaluatorIterate

// Public function; see .h
int GIndexesSequenceNext(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, int gRequestCriterionCount,
  char b_descending, char *indexSequenceBuffer, int *an_entry) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(sp_gRequestCriteria[0].indexLabel < handle->indexesNumber) 
  struct INDEX_SEQUENCE *a_indexSequence = (struct INDEX_SEQUENCE*)indexSequenceBuffer;
  struct GC_EVALUATOR gcEvaluator = { UNDEFINED } ;
  m_C_STACK_INIT(gcEvaluator,5)

  char status = (char)UNDEFINED;
  do {
    m_TRACK_IF(m_GIndexSequenceNext(handle->vnhs_indexes + sp_gRequestCriteria[0].indexLabel,
      b_descending, a_indexSequence, an_entry) != RETURNED) 
    if (*an_entry >= 0) {
      m_TRACK_IF(m_GcEvaluatorReset(&gcEvaluator) != RETURNED)
      const struct G_REQUEST_CRITERION* p_gRequestCriterionPtr = sp_gRequestCriteria;

      int i = 0; for (; i < gRequestCriterionCount;
        i++, p_gRequestCriterionPtr++) {

        int answer = ANSWER__YES; // a priori
        if (i > 0) m_TRACK_IF((answer = GIndexesSeekEntryEquate(handle,
          p_gRequestCriterionPtr->indexLabel,*an_entry,
          p_gRequestCriterionPtr->indexSeekFlags, p_gRequestCriterionPtr->cr_gKeys)) < 0)

        m_TRACK_IF(m_GcEvaluatorIterate(&gcEvaluator, answer == ANSWER__YES,
          p_gRequestCriterionPtr->criteriaOpFlags) != RETURNED)

        m_C_STACK_PEEK(gcEvaluator,status) 
        if (status == 'O' || status == 'K') break; 
      } // for
  // Break "criteria handling" loop if entry not rejected by extra criteria 
m_ASSERT(om_GcEvaluatorDepth(&gcEvaluator) == 0) 
      m_C_STACK_PEEK(gcEvaluator,status)
m_ASSERT(status == 'O' || status == 'K') 
      if (status == 'O') break;
    } else break; // No more entry => finished 
  } while (b_TRUE) ;
  m_C_STACK_FREE(gcEvaluator)

  m_DIGGY_VAR_D(*an_entry)
  m_DIGGY_RETURN(RETURNED)
} // GIndexesSequenceNext


// Public function; see .h
int GIndexesSequenceCurrent(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, const char *p_indexSequenceBuffer,
  int *an_entry) {
  m_DIGGY_BOLLARD_S()
  const struct INDEX_SEQUENCE* ap_indexSequence = (struct INDEX_SEQUENCE*)p_indexSequenceBuffer;
  m_ASSERT(sp_gRequestCriteria[0].indexLabel < handle->indexesNumber) 
  struct G_INDEX *a_index = handle->vnhs_indexes + sp_gRequestCriteria[0].indexLabel;
  
  m_TRACK_IF(m_GIndexSequenceCurrent(a_index,ap_indexSequence,an_entry) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GIndexesSequenceCurrent


// Public function; see .h
int GIndexesVerifyEnabled (G_INDEXES_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN(handle->indexesNumber > 0?  ANSWER__YES: ANSWER__NO) 
} // GIndexesVerifyEnabled

// Public function; see .h
int GIndexesVerify (G_INDEXES_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes ;

  for ( ; i < handle->indexesNumber ; i++, s_index++) {
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
int GIndexesVerifyCount (G_INDEXES_HANDLE handle,int *ac_commonCount) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(handle->indexesNumber > 0)

  *ac_commonCount = UNDEFINED; 
  int n_lastCount = -1;
  int completed = COMPLETED__OK; // a priori
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  int i = 0;
  for ( ; i < handle->indexesNumber ; i++, s_index++) {
    int thatCount = om_GIndexGetCount(s_index);
    if (n_lastCount != -1 && thatCount != n_lastCount) {
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
int GIndexesVerifyEntry (G_INDEXES_HANDLE handle, int entry, int expectedHits) {
  m_DIGGY_BOLLARD_S()

  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  for ( ; i < handle->indexesNumber ; i++, s_index++) {
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
int GIndexesClear (G_INDEXES_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  int i = 0 ;
  struct G_INDEX *s_index = handle->vnhs_indexes ;
  for ( ; i < handle->indexesNumber ; i++, s_index++) {
    om_GIndexClear(s_index);
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GIndexesClear


// Public function; see .h
int GIndexesDestroyInstance(G_INDEXES_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  int i = 0;
  struct G_INDEX *cs_index = xh_handle->vnhs_indexes;
  for ( ; i < xh_handle->indexesNumber ; i++, cs_index++) {
    om_GIndexFree(cs_index);
  } // for
  if (xh_handle->indexesNumber > 0) {
    free(xh_handle->vnhs_indexes);
    free(xh_handle->vnhs_gKeyCounts);
  } // if
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // GIndexesDestroyInstance
