// c-ansi/g-index.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

// #SEE double-inclusion-4-generic-imaged-enums @ flint/images.h <__C_ANSI_G_INDEX_H_INCLUDED__>
#ifndef __C_ANSI_G_INDEX_H_INCLUDED__
#define __C_ANSI_G_INDEX_H_INCLUDED__ 0
#endif

#if __C_ANSI_G_INDEX_H_INCLUDED__ == 0 || __C_ANSI_G_INDEX_H_INCLUDED__ == 2 
#if __C_ANSI_G_INDEX_H_INCLUDED__ == 0

#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-ansi/c-stack.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// G-Indexes:
// ---------

// Index's virtual function to compare an item with a key.
//
// Passed:
// - r_handle:
// - indexLabel:
// - gKeyRank:
// - aEntry: entry for "A"
// - n_bEntry: entry for "B"
// - cr_bGKeys: raw key(s) for "B" 
//
// Ret: comparison between "A" (entry) and "B" (key)
// - LESS_THAN__COMPARISON:
// - EQUAL_TO__COMPARISON:
// - GREATER_THAN__COMPARISON:
// - -1: unexpected problem; anomaly is raised
typedef int (*ENTRY_RAW_COMPARE_FUNCTION) (void *r_handle, int indexLabel, int gKeyRank,
  int aEntry, int n_bEntry, void *cr_bGKeys) ;

// Index's virtual function to adequate an item with a key.
//
// Passed:
// - r_handle:
// - indexLabel:
// - gKeyRank:
// - aEntry: entry for "A"
// - r_bGKeys: raw key(s) for "B" 
//
// Ret: adequation between "A" (entry) and "B" (key) ?
// - ANSWER__YES: 
// - ANSWER__NO: 
// - -1: unexpected problem; anomaly is raised
typedef int (*ENTRY_RAW_EQUATE_FUNCTION) (void *r_handle, int indexLabel, int gKeyRank,
  int aEntry, void *r_bGKeys);

struct G_INDEXES ; // Private!
typedef struct G_INDEXES* G_INDEXES_HANDLE; // Public handle

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
int GIndexesCreateInstance(G_INDEXES_HANDLE *azh_handle,
  ENTRY_RAW_COMPARE_FUNCTION entryRawCompareFunction,
  ENTRY_RAW_EQUATE_FUNCTION entryRawEquateFunction, void* r_entryRawFunctionsHandle);

// Passed:
// - handle:
// - itemsPhysicalNumber:
// - gKeyCount:
//
// Return:
// - >= 0: new index label (technically equal to entry in indexes array)
// - -1: anomaly is raised
int GIndexesAddIndex(G_INDEXES_HANDLE handle, int itemsPhysicalNumber,
  int gKeyCount) ;

// Return number of indexes
// Passed:
// - handle:
//
// Ret:
// - >= 0: number o indexes
int o_GIndexesGetIndexCount(G_INDEXES_HANDLE handle) ;


// Passed:
// - handle:
// - newItemsPhysicalNumber:
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GIndexesResize (G_INDEXES_HANDLE handle, int newItemsPhysicalNumber) ;


// Add reference on item in all indexes (no action if already referenced in some index)
//
// Passed:
// - handle:
// - entry:
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GIndexesAdd(G_INDEXES_HANDLE, int entry) ; 

// Remove reference on item in all indexes (no action if not referenced in some index)
//
// Passed:
// - handle:
// - entry:
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GIndexesRemove(G_INDEXES_HANDLE handle, int entry) ;

// Return number of items (managed by ALL indexes)  
//
// Passed:
// - handle:
//
// Ret:
// - >= 0: number of items seen by indexes
int o_GIndexesGetItemCount(G_INDEXES_HANDLE handle) ;


// Index entries "block"
struct INDEX_ENTRY_BLOCK {
  int first; // >= 0
  int last; // >= first
} ;

// Binary search of "B" in index
//
// Passed:
// - handle:
// - indexLabel: 
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
int GIndexesBSearch(G_INDEXES_HANDLE handle, int indexLabel, int n_bEntry, void *cr_bGKeys,
  int *an_indexEntry, int *a_top, struct INDEX_ENTRY_BLOCK *cac_indexEntries) ; 

// Passed:
// - handle:
// - indexLabel: 
// - indexEntry:
//
// Ret:
//- >= 0: item entry 
int o_GIndexesGetItemEntry(G_INDEXES_HANDLE handle, int indexLabel, int indexEntry) ;


// * Truly index-based seek flags:
// NOT key-based seek flag: mutually exclusive with other truly index-based seek flags: 
#define INDEX_SEEK_FLAG__ANY     0x01
// Key-based seek flags: 
#define INDEX_SEEK_FLAG__EQUAL   0x02
#define INDEX_SEEK_FLAG__LESS    0x04
#define INDEX_SEEK_FLAG__GREATER 0x08 
// * NON-index based seek flag: mutually exclusive with truly index-based seek flags: 
#define INDEX_SEEK_FLAG__LIKE    0x10 
  
#endif // __C_ANSI_G_INDEX_H_INCLUDED__ == 0

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ __C_ANSI_G_INDEX_H_INCLUDED__
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

#if __C_ANSI_G_INDEX_H_INCLUDED__ == 0

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
int GIndexesSeekEntryEquate(G_INDEXES_HANDLE handle, int indexLabel, int aEntry,
  unsigned int indexSeekFlags, void *r_bGKeys) ; 


// Ret:
// - ANSWER__YES: indexes are enabled (i.e at least one index has been added) 
// - ANSWER__NO: indexes are NOT enabled
// - -1: anomaly is raised
int GIndexesVerifyEnabled (G_INDEXES_HANDLE handle) ;

// Passed:
// - handle:
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: some index is corrupted
// - -1: anomaly is raised
int GIndexesVerify (G_INDEXES_HANDLE handle) ;

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
int GIndexesVerifyCount (G_INDEXES_HANDLE handle,int *ac_commonCount) ;

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
int GIndexesVerifyEntry (G_INDEXES_HANDLE handle, int entry, int expectedHits) ;


// Passed:
// - handle
//
// Ret:
// - RETURNED:
// - -1: anomaly is raised
int GIndexesClear (G_INDEXES_HANDLE handle) ;


// Passed:
// - xh_handle:
// 
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int GIndexesDestroyInstance(G_INDEXES_HANDLE xh_handle) ;


#endif // __C_ANSI_G_INDEX_H_INCLUDED__ == 0

// Manage double inclusion: update inclusion state:
#if __C_ANSI_G_INDEX_H_INCLUDED__ == 0
#undef __C_ANSI_G_INDEX_H_INCLUDED__
// Manage double inclusion: provoke 2nd inclusion:
#define __C_ANSI_G_INDEX_H_INCLUDED__ 2
#include "c-ansi/g-index.h"
#else
#undef __C_ANSI_G_INDEX_H_INCLUDED__
#define __C_ANSI_G_INDEX_H_INCLUDED__ 3
#endif

#endif // __C_ANSI_G_INDEX_H_INCLUDED__ == 0 || __C_ANSI_G_INDEX_H_INCLUDED__ == 2

