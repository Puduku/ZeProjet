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

// Index's virtual function to compare an item with a key.
//
// Passed:
// - r_handle:
// - indexLabel:
// - keyRank:
// - aEntry: entry for "A"
// - n_bEntry: entry for "B"
// - cpr_bKeys: raw key(s) for "B" 
//
// Ret: comparison between "A" (entry) and "B" (key)
// - LESS_THAN__COMPARISON:
// - EQUAL_TO__COMPARISON:
// - GREATER_THAN__COMPARISON:
// - -1: unexpected problem; anomaly is raised
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

struct GREEN_INDEXES ; // Private!
typedef struct GREEN_INDEXES* GREEN_INDEXES_HANDLE; // Public handle

// Passed:
// - azh_handle:
// - entryRawCompareFunction:
// - entryRawEquateFunction:
// - r_entryRawFunctionsHandle:
// 
// Changed:
// - *azh_handle:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GreenIndexesCreateInstance(GREEN_INDEXES_HANDLE *azh_handle,
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction,
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction, void* r_entryRawFunctionsHandle);

// Passed:
// - handle:
// - itemsPhysicalNumber:
// - keyCount:
//
// Return:
// new index label (technically equal to entry in indexes array)
int GreenIndexesAddIndex(GREEN_INDEXES_HANDLE handle, int itemsPhysicalNumber,
  int keyCount) ;

// Passed:
// - handle:
// - newItemsPhysicalNumber:
int GreenIndexesResize (GREEN_INDEXES_HANDLE handle, int newItemsPhysicalNumber) ;

// Add reference on item in all indexes (no action if already referenced in some index)
//
// Passed:
// - handle:
// - entry:
int GreenIndexesAdd(GREEN_INDEXES_HANDLE, int entry) ; 

// Remove reference on item in all indexes (no action if not referenced in some index)
//
// Passed:
// - handle:
// - entry:
int GreenIndexesRemove(GREEN_INDEXES_HANDLE handle, int entry) ;


// Request criteria:
// -----------------

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
static inline int m_OpenBracketCount(unsigned int criteriaOpFlags) {
  if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN3)) return 3;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN2)) return 2;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)) return 1;
  return 0;
} // m_OpenBracketCount 

// Passed:
// - criteriaOpFlags
static inline int m_CloseBracketCount(unsigned int criteriaOpFlags) {
  if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3)) return 3;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)) return 2;
  else if (b_FLAG_SET_ON(criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)) return 1;
  return 0;
} // m_CloseBracketCount 

// May or may be NOT index-based selection:
struct G_REQUEST_CRITERION {
  int indexLabel;
  unsigned int indexSeekFlags;
  const void *cfpr_keys; // Only significant with Key-based seek flag(s)
  unsigned int criteriaOpFlags;
} ;

// Suggested max is 5, but may be adapted
#define REQUEST_CRITERIA_MAX5 5


// Index sequence:
// ---------------

// Index entries "block"
struct INDEX_ENTRY_BLOCK {
  int first; // >= 0
  int last; // >= first
} ;

//
struct INDEX_SEQUENCE {
  // index entries "blocks":
  int indexEntryBlockCount2; // between 0 and 2 : 0 => 'disabled' ; >0 -> 'enabled' 
  struct INDEX_ENTRY_BLOCK indexEntryBlocks2[2];
  // Fields below are only significant if 'enabled' (indexEntryBlockCount2 > 0) :
  int cv_firstIndexEntry; // first index entry for ALL "blocks"  
  int cv_lastIndexEntry; // last index entry for ALL "blocks"  
  int ci_indexEntryCursor; // "current" index entry: < cv_firstIndexEntry => Ascending:"soft reset",
    // DEscending:'no more' ; between [indexEntries[0].first - cv_lastIndexEntry] => 'in sequence' ;
    // > cv_lastIndexEntry => Ascending:"no more", DEscending:"soft reset" 
  const struct INDEX_ENTRY_BLOCK *cv_indexEntryBlockPtr; // "block" corresponding to "current" index entry 
} ;

// Disable index sequence (disambiguation purpose...)
// 
// Ret:
// - RETURNED: Ok
static inline int om_IndexSequenceDisable(struct INDEX_SEQUENCE *a_indexSequence) {
  m_DIGGY_BOLLARD_S()
  a_indexSequence->indexEntryBlockCount2 = 0;
  m_DIGGY_RETURN(RETURNED);
} // om_IndexSequenceDisable
  

// Index iterator:
// ---------------

// An index iterator:
// - Request criteria
// - Index sequence

#define b_ASCENDING b_FALSE0
#define b_DESCENDING b_TRUE

struct INDEX_ITERATOR {
  int criteriaCount5 ; // see REQUEST_CRITERIA_MAX5 
  struct G_REQUEST_CRITERION criteria[REQUEST_CRITERIA_MAX5];
  char b_descending;
  struct INDEX_SEQUENCE indexSequence;
};

// Passed:
// - criterion1:
//
// Ret: new indexIterator
static inline struct INDEX_ITERATOR om_IndexIterator(struct G_REQUEST_CRITERION criterion1) {
  struct INDEX_ITERATOR indexIterator = { .criteriaCount5 = 1};
  indexIterator.criteria[0] = criterion1;
  indexIterator.b_descending = b_ASCENDING;
  om_IndexSequenceDisable(&indexIterator.indexSequence);
  return indexIterator;
} // om_IndexIterator

// Passed:
// - a_indexIterator:
// - m_criterion:
// 
// Changed:
// - *a_indexIterator:
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_IndexIteratorAddCriterion(struct INDEX_ITERATOR *a_indexIterator,
  struct G_REQUEST_CRITERION criterion) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(a_indexIterator->criteriaCount5 < REQUEST_CRITERIA_MAX5)
  int i = a_indexIterator->criteriaCount5;\
  a_indexIterator->criteria[i] = criterion;\
  a_indexIterator->criteriaCount5 ++;\
  m_DIGGY_RETURN(RETURNED)
} // m_IndexIteratorAddCriterion
  

// Passed:
// - a_indexIterator:
// - b_descending:
// 
// Changed:
// - *a_indexIterator:
//
// Ret:
// - RETURNED: Ok
static inline int om_IndexIteratorReset(struct INDEX_ITERATOR* a_indexIterator, char b_descending) {
  m_DIGGY_BOLLARD_S()
  a_indexIterator->b_descending = b_descending;\
  om_IndexSequenceDisable(&a_indexIterator->indexSequence);
  m_DIGGY_RETURN(RETURNED)
} // om_IndexIteratorReset


// Update index iterator sequence.
//
// Passed:
// - handle: 
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
int GreenIndexesSeek(GREEN_INDEXES_HANDLE handle, struct INDEX_ITERATOR *a_indexIterator,
  int *nan_entry) ;

// Get current entry in index sequence.
//
// Passed:
// - handle:
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
int GreenIndexesCurrent(GREEN_INDEXES_HANDLE handle, struct INDEX_ITERATOR *a_indexIterator,
  int *an_entry) ;


// Ret:
// - ANSWER__YES: indexes are enabled (i.e at least one index has been added) 
// - ANSWER__NO: indexes are NOT enabled
// - -1: anomaly is raised
int GreenIndexesVerifyEnabled (GREEN_INDEXES_HANDLE handle) ;

// Passed:
// - handle:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: some index is corrupted
// - -1: anomaly is raised
int GreenIndexesVerify (GREEN_INDEXES_HANDLE handle) ;

// NOT callable if indexes are not enabled 
// 
// Passed:
// - handle:
//
// Changed:
// - *ac_commonCount: only significant with COMPLETED__OK
//
// Ret:
// - COMPLETED__OK: all indexes have the same number of entries
// - COMPLETED__BUT: at least two indexes have different numbers of entries
// - -1: anomaly is raised
int GreenIndexesVerifyCount (GREEN_INDEXES_HANDLE handle,int *ac_commonCount) ;

// 
// Passed:
// - handle:
// - entry: entry to verify presence in different indexes
// - expectedHits : typically 0 or 1
//
// Returned
// - COMPLETED__OK: number of "hits" (aka number of times the entry is present in the stack)
//                  is as expected is as expected in EACH index
// - COMPLETED__BUT: number of "hits" NOT as expected in 
// - -1: unexpected problem ; anomaly is raised
int GreenIndexesVerifyEntry (GREEN_INDEXES_HANDLE handle, int entry, int expectedHits) ;

// Passed:
// - handle
//
// Ret:
// - RETURNED:
// - -1: anomaly is raised
int GreenIndexesClear (GREEN_INDEXES_HANDLE handle) ;

// Passed:
// - xh_handle:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GreenIndexesDestroyInstance(GREEN_INDEXES_HANDLE xh_handle) ;


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

