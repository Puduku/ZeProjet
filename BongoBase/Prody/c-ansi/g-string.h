// c-ansi/g-string.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_G_STRING_H_INCLUDED
#define __C_ANSI_G_STRING_H_INCLUDED

#include <string.h>

#include "c-ansi/diggy.h"
#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/green.h"
#include "c-ansi/p-string.h"


// structure of g-string instance (GREEN item) :
struct G_STRING { // #REF struct-G_STRING
  // GREEN conventions for g-string:
  // "disengaged" state <=> nhi_string == NULL
  // "engaged" state (also called "copied" state) <=> nhi_string != NULL
  char *nhi_string; // ALWAYS '\0'-terminated string (when "copied")
  int c_bufferSize; // only significant in "copied" (alias "engaged") state (>= 1)
  int c_copiedLength; // idem ; (>= 0) ACTUAL string length (NOT taking into account ending '\0': 
  // ALWAYS < c_bufferSize ;
  // ALWAYS >= strlen(nhi_string) ;
  // WHEN == strlen(nhi_string), this "empiric" check asserts the string contains NO '\0' char
  // (excepted the ending '\0').
  struct P_STRING cv_pString; // g-string's "logical" string portion 
    // -when g-string is "copied" (nhi_string != NULL) : corresponds to g-string "physical" value
    // -when g-string is not "copied" (nhi_string == NULL) : pure "logical" string portion 
  union { // according to g-string's conveyance:
    GENERIC_INTEGER cen_value; // 'valued string' : its acolyt value ; default value is 0
    void *cnhr_handle; // 'named object' : its head handle ; NULL means no head handle 
  } acolyt; 
} ;
// NOTICE: In initial ("zeroified") state, a g-string is "logically" an empty string... 

// Formal g-string stuff :
typedef struct G_STRING *G_STRING_STUFF;


// Get g-string's "logical" string. 
static inline struct P_STRING m_GStringGetLogicalPString(G_STRING_STUFF stuff) {
  return stuff->cv_pString; 
} // m_GStringGetLogicalPString 

// #REF GStringCopy
// Copy (or concatenate...) a string portion into a "g-string".
// ATTENTION: source and destination MAY NOT overlap
// Note1: concatenation of empty string is a neutral operation; however, if  "destination" was 
// "disengaged" (nhi_string == NULL), an empty string is well COPIED in destination. 
// Note2: the acolyt is NOT changed.
//
// Passed:
// - stuff : "destination" g-string
// - n_<offset>: offset in destination g-string to apply the copy
//   + -1 special value: "end of string"; set to current copied length => i.e concatenation
//   + >= 0: "offset copy" ; 0 corresponds to "ordinary" copy ;
//     REM: if offset > current copied length, the offset is rectified (concatenation)
// - pString: use empty string for neutral operation
//
// Modified:
// - stuff: ALWAYS in "copied" state (i.e nhi_string != NULL, whatever!)
//
// Returned:
// - >= 0: OK, copied (copied length of dest string)
// - -1: unexpected problem
int GStringCopy(G_STRING_STUFF stuff, int n_offset, struct P_STRING pString);


// Copy (or concatenate...) a g-string into a "g-string"
// Note1: the acolyt is NOT copied
// See GStringCopy() above
static inline int m_GStringGCopy(G_STRING_STUFF stuff, int n_offset, G_STRING_STUFF p_gStringStuff) {
  m_DIGGY_BOLLARD()
  struct P_STRING pString =  m_GStringGetLogicalPString(p_gStringStuff);
  m_DIGGY_RETURN(GStringCopy(stuff,n_offset,pString))
} // m_GStringGCopy


// Import "logically" a string portion in a g-string ; the string portion is not physically
// copied if g-string is purely "logical". 
// copy.
// TODO: edviter appel ag vraie fonction si la g-string est "purement logique"...
// 
// Passed:
// - stuff:
// - f_pString:
// 
// Changed:
// - stuff: remains pure "logical" if possible
//
// Returned:
// - COMPLETED__OK: imported as "pure" logical string portion 
// - COMPLETED__BUT: imported, but "physical" copy was needed ; if it's not normal, don't
//   hesitate to raise ANOMALY__NON_PURE_LOGICAL_G_STRING ...
// - -1: unexpected problem
int GStringImport(G_STRING_STUFF stuff, struct P_STRING f_pString);


// See GStringImport() above...
#define ANOMALY__NON_PURE_LOGICAL_G_STRING "NOT a pure logical g-string"


// #REF GStringPrintf
// Produce formatted output "alla printf" for a "g-string" (recipient)
//
// Passed:
// - stuff : "destination" g-string
// - n_offset: offset in destination g-string to apply the "printf"
//   + -1 special value : end of string => i.e concatenation
//   + >= 0: "offset copy" ; 0 corresponds to "normal" printf ;
//     REM: if offset > current length, the function / macro does a concatenation
// - p_format: "format" string (following "printf" conventions)
// - ... : arguments corresponding to format
//
// Returned:
// - >= 0: OK, length of string
// - -1: unexpected problem
int GStringPrintf(G_STRING_STUFF stuff,  int n_offset,  const char *p_format, ...);


// Convert g-string content 
// 
// Passed:
// - stuff : g-string to convert
// - n_isNeutralCharFunction: handling of "neutral" chars (white spaces, etc.) 
//   + NULL: NO char elimination applied before conversion
//   + != NULL: elimination of each "neutral" char in lieu of conversion
// - toCharFunction: 
// 
// Changed:
// - stuff : converted g-string 
//
// Ret:
// - COMPLETED_OK: g-string succesfully converted 
// - COMPLETED_BUT: g-string is a pure "logical" g-string => NOT converted 
// - -1: unexpected problem
int GStringConvert(G_STRING_STUFF stuff,  IS_CHAR_FUNCTION n_isNeutralCharFunction,
  TO_CHAR_FUNCTION toCharFunction) ;
                   

// "Clear" a g-string 
// 
// Passed:
// - stuff : "destination" g-string
//
// Modified:
// - stuff: "logically" empty string with "initial" acolyt
#define m_G_STRING_CLEAR(/*G_STRING_STUFF*/stuff) {\
  m_TRACK_IF(GStringImport(stuff,m_PString(GOOD_OLD_EMPTY_C_STRING)) < 0)\
  memset(&(stuff)->acolyt,0,sizeof((stuff)->acolyt));\
}
  

// "g-string set"
// --------------


// #REF G_STRING_SET_STUFF
// "g-string set" == array of g-strings (fixed number of elements)
// Hence, a g-string set stuff is exacltly the g-string stuff of the first element of the set.
// To retrieve a specific g-string element of a set, just apply the "good old" pointer arithmetic. 
typedef G_STRING_STUFF g_G_STRING_SET_STUFF;

#define FIRST_ELEMENT0 0

// #REF GStringSetCreateInstance <g-string set>.
// Creation and initialization of a single <g-string set>.
//
// Passed:
// - azh_stuff: address of <g-string set> stuff to initialize...
// - cardinality : #SKIP
//
// Modified
// - *azh_stuff: <g-string set> initialized (in "disengaged state")
//
// Ret:
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int GStringSetCreateInstance(g_G_STRING_SET_STUFF *azh_stuff,  int cardinality);

// #SEE GStringSetCreateInstance <g-string>
#define /*int*/ G_STRING_CREATE_INSTANCE(/*G_STRING_STUFF* */ azh_stuff) \
GStringSetCreateInstance(azh_stuff,1)


// #REF GStringSetDestroyInstance <g-string set>
// (Disengage and) free <g-string set> instance...
// ATTENTION: this function is NOT SUITABLE for g-strings conveyed as 'named objects'.
//
// Passed:
// - xh_notNamedObjectStuff: 
// - cardinality: see GStringSetCreateInstance() 
//
// Modifed ;
// - xh_notNamedObjectStuff: this stuff is no more valid 
// 
// Returned
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int GStringSetDestroyInstance(g_G_STRING_SET_STUFF xh_notNamedObjectStuff,  int cardinality);

// #SEE GStringSetDestroyInstance <g-string>
#define /*int*/ G_STRING_DESTROY_INSTANCE(/*G_STRING_STUFF*/ xh_notNamedObjectStuff) \
  GStringSetDestroyInstance(xh_notNamedObjectStuff,1)


// "g-strings collections"
// -----------------------

struct G_STRINGS ; // Private structure !
typedef struct G_STRINGS * G_STRINGS_HANDLE; // Public handle

// #REF enum-G_STRING_CONVEYANCE
enum { //
  VALUED_STRING__G_STRING_CONVEYANCE, // acolyt is value
   NAMED_OBJECT__G_STRING_CONVEYANCE, // acolyt is object handle
} ;

// Passed:
// -xhr_handle: head handle of instance to liquidate 
//
// Ret:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised 
typedef int (*NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION) (void *xhr_handle) ;

// #REF GStringsCreateInstance <g-string set>
// Creation and initialization of a "<g-string set> collection".
// 
// Passed:
// - azh_handle: (address of) new handle to initialize
// - expectedItemsNumber:
//   #SEE GreenCollectionCreateInstance-expectedItemsNumber@c-ansi/green.h <<g-string set>>
// - gStringSetCardinality: number of elements in a set (>0 ; 0 value is taken for a BUG) #SKIP
// - n_gStringConveyance: (-1 special value: not used) general conveyance
//   #see enum-G_STRING_CONVEYANCE 
// - cnfps_gStringConveyances: (not significant if general conveyance is specified; NULL special
//   value: not used) 
// - c_namedObjectDestroyInstanceFunction: only significant with NAMED_OBJECT__G_STRING_CONVEYANCE
// 
// Modified:
// - *azh_handle:
//
// Returned:
// - RETURNED: Ok, <g-string set> collection is instancied
// - -1: unexpected problem; anomaly is raised 
int GStringsCreateInstance(G_STRINGS_HANDLE* azh_handle, int expectedItemsNumber,
  int gStringSetCardinality, int n_gStringConveyance, const int *cnfps_gStringConveyances,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION c_namedObjectDestroyInstanceFunction);

// #REF G_STRINGS_CREATE_INSTANCE <g-string>
// #SEE GStringsCreateInstance <g-string>
#define /*int*/ G_STRINGS_CREATE_INSTANCE(/*G_STRINGS_HANDLE*/ azh_handle,\
  /*int*/ expectedItemsNumber)  GStringsCreateInstance(azh_handle,expectedItemsNumber,1,-1,NULL,\
  (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED)


// #REF GStringsFetch <gStringSet> 
// #SEE GreenCollectionFetch @ c-ansi/green.h <<gStringSet>>
int GStringsFetch(G_STRINGS_HANDLE cp_handle, int n_entry, g_G_STRING_SET_STUFF* acnt_gStringSetStuff);


// #REF GStringsGetCount <gStringSet> 
// #SEE GreenCollectionGetCount@c-ansi/green.h <<gStringSet>>
int GStringsGetCount (G_STRINGS_HANDLE cp_handle,  g_G_STRING_SET_STUFF *navnt_gStringSetStuff);


enum { // #REF enum-G_KEYS_COMPARISON 
        P_STRING__G_KEYS_COMPARISON, //
 INTRINSIC_VALUE__G_KEYS_COMPARISON, //
    ACOLYT_VALUE__G_KEYS_COMPARISON, // mutually exclusive with other ACOLYT_*__G_KEYS_COMPARISON
   ACOLYT_HANDLE__G_KEYS_COMPARISON, // mutually exclusive with other ACOLYT_*__G_KEYS_COMPARISON
} ;

// #REF P_STRING_INTRINSIC_VALUE_FUNCTION - "custom" function definition
// 
// Passed:
// - pr_handle: private handle ; protected against modification (i.e "thread safe")
// TODO: protection conditionnelle à l'edtat "frozen" ???? 
// - ap_pString: 
// 
// Ret:
// (Intrinsic) value corresponding to string portion
typedef GENERIC_INTEGER (*P_STRING_INTRINSIC_VALUE_FUNCTION) (void *pr_handle,
  const struct P_STRING *ap_pString);


// #REF GStringsAddIndex <g-string set> <key1>
// #SEE GreenCollectionAddIndex@c-ansi/green.h  <<g-string set>> 
// Passed parameters for new index:
// - keysNumber: >= 1; 1:plain index; >=2:compound index  #SKIP
// - key1GStringSetElement: between [0 ; <g-string set cardinality>[ ; the g-string set's
//   element serving as first key of the index
// - key1GKeysComparison: #see enum-G_KEYS_COMPARISON
//   ACOLYT_VALUE__G_KEYS_COMPARISON : only possible with VALUED_STRING__G_STRING_CONVEYANCE
//   ACOLYT_HANDLE__G_KEYS_COMPARISON : only possible with NAMED_OBJECT__G_STRING_CONVEYANCE
// - cn_key1IsNeutralCharFunction: only significant with P_STRING__G_KEYS_COMPARISON 
//   + NULL: DO NOT eliminate neutral chars before comparison 
//   + non NULL: eliminate neutral chars before comparison 
// - cn_key1ToCharFunction: only significant with P_STRING__G_KEYS_COMPARISON 
//   + NULL: NO conversion applied before comparison 
//   + non NULL: conversion applied before comparison 
// - c_key1PStringIntrinsicValueFunction: only significant with INTRINSIC_VALUE__G_KEYS_COMPARISON
// - cfpr_key1PStringIntrinsicValueFunctionHandle: only significant with
//   INTRINSIC_VALUE__G_KEYS_COMPARISON
// - ... : g-string set(s element for second key (etc.) if any ...
int GStringsAddIndex(G_STRINGS_HANDLE handle,  int keysNumber, int key1GStringSetElement,
  int key1GKeysComparison,  IS_CHAR_FUNCTION cn_key1IsNeutralCharFunction,
  TO_CHAR_FUNCTION cn_key1ToCharFunction,
  P_STRING_INTRINSIC_VALUE_FUNCTION c_key1PStringIntrinsicValueFunction,
  void *cfpr_key1PStringIntrinsicValueFunctionHandle,  ...);

// #SEE GStringsAddIndex <g-string> <key>
// Add an index for single (aka cardinality 1) g-strings collection for LEXICAL comparison
// Nb: Plain index => one single key
static inline int m_GStringsAddPlainLexicalIndex(G_STRINGS_HANDLE handle,
  IS_CHAR_FUNCTION n_keyIsNeutralCharFunction, TO_CHAR_FUNCTION n_keyToCharFunction) {
  m_DIGGY_BOLLARD_S()
  m_DIGGY_RETURN(GStringsAddIndex(handle, 1, 0, P_STRING__G_KEYS_COMPARISON,
     n_keyIsNeutralCharFunction, n_keyToCharFunction, (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,
     (void*)UNDEFINED))
} // m_GStringsAddPlainLexicalIndex


// (Internal use)
union BARE_G_KEY {
  struct P_STRING cp_pString; // with P_STRING__G_KEYS_COMPARISON
  GENERIC_INTEGER cen_intrinsicValue; // with INTRINSIC_VALUE___G_KEYS_COMPARISON
  GENERIC_INTEGER cen_acolytValue; // with ACOLYT_VALUE__G_KEYS_COMPARISON 
  void *cnr_acolytHandle; // with ACOLYT_HANDLE__G_KEYS_COMPARISON
}; 

// #REF struct-G_KEY
struct G_KEY { //
  int gKeysComparison ; // #SEE enum-G_KEYS_COMPARISON
  union BARE_G_KEY bare ;// bare g-key 
}; 

// Establish a g-key for string (lexical) comparison
//
// Passed:
// - p_pString:
//
// Ret:
// - g-key
static inline struct G_KEY m_GKey_PString(const struct P_STRING p_pString) {\
  struct G_KEY gKey = { .gKeysComparison = P_STRING__G_KEYS_COMPARISON,
    .bare.cp_pString = p_pString }; 
  return gKey;
} // GKey_PString 

// Establish a g-key for 'intrinsic (generic) values' comparison
//
// Passed:
// - en_intrinsicValue:
//
// Ret:
// - g-key
static inline struct G_KEY m_GKey_IntrinsicValue(GENERIC_INTEGER en_intrinsicValue) {\
  struct G_KEY gKey = { .gKeysComparison = INTRINSIC_VALUE__G_KEYS_COMPARISON,
    .bare.cen_intrinsicValue = en_intrinsicValue }; 
  return gKey;
} // GKey_PString 


// Establish a g-key for 'acolyt (generic) values' comparison
//
// Passed:
// - uen_acolytValue:
//
// Ret:
// - g-key
static inline struct G_KEY m_GKey_AcolytValue(GENERIC_INTEGER en_acolytValue) {\
  struct G_KEY gKey = { .gKeysComparison = ACOLYT_VALUE__G_KEYS_COMPARISON,
    .bare.cen_acolytValue = en_acolytValue }; 
  return gKey;
} // GKey_PString 

// Establish a g-key for 'acolyt handles' comparison
//
// Passed:
// - unr_acolytHandle:
//
// Ret:
// - g-key
static inline struct G_KEY m_GKey_AcolytHandle(void* nr_acolytHandle) {\
  struct G_KEY gKey = { .gKeysComparison = ACOLYT_HANDLE__G_KEYS_COMPARISON,
    .bare.cnr_acolytHandle = nr_acolytHandle }; 
  return gKey;
} // GKey_PString 


// #REF GStringsIndexRequest <gStringSet>
// #SEE GreenCollectionIndexRequest@c-ansi/green.h  <<gStringSet>> <keys> 
// Passed keys :
// - cfps_keys1: (1st criterion) search key(s) value(s) of item (regarding index) ;
//   not significant without actual index seek flag (INDEX_SEEK_FLAGS__ANY)
//   type of comparison (see enum-G_KEYS_COMPARISON) must correspond to that assigned to index
//   (see GStringsAddIndex() above) 
// - ... (variadic parameters) : search key(s) value(s) for other criteria...  
int GStringsIndexRequest(G_STRINGS_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaNumber,
  int indexLabel1, unsigned int indexSeekFlags1, const struct G_KEY *cfps_keys1, ...);


// #SEE m_GRequestCriterium  
static inline struct G_REQUEST_CRITERIUM m_GRequestCriterium_GKeys(int indexLabel,
  unsigned int indexSeekFlags, const struct G_KEY* cfps_keys, unsigned int u_criteriaOpFlags) {
  return m_GRequestCriterium(indexLabel,indexSeekFlags, cfps_keys, u_criteriaOpFlags);
} // m_GRequestCriterium_GKeys

// #SEE GStringsIndexRequest <gStringSet>
int GStringsIndexRequestR(G_STRINGS_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaNumber,
  const struct G_REQUEST_CRITERIUM *sp_criteria) ;


// #REF GStringsIndexFetch <gStringSet>
// #SEE GreenCollectionIndexRequest@c-ansi/green.h  <<gStringSet>> 
int GStringsIndexFetch(G_STRINGS_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, unsigned int indexFetchFlags,
  g_G_STRING_SET_STUFF *acvnt_gStringSetStuff, int *nacvn_entry);

// Simple fetch (one single criterium)
// #SEE GStringsIndexRequest <gStringSet>
// #SEE GStringsIndexFetch <gStringSet>
static inline int m_GStringsIndexSingleFetch(G_STRINGS_HANDLE cp_handle, 
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, 
  int indexLabel, unsigned int indexSeekFlags, const struct G_KEY *cfps_keys,
  unsigned int indexFetchFlags, g_G_STRING_SET_STUFF *acvnt_gStringSetStuff, int *nacvn_entry) {
  m_DIGGY_BOLLARD_S() 
  m_TRACK_IF(GStringsIndexRequest(cp_handle,nf_indexRequestAutomaticBuffer,1,indexLabel,
    indexSeekFlags, cfps_keys) != RETURNED) 
  int result = GStringsIndexFetch(cp_handle,nf_indexRequestAutomaticBuffer,indexFetchFlags,
    acvnt_gStringSetStuff,nacvn_entry);
  m_TRACK_IF(result < 0)
  m_DIGGY_RETURN(result)
} // m_GStringsIndexSingleFetch
  

// #REF GStringsVerifyIndexes <g-string set>
// #SEE GreenCollectionVerifyIndexes@c-ansi/green.h  <<g-string set>>
int GStringsVerifyIndexes(G_STRINGS_HANDLE handle);


typedef struct G_STRING *G_STRINGS_ARRAY;


// #REF GStringsPullOut
// #SEE GreenCollectionPullOut @ c-ansi/green.h
int GStringsPullOut(G_STRINGS_HANDLE handle, G_STRINGS_ARRAY *at_gStringsArray);


// #REF GStringsFreeze
// #SEE GreenCollectionFreeze @ c-ansi/green.h
int GStringsFreeze(G_STRINGS_HANDLE handle, G_STRINGS_ARRAY **nap_gStringsArray);


// #REF GET_G_STRING_SET_STUFF <g-string set>
// Get particular item of a <g-string-set> array...
//
// Passed:
// - u_greenArray: see GStringsPullOut() / GStringsFreeze() 
// - u_entry: >= 0 (0 for 1st entry, etc.)
// - u_gStringSetCardinality: that passed to GStringsCreateInstance()...
//
// Ret: <g-string-set> stuff 
#define /*g_G_STRING_SET_STUFF*/ GET_G_STRING_SET_STUFF(/*char* */ u_greenArray,  /*int*/ u_entry,\
  /*int*/ u_gStringSetCardinality) \
( (g_G_STRING_SET_STUFF) r_GET_GREEN_ITEM_STUFF(u_greenArray,u_entry,\
  sizeof(struct G_STRING)*u_gStringSetCardinality) )


#define b_FULL_CLEAR b_TRUE
#define b_LIGHT_CLEAR b_FALSE0

// #REF GStringsClear <g-string set>
// #SEE GreenCollectionClear @ c-ansi/green.h <<g-string set>>
// Passed (specifc) : 
// - b_fullClear : 
//   + b_LIGHT_CLEAR (FALSE) : only formal reset of the collection 
//   + b_FULL_CLEAR (TRUE) : each greeen item is also individually cleared 
int GStringsClear(G_STRINGS_HANDLE handle, char b_fullClear);


// Handling g-strings conveyance...
// -----------------------------

// Retrieve g-strings conveyance of the collection.
// 
// Passed:
// - handle: gtrings collection handle 
// - element:
//
// Ret:
// >= 0: conveyance 
// -1: unexpected problem; anomaly is raised
int GStringsGetConveyance(G_STRINGS_HANDLE handle, int element) ;

// Check if g-string's conveyance is "okay" vis-a-vis a collection...
//
// Passed:
// - n_gStringsHandle: (NULL special value: not provided => NO check)  
// - c_element: (only significant if g-strings collection is provided)
// - c_expectedGStringConveyance: (only significant if g-strings collection is provided) 
#define m_CHECK_G_STRINGS_COLLECTION_CONVEYANCE(/*G_STRING_HANDLE*/n_gStringsHandle,\
  /*int*/c_element, /*int*/c_expectedGStringConveyance) {\
  if (n_gStringsHandle != NULL) {\
    int collectionGStringConveyance = GStringsGetConveyance(n_gStringsHandle,c_element) ;\
    m_TRACK_IF(collectionGStringConveyance < 0) \
    m_ASSERT(collectionGStringConveyance == c_expectedGStringConveyance) \
  } \
} 
  

// #REF GStringsDestroyInstance <g-string set> 
// Disengage and destroy <g-string set>s collection.
//
// Returned:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int GStringsDestroyInstance(G_STRINGS_HANDLE xh_handle);

#endif // __C_ANSI_G_STRING_H_INCLUDED
