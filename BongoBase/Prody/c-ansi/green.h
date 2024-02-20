// c-ansi/green.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

// #SEE double-inclusion-4-generic-imaged-enums @ flint/images.h <__C_ANSI_GREEN_H_INCLUDED__>

#ifndef __C_ANSI_GREEN_H_INCLUDED__
#define __C_ANSI_GREEN_H_INCLUDED__ 0
#endif

#if __C_ANSI_GREEN_H_INCLUDED__ == 0 || __C_ANSI_GREEN_H_INCLUDED__ == 2 

#if __C_ANSI_GREEN_H_INCLUDED__ == 0

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/magic.h"
#include "flint/flags.h"


////////////////////////////
#include "c-ansi/green.topo"
////////////////////////////


// Green item handlers ("virtual objects") 
// ---------------------------------------


// #REF GREEN_HANDLER__DISENGAGE_FUNCTION - Custom function definition.
// Disengage green item but DO NOT free the instance!
//
// Passed:
// - r_handle: private handle 
// - r_greenItemStuff: "green item" handle (== pointer) :
//   The function must first check the state of the item and then take the apposite disposition:
//   + "engaged" state => need ad hoc disengaging operations
//   + yet in "disengaged state" => nothing to do
//
// Modified:
// - r_greenItemStuff: item is in "disengaged" state
//
// Returned:
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
typedef int (*GREEN_HANDLER__DISENGAGE_FUNCTION) (void *r_handle, char *r_greenItemStuff) ;

#define INDEX_LABEL0 0

// #REF GREEN_HANDLER__COMPARE_FUNCTION <GreenItem> - Custom function definition...
// Compare "A" and "B" keys values.
// - "A" key(s) are extracted from "A" greem item. 
// - "B" key(s) :
//   + are extracted from "B" greem item when such item present
//   + are used "directly" otherwise 
//
// Passed:
// - cpr_handle: private handle ; protected instance if collection is frozen 
// - b_frozen: TRUE if collection is frozen
// - indexLabel: specific index label (see <GreenItem>AddIndex() function)
// - keyRank: between 0 (0 == 1ST rank) and <number of index keys> - 1
// - pr_a<GreenItem>Stuff: <GreenItem> containing the A key(s) 
// - npr_b<GreenItem>Stuff: B key(s):
//   + != NULL : <GreenItem> containing the B key(s) 
//   + NULL special address: see "raw" B key(s) 
// - cpr_bKeys: only significant if B <GreenItem> is not provided ; "raw" B key(s)
//
// Returned:
// - >=0: "comparison" between :"A" and "B" with that key component... 
//   + LESS_THAN__COMPARISON : "A" 'less than' "B" 
//   + EQUAL_TO__COMPARISON : "A" and "B" are "identical"
//   + GREATER_THAN__COMPARISON : "A" 'greater than' "B" 
// - -1: unexpected problem; anomaly is raised
typedef int (*GREEN_HANDLER__COMPARE_FUNCTION) (void *cpr_handle,  char b_frozen,
  int indexLabel, int keyRank,  char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff,
  void *cpr_bKeys) ;


// #REF GREEN_HANDLER__EQUATE_FUNCTION <GreenItem> - Custom function definition...
// Adequate "A" item with "B" key(s) value. 
//
// Passed:
// - cpr_handle: private handle ; protected instance if collection is frozen 
// - b_frozen: TRUE if collection is frozen
// - indexLabel: specific index label (see <GreenItem>AddIndex() function)
// - keyRank: between 0 (0 == 1ST rank) and <number of index keys> - 1
// - pr_a<GreenItem>Stuff: <GreenItem> containing the A key(s) 
// - pr_bKeys: "raw" B key(s)
//
// Returned:
// - >=0: "equation" between :"A" and "B" with that key component... 
//   + ANSWER__YES : "A" item and "B" key(s) are similar 
//   + ANSWER__NO : "A" item and "B" key(s) are NOT similar 
// - -1: unexpected problem; anomaly is raised
typedef int (*GREEN_HANDLER__EQUATE_FUNCTION) (void *cpr_handle,  char b_frozen,
  int indexLabel, int keyRank,  char *pr_aGreenItemStuff, void *pr_bKeys) ;

 
// Green collections
// -----------------

// Private structure:
struct GREEN_COLLECTION;
// Public handle:
typedef struct GREEN_COLLECTION *GREEN_COLLECTION_HANDLE;


#define BATEAU__EXPECTED_ITEMS_NUMBER 100

// (Create and) initialize a green collection (aka. of green items)
//
// Passed:
// - azh_handle: (pointer to) handle to be initialized
// - expectedItemsNumber: 
//   #REF GreenCollectionCreateInstance-expectedItemsNumber <green item>
//   >0 value; representative number of <green item>s present in the collection.
//   This value helps the module to adopt better strategy regarding memory allocation...
//   + choose value close to the expected MINIMAL to favor efficient usage of MEMORY
//   + choose value close to the expected MAXIMAL to favor efficient usage of CPU
//   => and if you have no idea, simply specify BATEAU__EXPECTED_ITEMS_NUMBER #ENDREF
// - greenItemSize: size (in bytes) of a green item
// - n_greenHandlerDisengageFunction:
//   + NULL special value: no need to disengage <green item>s (=> "ULTRA-green" type)
//   + non NULL pointer: ad hoc function to release resources (called for each <green item>) before
//     destroying <green collection>)
// - n_greenHandlerCompareFunction:
//   + NULL special value: indexes not used in the <green collection>
//   + non NULL pointer: allows to use indexes in the <green collection>
// - n_greenHandlerEquateFunction:
//   + NULL special value: "non-indexed" requests not used in the <green collection>
//   + non NULL pointer: allows to combine "non-indexed" search in requests 
// - cfr_greenHandlerHandle: not significant if no "hook" function is provided ; handler's
//   private handle 
// TODO: ??? sequence fetching limit mechanism (protection against endless loops...)
//
// Modified
// - *azh_handle: <green collection> is instancied and initialized ; its handle
//
// Returned:
// - RETURNED: Ok,
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionCreateInstance(GREEN_COLLECTION_HANDLE *azh_handle,  int expectedItemsNumber,
  int greenItemSize,  GREEN_HANDLER__DISENGAGE_FUNCTION n_greenHandlerDisengageFunction,
  GREEN_HANDLER__COMPARE_FUNCTION n_greenHandlerCompareFunction,
  GREEN_HANDLER__EQUATE_FUNCTION n_greenHandlerEquateFunction, void *cfr_greenHandlerHandle) ;


// #REF smart-fetch-notices
// WARNING: "smart fetch"  operation :
// - is NOT allowed if the collection is "frozen"
// - invalidates current value of any variable pointing to the collection's items (t_ indicator)  
// - in particular, automatically cancels "pulled out" (pseudo-)mode - that is, the physical items
//   array that was pulled out is NO MORE usable. 
// Note: a "smart fetch" operation may increase:
// - the FORMAL item(s) count of the collection.
// and
// - the PHYSICAL number of items allocated in the collection 

// #REF fetched-item-notices
// NOTICES:
// - (t_ indicator) This stuff (pointer) is technically VALID until next "smart fetch" operation in
//   the collection. (see GreenCollectionFetch() and GreenCollectionIndexFetch() functions) 
// - If you use indexes (see GreenCollectionAddIndex() function), those indexes will be
//   automatically synchronized against that item (unless the collection is "frozen" in which
//   case index(es) synchronization is not necessary) in the course of next operation (i.e next
//   GreenCollection...() function call) on the collection.
//   That is, the initialization of or modifications in the key(s) values of the item MUST be
//   fulfilled STRAIGHT AWAY after that call. Otherwise, the index(es) may be NOT correctly synchronized. 
// - If the fetched item corresponds to an earlier "removed" item (so-called "gap"), the item has
//   the same state as before its "removal".


// #REF GreenCollectionFetch <greenItem> 
// Obtain an emplacement for a <greenItem> (in the collection).
// This function expects (and manage) presence of "gaps" in the collection. Gaps can only appear
// when index(es) is (are) enabled  (see GreenCollectionCreateInstance() below). If you do not use
// indexes, you are ensured to hold a "contiguous" collection (in which all items are simply added
// one after the other)...
//
// Passed:
// - cp_handle: collection handle - see GreenCollectionCreateInstance() ; "protected instance" if 
//   the collection is frozen.
// - n_entry: entry in the collection 
//   + -1 special value: "smart fetch" ; find free emplacement in the collection ; either use next
//     gap (if available) otherwise, add an emplacement at the end of the collection (this will
//     automatically increment the formal number of items in the collection (by 1)
//     #SEE smart-fetch-notices
//   + >= 0: entry for the item in the collection (0 == 1st entry) supposedly directly "fetchable" ;
//     that is, if that position exceeds that reflected by formal item(s) count, or indicates a 
//     "removed" item (i.e gap), then that entry will not be fetched.
// - acntr_<greenItem>Stuff: address of <greenItem> "stuff" to be fetched
//
// Modified:
// - *acntr_<greenItem>Stuff: corresponding <greenItem> "stuff" ; 
//   + NULL pointer: only possible with "direct fetch" (n_entry >= 0) ; the entry is not directly
//     "fetchable"
//   + otherwise: fetched item
//     #SEE fetched-item-notices
//
// Returned:
// - >= 0: OK, entry in the collection (NOTE: that entry is actually fetched if and only if
//         *acntr_<greenItem>Stuff != NULL)
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionFetch(GREEN_COLLECTION_HANDLE cp_handle, int n_entry,
  char **acntr_greenItemStuff) ;


// #REF GreenCollectionGetCount <greenItem> 
// Return the FORMAL number of <greenItem>(s) in the collection.
// NB: That formal count comprehend the possible "gaps" (when working with the indexes).
// (hense, the only way to decrease that number is to call GreenCollectionClear() - see below).
//
// Passed:
// - cp_handle: collection handle - see GreenCollectionCreateInstance() ; "protected instance" if 
//   the collection is frozen.
// - navntr_<greenItem>Stuff:
//   + NULL special pointer : do not need to fetch the last <greenItem>
//   + non NULL pointer: address of stuff pointing on <greenItem> to fetch 
//
// Modified:
// - *navntr_<greenItem>Stuff: (if asked) :
//   + NULL special address: current FORMAL <greenItem> count is 0 OR
//     the last item is NOT directly "fetchable" (i.e is a gap)
//   + non NULL: stuff pointing to the "last" item of the collection ;
//     #SEE fetched-item-notices
//
// Returned:
// - >= 0 : current "item(s) count" of the collection.
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionGetCount(GREEN_COLLECTION_HANDLE cp_handle, char **navntr_greenItemStuff ) ;


// #REF GreenCollectionAddIndex  <green item> 
// Add a "formal index" to the collection. Allows to sort and quickly retrieve <green item>s in the
// collection.
// NB: we also need to add a "formal index" to enable "non-indexed" requests.
// This function is only callable if the collection does support indexes (See the
// initialization function of the collection) 
// Notice: this operation is not possible when:
// - the collection is "frozen".
// OR
// - the collection is not empty (see GreenCollectionClear()) TODO: allow "hot" index creation 
//
// Passed:
// - handle: <green item>s collection handle 
// - keysNumber: >= 1; 1:plain index; >=2:compound index  #SKIP
//
// Returned: 
// - >= 0 : index label (0 for 1st index added, etc.)
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionAddIndex(GREEN_COLLECTION_HANDLE handle,  int keysNumber) ;


// * Truly index-based seek flags:
// NOT key-based seek flag: mutually exclusive with other truly index-based seek flags: 
#define INDEX_SEEK_FLAG__ANY     0x01
// Key-based seek flags: 
#define INDEX_SEEK_FLAG__EQUAL   0x02
#define INDEX_SEEK_FLAG__LESS    0x04
#define INDEX_SEEK_FLAG__GREATER 0x08 
// * NON-index based seek flag: mutually exclusive with truly index-based seek flags: 
#define INDEX_SEEK_FLAG__LIKE    0x10 

#endif // __C_ANSI_GREEN_H_INCLUDED__ == 0

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ __C_ANSI_GREEN_H_INCLUDED__
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

#if __C_ANSI_GREEN_H_INCLUDED__ == 0

// This variable is required by m_INDEX_REQUEST_AUTOMATIC_BUFFER() macro below. 
extern const int p_indexRequestAutomaticBufferSize;

typedef char *INDEX_REQUEST_AUTOMATIC_BUFFER ;

// This macro is useful when the collection is "frozen" (after a call to GreenCollectionFreeze()).
// Put (in stack) the needed "index request buffer" set-up by GreenCollectionIndexRequest() and used
// by GreenCollectionIndexFetch().
#define m_INDEX_REQUEST_AUTOMATIC_BUFFER(m_indexRequestAutomaticBuffer) \
char m_indexRequestAutomaticBuffer[p_indexRequestAutomaticBufferSize] ; 

#define CRITERIA_OP_FLAG__CLOSE1      0x001 // Close one bracket before op.
#define CRITERIA_OP_FLAG__CLOSE2      0x002 // Close two brackets before op.
#define CRITERIA_OP_FLAG__CLOSE3      0x004 // Close three brackets before op.
#define CRITERIA_OP_FLAG__AND         0x010 // And op.
#define CRITERIA_OP_FLAG__OR          0x020 // Or op.
#define CRITERIA_OP_FLAG__OPEN1       0x100 // Open one bracket after op. 
#define CRITERIA_OP_FLAG__OPEN2       0x200 // Open two brackets after op. 
#define CRITERIA_OP_FLAG__OPEN3       0x400 // Open three brackets after op. 

#endif // __C_ANSI_GREEN_H_INCLUDED__ == 0

#undef __FLINT_IMAGES_H_INCLUDED__
#define __FLINT_IMAGES_H_INCLUDED__ __C_ANSI_GREEN_H_INCLUDED__
#include "flint/images.h"

// #REF enum-CRITERIA_OP
m_DEFINE_ENUM_ALIAS_BEGIN(m_CriteriaOpFlagsImage)
  // Index request operators (flags) 
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__AND, CRITERIA_OP_FLAG__AND)
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__OR, CRITERIA_OP_FLAG__OR)
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__AND_OPEN,
    CRITERIA_OP_FLAG__AND | CRITERIA_OP_FLAG__OPEN1) 
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__OR_OPEN,
    CRITERIA_OP_FLAG__OR | CRITERIA_OP_FLAG__OPEN1)
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__CLOSE_AND,
    CRITERIA_OP_FLAG__CLOSE1 | CRITERIA_OP_FLAG__AND) 
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__CLOSE_OR,
    CRITERIA_OP_FLAG__CLOSE1 | CRITERIA_OP_FLAG__OR) 
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__CLOSE_AND_OPEN,
    CRITERIA_OP_FLAG__CLOSE1 | CRITERIA_OP_FLAG__AND | CRITERIA_OP_FLAG__OPEN1) 
  m_ENUM_ALIAS_VAL(CRITERIA_OP_FLAGS__CLOSE_OR_OPEN,
    CRITERIA_OP_FLAG__CLOSE1 | CRITERIA_OP_FLAG__OR | CRITERIA_OP_FLAG__OPEN1) 
m_DEFINE_ENUM_ALIAS_END()

#if __C_ANSI_GREEN_H_INCLUDED__ == 0

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

// name > 'a' open_and sex = 'F' or_close age > 20 

// #REF GreenCollectionIndexRequest <greenItem> <keys> 
// Construct a request for indexed fetch - see GreenCollectionIndexFetch()
//
// Passed:
// - handle:
// - nf_indexRequestAutomaticBuffer:
//   This buffer is mandatory when then collection is "frozen" (See GreenCollectionFreeze()) 
//   NULL special address: use internal structure => NOT "thread safe" / "re-entrant"
//   non NULL: buffer on stack => allows (thread) re-entrancy ; that buffer will be used by 
//   GreenCollectionIndexFetch() index fetching function... 
// - int criteriaNumber:
// - indexLabel1: (1st criterion) >= 0: see GreenCollectionCreateInstance()
// - indexSeekFlags1: (1st criterion)
// - cfpr_keys1:  (1st criterion) search key(s) value(s) of item (regarding index) ;
//   not significant without actual index seek flag (INDEX_SEEK_FLAGS__ANY)
// - optional criteria (...) : when criteriaNumber > 1
//  + criteriaOpFlags1 : only CRITERIA_OP_FLAGS__AND / CRITERIA_OP_FLAGS__AND_OPEN combinations are
//    allowed
//  + indexLabel2:
//  + c_indexSeekFlags2:
//  + cfpr_keys2: 
//  + criteriaOpFlags2 :
//  (etc.)
// 
// Changed:
// - nf_indexRequestAutomaticBuffer : (if used) initialized 
//
// Ret:
//
// - RETURNED: Ok
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionIndexRequest(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaNumber,
  int indexLabel1, unsigned int indexSeekFlags1, void *cfpr_keys1, ...);

// Indicates whether key value parameter is significant with index request... 
//
// Passed:
// - indexSeekFlags: SEE enum-INDEX_SEEK
//
// Return:
// - True : search key(s) value(s) is(are) significant with index request 
// - False : search key(s) value(s) is(are) not significant with index request 
#define b_SIGNIFICANT_GREEN_COLLECTION_INDEX_KEYS(/*unsigned int*/indexSeekFlags) \
  (b_FLAG_SET_OFF(indexSeekFlags,INDEX_SEEK_FLAG__ANY))

     
enum {
  INDEX_FETCH_FLAG__NEXT       = 0x01, 
  INDEX_FETCH_FLAG__RESET      = 0x02, 
  // INDEX_FETCH_FLAG [__CHANGE, __REMOVE and __READ] are mutually exclusive: 
  INDEX_FETCH_FLAG__CHANGE     = 0x04, // must be combined with INDEX_FETCH_FLAG__RESET
  INDEX_FETCH_FLAG__REMOVE     = 0x08, // must be combined with INDEX_FETCH_FLAG__RESET
  INDEX_FETCH_FLAG__SMART      = 0x10, // must be combined with INDEX_FETCH_FLAG [__RESET,
    // __NEXT and __CHANGE]
  INDEX_FETCH_FLAG__READ       = 0x20, // must be combined with INDEX_FETCH_FLAG__RESET
  INDEX_FETCH_FLAG__DESCENDING = 0x40, // must be combined with INDEX_FETCH_FLAG__RESET
} ;

#endif // __C_ANSI_GREEN_H_INCLUDED__ == 0

// #REF enum-INDEX_FETCH
m_DEFINE_ENUM_ALIAS_BEGIN(m_IndexFetchFlagsImage)
  // - "Compatible" modes:
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__FETCH             ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__CHANGE | INDEX_FETCH_FLAG__NEXT | INDEX_FETCH_FLAG__SMART) // If not found, do
    // "smart fetch" (*) (**)
    m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__SEEK_ONLY       ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__CHANGE | INDEX_FETCH_FLAG__NEXT) // No item added if doesn't exist (*)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__REMOVE            ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__REMOVE | INDEX_FETCH_FLAG__NEXT) // Remove from indexes (if exists) (*) (**)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__READ_ONLY         ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__READ | INDEX_FETCH_FLAG__NEXT) // No item added if doesn't exist ; this mode
    // actually disables indexes' synchronization and starts "read only" sequence" 
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__READ_NEXT         ,INDEX_FETCH_FLAG__NEXT) // read next
    // item of sequence
    // (*) : breaks select sequence if any
    // (**) : NOT ALLOWED if the collection is "frozen"
    // - "Extended" modes:
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__CURRENT           ,ALL_FLAGS_OFF0)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__NEXT              ,INDEX_FETCH_FLAG__NEXT) 
    // Prepare new selection ; NO item fetched (,> always RESULT__NOT_FOUND) :  
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__SELECT            ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__CHANGE)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__SELECT_READ       ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__READ)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__RESET             ,INDEX_FETCH_FLAG__RESET)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__SELECT_DESCENDING ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__CHANGE | INDEX_FETCH_FLAG__DESCENDING)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__SELECT_READ_DESCENDING ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__READ | INDEX_FETCH_FLAG__DESCENDING)
  m_ENUM_ALIAS_VAL(INDEX_FETCH_FLAGS__RESET_DESCENDING  ,INDEX_FETCH_FLAG__RESET |
    INDEX_FETCH_FLAG__DESCENDING)
m_DEFINE_ENUM_ALIAS_END()

#if __C_ANSI_GREEN_H_INCLUDED__ == 0

// #REF GreenCollectionIndexFetch <greenItem> <keys> 
// Fetch item of collection using an index.
//
// Passed:
// - cp_handle: collection handle - see GreenCollectionCreateInstance() ; "protected instance" if 
//   the collection is frozen.
// - nf_indexRequestAutomaticBuffer:
//   This buffer is mandatory when the collection is "frozen" (See GreenCollectionFreeze()) 
//   NULL special address: use internal structure => NOT "thread safe" / "re-entrant"
//   non NULL: buffer on stack => allows (thread) re-entrancy ; that buffer must correspond to that 
//   passed to GreenCollectionIndexRequest() function... 
// - indexFetchFlags: #SEE enum-INDEX_FETCH
//   #SEE smart-fetch-notices
// - nacvn_entry: NULL address if not used
//
// Changed:
// - *acvntr_<greenItem>Stuff: 
//   + status RESULT__FOUND: (never NULL) corresponding green item "stuff"0
//   + status RESULT__NOT_FOUND: 
//     . with INDEX_FETCH_FLAGS__FETCH: new item is at your disposal ; note that item MAY BE yet in
//       "engaged" state when the emplacement corresponded to a gap that has been "recycled"...
//     . otherwise (INDEX_FETCH_FLAGS__SEEK_ONLY / READ_ONLY / READ_NEXT / REMOVE): set to NULL
//       special value
//   #SEE fetched-item-notices
// - *nacvn_entry: (if used)
//   + status RESULT__FOUND: corresponding item entry 
//   + status RESULT__NOT_FOUND: 
//     . with INDEX_FETCH_FLAGS__FETCH: entry of new item at your disposal 
//     . otherwise (INDEX_FETCH_FLAGS__SEEK_ONLY / READ_ONLY / READ_NEXT / REMOVE): set to -1
//       special value
//
// returned status :
// - RESULT__FOUND: Found item in index.
// - RESULT__NOT_FOUND: not found via this index ;
//   + if you "enforced" fetching (INDEX_FETCH_FLAGS__FETCH) : the fetched item is "new"
//   + otherwise, NO item has been fetched
// - -1: unexpected problem ; anomaly is raised
// TODO: FETCH new: mechanism to check that the inserted item matches with the search key 
int GreenCollectionIndexFetch(GREEN_COLLECTION_HANDLE cp_handle, 
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, unsigned int indexFetchFlags, 
  char **acvntr_greenItemStuff, int *nacvn_entry);
// TODO : prevoir mechanisme optionel qui verifie qu'une fois insere, le NOUVEL item
//        est bien vu par l'index...



// Refresh the indexes regarding last "fetched" item.
// You can call that function just after having initialized / updated fetched item's content.
// However such calls are actually not necessary since indexes synchronization is made
// automatically.
// TODO: -soit on supprime cette fonction publique 
// -soit on permet de "disabler" la synchronisation automatique des index.
//
// Returned:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionRefreshIndexes(GREEN_COLLECTION_HANDLE handle); 


// #REF GreenCollectionVerifyIndexes <green item>
// TODO: si on peut pas appeler cette  fonction si la collection est geled, c'est FOIREUX.
// Verify that the indexes are correctly synchronized vis-a-vis the collection.
// Indeed, a desynchronisation might occur if, "by accident", you modify items fetched in a "read
// only" sequence.
//
// return :
// - COMPLETED__OK:
// - COMPLETED__BUT: issue(s) detected ; if you are sure not having modified the items in a "read
//   only" sequence, then consider this case is abnormal (and promptly abort program execution...)
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionVerifyIndexes(GREEN_COLLECTION_HANDLE handle) ;


// If GreenCollectionVerifyIndexes() has indicated an index desynchronization but
// you are certain using the indexes correctly, you can raise that anomaly...
#define ANOMALY__CORRUPTED_INDEXES "Index(es) is (are) corrupted!"


// To be called when you "switch" into "pull out" mode - allows to access to all items of the
// collection "at the same time" and avoid multiple calls to GreenCollectionFetch() function...
// NOTICE: You MAY NOT "pull out" the collection if you use index(es).
//
// Passed:
// - handle: see GreenCollectionCreateInstance()
// - ap_greenArray: green array pointer's address 
//
// Changed:
// - *atp_greenArray: PHYSICAL green array (start address) ; 
//   NOTICE: (t_) the array is technically valid till a "smart fetch" is operated in the array. 
//   (See GreenCollectionFetch() and GreenCollectionIndexFetch() functions).
//
// Returned:
// - >= 0: number of PHYSICAL items (> 0)
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionPullOut(GREEN_COLLECTION_HANDLE handle,  char **at_greenArray) ;

// #REF GreenCollectionFreeze
// "Freeze" the collection:
// - forbids any further (formal) modification of the collection.
// - disables index(es) synchronization
// - obliges usage of automatic index iterator with GreenCollectionIndexFetch() function 
// - warns keys comparison function that handler's (private) instance must be protected (non modifiable)
// => allow to consider the collection globally as a "protected instance" (i.e "thread safe") ;
//    the protection of the handler's private instance of the keys comparison function is of
//    course purely "formal"...
// Note: for sake of simplicity, it is not possible to "un-freeze" a collection...
//
// Passed;
// - handle: see GreenCollectionCreateInstance() 
// - nap_greenArray:
//   + NULL special pointer: not used
//   + != NULL: you want "direct access" to the green items of the collection
//
// Changed:
// - *nap_greenArray: green array (start address) ; 
//   WARNING: you MAY NOT modify the green items via this array
//   (though there is no technical restriction...) 
//
// Returned:
// - >= 0: number of PHYSICAL items (> 0)
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionFreeze(GREEN_COLLECTION_HANDLE handle,  char **nap_greenArray) ;

// TODO: interest de cette macro (dans .h du moins ) ???
// Get green item stuff of raw green array (as raw address)...
//
// Passed:
// - u_greenArray: see GreenCollectionPullOut() / GreenCollectionFreeze() 
// - u_entry: >= 0 (0 for 1st entry, etc.)
// - u_greenItemSize: that passed to GreenCollectionCreateInstance()...
//
// Ret: green item (raw address) 
#define /*char* */ r_GET_GREEN_ITEM_STUFF(/*char* */ u_greenArray,  /*int*/ u_entry,\
  /*int*/ u_greenItemSize) \
( (u_greenArray) + (u_entry)*(u_greenItemSize) )


// TODO: interest de cette macro (dans .h du moins ) ???
// Get green item stuff of raw green array (exact stuff type)...
//
// Passed:
// - u_greenArray: see GreenCollectionPullOut() / GreenCollectionFreeze() 
// - u_entry: >= 0 (0 for 1st entry, etc.)
// - m_actualGreenItemType: the ACTUAL type of green item...
//
// Ret: green item stuff (address of green item's ACTUAL type) 
#define GET_ACTUAL_GREEN_ITEM_STUFF(/*char* */ u_greenArray,  /*int*/ u_entry,\
  m_actualGreenItemType)  ( ((m_actualGreenItemType *)(u_greenArray)) + (u_entry) )


// #REF GreenCollectionClear <green item> - Reset <green item>s collection, that is 
// reset FORMAL items count to 0 (but DO NOT touch at the physical items)
// Any attached index is also "cleared"...
// Notice: this operation is not possible if the collection is "frozen".
//
// Passed:
// - handle: <green item>s collection handle
//
// Modified:
// - handle: reset (i.e formal number of <green item>s == 0)
//
// Returned:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionClear(GREEN_COLLECTION_HANDLE handle) ;


// #REF GreenCollectionDestroyInstance <green item> - Release and destroy a <green item> collection.
//
// Returned:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int GreenCollectionDestroyInstance(GREEN_COLLECTION_HANDLE xh_handle) ;

#endif // __C_ANSI_GREEN_H_INCLUDED__ == 0

// Manage double inclusion: update inclusion state:
#if __C_ANSI_GREEN_H_INCLUDED__ == 0
#undef __C_ANSI_GREEN_H_INCLUDED__
// Manage double inclusion: provoke 2nd inclusion:
#define __C_ANSI_GREEN_H_INCLUDED__ 2
#include "c-ansi/green.h"
#else
#undef __C_ANSI_GREEN_H_INCLUDED__
#define __C_ANSI_GREEN_H_INCLUDED__ 3
#endif

#endif // __C_ANSI_GREEN_H_INCLUDED__ == 0 || __C_ANSI_GREEN_H_INCLUDED__ == 2
