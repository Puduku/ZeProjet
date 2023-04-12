// c-ansi/green.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_GREEN_H_INCLUDED
#define __C_ANSI_GREEN_H_INCLUDED

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/magic.h"



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
#define INDEX_LABEL1 1

// #REF GREEN_HANDLER__KEYS_COMPARE_FUNCTION <GreenItem> - Custom function definition...
// Extract A and B key(s) value(s) from green items and compare those two keys values.
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
typedef int (*GREEN_HANDLER__KEYS_COMPARE_FUNCTION) (void *cpr_handle,  char b_frozen,
  int indexLabel, int keyRank,  char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff,
  void *cpr_bKeys) ;

 
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
// - n_greenHandlerKeysCompareFunction:
//   + NULL special value: indexes not used in the <green collection>
//   + non NULL pointer: allows to use indexes in the <green collection>
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
  GREEN_HANDLER__KEYS_COMPARE_FUNCTION n_greenHandlerKeysCompareFunction,
  void *cfr_greenHandlerHandle) ;


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
// Add an index to the collection. Allows to sort and quickly retrieve <green item>s in the
// collection.
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


enum { // #REF enum-INDEX_FETCH
  INDEX_FETCH__FETCH,      // if not found, do "smart fetch" (*) (**)
  INDEX_FETCH__SEEK_ONLY,  // No item added if doesn't exist (*)
  INDEX_FETCH__READ_ONLY,  // no item added if doesn't exist ; this mode actually
                           // disables indexes' synchronization and starts "read only" sequence" ;
                           // see INDEX_FETCH__READ_NEXT
  INDEX_FETCH__READ_NEXT,  // no item added if doesn't exist ; read next item of
                           // "read only" sequence started with INDEX_FETCH__READ_ONLY
  INDEX_FETCH__REMOVE,     // Remove from indexes (if exists) (*) (**)
                           // (*) : breaks "read only" sequence if any
                           // (**) : NOT ALLOWED if the collection is "frozen"
} ;

enum { // #REF enum-INDEX_SEEK
  // seek in collection regarding index
  INDEX_SEEK__UP, // Seek FIRST (lowest) item of collection in index (and look up)
  INDEX_SEEK__DOWN, // Seek LAST (highest) item of collection in index (and look down)
  INDEX_SEEK__KEY, // Seek item matching key
  INDEX_SEEK__BOTTOM_UP, // Seek FIRST (lowest) among items matching or FOLLOWING key (and look up)
  INDEX_SEEK__BOTTOM_DOWN, // Seek LAST (highest) among items matching or FOLLOWING key (and look down)
  INDEX_SEEK__TOP_DOWN, // Seek LAST (highest) among items matching or PRECEDING key (and look down)
  INDEX_SEEK__TOP_UP, // Seek FIRST (lowest) among items matching or PRECEDING key (and look up)
} ;

// Example: fetching an item in that collection:
// +-- FIRST (i.e "lowest") item in collection with index
// |        +-- LAST (i.e "highest") item in collection with index
// v        v
// B; C; E; F
//
// => Simple fetch (No sequence); NOT valid for INDEX_FETCH__READ_NEXT
//      Key         : A  B  C  D  E  F  G
//      -----------------------------------
// I S| UP (*)      : B  B  B  B  B  B  B  |F I
// N E| DOWN (*)    : F  F  F  F  F  F  F  |E T
// D E| KEY         : -  B  C  -  E  F  -  |T E
// E K| BOTTOM_UP   : B  B  C  E  E  F  -  |C M
// X _| BOTTOM_DOWN : F  F  F  F  F  F  -  |H
// _ _| TOP_DOWN    : -  B  C  C  E  F  F  |E
//    | TOP_UP      : -  B  B  B  B  B  B  |D
// Legend: -: not found    (*): search key is not significant
//
// => Fetching in "read only" sequence;
//    INDEX_FETCH__READ_ONLY / READ_NEXT
//      #call (*)     :#1 #2 #3 #4 #5 #6
//      --------------------------------
// I S| UP            : B  C  E  F  -  - |F I
// N E| DOWN          : F  E  C  B  -  - |E T
// D E| KEY A         : -  -  -  -  -  - |T E
// E K| KEY B         : B  -  -  -  -  - |C M
// X _| KEY C         : C  -  -  -  -  - |H
// _ _| KEY D         : -  -  -  -  -  - |E
//    | KEY E         : E  -  -  -  -  - |D
//   (| KEY F         : F  -  -  -  -  - |
//   K| KEY G         : -  -  -  -  -  - |
//   e| BOTTOM_UP A   : B  C  E  F  -  - |
//   y| BOTTOM_UP B   : B  C  E  F  -  - |
//   )| BOTTOM_UP C   : C  E  F  -  -  - |
//    | BOTTOM_UP D   : E  F  -  -  -  - |
//    | BOTTOM_UP E   : E  F  -  -  -  - |
//    | BOTTOM_UP F   : F  -  -  -  -  - |
//    | BOTTOM_UP G   : -  -  -  -  -  - |
//    | BOTTOM_DOWN A : F  E  C  B  -  - |
//    | BOTTOM_DOWN B : F  E  C  B  -  - |
//    | BOTTOM_DOWN C : F  E  C  -  -  - |
//    | BOTTOM_DOWN D : F  E  -  -  -  - |
//    | BOTTOM_DOWN E : F  E  -  -  -  - |
//    | BOTTOM_DOWN F : F  -  -  -  -  - |
//    | BOTTOM_DOWN G : -  -  -  -  -  - |
//    | TOP_DOWN A    : -  -  -  -  -  - |
//    | TOP_DOWN B    : B  -  -  -  -  - |
//    | TOP_DOWN C    : C  B  -  -  -  - |
//    | TOP_DOWN D    : C  B  -  -  -  - |
//    | TOP_DOWN E    : E  C  B  -  -  - |
//    | TOP_DOWN F    : F  E  C  B  -  - |
//    | TOP_DOWN G    : F  E  C  B  -  - |
//    | TOP_UP A      : -  -  -  -  -  - |
//    | TOP_UP B      : B  -  -  -  -  - |
//    | TOP_UP C      : B  C  -  -  -  - |
//    | TOP_UP D      : B  C  -  -  -  - |
//    | TOP_UP E      : B  C  E  -  -  - |
//    | TOP_UP F      : B  C  E  F  -  - |
//    | TOP_UP G      : B  C  E  F  -  - |
// Legend: -: not found
//         (*): call #1           => READ_ONLY
//              calls #2, #3, ... => READ_NEXT

// This variable is required by m_INDEX_ITERATOR_AUTOMATIC_BUFFER() macro below. 
extern const int p_indexIteratorAutomaticBufferSize;

typedef char *INDEX_ITERATOR_AUTOMATIC_BUFFER ;

// This macro is useful when the collection is "frozen" (after a call to GreenCollectionFreeze()).
// Put (in stack) the needed "index iterator buffer" needed by GreenCollectionIndexFetch()
#define m_INDEX_ITERATOR_AUTOMATIC_BUFFER(m_indexIteratorAutomaticBuffer) \
char m_indexIteratorAutomaticBuffer[p_indexIteratorAutomaticBufferSize] ; 


// #REF GreenCollectionIndexFetch <greenItem> <keys> 
// Fetch item of collection using an index.
//
// Passed:
// - cp_handle: collection handle - see GreenCollectionCreateInstance() ; "protected instance" if 
//   the collection is frozen.
// - nf_indexIteratorAutomaticBuffer:
//   This buffer is mandatory when then collection is "frozen" (See GreenCollectionFreeze()) 
//   NULL special address: use internal structure => NOT safe for concurrency (i.e "thread safe"
//   and re-entrant)
//   non NULL: buffer on stack => allows (thread) re-entrancy ; SAME buffer must
//   be presented in index fetching reading sequence (INDEX_FETCH__READ_ONLY / READ_NEXT)
// - indexLabel: >= 0: see GreenCollectionCreateInstance()
//   NOTICE: changing index label automatically "resets" "read sequence" (see INDEX_FETCH__READ_NEXT)
// - indexFetch: #SEE enum-INDEX_FETCH
//   #SEE smart-fetch-notices
// - c_indexSeek: not significant with INDEX_FETCH__READ_NEXT ; #SEE enum-INDEX_SEEK
// - nacvn_entry: NULL address if not used
// - ccr_<keys>: search key(s) value(s) of item (regarding index) ;
//   not significant with INDEX_FETCH__READ_NEXT ;
//   not significant with INDEX_SEEK__UP / INDEX_SEEK__DOWN 
//
// Changed:
// - *acvntr_<greenItem>Stuff: (if used) :
//   + status RESULT__FOUND: (never NULL) corresponding green item "stuff" (=> ready for "update")
//   + status RESULT__NOT_FOUND: 
//     . with INDEX_FETCH__FETCH: new item is at your disposal ; note that item MAY BE yet in
//       "engaged" state when the emplacement corresponded to a gap that has been "recycled"...
//     . otherwise (INDEX_FETCH__SEEK_ONLY/READ_ONLY/READ_NEXT/REMOVE): set to NULL special value
//   #SEE fetched-item-notices
// - *nacvn_entry:
//   + status RESULT__FOUND: corresponding item entry 
//   + status RESULT__NOT_FOUND: 
//     . with INDEX_FETCH__FETCH: entry of new item at your disposal 
//     . otherwise (INDEX_FETCH__SEEK_ONLY/READ_ONLY/READ_NEXT/REMOVE): set to -1 special value
//
// returned status :
// - RESULT__FOUND: Found item in index.
// - RESULT__NOT_FOUND: not found via this index ;
//   + if you "enforced" fetching (INDEX_FETCH__FETCH) : the fetched item is "new"
//   + otherwize, NO item has been fetched
// - -1: unexpected problem ; anomaly is raised
// TODO: FETCH new: mechanism to check that the inserted item matches with the search key 
int GreenCollectionIndexFetch(GREEN_COLLECTION_HANDLE cp_handle,
  INDEX_ITERATOR_AUTOMATIC_BUFFER nf_indexIteratorAutomaticBuffer,  int indexLabel,
  int indexFetch,  int c_indexSeek,  char **acvntr_greenItemStuff,  int *nacvn_entry,
  void *ccr_keys);
// TODO: add *na_remainingCount param (utile pour si on a des doublons)...
// TODO : prevoir mechanisme optionel qui verifie qu'une fois insere, le NOUVEL item
//        est bien vu par l'index...


// Indicates whether index key parameter is significant with index fetching
//
// Passed:
// - indexFetch: SEE enum-INDEX_FETCH
// - c_indexSeek: not significant with INDEX_FETCH__READ_NEXT ; SEE enum-INDEX_SEEK
//
// Return:
// - True : search key(s) value(s) is(are) significant with index fetching
// - False : search key(s) value(s) is(are) not significant with index fetching
#define b_SIGNIFICANT_GREEN_COLLECTION_INDEX_KEYS(/*int*/indexFetch, /*int*/c_indexSeek) \
  ((indexFetch) != INDEX_FETCH__READ_NEXT && (c_indexSeek) != INDEX_SEEK__UP && (c_indexSeek) !=\
  INDEX_SEEK__DOWN)


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


#endif // __C_ANSI_GREEN_H_INCLUDED
