// c-ansi/green-index.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

// #SEE double-inclusion-4-generic-imaged-enums @ flint/images.h <__C_ANSI_GREEN_INDEX_H_INCLUDED__>
#ifndef __C_ANSI_GREEN_INDEX_H_INCLUDED__
#define __C_ANSI_GREEN_INDEX_H_INCLUDED__ 0
#endif

#if __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0 || __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 2 
#if __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0

#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-ansi/c-stack.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


extern char b_diggyGreenCollectionExam;

/////////// 1. GREEN INDEX ("proto" object......) ////////////////////

// Index's virtual function to compare two items.
//
// Passed:
// - r_handle:
// - indexLabel:
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
typedef int (*ENTRY_COMPARE_FUNCTION) (void *r_handle, int indexLabel, int aEntry, int n_bEntry,
  const void *cpr_bKeys) ;

struct GREEN_INDEX {
  int *hsc_array; // - Physical size given by global instance's itemsPhysicalNumber
                  // - 'count' first items are significant
  int count; // >= 0
  ENTRY_COMPARE_FUNCTION entryCompareFunction;
  void* r_entryFunctionsHandle;
  int indexLabel;
};

// Index entries "block"
struct INDEX_ENTRIES {
  int first; // >= 0
  int last; // >= first
} ;

// Binary search of "B" in index
//
// Passed:
// - a_index: index to use
// - n_bEntry: >= 0 => entry in index for "B"
// - cpr_bKeys: raw key(s) for "B" when entry for "B" not provided
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
int GreenIndexBSearch(struct GREEN_INDEX *a_index,
  int n_bEntry, const void *cpr_bKeys, int *an_indexEntry, int *a_top,
  struct INDEX_ENTRIES *cac_indexEntries) ;


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


// * Truly index-based seek flags:
// NOT key-based seek flag: mutually exclusive with other truly index-based seek flags: 
#define INDEX_SEEK_FLAG__ANY     0x01
// Key-based seek flags: 
#define INDEX_SEEK_FLAG__EQUAL   0x02
#define INDEX_SEEK_FLAG__LESS    0x04
#define INDEX_SEEK_FLAG__GREATER 0x08 
// * NON-index based seek flag: mutually exclusive with truly index-based seek flags: 
#define INDEX_SEEK_FLAG__LIKE    0x10 
 
#endif // __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ __C_ANSI_GREEN_INDEX_H_INCLUDED__
#include "flint/images.h"
 
// #REF enum-INDEX_SEEK
m_DEFINE_ENUM_ALIAS_BEGIN(m_IndexSeekFlagsImage)
  // seek in collection regarding index
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__ANY           ,INDEX_SEEK_FLAG__ANY)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__EQUAL         ,INDEX_SEEK_FLAG__EQUAL)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__LESS          ,INDEX_SEEK_FLAG__LESS)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__LESS_EQUAL    ,INDEX_SEEK_FLAG__EQUAL | INDEX_SEEK_FLAG__LESS)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__GREATER       ,INDEX_SEEK_FLAG__GREATER)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__GREATER_EQUAL ,INDEX_SEEK_FLAG__EQUAL | INDEX_SEEK_FLAG__GREATER)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__NOT_EQUAL     ,INDEX_SEEK_FLAG__LESS | INDEX_SEEK_FLAG__GREATER)
  m_ENUM_ALIAS_VAL(INDEX_SEEK_FLAGS__LIKE          ,INDEX_SEEK_FLAG__LIKE)
m_DEFINE_ENUM_ALIAS_END()

#if __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0

// Disable index sequence (disambiguation purpose...)
#define m_DISABLE_INDEX_SEQUENCE(m_indexSequence)  (m_indexSequence).indexEntriesNumber2 = 0;

///////////// 2. GREEN INDEXES "proto" object //////////////

#define CRITERIA_OP_FLAG__CLOSE1      0x001 // Close one bracket before op.
#define CRITERIA_OP_FLAG__CLOSE2      0x002 // Close two brackets before op.
#define CRITERIA_OP_FLAG__CLOSE3      0x004 // Close three brackets before op.
#define CRITERIA_OP_FLAG__AND         0x010 // And op.
#define CRITERIA_OP_FLAG__OR          0x020 // Or op.
#define CRITERIA_OP_FLAG__OPEN1       0x100 // Open one bracket after op. 
#define CRITERIA_OP_FLAG__OPEN2       0x200 // Open two brackets after op. 
#define CRITERIA_OP_FLAG__OPEN3       0x400 // Open three brackets after op. 

// Passed:
// - criteriaOpFlags
static inline int m_OpenBracketsNumber(unsigned int criteriaOpFlags) {
  if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN3)) return 3;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN2)) return 2;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)) return 1;
  return 0;
} // m_OpenBracketsNumber 

// Passed:
// - criteriaOpFlags
static inline int m_CloseBracketsNumber(unsigned int criteriaOpFlags) {
  if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3)) return 3;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)) return 2;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)) return 1;
  return 0;
} // m_CloseBracketsNumber 


typedef int (*ENTRY_RAW_COMPARE_FUNCTION) (void *r_handle, int indexLabel, int keyRank,
  int aEntry, int n_bEntry, const void *cpr_bKeys) ;

// Index's virtual function to adequate an item with a key.
//
// Passed:
// - r_handle:
// - indexLabel:
// - keyRank:
// - aEntry: entry for "A"
// - pr_bKeys: raw key(s) for "B" 
//
// Ret: adequation between "A" (entry) and "B" (key) ?
// - ANSWER__YES: 
// - ANSWER__NO: 
// - -1: unexpected problem; anomaly is raised
typedef int (*ENTRY_RAW_EQUATE_FUNCTION) (void *r_handle, int indexLabel, int keyRank,
  int aEntry, const void *pr_bKeys);

struct GREEN_INDEXES {
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction;
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction;
  void *r_entryRawFunctionsHandle;
  int indexesNumber ; // when ENABLED ; >= 0
  int *vnhs_keysNumbers; // NULL when indexesNumber == 0
  struct GREEN_INDEX *vnhs_indexes ; // NULL when indexesNumber == 0
};

// ENTRY_COMPARE_FUNCTION
int GreenIndexesEntryCompare (void *r_handle, int indexLabel, int aEntry,
  int n_bEntry, const void *cpr_bKeys);


#define b_ASCENDING b_FALSE0
#define b_DESCENDING b_TRUE

// Passed:
// - m_indexIterator:
// - m_criterion1:
#define m_INIT_INDEX_ITERATOR(/*struct INDEX_ITERATOR*/m_indexIterator,\
  /*struct G_REQUEST_CRITERION */m_criterion1) {\
  (m_indexIterator).criteriaNumber5 = 1;\
  (m_indexIterator).criteria[0] = m_criterion1;\
  (m_indexIterator).b_descending = b_ASCENDING;\
  m_DISABLE_INDEX_SEQUENCE((m_indexIterator).indexSequence)\
}

// Passed:
// - m_indexIterator:
// - m_criterion:
#define m_INIT_INDEX_ITERATOR__EXTRAS(/*struct INDEX_ITERATOR*/m_indexIterator,\
  /*struct G_REQUEST_CRITERION */m_criterion) {\
  m_ASSERT((m_indexIterator).criteriaNumber5 < 5)\
  int em_i = (m_indexIterator).criteriaNumber5;\
  (m_indexIterator).criteria[em_i] = m_criterion;\
  (m_indexIterator).criteriaNumber5 ++;\
}

// Passed:
// - m_indexIterator:
// - mb_descending:
#define m_HARD_RESET_INDEX_ITERATOR(/*struct INDEX_ITERATOR*/m_indexIterator, mb_descending) {\
  (m_indexIterator).b_descending = mb_descending;\
  m_DISABLE_INDEX_SEQUENCE((m_indexIterator).indexSequence)\
}

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
  const void *pr_bKeys) ;


// Passed:
// - mz_indexes:
// - m_entryRawCompareFunction:
// - mr_entryFunctionsHandle:
#define m_GREEN_INDEXES_INIT(/*struct GREEN_INDEXES*/mz_indexes,\
  /*ENTRY_RAW_COMPARE_FUNCTION*/m_entryRawCompareFunction,\
  /*ENTRY_RAW_EQUATE_FUNCTION*/m_entryRawEquateFunction,\
  /*void */mr_entryRawFunctionsHandle) {\
  (mz_indexes).entryRawCompareFunction = m_entryRawCompareFunction;\
  (mz_indexes).entryRawEquateFunction = m_entryRawEquateFunction;\
  (mz_indexes).r_entryRawFunctionsHandle = mr_entryRawFunctionsHandle;\
  (mz_indexes).indexesNumber = 0;\
  (mz_indexes).vnhs_keysNumbers = NULL;\
  (mz_indexes).vnhs_indexes = NULL;\
}


// Passed:
// - a_indexes:
// - itemsPhysicalNumber:
// - keysNumber:
//
// Return:
// new index label (technically equal to entry in indexes array)
int GreenIndexesAddIndex(struct GREEN_INDEXES *a_indexes, int itemsPhysicalNumber,
  int keysNumber) ;


// Passed:
// - a_indexes:
// - newItemsPhysicalNumber:
int GreenIndexesResize (struct GREEN_INDEXES *a_indexes, int newItemsPhysicalNumber) ;


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
  unsigned int indexSeekFlags, const void *pr_bKeys) ;


// May or may be NOT index-based selection:
struct G_REQUEST_CRITERION {
  int indexLabel;
  unsigned int indexSeekFlags;
  const void *cfpr_keys; // Only significant with Key-based seek flag(s)
  unsigned int criteriaOpFlags;
} ;

//
struct INDEX_SEQUENCE {
  // index entries "blocks":
  int indexEntriesNumber2; // between 0 and 2 : 0 => 'disabled' ; >0 -> 'enabled' 
  struct INDEX_ENTRIES indexEntries2[2];
  // Fields below are only significant if 'enabled' (indexEntriesNumber2 > 0) :
  int cv_firstIndexEntry; // first index entry for ALL "blocks"  
  int cv_lastIndexEntry; // last index entry for ALL "blocks"  
  int ci_indexEntryCursor; // "current" index entry: < cv_firstIndexEntry => Ascending:"soft reset",
    // DEscending:'no more' ; between [indexEntries[0].first - cv_lastIndexEntry] => 'in sequence' ;
    // > cv_lastIndexEntry => Ascending:"no more", DEscending:"soft reset" 
  const struct INDEX_ENTRIES *cv_indexEntriesPtr; // "block" corresponding to "current" index entry 
} ;

struct INDEX_ITERATOR {
  int criteriaNumber5 ;
  struct G_REQUEST_CRITERION criteria[5] ;
  char b_descending;
  struct INDEX_SEQUENCE indexSequence;
};

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
  int *nan_entry) ;

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
  struct INDEX_ITERATOR *a_indexIterator, int *an_entry) ;

// Ret:
// - ANSWER__YES: indexes are enabled (i.e at least one index has been added) 
// - ANSWER__NO: indexes are NOT enabled
// - -1: anomaly is raised
int GreenIndexesVerifyEnabled (struct GREEN_INDEXES *a_indexes) ;


// Passed:
// - a_indexes:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: some index is corrupted
// - -1: anomaly is raised
int GreenIndexesVerify (struct GREEN_INDEXES *a_indexes) ;

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
int GreenIndexesVerifyCount (struct GREEN_INDEXES *a_indexes,int *ac_commonCount) ;


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
int GreenIndexesVerifyEntry (struct GREEN_INDEXES *a_indexes, int entry, int expectedHits) ;


// Passed:
// - a_indexes
//
// Ret:
// - RETURNED:
// - -1: anomaly is raised
int GreenIndexesClear (struct GREEN_INDEXES *a_indexes) ;


#endif // __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0

// Manage double inclusion: update inclusion state:
#if __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0
#undef __C_ANSI_GREEN_INDEX_H_INCLUDED__
// Manage double inclusion: provoke 2nd inclusion:
#define __C_ANSI_GREEN_INDEX_H_INCLUDED__ 2
#include "c-ansi/green-index.h"
#else
#undef __C_ANSI_GREEN_INDEX_H_INCLUDED__
#define __C_ANSI_GREEN_INDEX_H_INCLUDED__ 3
#endif

#endif // __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 0 || __C_ANSI_GREEN_INDEX_H_INCLUDED__ == 2

