// c-ansi/green.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/green.h"
#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>


#include "c-ansi/diggy.h"
#include "testy-diggy.h"


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


// Binary search of "B" in index
//
// Passed:
// - a_index: index to use
// - entriesCompareFunction: index's (custom) comparison function
// - r_entriesCompareHandle: (custom) comparison function's private stuff
// - indexLabel:
// - n_bEntry: >= 0 => entry in index for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
//
// Modified:
// - *an_indexEntry: >=0 => found entry in index
//   -1 => "B" is not referenced in index ("Not found")
// - *a_top: "best" top index entry established so far ; when "Not found", corresponds to the
//   closest entry in the index that is on top of "B"
//
// Ret:
// - RETURNED
// - -1: anomaly is raised
static int GreenIndexBSearch(struct GREEN_INDEX *a_index,
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction, void *r_entriesCompareHandle, int indexLabel,
  int n_bEntry, void *cpr_bKeys, int *an_indexEntry, int *a_top) {
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
  int n_indexEntry;
  int c_lastIndexEntry; 
} ;

//
#define m_RESET_INDEX_SEQUENCE(m_indexSequence)  (m_indexSequence).n_indexEntry = -1;

// Seek an item via the index, holding a sequence when necessary.
//
// Passed:
// - a_index:
// - n_indexSeek: seek method
//   + -1 special value: not specified => fetch next item in sequence (if exists...)
//   + INDEX_SEEK__ KEY / BOTTOM_UP / BUTTOM_DOWN / TOP_DOWN / TOP_UP / UP / DOWN
// - ccr_keys: search key(s) ; only significant for
//   INDEX_SEEK__ KEY / BOTTOM_UP / BUTTOM_DOWN / TOP_DOWN / TOP_UP
// - a_indexSequence-> : current state of sequence
//
// Changed:
// - a_indexSequence-> : new state of sequence
// - *an_entry:
//   + -1 special value : not found
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static int GreenIndexSeek(struct GREEN_INDEX* a_index,
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction, void *r_entriesCompareHandle, int indexLabel,
  int n_indexSeek, void *ccr_keys, struct INDEX_SEQUENCE *a_indexSequence, int *an_entry) {
  m_DIGGY_BOLLARD_S()

  if (n_indexSeek == -1) { // Seek in existing sequence
    if (a_indexSequence->n_indexEntry != -1 && (a_indexSequence->n_indexEntry >= a_index->count ||
      a_indexSequence->c_lastIndexEntry >= a_index->count))  a_indexSequence->n_indexEntry = -1;
    if (a_indexSequence->n_indexEntry != -1) {
      if (a_indexSequence->n_indexEntry == a_indexSequence->c_lastIndexEntry)  a_indexSequence->
        n_indexEntry = -1 ;
      else {
        if (a_indexSequence->n_indexEntry < a_indexSequence->c_lastIndexEntry) a_indexSequence->
          n_indexEntry++;
        else a_indexSequence->n_indexEntry--;
      } // if
    } // if

  } else { // Undertake new seek (sequence)
    a_indexSequence->n_indexEntry = -1;
    if (a_index->count > 0) { // XXXXXXXXXXXXX TODO VERIFIER 
      int top = UNDEFINED;

      switch (n_indexSeek) {
      case INDEX_SEEK__UP:
        a_indexSequence->n_indexEntry = 0;
        a_indexSequence->c_lastIndexEntry = a_index->count-1;
      break; case INDEX_SEEK__DOWN:
        a_indexSequence->n_indexEntry = a_index->count-1;
        a_indexSequence->c_lastIndexEntry = 0;
      break; case INDEX_SEEK__KEY:
      case INDEX_SEEK__BOTTOM_UP:
      case INDEX_SEEK__TOP_DOWN:
      case INDEX_SEEK__TOP_UP:
      case INDEX_SEEK__BOTTOM_DOWN:
        m_TRACK_IF(GreenIndexBSearch(a_index, entriesCompareFunction, r_entriesCompareHandle,
          indexLabel, -1,ccr_keys, &a_indexSequence->n_indexEntry,&top) != RETURNED);
        switch (n_indexSeek) {
        case INDEX_SEEK__KEY:
          if (a_indexSequence->n_indexEntry != -1) a_indexSequence->c_lastIndexEntry = 
            a_indexSequence->n_indexEntry;
        break; case INDEX_SEEK__BOTTOM_UP:
        case INDEX_SEEK__BOTTOM_DOWN:
          if (a_indexSequence->n_indexEntry == -1 && top < a_index->count) a_indexSequence->
            n_indexEntry = top;
          if (a_indexSequence->n_indexEntry != -1) {
            a_indexSequence->c_lastIndexEntry = a_index->count-1;
            if (n_indexSeek == INDEX_SEEK__BOTTOM_DOWN) {
              m_SWAP_INTS(int, a_indexSequence->n_indexEntry, a_indexSequence->c_lastIndexEntry)
            } // if
          } // if
        break; default: // INDEX_SEEK__TOP_DOWN / TOP_UP
          if (a_indexSequence->n_indexEntry == -1 && top > 0) a_indexSequence->n_indexEntry = top
            - 1;
          if (a_indexSequence->n_indexEntry != -1) {
            a_indexSequence->c_lastIndexEntry = 0;
            if (n_indexSeek == INDEX_SEEK__TOP_UP) {
              m_SWAP_INTS(int, a_indexSequence->n_indexEntry,a_indexSequence->c_lastIndexEntry)
            } // if
          } // if
        } // switch
      break; default:
        m_RAISE(ANOMALY__VALUE__FMT_D,n_indexSeek)
      } // switch
    } // if
  } // if

  // a_indexSequence->n_indexEntry:
  // -1 : not found
  // >= 0 : entry found in index
  *an_entry = -1 ;
  if (a_indexSequence->n_indexEntry != -1) *an_entry = a_index->hsc_array[a_indexSequence->
    n_indexEntry];

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
      m_RAISE_VERBATIM_IF(comparison == EQUAL_TO__COMPARISON)
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

struct INDEX_ITERATOR {
  int indexLabel;
  struct INDEX_SEQUENCE indexSequence;
};

const int p_indexIteratorAutomaticBufferSize = sizeof(struct INDEX_ITERATOR);

struct GREEN_INDEXES {
  ENTRIES_COMPARE_FUNCTION entriesCompareFunction;
  void *r_entriesCompareHandle;
  int indexesNumber ; // when ENABLED ; >= 0
  struct GREEN_INDEX *vnhs_indexes ; // NULL when indexesNumber == 0
  //struct {
  //  int indexLabel;
  //  struct INDEX_SEQUENCE indexSequence;
  //} internalSequence;
  struct INDEX_ITERATOR internalIndexIterator;
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
  (mz_indexes).internalIndexIterator.indexLabel = 0;\
  m_RESET_INDEX_SEQUENCE((mz_indexes).internalIndexIterator.indexSequence)\
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
     (m_indexes).r_entriesCompareHandle, em_i, entry,(void *)UNDEFINED, &emn_indexEntry, &top) !=\
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
      (void*)UNDEFINED, &emn_indexEntry, &top) != RETURNED)\
    if (emn_indexEntry == -1) {\
      m_GREEN_INDEX_ADD(s_index,top,entry)\
    }\
  }\
}


// Seek an item using the indexes, holding a sequence when necessary.
//
// Passed:
// - a_indexes:
// - nf_indexIteratorAutomaticBuffer:
//   NULL special address: use internal structure => NOT re-entrant
//   non NULL: buffer on stack => allows (thread) re-entrancy 
// - indexLabel: index to use ; if changed, automatically reset sequence
// - n_indexSeek: seek method
//   + -1 special value: not specified => fetch next item in sequence (if exists...)
//   + INDEX_SEEK__ KEY / BOTTOM_UP / BUTTOM_DOWN / TOP_DOWN / TOP_UP / UP / DOWN
// - ccr_keys: search key(s) ; only significant for
//   INDEX_SEEK__ KEY / BOTTOM_UP / BUTTOM_DOWN / TOP_DOWN / TOP_UP
//
// Changed:
// - *an_entry:
//   + -1 special value : not found
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static int GreenIndexesSeek (struct GREEN_INDEXES* a_indexes,
  INDEX_ITERATOR_AUTOMATIC_BUFFER nf_indexIteratorAutomaticBuffer, int indexLabel,
  int n_indexSeek, void *ccr_keys, int *an_entry) {
  m_DIGGY_BOLLARD_S()
  m_RAISE_VERBATIM_IF(indexLabel >= a_indexes->indexesNumber) 
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes + indexLabel;
  
  struct INDEX_ITERATOR *indexIteratorPtr = (nf_indexIteratorAutomaticBuffer != NULL?
    (struct INDEX_ITERATOR *)nf_indexIteratorAutomaticBuffer: &(a_indexes->internalIndexIterator));
  if (indexIteratorPtr->indexLabel != indexLabel) {
    m_RESET_INDEX_SEQUENCE(indexIteratorPtr->indexSequence)
    indexIteratorPtr->indexLabel = indexLabel;
  } // if

  m_TRACK_IF(GreenIndexSeek(a_index, a_indexes->entriesCompareFunction,
    a_indexes->r_entriesCompareHandle, indexLabel, n_indexSeek, ccr_keys,
    &indexIteratorPtr->indexSequence, an_entry) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesSeek


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
  m_RAISE_VERBATIM_IF(a_indexes->indexesNumber == 0)

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
  m_RAISE_VERBATIM_IF(n_lastCount < 0)
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
  m_RAISE_VERBATIM_IF((m_gaps).count == 0)\
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
  m_RAISE_VERBATIM_IF((m_gaps).count >= itemsPhysicalNumber)\
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

struct GREEN_COLLECTION {
  int expectedItemsNumber ;

  GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction ;
  GREEN_HANDLER__KEYS_COMPARE_FUNCTION greenHandlerKeysCompareFunction;
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
  struct GAPS_STACK gaps; // TODO: ajouter h_ prefix vu qu'il faut appeler m_GAPS_STACK_FREE()
  int nv_fetched4ChangeEntry; // -1 special value: no entry currently "fetched"
                              // >= 0: entry currently "fetched" (ALIEN / ALIVE state)
};

// typedef GREEN_HANDLER__KEYS_COMPARE_FUNCTION
static int NotEnabledKeysCompare (void *cpr_handle, char b_frozen, int indexLabel, int keyRank,
  char *pr_aGreenItemStuff, char *npr_bGreenItemStuff, void *cpr_bKeys) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // NotEnabledKeysCompare


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
// The item state displayed in MICROMONITOR is ormamented with the following decorators:
// - |-- decorator emphases the effective "starting" of item monitoring
// - --| decorator emphases the effective "termination" of item monitoring
// - ... decorator emphases the "continuation" of item monitoring
// - [NADA] decorator simply stipulates that no item currently requires "monitoring"...
// - => [NADA] decorator emphases the entrance into a [NADA] phase
// Example of MICROMONITOR "display":
// MICROMONITOR: |-- ALIEN / DEAD ...


// Public function; see description in .h
int GreenCollectionCreateInstance (GREEN_COLLECTION_HANDLE *azh_handle, int expectedItemsNumber,
  int greenItemSize, GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction,
  GREEN_HANDLER__KEYS_COMPARE_FUNCTION n_greenHandlerKeysCompareFunction,
  void *cfr_greenHandlerHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  GREEN_COLLECTION_HANDLE handle = *azh_handle ;

  handle->expectedItemsNumber = expectedItemsNumber;

  handle->n_greenHandlerDisengageFunction = n_greenHandlerDisengageFunction ;
  handle->greenHandlerKeysCompareFunction =
  (n_greenHandlerKeysCompareFunction != NULL? n_greenHandlerKeysCompareFunction:
   NotEnabledKeysCompare);
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
  m_GAPS_STACK_INIT(handle->gaps,handle->itemsPhysicalNumber)

  handle->nv_fetched4ChangeEntry = -1 ;
  // MICROMONITOR: [NADA]
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionCreateInstance


// Resize physical arrays (NOT possible when the collection is "frozen")
//
// Passed:
// - handle:
// - increase: >0 (actually  the increment is handle->expectedItemsNumber)
static int GreenCollectionResize(GREEN_COLLECTION_HANDLE handle, int increase) {
  m_DIGGY_BOLLARD_S()
  m_RAISE_VERBATIM_IF(increase <= 0)
  m_RAISE_VERBATIM_IF(handle->b_frozen)

  increase = ((increase-1)/handle->expectedItemsNumber + 1) * handle->expectedItemsNumber;

  int newItemsPhysicalNumber = handle->itemsPhysicalNumber + increase;

  m_REALLOC(handle->h_greenArray, handle->greenItemSize*newItemsPhysicalNumber)
  memset(handle->h_greenArray + handle->greenItemSize * handle->itemsPhysicalNumber, 0,
    handle->greenItemSize * increase);

  m_GAPS_STACK_RESIZE(handle->gaps, newItemsPhysicalNumber)
  m_REALLOC_ARRAY(handle->hsc_flags, newItemsPhysicalNumber)
  m_TRACK_IF(GreenIndexesResize(&handle->indexes,newItemsPhysicalNumber) != 0)

  handle->itemsPhysicalNumber = newItemsPhysicalNumber;
  m_DIGGY_RETURN(increase)
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

  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  // or            [NADA]

  if (handle->nv_fetched4ChangeEntry < 0) {
    // MICROMONITOR: [NADA]
    m_DIGGY_RETURN(RETURNED)
  } // if

  m_RAISE_VERBATIM_IF(handle->b_frozen)

  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  m_ASSERT(b_ALL_FLAGS_OK(handle->hsc_flags[handle->nv_fetched4ChangeEntry],ALIEN_ALIVE__FLAGS))
m_DIGGY_INFO("handle->nv_fetched4ChangeEntry=%d Before m_GREEN_INDEXES_ADD()...",handle->nv_fetched4ChangeEntry)
  m_GREEN_INDEXES_ADD(handle->indexes,handle->nv_fetched4ChangeEntry)
  m_SET_FLAG_OFF(handle->hsc_flags[handle->nv_fetched4ChangeEntry],ALIEN_FLAG)
  handle->nv_fetched4ChangeEntry = -1;
  // MICROMONITOR: ... FAMED / ALIVE --| => [NADA]

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

  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

  *at_greenArray  = handle->h_greenArray;

  m_DIGGY_RETURN(handle->itemsPhysicalNumber)
} // GreenCollectionPullOut


// Public function; see description in .h
int GreenCollectionFreeze(GREEN_COLLECTION_HANDLE handle,  char **nap_greenArray) {
  m_DIGGY_BOLLARD()
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

  handle->b_frozen = b_TRUE;

  if (nap_greenArray != NULL) {
    *nap_greenArray  = handle->h_greenArray;
  } // if

  m_DIGGY_RETURN(handle->itemsPhysicalNumber)
} // GreenCollectionFreeze


enum {
  FETCH_4__READ,
  FETCH_4__CHANGE,
  FETCH_4__REMOVE
} ;


// Obtain or retrieve emplacement for a green set (in the collection's array).
//
// Passed:
// - cp_handle: green set array's handle ; protected instance if the collection is frozen
// - n_entry: entry in the list
//   + -1 special value: (smart fetch) find emplacement in the array : if gap(s) exist(s), re-use a
//     gap, otherwise, reserve a new emplacement at the end of the array....
//     (not allowed if the collection is frozen)
//   + >= 0: entry for the <set> in the list (0 == 1st entry) (direct fetch) ; if that position
//     either exceeds that reflected by formal <set>s count, or designate a gap, then the entry is
//     NOT fetched
// - c_fetch4: only significant with "direct fetch" (n_entry >= 0) : 
//   note1: "smart fetch" is tacitly a "fetch 4 change" 
//   note2: only "fetch 4 read only" allowed if the collection is frozen 
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
static int GreenCollectionFetchInternal (GREEN_COLLECTION_HANDLE cp_handle, int n_entry, int c_fetch4,
  char **acntr_greenItemStuff) {
  m_DIGGY_BOLLARD_S()
  m_RAISE_VERBATIM_IF(cp_handle->nv_fetched4ChangeEntry >= 0)
  // MICROMONITOR: [NADA]
  if (n_entry == -1) { // Smart fetch
    m_RAISE_VERBATIM_IF(cp_handle->b_frozen) 
    if ((n_entry = n_GAPS_STACK_GET_FIRST(cp_handle->gaps)) < 0) { // No gap
      n_entry = cp_handle->i_itemsCount ;
      // Ensure physical arrays are large enough vis-a-vis fetched entry
      if (n_entry >= cp_handle->itemsPhysicalNumber) {
        m_RAISE_VERBATIM_IF(n_entry > cp_handle->itemsPhysicalNumber)
        m_TRACK_IF(GreenCollectionResize(cp_handle,1) < 0)
        m_RAISE_VERBATIM_IF(n_entry >= cp_handle->itemsPhysicalNumber)
      } // if
      // New item
      m_SET_ALL_FLAGS(cp_handle->hsc_flags[n_entry],ALIEN_ALIVE__FLAGS)
      if (++(cp_handle->i_itemsCount) > cp_handle->v_maxItemsCount) cp_handle->v_maxItemsCount = 
        cp_handle->i_itemsCount;
      // MICROMONITOR: |-- ALIEN / ALIVE ...
    } else { // Use existing gap 
      m_RAISE_VERBATIM_IF(n_entry >= cp_handle->i_itemsCount)
      m_ASSERT(b_ALL_FLAGS_OK(cp_handle->hsc_flags[n_entry],ALIEN_DEAD__FLAGS))
      // MICROMONITOR: |-- ALIEN / DEAD ...
      m_GAPS_STACK_POP(cp_handle->gaps,n_entry)
      m_SET_FLAG_OFF(cp_handle->hsc_flags[n_entry],DEAD_FLAG)
      // MICROMONITOR: ... ALIEN / ALIVE ...
    } // if
    cp_handle->nv_fetched4ChangeEntry = n_entry;
    // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
    *acntr_greenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(cp_handle,n_entry);

  } else { // Direct fetch
    if (n_entry < cp_handle->i_itemsCount &&  // Existing item
      !b_FLAG_SET_ON(cp_handle->hsc_flags[n_entry],DEAD_FLAG)) { // It's NOT a gap
      m_ASSERT(b_ALL_FLAGS_OK(cp_handle->hsc_flags[n_entry],FAMED_ALIVE__FLAGS))
      // MICROMONITOR: |-- FAMED / ALIVE ...
      if (c_fetch4 != FETCH_4__READ) { 
        m_RAISE_VERBATIM_IF(cp_handle->b_frozen) 
        m_GREEN_INDEXES_REMOVE(cp_handle->indexes,n_entry)
        m_SET_FLAG_ON(cp_handle->hsc_flags[n_entry],ALIEN_FLAG)
        // MICROMONITOR: ... ALIEN / ALIVE ...
        if (c_fetch4 == FETCH_4__CHANGE) { 
          cp_handle->nv_fetched4ChangeEntry = n_entry;
          // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
        } else { // FETCH_4__REMOVE
          // MICROMONITOR: ... ALIEN / ALIVE ...
          m_GAPS_STACK_PUSH(cp_handle->gaps, n_entry, cp_handle->itemsPhysicalNumber)
          m_SET_FLAG_ON(cp_handle->hsc_flags[n_entry],DEAD_FLAG)
          // MICROMONITOR: ... ALIEN / DEAD --| => [NADA]
        } // if
      // } else { // FETCH_4__READ 
      // MICROMONITOR: ... FAMED / ALIVE --| => [NADA] 
      } // if
      *acntr_greenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(cp_handle,n_entry);
    } else { // Not fetchable 
      *acntr_greenItemStuff = NULL; 
      // MICROMONITOR: [NADA]
    } // if
  } // if

  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_DIGGY_RETURN(n_entry)
} // GreenCollectionFetchInternal


// Public function; see description in .h
int GreenCollectionGetCount (GREEN_COLLECTION_HANDLE cp_handle, char **navntr_greenItemStuff ) {
  m_DIGGY_BOLLARD()
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

  if (navntr_greenItemStuff != NULL) {
    if (cp_handle->i_itemsCount > 0) {

      int ret = GreenCollectionFetchInternal(cp_handle, cp_handle->i_itemsCount - 1,
        cp_handle->b_frozen? FETCH_4__READ: FETCH_4__CHANGE,  navntr_greenItemStuff); 
      m_TRACK_IF(ret < 0) 
      m_RAISE_VERBATIM_IF(ret != cp_handle->i_itemsCount - 1)
      m_RAISE_VERBATIM_IF(cp_handle->nv_fetched4ChangeEntry != -1 && cp_handle->nv_fetched4ChangeEntry
        != ret) 
      // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
      //               or [NADA] (if the collection is "frozen" or item is not fetchable)
    } else *navntr_greenItemStuff = NULL;
  } // if

  // TODO:
  // if (na_actualItemsCount != NULL) { 
  // TODO: macro m_GAPS_GET_COUNT()
  // *na_actualItemsCount =  cp_handle->i_itemsCount - cp_handle->gaps.count
  m_DIGGY_RETURN(cp_handle->i_itemsCount)
} // GreenCollectionGetCount


// Public function; see description in .h
int GreenCollectionFetch (GREEN_COLLECTION_HANDLE cp_handle, int n_entry,
  char **acntr_greenItemStuff) {
  m_DIGGY_BOLLARD()
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

  int entry = GreenCollectionFetchInternal(cp_handle,n_entry,
    cp_handle->b_frozen? FETCH_4__READ: FETCH_4__CHANGE, acntr_greenItemStuff);
  m_TRACK_IF(entry < 0)
  m_RAISE_VERBATIM_IF(cp_handle->nv_fetched4ChangeEntry != -1 && cp_handle->nv_fetched4ChangeEntry != 
    entry)
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA] (if the collection is "frozen" or item is not fetchable)

  m_DIGGY_RETURN(entry)
} // GreenCollectionFetch


// Public function; see description in .h
int GreenCollectionClear (GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD()
  m_RAISE_VERBATIM_IF(handle->b_frozen)
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]

  handle->i_itemsCount = 0 ;
  handle->nv_fetched4ChangeEntry = -1 ;
  m_GAPS_STACK_CLEAR(handle->gaps)
  m_TRACK_IF(GreenIndexesClear(&handle->indexes) != RETURNED)

  // MICROMONITOR: [NADA]
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionClear


// Public function; see description in .h
int GreenCollectionAddIndex (GREEN_COLLECTION_HANDLE handle, int keysNumber) {
  m_DIGGY_BOLLARD()
  m_RAISE_VERBATIM_IF(handle->b_frozen)
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

  // TODO: permettre d'ajouter des indexes ag chaud...
  m_RAISE_VERBATIM_IF(handle->i_itemsCount > 0)

  m_RAISE_VERBATIM_IF(keysNumber == 0)

  int newIndexLabel = GreenIndexesAddIndex(&handle->indexes, handle->itemsPhysicalNumber,
    keysNumber);
  m_TRACK_IF(newIndexLabel < 0)

  m_DIGGY_RETURN(newIndexLabel)
} // GreenCollectionAddIndex


// Public function; see description in .h
int GreenCollectionIndexFetch (GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_ITERATOR_AUTOMATIC_BUFFER nf_indexIteratorAutomaticBuffer,  int indexLabel,
  int indexFetch,  int c_indexSeek,  char **acvntr_greenItemStuff,  int *nacvn_entry,
  void *ccr_keys) {
  m_DIGGY_BOLLARD()
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]
  m_RAISE_VERBATIM_IF(nf_indexIteratorAutomaticBuffer == NULL && cp_handle->b_frozen) 

  int n_entry = UNDEFINED;
  m_TRACK_IF(GreenIndexesSeek(&cp_handle->indexes, nf_indexIteratorAutomaticBuffer, indexLabel,
    indexFetch == INDEX_FETCH__READ_NEXT? -1: c_indexSeek, ccr_keys, &n_entry) != RETURNED)
  int result = RESULT__FOUND;
  if (n_entry == -1) result = RESULT__NOT_FOUND; 

  // MICROMONITOR: [NADA]
  int n_fetch4 = -1 ; // No fetch a priori
  switch (indexFetch) {
  case INDEX_FETCH__READ_ONLY:
  case INDEX_FETCH__READ_NEXT:
    if (n_entry >= 0) n_fetch4 = FETCH_4__READ;
  break; case INDEX_FETCH__SEEK_ONLY:
    if (n_entry >= 0) n_fetch4 = (cp_handle->b_frozen? FETCH_4__READ: FETCH_4__CHANGE);
  break; case INDEX_FETCH__REMOVE:
    m_RAISE_VERBATIM_IF(cp_handle->b_frozen)
    if (n_entry >= 0) n_fetch4 = FETCH_4__REMOVE; 
  break; case INDEX_FETCH__FETCH:
    m_RAISE_VERBATIM_IF(cp_handle->b_frozen)
    n_fetch4 = FETCH_4__CHANGE; // Note: fetch 4 change is implicit with "smart fetch" but n_fetch4
                                // var MUST be explicitly set here even if n_entry == -1 because
                                // nullability is used in the flow control below (X)   
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,indexFetch)
  } // switch
  
  int n_fetchedEntry = -1; // a priori
  if (n_fetch4 != -1) { // See (X) above
    // MICROMONITOR: [NADA]
    n_fetchedEntry = GreenCollectionFetchInternal(cp_handle,n_entry,n_fetch4,acvntr_greenItemStuff);
    m_TRACK_IF(n_fetchedEntry < 0)
    m_RAISE_VERBATIM_IF(*acvntr_greenItemStuff == NULL)
    // See (X) above :
    m_RAISE_VERBATIM_IF(n_fetch4 == FETCH_4__CHANGE && cp_handle->nv_fetched4ChangeEntry !=
      n_fetchedEntry)
    // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
    //               or [NADA] (if the collection is "frozen", etc.)
  } else *acvntr_greenItemStuff = NULL; 
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]

  if (nacvn_entry != NULL) *nacvn_entry = n_fetchedEntry;

  m_DIGGY_RETURN(result)
} // GreenCollectionIndexFetch


// Public function; see description in .h
int GreenCollectionVerifyIndexes (GREEN_COLLECTION_HANDLE handle) {
  m_DIGGY_BOLLARD()
  // MICROMONITOR: ... ALIEN / ALIVE (fetched 4 change) ...
  //               or [NADA]
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MICROMONITOR: [NADA]

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

  int gapsCount = GapsStackVerifyCount(&handle->gaps);
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
      m_TRACK_IF((completed = GapsStackVerifyEntry(&handle->gaps,i,expectedHits)) < 0)
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
    for (; i < xh_handle->v_maxItemsCount ;
         i++, r_greenItemStuff += xh_handle->greenItemSize) {
      m_TRACK_IF(xh_handle->n_greenHandlerDisengageFunction(
        xh_handle->cr_greenHandlerHandle, r_greenItemStuff) != RETURNED)
    } // for
  } // if
  free(xh_handle->h_greenArray) ;

  m_GREEN_INDEXES_FREE(xh_handle->indexes)

  m_GAPS_STACK_FREE(xh_handle->gaps)

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionDestroyInstance
