// c-ansi/green-index.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/green-index.h"
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
// "GREEN INDEXES" ("proto" object)

char b_diggyGreenCollectionExam = b_TRUE;

/////////// 1. GREEN INDEX ("proto" object......) ////////////////////

// Passed:
// - az_index:
// - itemsPhysicalNumber:
// - m_entryCompareFunction:
// - mr_entryFunctionsHandle:
// - indexLabel:
#define m_GREEN_INDEX_INIT(/*struct GREEN_INDEX* */az_index,  /*int*/itemsPhysicalNumber, \
  /*ENTRY_COMPARE_FUNCTION*/m_entryCompareFunction, /*void* */mr_entryFunctionsHandle,\
  /*int*/m_indexLabel) {\
  m_MALLOC_ARRAY((az_index)->hsc_array,itemsPhysicalNumber)\
  (az_index)->count = 0;\
  (az_index)->entryCompareFunction = m_entryCompareFunction;\
  (az_index)->r_entryFunctionsHandle = mr_entryFunctionsHandle;\
  (az_index)->indexLabel = m_indexLabel;\
}


// Compare two items of the index
//
// Passed:
// - a_index:
// - aIndexEntry: entry in index for "A"
// - n_bEntry: >= 0 => entry in index for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
//
// Changed:
// - *m_comparison: "A - B" comparison between key of index' entry and passed key
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
#define m_GREEN_INDEX_COMPARE(/*struct GREEN_INDEX* */a_index, /*int*/aIndexEntry, /*int*/n_bEntry,\
  /*const void* */cpr_bKeys, /*int*/m_comparison) {\
  int em_aEntry = a_index->hsc_array[aIndexEntry];\
    m_TRACK_IF((m_comparison = (a_index)->entryCompareFunction((a_index)->r_entryFunctionsHandle,\
      (a_index)->indexLabel, em_aEntry, n_bEntry,cpr_bKeys)) < 0)\
  if (m_comparison == EQUAL_TO__COMPARISON && n_bEntry != -1)  m_comparison = \
    DIFFERENCE_2_COMPARISON(em_aEntry - n_bEntry) ;\
}

// See .h
int GreenIndexBSearch(struct GREEN_INDEX *a_index,
  int n_bEntry, const void *cpr_bKeys, int *an_indexEntry, int *a_top,
  struct INDEX_ENTRIES *cac_indexEntries) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;
  int i = UNDEFINED;

  int bottom = -1, // Best index entry "before"
    top = a_index->count; // Best index entry "after"

  *an_indexEntry = -1;
  while (top - bottom > 1) {
    i =  (top + bottom) >> 1 ;

    m_GREEN_INDEX_COMPARE(a_index, i,n_bEntry,cpr_bKeys, comparison)
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
      m_GREEN_INDEX_COMPARE(a_index, i,-1,cpr_bKeys, comparison)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == LESS_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    cac_indexEntries->first = i+1;
    for (i = *an_indexEntry+1 ; i < a_index->count ; i++) {
      m_GREEN_INDEX_COMPARE(a_index, i,-1,cpr_bKeys, comparison)
      if (comparison != EQUAL_TO__COMPARISON) {
        m_ASSERT(comparison == GREATER_THAN__COMPARISON) 
        break; 
      } // if
    } // for
    cac_indexEntries->last = i-1;
  } //if     
  m_DIGGY_RETURN(RETURNED)
} // GreenIndexBSearch

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

  if (a_indexSequence->indexEntriesNumber2 > 0 && a_indexSequence->ci_indexEntryCursor >= 
    a_indexSequence->indexEntries2[0].first && a_indexSequence->ci_indexEntryCursor <=
    a_indexSequence->cv_lastIndexEntry)
    *an_entry = a_index->hsc_array[a_indexSequence->ci_indexEntryCursor];
  else *an_entry = -1; // No current item 

  return RETURNED;  
} // m_GreenIndexCurrent 


// Update index sequence.
//
// Passed:
// - a_index:
// - b_descending: False:Ascending ; True:Descending 
// - c_indexSeekFlags: seek flags for sequence request ; only significant with "new sequence" case;
//   + INDEX_SEEK_FLAGS__ANY : NOT key-based selection 
//   + other flags : key-based selection
// - ccpr_keys: search key(s) ; only significant with new sequence's key-based selection 
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
static int GreenIndexSeek(struct GREEN_INDEX* a_index, char b_descending,
  unsigned int c_indexSeekFlags, const void *ccpr_keys, struct INDEX_SEQUENCE *a_indexSequence,
  int *nan_entry) {
  m_DIGGY_BOLLARD_S()

  if (nan_entry != NULL) { // "next item" case: seek next in existing sequence
m_DIGGY_VAR_D(a_indexSequence->indexEntriesNumber2)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].last)
    *nan_entry = -1; // "disabled" / "No more" a priori
    if (a_indexSequence->indexEntriesNumber2 > 0) { // "enabled"
      if (b_descending) {
        if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntries2[0].first) { 
          // NOT YET "no more"...
          if (a_indexSequence->ci_indexEntryCursor > a_indexSequence->cv_lastIndexEntry) {
            // "soft reset"
            a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntriesPtr =
              a_indexSequence->indexEntries2 + a_indexSequence->indexEntriesNumber2 - 1)->last;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } else if (--(a_indexSequence->ci_indexEntryCursor) <
            a_indexSequence->cv_indexEntriesPtr->first && a_indexSequence->cv_indexEntriesPtr >
            a_indexSequence->indexEntries2) {
            // Pass to next block:
            a_indexSequence->ci_indexEntryCursor = (--(a_indexSequence->cv_indexEntriesPtr))->last;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } // if
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntries2[0].first) 
            // STILL NOT YET "no more"
            *nan_entry = a_index->hsc_array[a_indexSequence->ci_indexEntryCursor];
        } // if
      } else { // Ascending...
        if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) { 
          // NOT YET "no more"...
          if (a_indexSequence->ci_indexEntryCursor < a_indexSequence->indexEntries2[0].first) {
            // "soft reset"
            a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntriesPtr =
              a_indexSequence->indexEntries2)->first;
m_DIGGY_VAR_D(a_indexSequence->ci_indexEntryCursor)
          } else if (++(a_indexSequence->ci_indexEntryCursor) >
            a_indexSequence->cv_indexEntriesPtr->last && a_indexSequence->cv_indexEntriesPtr <
            a_indexSequence->indexEntries2 + a_indexSequence->indexEntriesNumber2 - 1) {
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
    a_indexSequence->indexEntriesNumber2 = 0; // Empty ("disabled") selection a priori
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
        m_TRACK_IF(GreenIndexBSearch(a_index, -1,ccpr_keys, &n_indexEntry,&top,
          &c_indexEntries) != RETURNED);
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
          m_RAISE(ANOMALY__VALUE__D,c_indexSeekFlags)
        } // switch
      } // if 

m_DIGGY_VAR_D(n_firstIndexEntry)
m_DIGGY_VAR_D(n_lastIndexEntry)

      if (n_firstIndexEntry >= 0 && n_lastIndexEntry >= 0 && n_firstIndexEntry > n_lastIndexEntry) {
        a_indexSequence->indexEntriesNumber2 = 2; 
        a_indexSequence->indexEntries2[0].first = 0;
        a_indexSequence->indexEntries2[0].last = n_lastIndexEntry;
        a_indexSequence->indexEntries2[1].first = n_firstIndexEntry;
        a_indexSequence->cv_lastIndexEntry = a_indexSequence->indexEntries2[1].last =
          a_index->count-1;
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].last)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[1].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[1].last)
      } else if (n_firstIndexEntry >= 0 || n_lastIndexEntry >= 0) {
        a_indexSequence->indexEntriesNumber2 = 1; 
        if (n_firstIndexEntry < 0) n_firstIndexEntry = 0;
        if (n_lastIndexEntry < 0) n_lastIndexEntry = a_index->count-1;
        a_indexSequence->indexEntries2[0].first = n_firstIndexEntry;
        a_indexSequence->cv_lastIndexEntry =
          a_indexSequence->indexEntries2[0].last = n_lastIndexEntry;
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].first)
m_DIGGY_VAR_D(a_indexSequence->indexEntries2[0].last)
      } // if
m_DIGGY_VAR_D(a_indexSequence->indexEntriesNumber2)
      // Prime the pump: ("soft reset")
      a_indexSequence->ci_indexEntryCursor = (b_descending? a_indexSequence->cv_lastIndexEntry
        + 1: a_indexSequence->indexEntries2[0].first - 1);
      a_indexSequence->cv_indexEntriesPtr = a_indexSequence->indexEntries2;
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexSeek


// Passed:
// -a_index:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: index is corrupted
// - -1: anomaly is raised
static int GreenIndexVerify(struct GREEN_INDEX *a_index) {
  m_DIGGY_BOLLARD_S()
  int comparison = UNDEFINED;

#ifdef DIGGY
  if (b_diggyGreenCollectionExam) {
    m_DIGGY_INFO("Exam: a_index->count=%d (indexLabel=%d)",a_index->count,a_index->indexLabel)
    int i = 0 ; for (; i < a_index->count; i++) {
      m_DIGGY_INFO("  a_index->hsc_array[%d]=%d",i,a_index->hsc_array[i])
    } // for
  } // if 
#endif

  int j = 1; for (; j < a_index->count; j++) {
    m_GREEN_INDEX_COMPARE(a_index, j, a_index->hsc_array[j-1],
      (void *)(GENERIC_INTEGER) UNDEFINED, comparison)
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
  int *s_indexArrayPtr = a_index->hsc_array;
  int i = 0; for (; i < a_index->count ; i++, s_indexArrayPtr++) {
    if (*s_indexArrayPtr == entry) hits++ ;
  } // for

  if (hits != expectedHits) completed = COMPLETED__BUT;

  m_DIGGY_RETURN(completed)
} // GreenIndexVerifyEntry 

///////////// 2. GREEN INDEXES "proto" object //////////////

// ENTRY_COMPARE_FUNCTION
int GreenIndexesEntryCompare (void *r_handle, int indexLabel, int aEntry,
  int n_bEntry, const void *cpr_bKeys) {
  m_DIGGY_BOLLARD_S()
  struct GREEN_INDEXES *a_indexes = (struct GREEN_INDEXES *)r_handle;  
  int comparison = UNDEFINED;
  m_ASSERT(indexLabel < a_indexes->indexesNumber) 
  int j = 0; for ( ; j < a_indexes->vnhs_keysNumbers[indexLabel] ; j++) {
    m_TRACK_IF((comparison = a_indexes->entryRawCompareFunction(
      a_indexes->r_entryRawFunctionsHandle, indexLabel, j, aEntry, n_bEntry,cpr_bKeys)) < 0)
    if (comparison != EQUAL_TO__COMPARISON) break ;
  } // for
  m_DIGGY_RETURN(comparison)
} // GreenIndexesEntryCompare

// Adequate an item with a key.
//
// Passed:
// - a_indexes:
// - indexLabel:
// - aEntry: entry for "A"
// - pr_bKeys: raw key(s) for "B" 
//
// Ret: adequation between "A" (entry) and "B" (key) ?
// - ANSWER__YES: 
// - ANSWER__NO: 
// - -1: unexpected problem; anomaly is raised
int GreenIndexesEntryEquate(struct GREEN_INDEXES *a_indexes, int indexLabel, int aEntry,
  const void *pr_bKeys) {
  int answer = UNDEFINED;
  m_ASSERT(indexLabel < a_indexes->indexesNumber) 
  int j = 0; for ( ; j < a_indexes->vnhs_keysNumbers[indexLabel] ; j++) {
    m_TRACK_IF((answer = a_indexes->entryRawEquateFunction(a_indexes->r_entryRawFunctionsHandle,
      indexLabel, j, aEntry, pr_bKeys)) < 0)
    if (answer != ANSWER__YES) break ;
  } // for
  
  return answer;
} // GreenIndexesEntryEquate


// Passed:
// - a_indexes:
// - itemsPhysicalNumber:
// - keysNumber:
//
// Return:
// new index label (technically equal to entry in indexes array)
int GreenIndexesAddIndex(struct GREEN_INDEXES *a_indexes, int itemsPhysicalNumber,
  int keysNumber) {
  m_DIGGY_BOLLARD_S()
  m_REALLOC_ARRAY(a_indexes->vnhs_indexes,a_indexes->indexesNumber+1)
  m_REALLOC_ARRAY(a_indexes->vnhs_keysNumbers,a_indexes->indexesNumber+1)
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes + a_indexes->indexesNumber ;

  a_indexes->vnhs_keysNumbers[a_indexes->indexesNumber] = keysNumber;
  m_GREEN_INDEX_INIT(a_index, itemsPhysicalNumber, GreenIndexesEntryCompare, a_indexes,
    a_indexes->indexesNumber)
  m_DIGGY_RETURN(a_indexes->indexesNumber++)
} // GreenIndexesAddIndex


// Passed:
// - a_indexes:
// - newItemsPhysicalNumber:
int GreenIndexesResize (struct GREEN_INDEXES *a_indexes, int newItemsPhysicalNumber) {
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
    free((mx_indexes).vnhs_keysNumbers);\
  }\
}

// Remove reference on item in all indexes (no action if not referenced in some index)
//
// Passed:
// - m_indexes:
// - entry:
#define m_GREEN_INDEXES_REMOVE(/*struct GREEN_INDEXES*/m_indexes, /*int*/entry) {\
  int emn_indexEntry = UNDEFINED, em_top = UNDEFINED;\
  int em_i = 0;\
  struct GREEN_INDEX *s_index = (m_indexes).vnhs_indexes;\
  for ( ; em_i < (m_indexes).indexesNumber ; em_i ++, s_index++) {\
    m_TRACK_IF(GreenIndexBSearch(s_index, entry,(void *)UNDEFINED,&emn_indexEntry,&em_top,\
      (struct INDEX_ENTRIES*)UNDEFINED) != RETURNED)\
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
    m_TRACK_IF(GreenIndexBSearch(s_index, entry, (void*)UNDEFINED, &emn_indexEntry, &top,\
      (struct INDEX_ENTRIES*)UNDEFINED) != RETURNED)\
    if (emn_indexEntry == -1) {\
      m_GREEN_INDEX_ADD(s_index,top,entry)\
    }\
  }\
}

// Perform "equation" of entry with a key. 
//
// Passed:
// - a_indexes:
// - indexLabel: 
// - aEntry: "A" entry 
// - indexSeekFlags: 
// - pr_bKeys: "B" keys(s) 
//
// Returned:
// - >=0: "equation" between :"A" and "B" with that key component... 
//   + ANSWER__YES : "A" item and "B" key(s) are similar 
//   + ANSWER__NO : "A" item and "B" key(s) are NOT similar 
// - -1: unexpected problem; anomaly is raised
int GreenIndexesSeekEntryEquate(struct GREEN_INDEXES* a_indexes, int indexLabel, int aEntry,
  unsigned int indexSeekFlags, const void *pr_bKeys) {
  m_DIGGY_BOLLARD_S()
  if (indexSeekFlags == INDEX_SEEK_FLAGS__ANY) return ANSWER__YES;

  int answer = ANSWER__NO; // a priori
  if (indexSeekFlags != INDEX_SEEK_FLAGS__LIKE) {
    int comparison = GreenIndexesEntryCompare(a_indexes, indexLabel, aEntry, -1, pr_bKeys);
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
    m_TRACK_IF((answer = GreenIndexesEntryEquate(a_indexes,indexLabel, aEntry, pr_bKeys)) < 0)
  } // if 
  m_DIGGY_RETURN(answer)
} // GreenIndexesSeekEntryEquate

#define CRITERIA_STACK_SIZE 10

// Possible statuses : 'U': Unkown 'C': Canceled 'O': Ok 'K': KO
#define m_CRITERIA_HANDLER_CREATE() \
  unsigned int knownCriteriaOpFlags[CRITERIA_STACK_SIZE] ;\
  char statuses[CRITERIA_STACK_SIZE]; 
  
// Re-init criteria handler.
// 1st criterion op. flags are rectified if needed:
// - AND op. is set
// - OR op. is removed
//
// Passed:
// - a_indexIterator:
#define m_CRITERIA_HANDLER_RESET(/*struct INDEX_ITERATOR* */a_indexIterator) \
  int i_stackEntry = 0;\
  unsigned int *v_knownCriteriaOpFlagsPtr = knownCriteriaOpFlags;\
  char *v_statusPtr = statuses;\
  statuses[0] = 'U' ; \
  knownCriteriaOpFlags[0] = a_indexIterator->criteria[0].criteriaOpFlags;\
  m_SET_FLAG_ON(knownCriteriaOpFlags[0], CRITERIA_OP_FLAG__AND)\
  m_SET_FLAG_OFF(knownCriteriaOpFlags[0], CRITERIA_OP_FLAG__OR)\

// Handle equation and then closing brackets for current criterion
// criterion op. flags are rectified if needed:
// - closing bracket added to ensure AND op. precedence (over OR op.)
// - missing closing brackets (at the end of the expression) are added 
// - superfluous closing brackets are ignored
//
// Passed:
// - a_indexes: indexes global handler
// - a_indexIterator: contains criteria criteria
// - m_i: criterion entry 
#define m_CRITERIA_HANDLER_EQUATION_AND_CLOSE_BRACKETS(/*struct GREEN_INDEXES* */a_indexes,\
  /*struct INDEX_ITERATOR* */a_indexIterator, /*int*/m_i) \
if (m_i == 0) {\
  if (a_indexIterator->criteriaNumber5 == 1) statuses[0] = 'O' ; \
} else { \
  int em_cn = ((m_i)+1 == a_indexIterator->criteriaNumber5)? i_stackEntry: \
    m_CloseBracketsNumber(a_indexIterator->criteria[m_i].criteriaOpFlags);\
  if (em_cn == 0) {\
    if (*v_knownCriteriaOpFlagsPtr == ALL_FLAGS_OFF0) *v_knownCriteriaOpFlagsPtr = \
      a_indexIterator->criteria[m_i].criteriaOpFlags;\
    if ((m_i)+1 < a_indexIterator->criteriaNumber5 && b_FLAG_SET_ON(\
      *v_knownCriteriaOpFlagsPtr,CRITERIA_OP_FLAG__OR) && b_FLAG_SET_ON(\
      a_indexIterator->criteria[m_i+1].criteriaOpFlags, CRITERIA_OP_FLAG__AND)) em_cn++;\
  } else if (em_cn > i_stackEntry) em_cn = i_stackEntry; \
  if (*v_statusPtr == 'U') {\
    int answer = GreenIndexesSeekEntryEquate(a_indexes,\
      a_indexIterator->criteria[m_i].indexLabel,*nan_entry,\
      a_indexIterator->criteria[m_i].indexSeekFlags,\
      a_indexIterator->criteria[m_i].cfpr_keys);\
    switch (answer) {\
    case ANSWER__YES:\
      if (b_FLAG_SET_OFF(*v_knownCriteriaOpFlagsPtr,CRITERIA_OP_FLAG__AND) || \
        em_cn > 0 || (m_i)+1 == a_indexIterator->criteriaNumber5) *v_statusPtr = 'O';\
    break; case ANSWER__NO: \
      if (b_FLAG_SET_OFF(*v_knownCriteriaOpFlagsPtr,CRITERIA_OP_FLAG__OR) || \
        em_cn > 0 || (m_i)+1 == a_indexIterator->criteriaNumber5) *v_statusPtr = 'K'; \
    break; default:\
      m_TRACK()\
    }\
  }\
  int em_j = 0; for (; em_j < em_cn; em_j++) {\
    m_ASSERT(--i_stackEntry >= 0)\
    v_knownCriteriaOpFlagsPtr--; \
    v_statusPtr--; \
    if (em_j == em_cn-1 && *v_knownCriteriaOpFlagsPtr == ALL_FLAGS_OFF0) \
      *v_knownCriteriaOpFlagsPtr = a_indexIterator->criteria[m_i].criteriaOpFlags;\
    if (*v_statusPtr == 'U') {\
      if (b_FLAG_SET_OFF(*v_knownCriteriaOpFlagsPtr,CRITERIA_OP_FLAG__OR)) {\
        *v_statusPtr = 'O';\
      } else { \
        *v_statusPtr = 'K';\
      } \
    } \
    if (i_stackEntry > 0 && statuses[i_stackEntry+1] != 'C') {\
      m_ASSERT(*v_statusPtr == 'U')\
      if (statuses[i_stackEntry+1] == 'O' && b_FLAG_SET_OFF(*v_knownCriteriaOpFlagsPtr,\
        CRITERIA_OP_FLAG__AND)) *v_statusPtr = 'O' ; \
      else if (*v_statusPtr == 'K' && b_FLAG_SET_OFF(*v_knownCriteriaOpFlagsPtr,\
        CRITERIA_OP_FLAG__OR)) *v_statusPtr = 'K'; \
    } \
  } \
  if (*v_knownCriteriaOpFlagsPtr == ALL_FLAGS_OFF0) *v_knownCriteriaOpFlagsPtr =\
    a_indexIterator->criteria[m_i].criteriaOpFlags;\
} 

// Handle open brackets for current criterion
// Note: in this implementation, opening brackets are indeed handled AFTER handling of closing
// brackets...
// criterion op. flags are rectified if needed:
// - open bracket added to ensure AND op. precedence (over OR op.)
//
// Passed:
// - a_indexIterator: contains criteria criteria
// - m_i: criterion entry 
#define m_CRITERIA_HANDLER_OPEN_BRACKETS(/*struct INDEX_ITERATOR* */a_indexIterator, /*int*/m_i) {\
  int em_on = m_OpenBracketsNumber(a_indexIterator->criteria[m_i].criteriaOpFlags);\
  if (em_on == 0 && (m_i)+1 < a_indexIterator->criteriaNumber5 && b_FLAG_SET_ON(\
    *v_knownCriteriaOpFlagsPtr,CRITERIA_OP_FLAG__AND) && b_FLAG_SET_ON(\
    a_indexIterator->criteria[m_i+1].criteriaOpFlags, CRITERIA_OP_FLAG__OR)) em_on++;\
  int em_j = 0; for (; em_j < em_on; em_j++) {\
    m_ASSERT(++i_stackEntry < CRITERIA_STACK_SIZE);\
    v_knownCriteriaOpFlagsPtr++; \
    v_statusPtr++; \
    if (statuses[i_stackEntry-1] == 'U') *v_statusPtr = 'U' ;\
    else *v_statusPtr = 'C' ;\
    *v_knownCriteriaOpFlagsPtr = ALL_FLAGS_OFF0;\
  }\
}

// Break "criteria handling" loop if entry not rejected by extra criteria 
#define m_CRITERIA_HANDLER_CHECK_STATUS() \
  m_ASSERT(i_stackEntry == 0) \
  m_ASSERT(*v_statusPtr == 'O' || *v_statusPtr == 'K') \
  if (*v_statusPtr == 'O') break;

// Update index iterator sequence.
//
// Passed:
// - a_indexes: 
// - a_indexIterator-> : current index iterator
// - nan_entry: 
//   + == NULL: "new sequence" (aka "soft reset") case
//   + != NULL: "next item" case
//
// Changed:
// - a_indexIterator-> : new state of index iterator 
// - *nan_entry: (only significant whith "next item" case) item entry to retrieve : 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GreenIndexesSeek(struct GREEN_INDEXES* a_indexes, struct INDEX_ITERATOR *a_indexIterator,
  int *nan_entry) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexIterator->criteria[0].indexLabel < a_indexes->indexesNumber) 

  m_CRITERIA_HANDLER_CREATE()

  do {
    m_TRACK_IF(GreenIndexSeek(a_indexes->vnhs_indexes + a_indexIterator->criteria[0].indexLabel,
      a_indexIterator->b_descending, a_indexIterator->criteria[0].indexSeekFlags,
      a_indexIterator->criteria[0].cfpr_keys, &a_indexIterator->indexSequence, nan_entry) !=
      RETURNED) 
    if (nan_entry != NULL && *nan_entry >= 0) {
      m_CRITERIA_HANDLER_RESET(a_indexIterator)
      int i = 0; for (; i < a_indexIterator->criteriaNumber5; i++) {
        m_CRITERIA_HANDLER_EQUATION_AND_CLOSE_BRACKETS(a_indexes,a_indexIterator,i)
        m_CRITERIA_HANDLER_OPEN_BRACKETS(a_indexIterator,i)
      } // for
      m_CRITERIA_HANDLER_CHECK_STATUS()
    } else break; // No more entry => finished 
  } while (b_TRUE) ;

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
int GreenIndexesCurrent(struct GREEN_INDEXES* a_indexes,
  struct INDEX_ITERATOR *a_indexIterator, int *an_entry) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexIterator->criteria[0].indexLabel < a_indexes->indexesNumber) 
  struct GREEN_INDEX *a_index = a_indexes->vnhs_indexes + a_indexIterator->criteria[0].indexLabel;
  
  m_TRACK_IF(m_GreenIndexCurrent(a_index,&a_indexIterator->indexSequence,an_entry) != RETURNED) 

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesCurrent

// Ret:
// - ANSWER__YES: indexes are enabled (i.e at least one index has been added) 
// - ANSWER__NO: indexes are NOT enabled
// - -1: anomaly is raised
int GreenIndexesVerifyEnabled (struct GREEN_INDEXES *a_indexes) {
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
int GreenIndexesVerify (struct GREEN_INDEXES *a_indexes) {
  m_DIGGY_BOLLARD_S()
  int completed = COMPLETED__OK; // a priori
  int i = 0;
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;

  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
m_DIGGY_VAR_D(i)
    switch (GreenIndexVerify(s_index)) {
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
int GreenIndexesVerifyCount (struct GREEN_INDEXES *a_indexes,int *ac_commonCount) {
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
int GreenIndexesVerifyEntry (struct GREEN_INDEXES *a_indexes, int entry, int expectedHits) {
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
int GreenIndexesClear (struct GREEN_INDEXES *a_indexes) {
  m_DIGGY_BOLLARD_S()
  int i = 0 ;
  struct GREEN_INDEX *s_index = a_indexes->vnhs_indexes ;
  for ( ; i < a_indexes->indexesNumber ; i++, s_index++) {
    m_GREEN_INDEX_CLEAR(s_index)
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GreenIndexesClear
