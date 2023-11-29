// c-ansi/green.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/green.h"
#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>


#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// Overview: 
// "GREEN INDEX" ("proto" object)
// ^ uses
// "GREEN INDEXES" "GAPS STACK" ("proto" objects)
// ^ uses          ^ uses
// ==== "GREEN COLLECTION" ==== ("genuine" object)


////// 1. some "Utils" (bah!)................


// Passed:
// - m_intType: int, long, etc.
// - m_intVar1:
// - m_intVar2:
#define m_SWAP_INTS(m_intType,m_intVar1,m_intVar2) {\
  m_intType swap = (m_intVar2);\
  (m_intVar2) = (m_intVar1);\
  (m_intVar1) = swap;\
}

#ifdef DIGGY
char b_diggyGreenCollectionExam = b_FALSE0;
#endif

/////////// 2. GREEN INDEX ("proto" object......) ////////////////////

struct GREEN_INDEX {
  int keysNumber; // >= 1
  int *hsc_array; // - Physical size given by global instance's itemsPhysicalNumber
                  // - 'count' first items are significant
  int count; // >= 0
};

// Passed:
// - az_index:
// - itemsPhysicalNumber:
#define m_GREEN_INDEX_INIT(/*struct GREEN_INDEX* */az_index,  /*int*/itemsPhysicalNumber,\
  /*int*/keysNumber) {\
  m_MALLOC_ARRAY((az_index)->hsc_array,itemsPhysicalNumber)\
  (az_index)->keysNumber = keysNumber;\
  (az_index)->count = 0;\
}

// Index's virtual function to compare to items of the index.
//
// Passed:
// - r_handle:
// - indexLabel:
// - int keyRank: indicate the key commponent to use for comparison
// - aEntry: entry for "A"
// - n_bEntry: >= 0 => entry for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
//
// Ret: 
// - >=0: "comparison" between :"A" and "B" with that key component... 
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
// - -1: unexpected problem; anomaly is raised
typedef int (*ENTRIES_COMPARE_FUNCTION) (void *r_handle, int indexLabel, int keyRank,
  int aEntry, int n_bEntry, void *cpr_bKeys) ;

// Compare two items of the index
//
// Passed:
// - a_index:
// - entriesCompareFunction:
// - r_entriesCompareHandle:
// - aIndexEntry: entry in index for "A"
// - n_bEntry: >= 0 => entry in index for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
//
// Changed:
// - *m_comparison: "A - B" comparison between key of index' entry and passed key
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
#define m_GREEN_INDEX_COMPARE(/*struct GREEN_INDEX* */a_index,\
  /*ENTRIES_COMPARE_FUNCTION*/entriesCompareFunction,\
  /*void* */r_entriesCompareHandle,  /*int*/indexLabel,  /*int*/aIndexEntry,  /*int*/n_bEntry,\
  /*void* */cpr_bKeys,  /*int*/m_comparison) {\
  int em_aEntry = a_index->hsc_array[aIndexEntry];\
  int em_j = 0;\
  for ( ; em_j < (a_index)->keysNumber ; em_j++) {\
    m_TRACK_IF((m_comparison = entriesCompareFunction(r_entriesCompareHandle, indexLabel,\
      em_j, em_aEntry, n_bEntry,cpr_bKeys)) < 0)\
    if (m_comparison != EQUAL_TO__COMPARISON) break ;\
  }\
  if (m_comparison == EQUAL_TO__COMPARISON && n_bEntry != -1)  m_comparison = \
    DIFFERENCE_2_COMPARISON(em_aEntry - n_bEntry) ;\
}


// Passed:
// - a_index
//
// Ret: Number of keys (>= 1)
#define /*int*/ GREEN_INDEX_GET_KEYS_NUMBER(/*struct GREEN_INDEX* */a_index) \
  ((a_index)->keysNumber)

// Index entries "block"
struct INDEX_ENTRIES {
  int first; // >= 0
  int last; // >= first
} ; 

// Binary search of "B" in index
//
// Passed:
// - a_index: index to use
// - entriesCompareFunction: index's (custom) comparison function
// - r_entriesCompareHandle: (custom) comparison function's private stuff
// - indexLabel:
// - n_bEntry: >= 0 => entry in index for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
// - nac_indexEntries: NULL if not used 
//
// Modified:
// - *an_indexEntry: >=0 => found entry in index
//   -1 => "B" is not referenced in index ("Not found")
// - *a_top: "best" top index entry established so far ; when "Not found", corresponds to the
//   closest entry in the index that is on top of "B"
// - *nac_indexEntries: (when used) only significant if found entry in index; delimit ALL "B"
//   entries 
//
// Ret:
// - RETURNED
// - -1: anomaly is raised
static int GreenIndexBSearch(struct GREEN_INDEX *a_index,
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction, void *r_entriesCompareHandle, int indexLabel,
  int n_bEntry, void *cpr_bKeys, int *an_indexEntry, int *a_top,
  struct INDEX_ENTRIES *nac_indexEntries) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;
  int i = UNDEFINED;

  int bottom = -1, // Best index entry "before"
    top = a_index->count; // Best index entry "after"

  *an_indexEntry = -1;
  while (top - bottom > 1) {
    i =  (top + bottom) >> 1 ;

    m_GREEN_INDEX_COMPARE(a_index, entriesCompareFunction,r_entriesCompareHandle, indexLabel,
      i,n_bEntry,cpr_bKeys, comparison)
    switch (comparison) {
    case GREATER_THAN__COMPARISON:
      top = i ;
    break; case LESS_THAN__COMPARISON:
      bottom = i ;
    break; case EQUAL_TO__COMPARISON:
      *an_indexEntry = i ;
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,comparison)
    } // switch
    if (*an_indexEntry >= 0) break;
  } // while
  *a_top = top;
  if (*an_indexEntry >= 0 && nac_indexEntries != NULL) {
    for (i = *an_indexEntry-1 ; i >= 0 ; i--) {
      m_GREEN_INDEX_COMPARE(a_index, entriesCompareFunction,r_entriesCompareHandle, indexLabel,
        i,-1,cpr_bKeys, comparison)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == LESS_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    nac_indexEntries->first = i+1;
    for (i = *an_indexEntry+1 ; i < a_index->count ; i++) {
      m_GREEN_INDEX_COMPARE(a_index, entriesCompareFunction,r_entriesCompareHandle, indexLabel,
        i,-1,cpr_bKeys, comparison)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == GREATER_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    nac_indexEntries->last = i-1;
  } //if     
  m_DIGGY_RETURN(RETURNED)
} // GreenIndexBSearch


// Passed:
// - a_index:
// - indexEntry:
#define m_GREEN_INDEX_REMOVE(/*struct GREEN_INDEX* */a_index, /*int*/indexEntry) \
  memmove((a_index)->hsc_array + (indexEntry),  (a_index)->hsc_array + (indexEntry) + 1,\
  sizeof(int) * (((a_index)->count -= 1) - (indexEntry)));


// Passed:
// - a_index:
// - newIndexEntry:
// - newEntry:
#define m_GREEN_INDEX_ADD(/*struct GREEN_INDEX* */a_index,  /*int*/newIndexEntry,\
  /*int*/newEntry) {\
  memmove((a_index)->hsc_array + newIndexEntry + 1,  (a_index)->hsc_array + newIndexEntry,\
    sizeof(int) * (s_index->count++ - newIndexEntry));\
  (a_index)->hsc_array[newIndexEntry] = newEntry;\
}


// Passed:
// - a_index:
#define m_GREEN_INDEX_CLEAR(/*struct GREEN_INDEX* */a_index) {\
  (a_index)->count = 0 ;\
}

// Passed:
// - a_index:
//
// Ret
// number of "active" index entries
#define GREEN_INDEX_GET_COUNT(/*struct GREEN_INDEX* */a_index)  ((a_index)->count)


// Passed:
// - ax_index:
#define m_GREEN_INDEX_FREE(/*struct GREEN_INDEX* */ax_index) {\
  free(ax_index->hsc_array);\
}

// Passed:
// - a_index:
// - newItemsPhysicalNumber:
#define m_GREEN_INDEX_RESIZE(/*struct GREEN_INDEX* */a_index,  /*int*/newItemsPhysicalNumber) {\
  m_REALLOC_ARRAY((a_index)->hsc_array, newItemsPhysicalNumber)\
}


//
struct INDEX_SEQUENCE {
  // index entries "blocks":
  int indexEntriesNumber5; // between 0 and 5 : 0 => 'disabled' ; >0 -> 'enabled' 
  struct INDEX_ENTRIES indexEntries5[5]; 
  // Fields below are only significant if 'enabled' (indexEntriesNumber5 > 0) :
  int cv_firstIndexEntry; // first index entry for ALL "blocks"  
  int cv_lastIndexEntry; // last index entry for ALL "blocks"  
  int ci_indexEntryCursor; // "current" index entry: < cv_firstIndexEntry => Ascending:"soft reset",
    // DEscending:'no more' ; between [indexEntries[0].first - cv_lastIndexEntry] => 'in sequence' ;
    // > cv_lastIndexEntry => Ascending:"no more", DEscending:"soft reset" 
  const struct INDEX_ENTRIES *cv_indexEntriesPtr; // "block" corresponding to "current" index entry 
} ;

// Disable index sequence (disambiguation purpose...)
#define m_DISABLE_INDEX_SEQUENCE(m_indexSequence)  (m_indexSequence).indexEntriesNumber5 = 0;

// Get current entry in index sequence.
//
// Passed:
// - a_index:
// - a_indexSequence-> : current state of sequence
//
// Changed:
// - *an_entry: 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_GreenIndexCurrent(struct GREEN_INDEX* a_index,
  struct INDEX_SEQUENCE *a_indexSequence, int *an_entry) {

  if (a_indexSequence->indexEntriesNumber5 > 0 && a_indexSequence->ci_indexEntryCursor >= 
    a_indexSequence->indexEntries5[0].first && a_indexSequence->ci_indexEntryCursor <=
    a_indexSequence->cv_lastIndexEntry)
    *an_entry = a_index->hsc_array[a_indexSequence->ci_indexEntryCursor];
  else *an_entry = -1; // No current item 

  return RETURNED;  
} // m_GreenIndexCurrent 


// Update index sequence.
//
// Passed:
// - a_index:
// - entriesCompareFunction:
// - r_entriesCompareHandle:
// - indexLabel:
// - b_descending: False:Ascending ; True:DEscending 
// - c_indexSeekFlags: seek flags for sequence request ; only significant with "new sequence" case;
//   + INDEX_SEEK_FLAGS__ANY : NOT key-based selection 
//   + other flags : key-based selection
// - ccr_keys: search key(s) ; only significant with new sequence's key-based selection 
// - a_indexSequence-> : current state of sequence
// - nan_entry: 
//   + == NULL:  "new sequence" (aka "soft reset") case
//   + != NULL:  "next item" case
//
// Changed:
// - a_indexSequence-> : new state of sequence
// - *nan_entry: (only significant whith "next item" case) item entry to retrieve : 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static int GreenIndexSeek(struct GREEN_INDEX* a_index,
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction, void *r_entriesCompareHandle, int indexLabel,
  char b_descending, unsigned int c_indexSeekFlags, void *ccr_keys,
  struct INDEX_SEQUENCE *a_indexSequence, int *nan_entry) {
  m_DIGGY_BOLLARD_S()

  if (nan_entry != NULL) { // "next item" case: seek next in existing sequence
m_DIGGY_VAR_D(a_indexSequence->indexEntriesNumber5)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].last)
    *nan_entry = -1; // "disabled" / "No more" a priori
    if (a_indexSequence->indexEntriesNumber5 > 0) { // "enabled"
      if (b_descending) {
        if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntries5[0].first) { 
          // NOT YET "no more"...
          if (a_indexSequence->ci_indexEntryCursor > a_indexSequence->cv_lastIndexEntry) {
            // "soft reset"
            a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntriesPtr =
              a_indexSequence->indexEntries5 + a_indexSequence->indexEntriesNumber5 - 1)->last;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } else if (--(a_indexSequence->ci_indexEntryCursor) <
            a_indexSequence->cv_indexEntriesPtr->first && a_indexSequence->cv_indexEntriesPtr >
            a_indexSequence->indexEntries5) {
            // Pass to next block:
            a_indexSequence->ci_indexEntryCursor = (--(a_indexSequence->cv_indexEntriesPtr))->last;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } // if
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntries5[0].first) 
            // STILL NOT YET "no more"
            *nan_entry = a_index->hsc_array[a_indexSequence->ci_indexEntryCursor];
        } // if
      } else { // Ascending...
        if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) { 
          // NOT YET "no more"...
          if (a_indexSequence->ci_indexEntryCursor < a_indexSequence->indexEntries5[0].first) {
            // "soft reset"
            a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntriesPtr =
              a_indexSequence->indexEntries5)->first;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } else if (++(a_indexSequence->ci_indexEntryCursor) >
            a_indexSequence->cv_indexEntriesPtr->last && a_indexSequence->cv_indexEntriesPtr <
            a_indexSequence->indexEntries5 + a_indexSequence->indexEntriesNumber5 - 1) {
            // Pass to next block:
            a_indexSequence->ci_indexEntryCursor = (++(a_indexSequence->cv_indexEntriesPtr))->first;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } // if
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) 
            // STILL NOT YET "no more"
            *nan_entry = a_index->hsc_array[a_indexSequence->ci_indexEntryCursor];
        } // if
      } // if
    } // if
m_DIGGY_VAR_D(*nan_entry)

  } else { // "new sequence" case
    a_indexSequence->indexEntriesNumber5 = 0; // Empty ("disabled") selection a priori
    if (a_index->count > 0) { 
m_DIGGY_VAR_INDEX_SEEK_FLAGS(c_indexSeekFlags)

      int n_firstIndexEntry = -1; // Not set a priori
      int n_lastIndexEntry = -1; // Not set a priori
      if (b_FLAG_SET_ON(c_indexSeekFlags,INDEX_SEEK_FLAG__ANY)) { // NOT key-based selection
        n_firstIndexEntry = 0; 
      } else { // key-based selection
        int top = UNDEFINED;
        int n_indexEntry = UNDEFINED;
        struct INDEX_ENTRIES c_indexEntries; // UNDEFINED
        m_TRACK_IF(GreenIndexBSearch(a_index, entriesCompareFunction, r_entriesCompareHandle,
          indexLabel, -1,ccr_keys, &n_indexEntry,&top,&c_indexEntries) != RETURNED);
        switch (c_indexSeekFlags) {
        case INDEX_SEEK_FLAGS__EQUAL:
          if (n_indexEntry != -1) {
            n_firstIndexEntry = c_indexEntries.first;
            n_lastIndexEntry = c_indexEntries.last;
          } // if
        break; case INDEX_SEEK_FLAGS__GREATER_EQUAL:
          if (n_indexEntry != -1) n_firstIndexEntry = c_indexEntries.first; 
          else if (top < a_index->count) n_firstIndexEntry = top;
        break; case INDEX_SEEK_FLAGS__GREATER:
          if (n_indexEntry != -1 && c_indexEntries.last+1 < a_index->count) n_firstIndexEntry =
            c_indexEntries.last+1;
          else if (n_indexEntry == -1 && top < a_index->count) n_firstIndexEntry = top;
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
            if (c_indexEntries.last+1 < a_index->count) n_firstIndexEntry = c_indexEntries.last+1;
          } else n_firstIndexEntry = 0; 
        break; default: // INDEX_SEEK_FLAGS__LIKE case 
          m_RAISE(ANOMALY__VALUE__FMT_D,c_indexSeekFlags)
        } // switch
      } // if 

m_DIGGY_VAR_D(n_firstIndexEntry)
m_DIGGY_VAR_D(n_lastIndexEntry)

      if (n_firstIndexEntry >= 0 && n_lastIndexEntry >= 0 && n_firstIndexEntry > n_lastIndexEntry) {
        a_indexSequence->indexEntriesNumber5 = 2; 
        a_indexSequence->indexEntries5[0].first = 0;
        a_indexSequence->indexEntries5[0].last = n_lastIndexEntry;
        a_indexSequence->indexEntries5[1].first = n_firstIndexEntry;
        a_indexSequence->cv_lastIndexEntry = a_indexSequence->indexEntries5[1].last =
          a_index->count-1;
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].last)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[1].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[1].last)
      } else if (n_firstIndexEntry >= 0 || n_lastIndexEntry >= 0) {
        a_indexSequence->indexEntriesNumber5 = 1; 
        if (n_firstIndexEntry < 0) n_firstIndexEntry = 0;
        if (n_lastIndexEntry < 0) n_lastIndexEntry = a_index->count-1;
        a_indexSequence->indexEntries5[0].first = n_firstIndexEntry;
        a_indexSequence->cv_lastIndexEntry =
          a_indexSequence->indexEntries5[0].last = n_lastIndexEntry;
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries5[0].last)
      } // if
m_DIGGY_VAR_D(a_indexSequence->indexEntriesNumber5)
      // Prime the pump: ("soft reset")
      a_indexSequence->ci_indexEntryCursor = (b_descending? a_indexSequence->cv_lastIndexEntry
        + 1: a_indexSequence->indexEntries5[0].first - 1);
      a_indexSequence->cv_indexEntriesPtr = a_indexSequence->indexEntries5;
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexSeek


// Passed:
// -a_index:
// -entriesCompareFunction:
// -r_entriesCompareHandle:
// -indexLabel:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: index is corrupted
// - -1: anomaly is raised
static int GreenIndexVerify(struct GREEN_INDEX *a_index,
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction, void *r_entriesCompareHandle, int indexLabel) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;

#ifdef DIGGY
  if (b_diggyGreenCollectionExam) {
    int i = 0 ;
    m_DIGGY_INFO("Exam: a_index->count=%d (indexLabel=%d)",a_index->count,indexLabel)
    for (; i < a_index->count; i++) {
      m_DIGGY_INFO("  a_index->hsc_array[%d]=%d",i,a_index->hsc_array[i])
    } // for
  } // if 
#endif

  int j = 1;
  for (; j < a_index->count; j++) {
    m_GREEN_INDEX_COMPARE(a_index, entriesCompareFunction,r_entriesCompareHandle, indexLabel,j,
      a_index->hsc_array[j-1],(void *)(GENERIC_INTEGER) UNDEFINED, comparison)
    if (comparison != GREATER_THAN__COMPARISON) {
      m_ASSERT(comparison != EQUAL_TO__COMPARISON)
      break;
    } // if
  } // for

  if (j < a_index->count) {
    m_DIGGY_RETURN(COMPLETED__BUT)
  } // if

  m_DIGGY_RETURN(COMPLETED__OK)
} // GreenIndexVerify

// Passed:
// - a_index:
// - entry:
// - expectedHits:
//
// Returned
// - COMPLETED__OK: number of "hits" (aka number of times the entry is present in the index)
//                  is as expected
// - COMPLETED__BUT: number of "hits" NOT as expected
// - -1: unexpected problem ; anomaly is raised
static int GreenIndexVerifyEntry(struct GREEN_INDEX *a_index, int entry, int expectedHits) {
  m_DIGGY_BOLLARD_S()

  int completed = COMPLETED__OK;
  
  int hits = 0;
  int i = 0;
  int *s_indexArrayPtr = a_index->hsc_array;
  for (; i < a_index->count ; i++, s_indexArrayPtr++) {
    if (*s_indexArrayPtr == entry) hits++ ;
  } // for

  if (hits != expectedHits) completed = COMPLETED__BUT;

  m_DIGGY_RETURN(completed)
} // GreenIndexVerifyEntry 

///////////// 3. GREEN INDEXES "proto" object //////////////


struct GREEN_INDEXES {
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction;
  void *r_entriesCompareHandle;
  int indexesNumber ; // when ENABLED ; >= 0
  struct GREEN_INDEX *vnhs_indexes ; // NULL when indexesNumber == 0
};


// Passed:
// - mz_indexes:
// - entriesCompareFunctionParam:
// - r_entriesCompareHandleParam:
#define m_GREEN_INDEXES_INIT(/*struct GREEN_INDEXES*/mz_indexes,\
  /*ENTRIES_COMPARE_FUNCTION*/entriesCompareFunctionParam,\
  /*void */r_entriesCompareHandleParam) {\
  (mz_indexes).entriesCompareFunction = entriesCompareFunctionParam;\
  (mz_indexes).r_entriesCompareHandle = r_entriesCompareHandleParam;\
  (mz_indexes).indexesNumber = 0;\
  (mz_indexes).vnhs_indexes = NULL;\
}


// Passed:
// - a_indexes:
// - itemsPhysicalNumber:
// - keysNumber:
//
// Return:
// new index label (technically equal to entry in indexes array)
static int GreenIndexesAddIndex(struct GREEN_INDEXES *a_indexes, int itemsPhysicalNumber,
  int keysNumber) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY(a_indexes->vnhs_indexes,a_indexes->indexesNumber+1)
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes + a_indexes->indexesNumber ;

  m_GREEN_INDEX_INIT(a_index, itemsPhysicalNumber,keysNumber)
  m_DIGGY_RETURN(a_indexes->indexesNumber++)
} // GreenIndexesAddIndex


// Passed:
// - a_indexes:
// - newItemsPhysicalNumber:
static int GreenIndexesResize (struct GREEN_INDEXES *a_indexes, int newItemsPhysicalNumber) {
  m_DIGGY_BOLLARD_S()
  int i = 0;
  struct GREEN_INDEX *cs_index = a_indexes->vnhs_indexes;
  for ( ; i < a_indexes->indexesNumber ; i++, cs_index++) {
    m_GREEN_INDEX_RESIZE(cs_index, newItemsPhysicalNumber)
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesResize


// Passed:
// - mx_indexes:
#define m_GREEN_INDEXES_FREE(/*struct GREEN_INDEXES*/mx_indexes) {\
  int em_i = 0;\
  struct GREEN_INDEX *emcs_index = (mx_indexes).vnhs_indexes;\
  for ( ; em_i < (mx_indexes).indexesNumber ; em_i++, emcs_index++) {\
    m_GREEN_INDEX_FREE(emcs_index);\
  }\
  if ((mx_indexes).indexesNumber > 0) {\
    free((mx_indexes).vnhs_indexes);\
  }\
}


// Remove reference on item in all indexes (no action if not referenced in some index)
//
// Passed:
// - m_indexes:
// - entry:
#define m_GREEN_INDEXES_REMOVE(/*struct GREEN_INDEXES*/m_indexes, /*int*/entry) {\
  int emn_indexEntry = UNDEFINED, top = UNDEFINED;\
  int em_i = 0;\
  struct GREEN_INDEX *s_index = (m_indexes).vnhs_indexes;\
  for ( ; em_i < (m_indexes).indexesNumber ; em_i ++, s_index++) {\
    m_TRACK_IF(GreenIndexBSearch(s_index, (m_indexes).entriesCompareFunction,\
     (m_indexes).r_entriesCompareHandle,em_i,entry,(void *)UNDEFINED,&emn_indexEntry,&top,NULL) !=\
     RETURNED)\
    if (emn_indexEntry >= 0) {\
      m_GREEN_INDEX_REMOVE(s_index,emn_indexEntry)\
    }\
  }\
}


// Add reference on item in all indexes (no action if already referenced in some index)
//
// Passed:
// - m_indexes:
// - entry:
#define m_GREEN_INDEXES_ADD(/*struct GREEN_INDEXES*/ m_indexes, /*int*/ entry) {\
  int emn_indexEntry = UNDEFINED, top = UNDEFINED;\
  int em_i = 0;\
  struct GREEN_INDEX *s_index = (m_indexes).vnhs_indexes;\
  for ( ; em_i < (m_indexes).indexesNumber ; em_i ++, s_index++) {\
    m_TRACK_IF(GreenIndexBSearch(s_index,\
      (m_indexes).entriesCompareFunction,(m_indexes).r_entriesCompareHandle, em_i, entry,\
      (void*)UNDEFINED, &emn_indexEntry, &top, NULL) != RETURNED)\
    if (emn_indexEntry == -1) {\
      m_GREEN_INDEX_ADD(s_index,top,entry)\
    }\
  }\
}

// May or may be NOT index-based selection:
struct SELECTION {
  int indexLabel;
  unsigned int indexSeekFlags;
  void *cfpr_keys; // Only significant with Key-based seek flag(s)
} ;
  

struct INDEX_ITERATOR {
  int selectionsNumber5 ;
  struct SELECTION selections[5] ;
  char b_descending; 
  struct INDEX_SEQUENCE indexSequence;
};

#define b_ASCENDING b_FALSE0
#define b_DESCENDING b_TRUE

#define m_INIT_INDEX_ITERATOR(/*struct INDEX_ITERATOR*/m_indexIterator, /*int*/m_indexLabel1,\
  /*unsigned int*/m_indexSeekFlags1, /*void* */mcfpr_keys1) {\
  (m_indexIterator).selectionsNumber5 = 1;\
  (m_indexIterator).selections[0].indexLabel = m_indexLabel1;\
  (m_indexIterator).selections[0].indexSeekFlags = m_indexSeekFlags1;\
  (m_indexIterator).selections[0].cfpr_keys = mcfpr_keys1;\
  (m_indexIterator).b_descending = b_ASCENDING;\
  m_DISABLE_INDEX_SEQUENCE((m_indexIterator).indexSequence)\
}

#define m_INIT_INDEX_ITERATOR__ARGS(/*struct INDEX_ITERATOR*/m_indexIterator, /*int*/m_indexLabel,\
  /*unsigned int*/m_indexSeekFlags, /*void* */mcfpr_keys) {\
  int em_i = (m_indexIterator).selectionsNumber5;\
  (m_indexIterator).selections[em_i].indexLabel = m_indexLabel;\
  (m_indexIterator).selections[em_i].indexSeekFlags = m_indexSeekFlags;\
  (m_indexIterator).selections[em_i].cfpr_keys = mcfpr_keys;\
  (m_indexIterator).selectionsNumber5 ++;\
}

#define m_HARD_RESET_INDEX_ITERATOR(/*struct INDEX_ITERATOR*/m_indexIterator, mb_descending) {\
  (m_indexIterator).b_descending = mb_descending;\
  m_DISABLE_INDEX_SEQUENCE((m_indexIterator).indexSequence)\
}

// Seek an item using the indexes, using sequence when necessary.
//
// Passed:
// - a_indexes:
// - a_indexIterator:
// - nan_entry: 
//   + == NULL:  "soft reset" case
//   + != NULL:  "next item" case
//
// Changed:
// - a_indexIterator->: either "soft reset" / "in sequence" / "no more" state
// - *nan_entry: (only significant whith "next item" case) item entry to retrieve : 
//   + -1 special value : not found
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static int GreenIndexesSeek(struct GREEN_INDEXES* a_indexes, struct INDEX_ITERATOR *a_indexIterator,
  int *nan_entry) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexIterator->selections[0].indexLabel < a_indexes->indexesNumber) 
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes +
    a_indexIterator->selections[0].indexLabel;
  
  m_ASSERT(a_indexIterator->selectionsNumber5 == 1)
  m_TRACK_IF(GreenIndexSeek(a_index, a_indexes->entriesCompareFunction,
    a_indexes->r_entriesCompareHandle, a_indexIterator->selections[0].indexLabel,
    a_indexIterator->b_descending, a_indexIterator->selections[0].indexSeekFlags,
    a_indexIterator->selections[0].cfpr_keys, &a_indexIterator->indexSequence,
    nan_entry) != RETURNED) 
    if (nan_entry != NULL) m_DIGGY_VAR_D(*nan_entry)
  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesSeek

// Get current entry in index sequence.
//
// Passed:
// - a_indexes:
// - a_indexIterator:
//
// Changed:
// - *an_entry: 
//   + -1 special value : not found
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static int GreenIndexesCurrent(struct GREEN_INDEXES* a_indexes,
  struct INDEX_ITERATOR *a_indexIterator, int *an_entry) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexIterator->selectionsNumber5 == 1)
  m_ASSERT(a_indexIterator->selections[0].indexLabel < a_indexes->indexesNumber) 
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes + a_indexIterator->selections[0].indexLabel;
  
  m_TRACK_IF(m_GreenIndexCurrent(a_index,&a_indexIterator->indexSequence,an_entry) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesCurrent

// Ret:
// - ANSWER__YES: indexes are enabled (i.e at least one index has been added) 
// - ANSWER__NO: indexes are NOT enabled
// - -1: anomaly is raised
static int GreenIndexesVerifyEnabled (struct GREEN_INDEXES *a_indexes) {
  m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN(a_indexes->indexesNumber > 0?  ANSWER__YES: ANSWER__NO) 
} // GreenIndexesVerifyEnabled


// Passed:
// - a_indexes:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: some index is corrupted
// - -1: anomaly is raised
static int GreenIndexesVerify (struct GREEN_INDEXES *a_indexes) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;

  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
    switch (GreenIndexVerify(s_index, a_indexes->entriesCompareFunction,
      a_indexes->r_entriesCompareHandle, i)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT:
      completed = COMPLETED__BUT;
    break; default:
      m_TRACK()
    } // switch
    if (completed == COMPLETED__BUT) break;
  } // for

  m_DIGGY_RETURN(completed)
} // GreenIndexesVerify


// NOT callable if indexes are not enabled 
// 
// Passed:
// - a_indexes:
//
// Changed:
// - *ac_commonCount: only significant with COMPLETED__OK
//
// Ret:
// - COMPLETED__OK: all indexes have the same number of entries
// - COMPLETED__BUT: at least two indexes have different numbers of entries
// - -1: anomaly is raised
static int GreenIndexesVerifyCount (struct GREEN_INDEXES *a_indexes,int *ac_commonCount) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexes->indexesNumber > 0)

  *ac_commonCount = UNDEFINED; 
  int n_lastCount = -1;
  int completed = COMPLETED__OK; // a priori
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;
  int i = 0;
  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
    int thatCount = GREEN_INDEX_GET_COUNT(s_index);
    if (n_lastCount != -1 && thatCount != n_lastCount) {
      completed = COMPLETED__BUT;
      break;
    } // if 
    n_lastCount = thatCount ;
  } // for
  m_ASSERT(n_lastCount >= 0)
  if (completed == COMPLETED__OK) *ac_commonCount = n_lastCount;
 
  m_DIGGY_RETURN(completed)
} // GreenIndexesVerifyCount


// 
// Passed:
// - a_indexes:
// - entry: entry to verify presence in different indexes
// - expectedHits : typically 0 or 1
//
// Returned
// - COMPLETED__OK: number of "hits" (aka number of times the entry is present in the stack)
//                  is as expected is as expected in EACH index
// - COMPLETED__BUT: number of "hits" NOT as expected in 
// - -1: unexpected problem ; anomaly is raised
static int GreenIndexesVerifyEntry (struct GREEN_INDEXES *a_indexes, int entry, int expectedHits) {
  m_DIGGY_BOLLARD_S()

  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;
  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
    switch (GreenIndexVerifyEntry(s_index, entry, expectedHits)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT:
      completed = COMPLETED__BUT;
    break; default:
      m_TRACK()
    } // switch
    if (completed == COMPLETED__BUT) break;
  } // for

  m_DIGGY_RETURN(completed)
} // GreenIndexesVerifyEntry 


// Passed:
// - a_indexes
//
// Ret:
// - RETURNED:
// - -1: anomaly is raised
static int GreenIndexesClear (struct GREEN_INDEXES *a_indexes) {
  m_DIGGY_BOLLARD_S()
  int i = 0 ;
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;
  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
    m_GREEN_INDEX_CLEAR(s_index)
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesClear


///////////////// 4. GAPS STACK ("proto" object...........) /////////////////

// TODO: use c-ansi/stack.h

struct GAPS_STACK {
  int *hsc_stack ; // - Physical size given by itemsPhysicalNumber
                   // - 'count' first items are significant
  int count ;
} ;

// Passed:
// - mz_gaps:
// - itemsPhysicalNumber:
#define m_GAPS_STACK_INIT(/*struct GAPS_STACK*/mz_gaps,  /*int*/itemsPhysicalNumber) {\
  m_MALLOC_ARRAY((mz_gaps).hsc_stack,itemsPhysicalNumber)\
  (mz_gaps).count = 0;\
}

// Passed:
// - m_gaps:
// - newItemsPhysicalNumber:
#define m_GAPS_STACK_RESIZE(/*struct GAPS_STACK*/m_gaps,  /*int*/newItemsPhysicalNumber) \
  m_REALLOC_ARRAY((m_gaps).hsc_stack, newItemsPhysicalNumber)


// Passed:
// - m_gaps:
// - m_poppedEntry:
#define m_GAPS_STACK_POP(/*struct GAPS_STACK*/m_gaps,  /*int*/m_poppedEntry) {\
  m_ASSERT((m_gaps).count > 0)\
  m_poppedEntry = (m_gaps).hsc_stack[--((m_gaps).count)];\
}

// Passed:
// - m_gaps:
//
// Ret:
// - >= 0 => first entry marked as "gap"
// - -1 => no gap
#define n_GAPS_STACK_GET_FIRST(/*struct GAPS_STACK*/m_gaps) ( ((m_gaps).count) > 0 ?\
  (m_gaps).hsc_stack[(m_gaps).count - 1] : -1 )


// Passed:
// - m_gaps:
// - pushedEntry:
// - itemsPhysicalNumber:
#define m_GAPS_STACK_PUSH(/*struct GAPS_STACK*/m_gaps,  /*int*/pushedEntry,\
  /*int*/itemsPhysicalNumber) {\
  m_ASSERT((m_gaps).count < itemsPhysicalNumber)\
  (m_gaps).hsc_stack[((m_gaps).count)++] = pushedEntry;\
}

// Passed:
// - m_gaps:
#define m_GAPS_STACK_CLEAR(/*struct GAPS_STACK*/m_gaps) m_gaps.count = 0;

// Passed:
// - a_gap
// - entry: entry to verify presence
// - expectedHits : typically 0 or 1
//
// Returned
// - COMPLETED__OK: number of "hits" (aka number of times the entry is present in the stack)
//                  is as expected
// - COMPLETED__BUT: number of "hits" NOT as expected
// - -1: unexpected problem ; anomaly is raised
static int GapsStackVerifyEntry (struct GAPS_STACK *a_gaps, int entry, int expectedHits) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK;
  
  int hits = 0;
  int i = 0;
  int *s_stackPtr = a_gaps->hsc_stack;
  for (; i < a_gaps->count ; i++, s_stackPtr++) {
    if (*s_stackPtr == entry) hits++ ;
  } // for

  if (hits != expectedHits) completed = COMPLETED__BUT;

  m_DIGGY_RETURN(completed)
} // GapsStackVerifyEntry 

//
// Returned
// - >= 0: number items in the stack 
// - -1: unexpected problem ; anomaly is raised
static int GapsStackVerifyCount (struct GAPS_STACK *a_gaps) {
  m_DIGGY_BOLLARD_S()

#ifdef DIGGY
  if (b_diggyGreenCollectionExam) {
    int i = 0 ;
    m_DIGGY_INFO("Exam: a_gaps->count=%d",a_gaps->count)
    for (; i < a_gaps->count; i++) {
      m_DIGGY_INFO("  a_gaps->hsc_stack[%d]=%d",i,a_gaps->hsc_stack[i])
    } // for
  } // if 
#endif

  m_DIGGY_RETURN(a_gaps->count)  
} // GapsStackVerifyCount 

// Passed:
// - ax_index:
#define m_GAPS_STACK_FREE(/*struct GAPS_STACK*/mx_gaps)  free((mx_gaps).hsc_stack);


////////////////// 5. GREEN COLLECTIONS  //////////////////

// Possible status flags
// DEAD or ALIVE ?
#define  DEAD_FLAG 0x01
// ALIEN or FAMED ?
#define ALIEN_FLAG 0x02

enum {
  FETCH_4__CHANGE,
  FETCH_4__READ,
  FETCH_4__REMOVE
} ;

struct INDEX_REQUEST {
  struct INDEX_ITERATOR iterator;
  int fetch4; 
} ;
 
#define m_INIT_INDEX_REQUEST(/*struct INDEX_REQUEST*/m_indexRequest, /*int*/m_indexLabel1,\
  /*unsigned int*/m_indexSeekFlags1, /*void* */mcfpr_keys1) {\
  m_INIT_INDEX_ITERATOR((m_indexRequest).iterator, m_indexLabel1, m_indexSeekFlags1, mcfpr_keys1) \
  (m_indexRequest).fetch4 = FETCH_4__CHANGE;\
}

#define m_INIT_INDEX_REQUEST__ARGS(/*struct INDEX_REQUEST*/m_indexRequest, /*int*/m_indexLabel,\
  /*unsigned int*/m_indexSeekFlags, /*void* */mcfpr_keys) {\
  m_INIT_INDEX_ITERATOR__ARGS((m_indexRequest).iterator, m_indexLabel, m_indexSeekFlags,\
    mcfpr_keys) \
}

#define m_HARD_RESET_INDEX_REQUEST(/*struct INDEX_REQUEST*/m_indexRequest, mb_descending,\
  /*int*/m_fetch4) {\
  m_HARD_RESET_INDEX_ITERATOR((m_indexRequest).iterator, mb_descending) \
  (m_indexRequest).fetch4 = m_fetch4;\
}

const int p_indexRequestAutomaticBufferSize = sizeof(struct INDEX_REQUEST);

struct GREEN_COLLECTION {
  int expectedItemsNumber ;

  GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction ;
  GREEN_HANDLER__KEYS_COMPARE_FUNCTION greenHandlerKeysCompareFunction;
  GREEN_HANDLER__EQUATE_FUNCTION greenHandlerEquateFunction;
  void *cr_greenHandlerHandle;

  char b_automaticIndexesRefresh;
  char b_frozen ;
  int greenItemSize;
  char *h_greenArray ;
  unsigned char *hsc_flags ; // only "logically" referenced items' flags are significant
  int itemsPhysicalNumber ; // number of items that are physically initialized
  int i_itemsCount; // number of items "logically" referenced
  int v_maxItemsCount; // max number of items "logically" referenced
  struct GREEN_INDEXES indexes;
  struct INDEX_REQUEST internalIndexRequest;
  struct GAPS_STACK h_gaps; 
  // "Monitored" entries "fetched for change" (in ALIEN / ALIVE state) : 
  int fetched4ChangeEntriesNumber;  
  int fetched4ChangeEntriesPhysicalNumber;  
  int *h_fetched4ChangeEntries;  
};

// typedef GREEN_HANDLER__KEYS_COMPARE_FUNCTION
static int NotEnabledKeysCompare(void *cpr_handle, char b_frozen, int indexLabel, int keyRank,
  char *pr_aGreenItemStuff, char *npr_bGreenItemStuff, void *cpr_bKeys) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // NotEnabledKeysCompare

// typedef GREEN_HANDLER__EQUATE_FUNCTION
static int  NotEnabledEquate(void *cpr_handle,  char b_frozen,
  int indexLabel, int keyRank,  char *pr_aGreenItemStuff, void *pr_bKeys) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // NotEnabledEquate


// Get green set's handle corresponding to item entry in the collection...
//
// Passed:
// - handle:
// - entry:
#define /*char* */ r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(/*GREEN_COLLECTION_HANDLE*/ handle,\
  /*int*/entry)  r_GET_GREEN_ITEM_STUFF((handle)->h_greenArray, (entry), (handle)->greenItemSize)


 // ENTRIES_COMPARE_FUNCTION
static int GreenCollectionEntriesCompare (void *r_handle, int indexLabel, int keyRank, int aEntry,
  int n_bEntry, void *cpr_bKeys) {
  m_DIGGY_BOLLARD_S()
  GREEN_COLLECTION_HANDLE handle = (GREEN_COLLECTION_HANDLE) r_handle;

  char *r_aGreenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(handle,aEntry);
  char *nr_bGreenItemStuff =
  (n_bEntry == -1? NULL: r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(handle,n_bEntry));

  int comparison = handle->greenHandlerKeysCompareFunction(handle->cr_greenHandlerHandle,
    handle->b_frozen, indexLabel,keyRank, r_aGreenItemStuff, nr_bGreenItemStuff, cpr_bKeys);
  m_TRACK_IF(comparison < 0)

  m_DIGGY_RETURN(comparison)
} // GreenCollectionEntriesCompare


// Formal "status flagS"...
#define  ALIEN_DEAD__FLAGS ALIEN_FLAG | DEAD_FLAG
#define ALIEN_ALIVE__FLAGS ALIEN_FLAG
#define  FAMED_DEAD__FLAGS DEAD_FLAG
#define FAMED_ALIVE__FLAGS ALL_FLAGS_OFF0 


// NOTICE: "MICROMONITOR"...
// MICROMONITOR is an imaginary device allowing to follow the items' state in a green collection.
// MICROMONITOR is actually nothing but a bunch of comments written in this source file. The
// aim is thus to improve design comprehension - in particular synchronization between "indexes"
// and "gaps stack"...
// MICROMONITOR: Legend - item "state" is a combination of two distinct elementary statuses:
// - First, item's status vis-a-vis the INDEXES:
//   + ALIEN: item is NOT referenced in the indexes
//   + FAMED: item is referenced in the indexes
// - Second, item's status vis-a-vis the GAPS STACK:
//   + DEAD: the item is referenced as gap
//   + ALIVE: the item is NOT referenced in the "gaps" stack
// Any item is supposed to reach one of those two "stable states":
// - ALIEN / DEAD : item is not used ; the emplacement is "free"
// OR
// - FAMED / ALIVE : item is "in use" ; the emplacement is "occupied"
// However, during transcient modification period (i.e, when an item is "fetched"), ONE item of
// the collection may take the following "transitory" state:
// ALIEN / ALIVE (fetched 4 change) : barely fetched item "removed" from indexes ; the emplacement
// has been "reserved"; fetched item is "waiting" for update...
// Example of MICROMONITOR "display":
// MICROMONITOR: ALIEN / DEAD

// NOTICE: "MINIMONITOR"
// MINIMONITOR is another imaginary device allowing to follow the lists of ALIEN / ALIVE items 
// currently "fetched for change". The followings "decorators" are used:  
// - NADA decorator stipulates that no item is currently "monitored"...
// - ANY+ decorator stipulates that one or more item(s) is(are) currently "monitored"...
// - ANY decorator simply makes no presumption about currently "monitored" items...    
// Example of MINIMONITOR "display":
// MINIMONITOR: NADA


// Public function; see description in .h
int GreenCollectionCreateInstance(GREEN_COLLECTION_HANDLE *azh_handle,  int expectedItemsNumber,
  int greenItemSize, GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction,
  GREEN_HANDLER__KEYS_COMPARE_FUNCTION n_greenHandlerKeysCompareFunction,
  GREEN_HANDLER__EQUATE_FUNCTION n_greenHandlerEquateFunction, void *cfr_greenHandlerHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  GREEN_COLLECTION_HANDLE handle = *azh_handle ;

  handle->expectedItemsNumber = expectedItemsNumber;

  handle->n_greenHandlerDisengageFunction = n_greenHandlerDisengageFunction ;
  handle->greenHandlerKeysCompareFunction =
  (n_greenHandlerKeysCompareFunction != NULL? n_greenHandlerKeysCompareFunction:
   NotEnabledKeysCompare);
  handle->greenHandlerEquateFunction =
  (n_greenHandlerEquateFunction != NULL? n_greenHandlerEquateFunction:
   NotEnabledEquate);
  handle->cr_greenHandlerHandle = cfr_greenHandlerHandle;

  handle->b_frozen = b_FALSE0;
  // TODO: add b_automaticIndexesRefresh parameter
  handle->b_automaticIndexesRefresh = b_TRUE;

  handle->greenItemSize = greenItemSize;
  handle->itemsPhysicalNumber = expectedItemsNumber;

  m_CALLOC(handle->h_greenArray,handle->itemsPhysicalNumber,handle->greenItemSize)
  m_MALLOC_ARRAY(handle->hsc_flags, handle->itemsPhysicalNumber)
  handle->v_maxItemsCount = handle->i_itemsCount = 0 ;
  m_GREEN_INDEXES_INIT(handle->indexes,GreenCollectionEntriesCompare,(void *) handle)
  m_GAPS_STACK_INIT(handle->h_gaps,handle->itemsPhysicalNumber)
  m_INIT_INDEX_REQUEST(handle->internalIndexRequest,INDEX_LABEL0,ALL_FLAGS_OFF0,(void*)UNDEFINED)

  m_MALLOC_ARRAY(handle->h_fetched4ChangeEntries,handle->fetched4ChangeEntriesPhysicalNumber =
    expectedItemsNumber)
  handle->fetched4ChangeEntriesNumber = 0;
  // MINIMONITOR: NADA
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionCreateInstance


// Resize physical arrays (NOT possible when the collection is "frozen")
//
// Passed:
// - handle:
static int GreenCollectionResize(GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(!handle->b_frozen)

  int newItemsPhysicalNumber = handle->itemsPhysicalNumber + handle->expectedItemsNumber;

  m_REALLOC(handle->h_greenArray, handle->greenItemSize*newItemsPhysicalNumber)
  memset(handle->h_greenArray + handle->greenItemSize * handle->itemsPhysicalNumber, 0,
    handle->greenItemSize * handle->expectedItemsNumber);

  m_GAPS_STACK_RESIZE(handle->h_gaps, newItemsPhysicalNumber)
  m_REALLOC_ARRAY(handle->hsc_flags, newItemsPhysicalNumber)
  m_TRACK_IF(GreenIndexesResize(&handle->indexes,newItemsPhysicalNumber) != 0)

  handle->itemsPhysicalNumber = newItemsPhysicalNumber;
  m_DIGGY_RETURN(handle->expectedItemsNumber)
} // GreenCollectionResize


// Passed:
// - handle:
//
// Ret:
// - RETURNED:
// - -1: anomaly is raised
static int GreenCollectionRefreshIndexesInternal(GREEN_COLLECTION_HANDLE handle, char b_internal) {
  m_DIGGY_BOLLARD_S()

  if (b_internal && !handle->b_automaticIndexesRefresh) {
    m_DIGGY_RETURN(RETURNED)
  } // if

  // MINIMONITOR: ANY

  if (handle->fetched4ChangeEntriesNumber == 0) {
    // MINIMONITOR: NADA
    m_DIGGY_RETURN(RETURNED)
  } // if

  m_ASSERT(!handle->b_frozen)

  // MINIMONITOR: ANY+
  int i = 0;
  int *fetched4ChangeEntryPtr = handle->h_fetched4ChangeEntries; 
  for (; i < handle->fetched4ChangeEntriesNumber; i++, fetched4ChangeEntryPtr++) { 
    // MICROMONITOR: ALIEN / ALIVE (fetched 4 change)
    m_ASSERT(b_ALL_FLAGS_OK(handle->hsc_flags[*fetched4ChangeEntryPtr],ALIEN_ALIVE__FLAGS))
m_DIGGY_INFO("*fetched4ChangeEntryPtr=%d Before m_GREEN_INDEXES_ADD()...",*fetched4ChangeEntryPtr)
    m_GREEN_INDEXES_ADD(handle->indexes,*fetched4ChangeEntryPtr)
    m_SET_FLAG_OFF(handle->hsc_flags[*fetched4ChangeEntryPtr],ALIEN_FLAG)
    // MICROMONITOR: FAMED / ALIVE
  } // for
  handle->fetched4ChangeEntriesNumber = 0; 
  // MINIMONITOR: NADA

  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionRefreshIndexesInternal

// Public function; see description in .h
int GreenCollectionRefreshIndexes (GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_FALSE0) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionRefreshIndexes


// Public function; see description in .h
int GreenCollectionPullOut (GREEN_COLLECTION_HANDLE handle, char **at_greenArray) {
  m_DIGGY_BOLLARD()
  m_ASSERT(GreenIndexesVerifyEnabled(&handle->indexes))

  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  *at_greenArray  = handle->h_greenArray;

  m_DIGGY_RETURN(handle->itemsPhysicalNumber)
} // GreenCollectionPullOut


// Public function; see description in .h
int GreenCollectionFreeze(GREEN_COLLECTION_HANDLE handle,  char **nap_greenArray) {
  m_DIGGY_BOLLARD()
  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  handle->b_frozen = b_TRUE;

  if (nap_greenArray != NULL) {
    *nap_greenArray  = handle->h_greenArray;
  } // if

  m_DIGGY_RETURN(handle->itemsPhysicalNumber)
} // GreenCollectionFreeze

static inline int m_GreenCollectionAddFetched4ChangeEntry(GREEN_COLLECTION_HANDLE handle,
  int entry) { 
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_D(entry)
  m_ASSERT(!handle->b_frozen)
  if (handle->fetched4ChangeEntriesNumber == handle->fetched4ChangeEntriesPhysicalNumber) {
    m_REALLOC(handle->h_fetched4ChangeEntries,handle->fetched4ChangeEntriesPhysicalNumber +=
      handle->expectedItemsNumber)
  } // if

  handle->h_fetched4ChangeEntries[handle->fetched4ChangeEntriesNumber++] = entry;
  m_DIGGY_RETURN(RETURNED)
} // m_GreenCollectionAddFetched4ChangeEntry

// Obtain or retrieve emplacement for a green item (in the collection's array).
//
// Passed:
// - cp_handle: green colletion's handle ; protected instance if the collection is frozen
// - n_entry: entry in the list
//   + -1 special value: (smart fetch) find emplacement in the array : if gap(s) exist(s), re-use a
//     gap, otherwise, reserve a new emplacement at the end of the array....
//     (not allowed if the collection is frozen)
//   + >= 0: entry for the <item> in the list (0 == 1st entry) (direct fetch) ; if that position
//     either exceeds that reflected by formal <item>s count, or designate a gap, then the entry is
//     NOT fetched
// - fetch4:  
//   + must be == FETCH_4__CHANGE when n_entry < 0 ("smart fetch") 
//   + must be == FETCH_4__READ if the collection is frozen 
//
// Changed:
// - *acntr_greenItemStuff:
//   + NULL pointer: only possible with "direct fetch" (n_entry >= 0) ; the entry is not directly
//     "fetchable"
//   + otherwise: fetched item
//
// Returned:
// - >= 0: OK, entry in the collection
// - -1: unexpected problem ; anomaly is raised
static int GreenCollectionFetchInternal (GREEN_COLLECTION_HANDLE cp_handle, int n_entry, int fetch4,
  char **acntr_greenItemStuff) {
  m_DIGGY_BOLLARD_S()
  // MINIMONITOR: ANY
m_DIGGY_VAR_D(n_entry) 
  if (n_entry == -1) { // Smart fetch
    m_ASSERT(!cp_handle->b_frozen) 
    m_ASSERT(fetch4 == FETCH_4__CHANGE)
    if ((n_entry = n_GAPS_STACK_GET_FIRST(cp_handle->h_gaps)) < 0) { // No gap
      n_entry = cp_handle->i_itemsCount ;
      // Ensure physical arrays are large enough vis-a-vis fetched entry
      if (n_entry >= cp_handle->itemsPhysicalNumber) {
        m_ASSERT(n_entry <= cp_handle->itemsPhysicalNumber)
        m_TRACK_IF(GreenCollectionResize(cp_handle) < 0)
        m_ASSERT(n_entry < cp_handle->itemsPhysicalNumber)
      } // if
      // New item
      m_SET_ALL_FLAGS(cp_handle->hsc_flags[n_entry],ALIEN_ALIVE__FLAGS)
      if (++(cp_handle->i_itemsCount) > cp_handle->v_maxItemsCount) cp_handle->v_maxItemsCount = 
        cp_handle->i_itemsCount;
      // MICROMONITOR: ALIEN / ALIVE 
    } else { // Use existing gap 
      m_ASSERT(n_entry < cp_handle->i_itemsCount)
      m_ASSERT(b_ALL_FLAGS_OK(cp_handle->hsc_flags[n_entry],ALIEN_DEAD__FLAGS))
      // MICROMONITOR: ALIEN / DEAD 
      m_GAPS_STACK_POP(cp_handle->h_gaps,n_entry)
      m_SET_FLAG_OFF(cp_handle->hsc_flags[n_entry],DEAD_FLAG)
      // MICROMONITOR: ALIEN / ALIVE 
    } // if

    m_TRACK_IF(m_GreenCollectionAddFetched4ChangeEntry(cp_handle,n_entry) != RETURNED)
m_DIGGY_VAR_D(n_entry) 
    // MICROMONITOR: ALIEN / ALIVE (fetched 4 change)
    // MINIMONITOR: ANY+
    *acntr_greenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(cp_handle,n_entry);
m_DIGGY_VAR_P(*acntr_greenItemStuff) 

  } else { // Direct fetch
    if (n_entry < cp_handle->i_itemsCount &&  // Existing item
      !b_FLAG_SET_ON(cp_handle->hsc_flags[n_entry],DEAD_FLAG)) { // It's NOT a gap
      m_ASSERT(b_ALL_FLAGS_OK(cp_handle->hsc_flags[n_entry],FAMED_ALIVE__FLAGS))
      // MICROMONITOR: FAMED / ALIVE 
      if (fetch4 != FETCH_4__READ) { 
        m_ASSERT(!cp_handle->b_frozen) 
        m_GREEN_INDEXES_REMOVE(cp_handle->indexes,n_entry)
        m_SET_FLAG_ON(cp_handle->hsc_flags[n_entry],ALIEN_FLAG)
        // MICROMONITOR: ALIEN / ALIVE
        if (fetch4 == FETCH_4__CHANGE) { 
          m_TRACK_IF(m_GreenCollectionAddFetched4ChangeEntry(cp_handle,n_entry) != RETURNED)
          // MICROMONITOR: ALIEN / ALIVE (fetched 4 change)
          // MINIMONITOR: ANY+
        } else { // FETCH_4__REMOVE
          // MICROMONITOR: ALIEN / ALIVE
          m_GAPS_STACK_PUSH(cp_handle->h_gaps, n_entry, cp_handle->itemsPhysicalNumber)
          m_SET_FLAG_ON(cp_handle->hsc_flags[n_entry],DEAD_FLAG)
          // MICROMONITOR: ALIEN / DEAD
        } // if
      // } else { // FETCH_4__READ 
        // MICROMONITOR: FAMED / ALIVE
      } // if
      *acntr_greenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(cp_handle,n_entry);
    } else { // Not fetchable 
      *acntr_greenItemStuff = NULL; 
      // MINIMONITOR: ANY
    } // if
  } // if

  // MINIMONITOR: ANY
  m_DIGGY_RETURN(n_entry)
} // GreenCollectionFetchInternal


// Public function; see description in .h
int GreenCollectionGetCount (GREEN_COLLECTION_HANDLE cp_handle, char **navntr_greenItemStuff ) {
  m_DIGGY_BOLLARD()
  // MINIMONITOR: ANY 
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  if (navntr_greenItemStuff != NULL) {
    if (cp_handle->i_itemsCount > 0) {

      int ret = GreenCollectionFetchInternal(cp_handle, cp_handle->i_itemsCount - 1,
        cp_handle->b_frozen? FETCH_4__READ: FETCH_4__CHANGE,  navntr_greenItemStuff); 
      m_TRACK_IF(ret < 0) 
      m_ASSERT(ret == cp_handle->i_itemsCount - 1)
      // MINIMONITOR: ANY (0 or 1 item) 
    } else *navntr_greenItemStuff = NULL;
  } // if

  // TODO:
  // if (na_actualItemsCount != NULL) { 
  // TODO: macro m_GAPS_GET_COUNT()
  // *na_actualItemsCount =  cp_handle->i_itemsCount - cp_handle->h_gaps.count
  m_DIGGY_RETURN(cp_handle->i_itemsCount)
} // GreenCollectionGetCount


// Public function; see description in .h
int GreenCollectionFetch (GREEN_COLLECTION_HANDLE cp_handle, int n_entry,
  char **acntr_greenItemStuff) {
  m_DIGGY_BOLLARD()
  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  int entry = GreenCollectionFetchInternal(cp_handle,n_entry,
    cp_handle->b_frozen? FETCH_4__READ: FETCH_4__CHANGE, acntr_greenItemStuff);
  m_TRACK_IF(entry < 0)

  // MINIMONITOR: ANY (0 or 1 item) 
  m_DIGGY_RETURN(entry)
} // GreenCollectionFetch


// Public function; see description in .h
int GreenCollectionClear (GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(!handle->b_frozen)
  // MINIMONITOR: ANY

  handle->i_itemsCount = 0 ;
  handle->fetched4ChangeEntriesNumber = 0 ;
  m_GAPS_STACK_CLEAR(handle->h_gaps)
  m_TRACK_IF(GreenIndexesClear(&handle->indexes) != RETURNED)

  // MINIMONITOR: NADA
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionClear


// Public function; see description in .h
int GreenCollectionAddIndex (GREEN_COLLECTION_HANDLE handle, int keysNumber) {
  m_DIGGY_BOLLARD()
  m_ASSERT(!handle->b_frozen)
  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  // TODO: permettre d'ajouter des indexes ag chaud...
  m_ASSERT(handle->i_itemsCount == 0)

  m_ASSERT(keysNumber > 0)

  int newIndexLabel = GreenIndexesAddIndex(&handle->indexes, handle->itemsPhysicalNumber,
    keysNumber);
  m_TRACK_IF(newIndexLabel < 0)

  m_DIGGY_RETURN(newIndexLabel)
} // GreenCollectionAddIndex

// Public function; see description in .h
int GreenCollectionIndexRequest(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaNumber,
  int indexLabel1, unsigned int indexSeekFlags1, void *cfpr_keys1, ...) {
  m_DIGGY_BOLLARD()

  m_ASSERT(indexSeekFlags1 != ALL_FLAGS_OFF0) 
  m_ASSERT(nf_indexRequestAutomaticBuffer != NULL || !cp_handle->b_frozen) 

  struct INDEX_REQUEST *indexRequestPtr = (nf_indexRequestAutomaticBuffer != NULL?
    (struct INDEX_REQUEST *)nf_indexRequestAutomaticBuffer: &(cp_handle->internalIndexRequest));

  m_INIT_INDEX_REQUEST(*indexRequestPtr,indexLabel1,indexSeekFlags1,cfpr_keys1)

  { va_list arguments;
    va_start(arguments,cfpr_keys1);

    int i = 1;
    unsigned int criteraOpFlags = UNDEFINED;
    for (; i < criteriaNumber;  i++) {
      criteraOpFlags = va_arg(arguments,unsigned int);
      indexLabel1 = va_arg(arguments,int);
      indexSeekFlags1 = va_arg(arguments,unsigned int);
      cfpr_keys1 = va_arg(arguments,void *);

      m_ASSERT(criteraOpFlags == CRITERIA_OP_FLAGS__AND)
      m_INIT_INDEX_REQUEST__ARGS(*indexRequestPtr,indexLabel1,indexSeekFlags1,cfpr_keys1)
    } // for 

    va_end(arguments);
  } // arguments

  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  m_TRACK_IF(GreenIndexesSeek(&cp_handle->indexes, &indexRequestPtr->iterator, NULL) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionIndexRequest


// Public function; see description in .h
int GreenCollectionIndexFetch(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, unsigned int indexFetchFlags,
  char **acvntr_greenItemStuff, int *nacvn_entry) {
  m_DIGGY_BOLLARD()
  // MINIMONITOR: ANY
  //m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
m_DIGGY_VAR_INDEX_FETCH_FLAGS(indexFetchFlags)

  struct INDEX_REQUEST *indexRequestPtr = (nf_indexRequestAutomaticBuffer != NULL?
    (struct INDEX_REQUEST *)nf_indexRequestAutomaticBuffer: &(cp_handle->internalIndexRequest));

  if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__RESET)) {
    m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
    // MINIMONITOR: NADA
    int fetch4 = FETCH_4__CHANGE; // a priori
    if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__READ)) fetch4 = FETCH_4__READ;
    else if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__REMOVE)) fetch4 = FETCH_4__REMOVE;
     
    m_HARD_RESET_INDEX_REQUEST(*indexRequestPtr,b_FLAG_SET_ON(indexFetchFlags,
      INDEX_FETCH_FLAG__DESCENDING),fetch4)
    m_TRACK_IF(GreenIndexesSeek(&cp_handle->indexes,&indexRequestPtr->iterator,NULL) != RETURNED)
  } // if    

  int n_entry = UNDEFINED;
  int result = RESULT__NOT_FOUND;

  if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__NEXT)) {
    m_TRACK_IF(GreenIndexesSeek(&cp_handle->indexes,&indexRequestPtr->iterator,&n_entry) != RETURNED)
  } else {
    m_TRACK_IF(GreenIndexesCurrent(&cp_handle->indexes,&indexRequestPtr->iterator,&n_entry) !=
      RETURNED)
  } // if
  if (n_entry != -1) result = RESULT__FOUND; 

  int n_fetch4 = -1 ; // No fetch a priori
  if ((n_entry != -1) || (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__NEXT) &&
    b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__RESET) &&
    b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__CHANGE) &&
    b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__SMART))) n_fetch4 = indexRequestPtr->fetch4 ;

  // MINIMONITOR: ANY 
  
  int n_fetchedEntry = -1; // a priori
  if (n_fetch4 != -1) { 
    n_fetchedEntry = GreenCollectionFetchInternal(cp_handle,n_entry,n_fetch4,acvntr_greenItemStuff);
    m_TRACK_IF(n_fetchedEntry < 0)
    m_ASSERT(*acvntr_greenItemStuff != NULL)
  } else *acvntr_greenItemStuff = NULL; 

  if (nacvn_entry != NULL) *nacvn_entry = n_fetchedEntry;

  m_DIGGY_RETURN(result)
} // GreenCollectionIndexFetch


// Public function; see description in .h
int GreenCollectionVerifyIndexes (GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD()
  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  int completed = COMPLETED__OK; // a priori 

#ifdef DIGGY
  if (b_diggyGreenCollectionExam) {
    m_DIGGY_INFO("Exam: handle->(h_greenArray=%p itemsPhysicalNumber=%d i_itemsCount=%d)",
      handle->h_greenArray,handle->itemsPhysicalNumber,handle->i_itemsCount)
    int i = 0 ;
    for (; i < handle->i_itemsCount; i++) {
      m_DIGGY_INFO("  handle->hsc_flags[%d]=%02x",i,handle->hsc_flags[i])
    } // for
  } // if 
#endif

  int gapsCount = GapsStackVerifyCount(&handle->h_gaps);
  m_TRACK_IF(gapsCount < 0)

  // 1. Are indexes enabled ? (if not, just verify there's no gap 
  switch (GreenIndexesVerifyEnabled(&handle->indexes)) {
  case ANSWER__YES:
  break; case ANSWER__NO:
    if (gapsCount != 0) completed = COMPLETED__BUT;
    m_DIGGY_RETURN(completed)
  break; default:
    m_TRACK()
  } // switch
  // assert: indexes are enabled    

  // 2. Verify the "global" counts 
  if (completed == COMPLETED__OK) {
    int c_indexedItemsCount = UNDEFINED;
    completed = GreenIndexesVerifyCount(&handle->indexes,&c_indexedItemsCount);
    m_TRACK_IF(completed < 0)
m_DIGGY_VAR_D(completed)
m_DIGGY_INFO("handle->i_itemsCount=%d c_indexedItemsCount=%d gapsCount=%d",handle->i_itemsCount,c_indexedItemsCount,gapsCount)
    if (completed == COMPLETED__OK) {
      if (handle->i_itemsCount != c_indexedItemsCount + gapsCount) completed = COMPLETED__BUT;
    } // if
  } // if 
m_DIGGY_VAR_D(completed)
  // 3. Verify  presence/absence of entries in indexes and gaps stack
  if (completed == COMPLETED__OK) {
m_DIGGY_VAR_D(completed)
    int i = 0;
    for (; i < handle->i_itemsCount; i++) {
      int expectedHits = 0;
      if (b_FLAG_SET_ON(handle->hsc_flags[i],DEAD_FLAG))  expectedHits = 1; 
      m_TRACK_IF((completed = GapsStackVerifyEntry(&handle->h_gaps,i,expectedHits)) < 0)
      if (completed == COMPLETED__BUT) break;
      expectedHits = 0;
      if (!b_FLAG_SET_ON(handle->hsc_flags[i],ALIEN_FLAG))  expectedHits = 1;
      m_TRACK_IF((completed = GreenIndexesVerifyEntry(&handle->indexes,i,expectedHits)) < 0) 
      if (completed == COMPLETED__BUT) break;
    } // for
  } // if 

m_DIGGY_VAR_D(completed)
  // 4. Verify index(es) good ordering 
  if (completed == COMPLETED__OK) {
    completed = GreenIndexesVerify(&handle->indexes);
    m_TRACK_IF(completed < 0)
  } // if

m_DIGGY_VAR_D(completed)
  m_DIGGY_RETURN(completed)
} // GreenCollectionVerifyIndexes


// Public function; see description in .h
int GreenCollectionDestroyInstance (GREEN_COLLECTION_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  if (xh_handle->n_greenHandlerDisengageFunction != NULL) {
    char *r_greenItemStuff = xh_handle->h_greenArray;
    int i = 0;
    for (; i < xh_handle->v_maxItemsCount ; i++, r_greenItemStuff += xh_handle->greenItemSize) {
      m_TRACK_IF(xh_handle->n_greenHandlerDisengageFunction(xh_handle->cr_greenHandlerHandle,
        r_greenItemStuff) != RETURNED)
    } // for
  } // if
  free(xh_handle->h_greenArray) ;

  m_GREEN_INDEXES_FREE(xh_handle->indexes)

  m_GAPS_STACK_FREE(xh_handle->h_gaps)

  free(xh_handle->h_fetched4ChangeEntries);

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionDestroyInstance
