// c-ansi/green.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/green.h"
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
// "GREEN INDEXES" "GAPS STACK" ("proto" objects)
// ^ uses          ^ uses
// ==== "GREEN COLLECTION" ==== ("genuine" object)


#ifdef DIGGY
extern char b_diggyGreenCollectionExam ;
#endif


///////////////// 1. GAPS STACK ("proto" object...........) /////////////////

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
  int *s_stackPtr = a_gaps->hsc_stack;
  int i = 0; for (; i < a_gaps->count ; i++, s_stackPtr++) {
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
    m_DIGGY_INFO("Exam: a_gaps->count=%d",a_gaps->count)
    int i = 0; for (; i < a_gaps->count; i++) {
      m_DIGGY_INFO("  a_gaps->hsc_stack[%d]=%d",i,a_gaps->hsc_stack[i])
    } // for
  } // if 
#endif

  m_DIGGY_RETURN(a_gaps->count)  
} // GapsStackVerifyCount 

// Passed:
// - ax_index:
#define m_GAPS_STACK_FREE(/*struct GAPS_STACK*/mx_gaps)  free((mx_gaps).hsc_stack);


////////////////// 2. GREEN COLLECTIONS  //////////////////

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
 
// Passed:
// - m_indexRequest:
// - m_criterion1:
//
// Ret: new index request
static inline struct INDEX_REQUEST om_IndexRequest(struct G_REQUEST_CRITERION criterion1) {
  struct INDEX_REQUEST indexRequest = { .fetch4 = FETCH_4__CHANGE };
  indexRequest.iterator = om_IndexIterator(criterion1); 
  return indexRequest;
} // om_IndexRequest

// Passed:
// - m_indexRequest:
// - m_criterion:
// 
// Changed:
// - *a_indexIterator:
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int m_IndexRequestAddCriterion(struct INDEX_REQUEST *a_indexRequest,
  struct G_REQUEST_CRITERION criterion) {
  m_DIGGY_BOLLARD_S()
  m_TRACK_IF(m_IndexIteratorAddCriterion(&a_indexRequest->iterator,criterion) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // m_IndexRequestAddCriterion


//
// Ret:
// - RETURNED: Ok
static inline int om_IndexRequestReset(struct INDEX_REQUEST *a_indexRequest, char b_descending,
  int fetch4) {
  m_DIGGY_BOLLARD_S()
  om_IndexIteratorReset(&a_indexRequest->iterator, b_descending);
  a_indexRequest->fetch4 = fetch4;
  m_DIGGY_RETURN(RETURNED)
} // om_IndexRequestReset

const int p_indexRequestAutomaticBufferSize = sizeof(struct INDEX_REQUEST);

struct GREEN_COLLECTION {
  int expectedItemCount ;

  GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction ;
  GREEN_HANDLER__COMPARE_FUNCTION greenHandlerCompareFunction;
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
  GREEN_INDEXES_HANDLE h_indexesHandle;
  struct INDEX_REQUEST internalIndexRequest;
  struct GAPS_STACK h_gaps; 
  // "Monitored" entries "fetched for change" (in ALIEN / ALIVE state) : 
  int fetched4ChangeEntryCount;  
  int fetched4ChangeEntriesPhysicalNumber;  
  int *h_fetched4ChangeEntries;  
};

// typedef GREEN_HANDLER__COMPARE_FUNCTION
static int NotEnabledCompare(void *cpr_handle, char b_frozen, int indexLabel, int keyRank,
  char *pr_aGreenItemStuff, char *npr_bGreenItemStuff, const void *cpr_bKeys) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // NotEnabledCompare

// typedef GREEN_HANDLER__EQUATE_FUNCTION
static int  NotEnabledEquate(void *cpr_handle,  char b_frozen,
  int indexLabel, int keyRank,  char *pr_aGreenItemStuff, const void *pr_bKeys) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // NotEnabledEquate


// Get green set's handle corresponding to item entry in the collection...
//
// Passed:
// - handle:
// - entry:
#define /*char* */ r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(/*GREEN_COLLECTION_HANDLE*/ handle,\
  /*int*/entry)  mr_GetGreenItemStuff((handle)->h_greenArray, (entry), (handle)->greenItemSize)


// ENTRY_RAW_COMPARE_FUNCTION
static int GreenCollectionEntryRawCompare (void *r_handle, int indexLabel, int keyRank, int aEntry,
  int n_bEntry, const void *cpr_bKeys) {
  m_DIGGY_BOLLARD_S()
  GREEN_COLLECTION_HANDLE handle = (GREEN_COLLECTION_HANDLE) r_handle;

  char *r_aGreenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(handle,aEntry);
  char *nr_bGreenItemStuff =
  (n_bEntry == -1? NULL: r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(handle,n_bEntry));

  int comparison = handle->greenHandlerCompareFunction(handle->cr_greenHandlerHandle,
    handle->b_frozen, indexLabel,keyRank, r_aGreenItemStuff, nr_bGreenItemStuff, cpr_bKeys);
  m_TRACK_IF(comparison < 0)

  m_DIGGY_RETURN(comparison)
} // GreenCollectionEntryRawCompare

// ENTRY_RAW_EQUATE_FUNCTION
static int GreenCollectionEntryRawEquate (void *r_handle, int indexLabel, int keyRank, int aEntry,
  const void *pr_bKeys) {
  m_DIGGY_BOLLARD_S()
  GREEN_COLLECTION_HANDLE handle = (GREEN_COLLECTION_HANDLE) r_handle;

  char *r_aGreenItemStuff = r_GREEN_COLLECTION_GET_GREEN_ITEM_STUFF(handle,aEntry);

  int answer = handle->greenHandlerEquateFunction(handle->cr_greenHandlerHandle,
    handle->b_frozen, indexLabel,keyRank, r_aGreenItemStuff, pr_bKeys);
  m_TRACK_IF(answer < 0)

  m_DIGGY_RETURN(answer)
} // GreenCollectionEntryRawEquate


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
int GreenCollectionCreateInstance(GREEN_COLLECTION_HANDLE *azh_handle,  int expectedItemCount,
  int greenItemSize, GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction,
  GREEN_HANDLER__COMPARE_FUNCTION n_greenHandlerCompareFunction,
  GREEN_HANDLER__EQUATE_FUNCTION n_greenHandlerEquateFunction, void *cfr_greenHandlerHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  GREEN_COLLECTION_HANDLE handle = *azh_handle ;

  handle->expectedItemCount = expectedItemCount;

  handle->n_greenHandlerDisengageFunction = n_greenHandlerDisengageFunction ;
  handle->greenHandlerCompareFunction =
  (n_greenHandlerCompareFunction != NULL? n_greenHandlerCompareFunction:
   NotEnabledCompare);
  handle->greenHandlerEquateFunction =
  (n_greenHandlerEquateFunction != NULL? n_greenHandlerEquateFunction:
   NotEnabledEquate);
  handle->cr_greenHandlerHandle = cfr_greenHandlerHandle;

  handle->b_frozen = b_FALSE0;
  // TODO: add b_automaticIndexesRefresh parameter
  handle->b_automaticIndexesRefresh = b_TRUE;

  handle->greenItemSize = greenItemSize;
  handle->itemsPhysicalNumber = expectedItemCount;

  m_CALLOC(handle->h_greenArray,handle->itemsPhysicalNumber,handle->greenItemSize)
  m_MALLOC_ARRAY(handle->hsc_flags, handle->itemsPhysicalNumber)
  handle->v_maxItemsCount = handle->i_itemsCount = 0 ;
  m_TRACK_IF(GreenIndexesCreateInstance(&handle->h_indexesHandle,GreenCollectionEntryRawCompare,
    GreenCollectionEntryRawEquate, (void *) handle) != RETURNED)
  m_GAPS_STACK_INIT(handle->h_gaps,handle->itemsPhysicalNumber)
  struct G_REQUEST_CRITERION defaultCriterion = m_GRequestCriterion(INDEX_LABEL0,ALL_FLAGS_OFF0,
    (void*)UNDEFINED, ALL_FLAGS_OFF0) ;
  handle->internalIndexRequest = om_IndexRequest(defaultCriterion);

  m_MALLOC_ARRAY(handle->h_fetched4ChangeEntries,handle->fetched4ChangeEntriesPhysicalNumber =
    expectedItemCount)
  handle->fetched4ChangeEntryCount = 0;
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

  int newItemsPhysicalNumber = handle->itemsPhysicalNumber + handle->expectedItemCount;

  m_REALLOC(handle->h_greenArray, handle->greenItemSize*newItemsPhysicalNumber)
  memset(handle->h_greenArray + handle->greenItemSize * handle->itemsPhysicalNumber, 0,
    handle->greenItemSize * handle->expectedItemCount);

  m_GAPS_STACK_RESIZE(handle->h_gaps, newItemsPhysicalNumber)
  m_REALLOC_ARRAY(handle->hsc_flags, newItemsPhysicalNumber)
  m_TRACK_IF(GreenIndexesResize(handle->h_indexesHandle,newItemsPhysicalNumber) != 0)

  handle->itemsPhysicalNumber = newItemsPhysicalNumber;
  m_DIGGY_RETURN(handle->expectedItemCount)
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

  if (handle->fetched4ChangeEntryCount == 0) {
    // MINIMONITOR: NADA
    m_DIGGY_RETURN(RETURNED)
  } // if

  m_ASSERT(!handle->b_frozen)

  // MINIMONITOR: ANY+
  int *fetched4ChangeEntryPtr = handle->h_fetched4ChangeEntries; 
  int i = 0; for (; i < handle->fetched4ChangeEntryCount; i++, fetched4ChangeEntryPtr++) {
    // MICROMONITOR: ALIEN / ALIVE (fetched 4 change)
    m_ASSERT(b_ALL_FLAGS_OK(handle->hsc_flags[*fetched4ChangeEntryPtr],ALIEN_ALIVE__FLAGS))
m_DIGGY_INFO("*fetched4ChangeEntryPtr=%d Before m_GREEN_INDEXES_ADD()...",*fetched4ChangeEntryPtr)
    m_TRACK_IF(GreenIndexesAdd(handle->h_indexesHandle,*fetched4ChangeEntryPtr) != RETURNED)
    m_SET_FLAG_OFF(handle->hsc_flags[*fetched4ChangeEntryPtr],ALIEN_FLAG)
    // MICROMONITOR: FAMED / ALIVE
  } // for
  handle->fetched4ChangeEntryCount = 0; 
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
  m_ASSERT(GreenIndexesVerifyEnabled(handle->h_indexesHandle) == ANSWER__NO)

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
  if (handle->fetched4ChangeEntryCount == handle->fetched4ChangeEntriesPhysicalNumber) {
    m_REALLOC(handle->h_fetched4ChangeEntries,handle->fetched4ChangeEntriesPhysicalNumber +=
      handle->expectedItemCount)
  } // if

  handle->h_fetched4ChangeEntries[handle->fetched4ChangeEntryCount++] = entry;
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
        m_TRACK_IF(GreenIndexesRemove(cp_handle->h_indexesHandle,n_entry) != RETURNED)
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
  handle->fetched4ChangeEntryCount = 0 ;
  m_GAPS_STACK_CLEAR(handle->h_gaps)
  m_TRACK_IF(GreenIndexesClear(handle->h_indexesHandle) != RETURNED)

  // MINIMONITOR: NADA
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionClear


// Public function; see description in .h
int GreenCollectionAddIndex (GREEN_COLLECTION_HANDLE handle, int keyCount) {
  m_DIGGY_BOLLARD()
  m_ASSERT(!handle->b_frozen)
  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  // TODO: permettre d'ajouter des indexes ag chaud...
  m_ASSERT(handle->i_itemsCount == 0)

  m_ASSERT(keyCount > 0)

  int newIndexLabel = GreenIndexesAddIndex(handle->h_indexesHandle, handle->itemsPhysicalNumber,
    keyCount);
  m_TRACK_IF(newIndexLabel < 0)

  m_DIGGY_RETURN(newIndexLabel)
} // GreenCollectionAddIndex

  
// Public function; see description in .h
int GreenCollectionIndexRequestR(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaCount,
  const struct G_REQUEST_CRITERION *sp_criteria) {
  m_DIGGY_BOLLARD()
  m_ASSERT(criteriaCount > 0)
  m_ASSERT(sp_criteria->indexSeekFlags != ALL_FLAGS_OFF0) 
  m_ASSERT(nf_indexRequestAutomaticBuffer != NULL || !cp_handle->b_frozen) 

  struct INDEX_REQUEST *indexRequestPtr = (nf_indexRequestAutomaticBuffer != NULL?
    (struct INDEX_REQUEST *)nf_indexRequestAutomaticBuffer: &(cp_handle->internalIndexRequest));

  //if (criteriaCount == 1) sp_criteria->criteriaOpFlags = ALL_FLAGS_OFF0;
  *indexRequestPtr = om_IndexRequest(*sp_criteria);

  int i = 1; for (; i < criteriaCount;  i++) {
    sp_criteria++; 
    m_TRACK_IF(m_IndexRequestAddCriterion(indexRequestPtr,*sp_criteria) != RETURNED)
  } // for 

  // MINIMONITOR: ANY
  m_TRACK_IF(GreenCollectionRefreshIndexesInternal(cp_handle,b_TRUE) != RETURNED)
  // MINIMONITOR: NADA

  m_TRACK_IF(GreenIndexesSeek(cp_handle->h_indexesHandle, &indexRequestPtr->iterator, NULL) !=
    RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionIndexRequestR

// Public function; see description in .h
int GreenCollectionIndexRequestV(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaCount,
  int indexLabel1, unsigned int indexSeekFlags1, void *cfpr_keys1, va_list extraCriteria) {
  m_DIGGY_BOLLARD()

  m_ASSERT(criteriaCount > 0)
  struct G_REQUEST_CRITERION s_criteria[criteriaCount] ;
  struct G_REQUEST_CRITERION *criterionPtr = s_criteria;

  unsigned int criteriaOpFlags = ALL_FLAGS_OFF0;
  if (criteriaCount > 1) criteriaOpFlags = va_arg(extraCriteria,unsigned int);
  *criterionPtr = m_GRequestCriterion(indexLabel1,indexSeekFlags1,cfpr_keys1,criteriaOpFlags);

  int i = 1; for (; i < criteriaCount;  i++) *(++criterionPtr) = m_GRequestCriterion(va_arg(
    extraCriteria,int), va_arg(extraCriteria,unsigned int),va_arg(extraCriteria,void *),
    va_arg(extraCriteria,unsigned int));

  m_TRACK_IF(GreenCollectionIndexRequestR(cp_handle,nf_indexRequestAutomaticBuffer,
    criteriaCount,s_criteria) != RETURNED) 
  
  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionIndexRequestV

// Public function; see description in .h
int GreenCollectionIndexRequest(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaCount,
  int indexLabel1, unsigned int indexSeekFlags1, void *cfpr_keys1, ...) {
  m_DIGGY_BOLLARD()

  va_list extraCriteria;
  va_start(extraCriteria,cfpr_keys1);

  m_TRACK_IF(GreenCollectionIndexRequestV(cp_handle,nf_indexRequestAutomaticBuffer,
    criteriaCount,indexLabel1,indexSeekFlags1,cfpr_keys1,extraCriteria) != RETURNED) 

  va_end(extraCriteria);

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
     
    om_IndexRequestReset(indexRequestPtr,b_FLAG_SET_ON(indexFetchFlags,
      INDEX_FETCH_FLAG__DESCENDING),fetch4);
    m_TRACK_IF(GreenIndexesSeek(cp_handle->h_indexesHandle,&indexRequestPtr->iterator,NULL) !=
      RETURNED)
  } // if    

  int n_entry = UNDEFINED;
  int result = RESULT__NOT_FOUND;

  if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__NEXT)) {
    m_TRACK_IF(GreenIndexesSeek(cp_handle->h_indexesHandle,&indexRequestPtr->iterator,&n_entry) !=
      RETURNED)
  } else {
    m_TRACK_IF(GreenIndexesCurrent(cp_handle->h_indexesHandle,&indexRequestPtr->iterator,&n_entry)
      != RETURNED)
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
    int i = 0 ; for (; i < handle->i_itemsCount; i++) {
      m_DIGGY_INFO("  handle->hsc_flags[%d]=%02x",i,handle->hsc_flags[i])
    } // for
  } // if 
#endif

  int gapsCount = GapsStackVerifyCount(&handle->h_gaps);
  m_TRACK_IF(gapsCount < 0)

  // 1. Are indexes enabled ? (if not, just verify there's no gap 
  switch (GreenIndexesVerifyEnabled(handle->h_indexesHandle)) {
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
    completed = GreenIndexesVerifyCount(handle->h_indexesHandle,&c_indexedItemsCount);
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
    int i = 0; for (; i < handle->i_itemsCount; i++) {
      int expectedHits = 0;
      if (b_FLAG_SET_ON(handle->hsc_flags[i],DEAD_FLAG))  expectedHits = 1; 
      m_TRACK_IF((completed = GapsStackVerifyEntry(&handle->h_gaps,i,expectedHits)) < 0)
      if (completed == COMPLETED__BUT) break;
      expectedHits = 0;
      if (!b_FLAG_SET_ON(handle->hsc_flags[i],ALIEN_FLAG))  expectedHits = 1;
      m_TRACK_IF((completed = GreenIndexesVerifyEntry(handle->h_indexesHandle,i,expectedHits)) < 0)
      if (completed == COMPLETED__BUT) break;
    } // for
  } // if 

m_DIGGY_VAR_D(completed)
  // 4. Verify index(es) good ordering 
  if (completed == COMPLETED__OK) {
    completed = GreenIndexesVerify(handle->h_indexesHandle);
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
    int i = 0; for (; i < xh_handle->v_maxItemsCount ; i++, r_greenItemStuff += xh_handle->greenItemSize) {
      m_TRACK_IF(xh_handle->n_greenHandlerDisengageFunction(xh_handle->cr_greenHandlerHandle,
        r_greenItemStuff) != RETURNED)
    } // for
  } // if
  free(xh_handle->h_greenArray) ;

  m_TRACK_IF(GreenIndexesDestroyInstance(xh_handle->h_indexesHandle) != RETURNED)

  m_GAPS_STACK_FREE(xh_handle->h_gaps)

  free(xh_handle->h_fetched4ChangeEntries);

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // GreenCollectionDestroyInstance
