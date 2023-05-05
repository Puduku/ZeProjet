// c-ansi/g-string.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_G_STRING_H_INCLUDED
#define __C_ANSI_G_STRING_H_INCLUDED

#include <string.h>

#include "c-ansi/diggy.h"
#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/green.h"
#include "c-ansi/c-string.h"


#define ANONYMOUS_TOKEN_ID0 0

// structure of g-string instance (GREEN item) :
struct G_STRING { // #REF struct-G_STRING
  // GREEN conventions for g-string:
  // "disengaged" state <=> nh_string == NULL
  // "engaged" state (also called "copied" state) <=> nh_string != NULL
  char *nh_string; // ALWAYS '\0'-terminated string (when "copied")
  int c_bufferSize; // only significant in "copied" (alias "engaged") state (>= 1)
  int c_copiedLength; // idem ; (>= 0) ACTUAL string length (NOT taking into account ending '\0': 
  // ALWAYS < c_bufferSize ;
  // ALWAYS >= strlen(nh_string) ;
  // WHEN == strlen(nh_string), this "empiric" check asserts the string contains NO '\0' char
  // (excepted the ending '\0').
  struct STRING_PORTION stringPortion; // g-string's "logical" value 
  int tokenId; // Default value: 0 
} ;

// Formal g-string stuff :
typedef struct G_STRING *G_STRING_STUFF;

// #REF Not-copied-Empty-Equivalence 
// CONVENTION: a non-"copied" g-string (NULL nh_string) is supposed strictly
// EQUIVALENT to EMPTY (like "") string. 


// "Fundamental" macro to "convert" a g-string into string portion. 
//
// Passed:
// - m_stringPortion: string portion to assign
// - p_gStringStuff: #SEE  Not-copied-Empty-Equivalence 
// 
// Updated:
// - m_stringPortion
#define m_ASSIGN_STRING_PORTION__G_STRING(/*struct STRING_PORTION*/m_stringPortion,\
  /*G_STRING_STUFF*/p_gStringStuff) {\
  (m_stringPortion) = (p_gStringStuff)->stringPortion;\
}

// See m_ASSIGN_STRING_PORTION__G_STRING() macro above
#define m_ASSIGN_LOCAL_STRING_PORTION__G_STRING(\
  /*struct STRING_PORTION*/m_localStringPortion,  /*G_STRING_STUFF*/p_gStringStuff) \
  struct STRING_PORTION m_localStringPortion;\
  m_ASSIGN_STRING_PORTION__G_STRING(m_localStringPortion,  p_gStringStuff)


// Directly import a string portion in a g-string WITHOUT creating (or updating) a string
// copy.
// 
// Passed:
// - stuff:
// - tokenId:
// - fp_stringPortion:
#define m_G_STRING_IMPORT(/*G_STRING_STUFF*/stuff, /*int*/m_tokenId,\
  /*const struct STRING_PORTION*/ fp_stringPortion) {\
  (stuff)->tokenId = (m_tokenId);\
  (stuff)->stringPortion = fp_stringPortion;\
} 

// Directly import a C-string in a g-string. 
// See m_G_STRING_IMPORT() above
#define m_G_STRING_C_IMPORT(/*G_STRING_STUFF*/stuff, /*int*/tokenId, /*const char* */ fp_cString) {\
  m_ASSIGN_LOCAL_C_STRING_PORTION(em_stringPortion,fp_cString)\
  m_G_STRING_IMPORT(stuff,tokenId,em_stringPortion)\
}


// #REF GStringCopy
// Copy (or concatenate...) a string portion into a "g-string".
// Note1: (*) mark useful values for  neutral operation  - i.e do not alter "destination" g-string; 
// unless of course "destination" was "disengaged" (nh_string == NULL) ; in such case, an empty
// string is copied in destination (SEE Not-copied-Empty-Equivalence) 
// Note2: the token id is NOT updated.
//
// Passed:
// - stuff : "destination" g-string
// - n_<offset>: offset in destination g-string to apply the copy
//   + -1 special value (*) : "end of string"; set to current copied length => i.e concatenation
//   + >= 0: "offset copy" ; 0 corresponds to "ordinary" copy ;
//     REM: if offset > current copied length, the offset is rectified (concatenation)
// - ap_stringPortion: 
//   (*) empty string for neutral operation
//
// Modified:
// - stuff: ALWAYS in "copied" state (i.e nh_string != NULL, whatever!)
//
// Returned:
// - >= 0: OK, copied (copied length of dest string)
// - -1: unexpected problem
int GStringCopy(G_STRING_STUFF stuff, int n_offset, const struct STRING_PORTION *ap_stringPortion);

// Copy (or concatenate...) a '\0'-terminated C string g into a "g-string"
// See GStringCopy() above
static inline int m_GStringCCopy(G_STRING_STUFF stuff, int n_offset, const char* p_cString) {
  m_DIGGY_BOLLARD()
  m_ASSIGN_LOCAL_C_STRING_PORTION(stringPortion, p_cString)
  m_DIGGY_RETURN(GStringCopy(stuff, n_offset, &stringPortion))
} // m_GStringCCopy

// Copy (or concatenate...) a g-string into a "g-string"
// Note1: the token id is NOT copied
// See GStringCopy() above
static inline int m_GStringGCopy(G_STRING_STUFF stuff, int n_offset, G_STRING_STUFF p_gStringStuff) {
  m_DIGGY_BOLLARD()
  m_ASSIGN_LOCAL_STRING_PORTION__G_STRING(stringPortion,p_gStringStuff)
  m_DIGGY_RETURN(GStringCopy(stuff,n_offset,&stringPortion))
} // m_GStringGCopy

// Clone a g-string. 
// Note: the token id is copied.
// See GStringCopy() above
static inline int m_GStringClone(G_STRING_STUFF stuff,  G_STRING_STUFF p_gStringStuff) {
  m_DIGGY_BOLLARD()
  m_ASSIGN_LOCAL_STRING_PORTION__G_STRING(stringPortion,p_gStringStuff)
  stuff->tokenId = p_gStringStuff->tokenId;
  m_DIGGY_RETURN(GStringCopy(stuff,0,&stringPortion))
} // m_GStringClone


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
// - >=0: Returned: string length after conversion (>= 0) 
// - -1: unexpected problem
int GStringConvert(G_STRING_STUFF stuff,  IS_CHAR_FUNCTION n_isNeutralCharFunction,
  TO_CHAR_FUNCTION toCharFunction) ;
                   

// #SEE Not-copied-Empty-Equivalence 
// 
// Passed:
// - stuff : "destination" g-string
//
// Modified:
// - stuff: "logically" empty string 
#define m_G_STRING_CLEAR(/*G_STRING_STUFF*/stuff) {\
  m_ASSIGN_LOCAL_EMPTY_STRING_PORTION(em_emptyStringPortion)\
  m_G_STRING_IMPORT(stuff,ANONYMOUS_TOKEN_ID0,em_emptyStringPortion)\
  if (stuff->nh_string != NULL) {\
    m_TRACK_IF(GStringCopy(stuff, 0, &em_emptyStringPortion) < 0)\
  }\
}  

#if 0
EXEMPLE VA_ARGS avec macros
#define /*int*/ G_STRING_PRINTF(/*G_STRING_STUFF*/ stuff,  /*int*/ n_offset,\
                  /*const char* */ p_format, ...) \
GStringPrintf(stuff,n_offset,p_format, ##__VA_ARGS__)
#endif

// "g-string set"
// --------------


// #REF G_STRING_SET_STUFF
// "g-string set" == array of g-strings (fixed number of elements)
// Hence, a g-string set stuff is exacltly the g-string stuff of the first element of the set.
// To retrieve a specific g-string element of a set, just apply the "good old" pointer arithmetic. 
typedef struct G_STRING *G_STRING_SET_STUFF;


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
int GStringSetCreateInstance(G_STRING_SET_STUFF *azh_stuff,  int cardinality);

// #SEE GStringSetCreateInstance <g-string>
#define /*int*/ G_STRING_CREATE_INSTANCE(/*G_STRING_STUFF* */ azh_stuff) \
GStringSetCreateInstance(azh_stuff,1)


// #REF GStringSetDestroyInstance <g-string set>
// (Disengage and) free <g-string set> instance...
//
// Passed:
// - xh_stuff:  
// - cardinality: see GStringSetCreateInstance() 
//
// Modifed ;
// - xh_stuff: this stuff is no more valid 
// 
// Returned
// - RETURNED
// - -1: unexpected problem ; anomaly is raised
int GStringSetDestroyInstance(G_STRING_SET_STUFF xh_stuff,  int cardinality);

// #SEE GStringSetDestroyInstance <g-string>
#define /*int*/ G_STRING_DESTROY_INSTANCE(/*G_STRING_STUFF*/ xh_stuff) \
GStringSetDestroyInstance(xh_stuff,1)


#define G_PARAM_NAME_ELEMENT    0
#define G_PARAM_VALUE_ELEMENT   1
#define G_PARAM_CARDINALITY 2


// "g-strings collections"
// -----------------------

struct G_STRINGS ; // Private structure !
typedef struct G_STRINGS * G_STRINGS_HANDLE; // Public handle


// #REF GStringsCreateInstance <g-string set>
// Creation and initialization of a "<g-string set> collection".
// 
// Passed:
// - azh_handle: (address of) new handle to initialize
// - expectedItemsNumber:
//   #SEE GreenCollectionCreateInstance-expectedItemsNumber@c-ansi/green.h <<g-string set>>
// - gStringSetCardinality: number of elements in a set (>0 ; 0 value is taken for a BUG) #SKIP
// 
// Modified:
// - *azh_handle:
//
// Returned:
// - RETURNED: Ok, <g-string set> collection is instancied
// - -1: unexpected problem; anomaly is raised 
int GStringsCreateInstance(G_STRINGS_HANDLE* azh_handle,  int expectedItemsNumber,
  int gStringSetCardinality);

// #SEE GStringsCreateInstance <g-string>
#define /*int*/ G_STRINGS_CREATE_INSTANCE(/*G_STRINGS_HANDLE*/ azh_handle,\
  /*int*/ expectedItemsNumber) \
GStringsCreateInstance(azh_handle,  expectedItemsNumber,1)


// #REF GStringsFetch <gStringSet> 
// #SEE GreenCollectionFetch @ c-ansi/green.h <<gStringSet>>
int GStringsFetch(G_STRINGS_HANDLE cp_handle,  int n_entry,  G_STRING_SET_STUFF* acnt_gStringSetStuff);


// #REF GStringsGetCount <gStringSet> 
// #SEE GreenCollectionGetCount@c-ansi/green.h <<gStringSet>>
int GStringsGetCount (G_STRINGS_HANDLE cp_handle,  G_STRING_SET_STUFF *navnt_gStringSetStuff);


enum { // #REF enum-G_KEYS_COMPARISON 
   STRING__G_KEYS_COMPARISON, //
 TOKEN_ID__G_KEYS_COMPARISON, //
    VALUE__G_KEYS_COMPARISON, //
} ;


//
// Passed:
// - r_handle:
// - ap_stringPortion: 
// 
// Ret:
// Value corresponding to string portion
typedef long int (*STRING_PORTION_VALUE_FUNCTION) (void *r_handle,
  const struct STRING_PORTION *ap_stringPortion);


// #REF GStringsAddIndex <g-string set> <key1>
// #SEE GreenCollectionAddIndex@c-ansi/green.h  <<g-string set>> 
// Passed parameters for new index:
// - keysNumber: >= 1; 1:plain index; >=2:compound index  #SKIP
// - key1GStringSetElement: between [0 ; <g-string set cardinality>[ ; the g-string set's
//   element serving as first key of the index
// - key1GKeysComparison: #see enum-G_KEYS_COMPARISON
// - cn_key1IsNeutralCharFunction: only significant with STRING__G_KEYS_COMPARISON 
//   + NULL: DO NOT eliminate neutral chars before comparison 
//   + non NULL: eliminate neutral chars before comparison 
// - cn_key1ToCharFunction: only significant with STRING__G_KEYS_COMPARISON 
//   + NULL: NO conversion applied before comparison 
//   + non NULL: conversion applied before comparison 
// - c_key1StringPortionValueFunction: only significant with VALUE__G_KEYS_COMPARISON
// - cfr_key1StringPortionValueFunctionHandle: only significant with VALUE__G_KEYS_COMPARISON
// - ... : g-string set(s element for second key (etc.) if any ...
int GStringsAddIndex(G_STRINGS_HANDLE handle,  int keysNumber,
  int key1GStringSetElement,  int key1GKeysComparison,
  IS_CHAR_FUNCTION cn_key1IsNeutralCharFunction,  TO_CHAR_FUNCTION cn_key1ToCharFunction,
  STRING_PORTION_VALUE_FUNCTION c_key1StringPortionValueFunction,
  void *cr_stringPortionValueFunctionHandle,  ...);

// #SEE GStringsAddIndex <g-string> <key>
// Nb: Plain index => one key
#define /*int*/ G_STRINGS_ADD_INDEX(/*G_STRINGS_HANDLE*/ handle,  /*int*/ keyGKeysComparison,\
  /*IS_CHAR_FUNCTION*/ cn_keyIsNeutralCharFunction,  /*TO_CHAR_FUNCTION*/ cn_keyToCharFunction, \
  /*STRING_PORTION_VALUE_FUNCTION*/ c_keyStringPortionValueFunction,\
  /*void* */cfr_keyStringPortionValueFunctionHandle, ...) \
  GStringsAddIndex(handle,  1,  0,  keyGKeysComparison,\
  cn_keyIsNeutralCharFunction, cn_keyToCharFunction,\
  c_keyStringPortionValueFunction,cfr_keyStringPortionValueFunctionHandle) 


// (Internal use)
union BARE_G_KEY {
  struct STRING_PORTION cp_stringPortion; // with STRING__G_KEYS_COMPARISON
  int c_tokenId; // with TOKEN_ID__G_KEYS_COMPARISON
  long int c_value; // with VALUE___G_KEYS_COMPARISON
}; 

// (Internal use)
#define m_ASSIGN_BARE_G_KEY__STRING(/*struct G_KEY*/m_bareGKey,\
  /*const struct STRING_PORTION*/p_stringPortion) \
  (m_bareGKey).cp_stringPortion = (p_stringPortion);

// (Internal use)
#define m_ASSIGN_BARE_G_KEY__TOKEN_ID(/*struct G_KEY*/m_bareGKey, /*int*/tokenId) \
  (m_bareGKey).c_tokenId = tokenId; 

// (Internal use)
#define m_ASSIGN_BARE_G_KEY__VALUE(/*struct G_KEY*/m_bareGKey,/*long int*/u_value) \
  (m_bareGKey).c_value = u_value;


// (Internal use)
#define m_ASSIGN_BARE_G_KEY__G_STRING(/*union BARE_G_KEY*/m_bareGKey, /*int*/gKeysComparison,\
  /*G_STRING_STUFF*/ p_gStringStuff,\
  /*STRING_PORTION_VALUE_FUNCTION*/c_stringPortionValueFunction,\
  /*void* */c_stringPortionValueFunctionHandle) {\
  switch (gKeysComparison) {\
  case STRING__G_KEYS_COMPARISON: \
    m_ASSIGN_BARE_G_KEY__STRING(m_bareGKey,(p_gStringStuff)->stringPortion)\
  break; case TOKEN_ID__G_KEYS_COMPARISON:\
    m_ASSIGN_BARE_G_KEY__TOKEN_ID(m_bareGKey,(p_gStringStuff)->tokenId)\
  break; case VALUE__G_KEYS_COMPARISON:\
    m_ASSIGN_BARE_G_KEY__VALUE(m_bareGKey, (c_stringPortionValueFunction)\
      (c_stringPortionValueFunctionHandle,&(p_gStringStuff)->stringPortion));\
  break; default:\
    m_RAISE(ANOMALY__VALUE__FMT_D,gKeysComparison)\
  }\
}

// #REF struct-G_KEY
struct G_KEY { //
  int gKeysComparison ; // #SEE enum-G_KEYS_COMPARISON
  union BARE_G_KEY bare ;//
}; 

// Assign a g-key for string (lexical) comparison
//
// Passed:
// - m_gKey: 
// - p_stringPortion:
#define m_ASSIGN_G_KEY__STRING(/*struct G_KEY*/ m_gKey, \
  /*const struct STRING_PORTION*/p_stringPortion) {\
  (m_gKey).gKeysComparison = STRING__G_KEYS_COMPARISON;\
  m_ASSIGN_BARE_G_KEY__STRING((m_gKey).bare, p_stringPortion)\
} 

// Assign a g-key for token id comparison
//
// Passed:
// - m_gKey: 
// - tokenId:
#define m_ASSIGN_G_KEY__TOKEN_ID(/*struct G_KEY*/ m_gKey, /*int*/ tokenId) {\
  (m_gKey).gKeysComparison = TOKEN_ID__G_KEYS_COMPARISON;\
  m_ASSIGN_BARE_G_KEY__TOKEN_ID((m_gKey).bare,tokenId) \
} 

// Assign a g-key for value comparison
//
// Passed:
// - m_gKey: 
// - u_value:
#define m_ASSIGN_G_KEY__VALUE(/*struct G_KEY*/ m_gKey, /*long int*/ u_value) {\
  (m_gKey).gKeysComparison = VALUE__G_KEYS_COMPARISON;\
  m_ASSIGN_BARE_G_KEY__VALUE((m_gKey).bare,u_value) \
} 

// Assign a g-key based on "external" g-string
//
// Passed:
// - m_gKey: 
// - gKeysComparison: #SEE enum-G_KEYS_COMPARISON 
// - p_gStringStuff: g-string used for comparison - must NOT be part of the collection 
//   #SEE fetched-item-notices @ c-ansi/green.h
#define m_ASSIGN_G_KEY__G_STRING(/*struct G_KEY*/ m_gKey,  /*int*/ gKeysComparison,\
  /*G_STRING_STUFF*/ p_gStringStuff,\
  /*STRING_PORTION_VALUE_FUNCTION*/c_stringPortionValueFunction,\
  /*void* */c_stringPortionValueFunctionHandle) {\
  (m_gKey).gKeysComparison = gKeysComparison;\
  m_ASSIGN_BARE_G_KEY__G_STRING(m_gKey.bare, (m_gKey).gKeysComparison, p_gStringStuff,\
    c_stringPortionValueFunction,c_stringPortionValueFunctionHandle)\
}


// #REF GStringsIndexFetch <gStringSet>
// #SEE GreenCollectionIndexFetch@c-ansi/green.h  <<gStringSet>> <keys> 
// Passed keys :
// - ccap_key1: key 1 search value(s) ; only significant when 
//   indexFetch != INDEX_FETCH__READ_NEXT and when c_indexSeek == INDEX_SEEK__KEY / BOTTOM / TOP.
//   type of comparison (see enum-G_KEYS_COMPARISON) must correspond to that assigned to index
//   (see GStringsAddIndex() above) 
// - ... (variadic parameters) : for "compound" keys: key2, key3, etc.
int GStringsIndexFetch(G_STRINGS_HANDLE cp_handle,
  INDEX_ITERATOR_AUTOMATIC_BUFFER nf_indexIteratorAutomaticBuffer,  int indexLabel,
  int indexFetch, int c_indexSeek,  G_STRING_SET_STUFF *acvnt_gStringSetStuff,
  const struct G_KEY *ccap_key1, ...);


// Fetch g-string item based on plain index 
// #SEE GStringsIndexFetch @ c-ansi/g-string.h <gString>
#define /*int*/ G_STRINGS_INDEX_FETCH(/*G_STRINGS_HANDLE*/ handle,\
  /*INDEX_ITERATOR_AUTOMATIC_BUFFER*/ nf_indexIteratorAutomaticBuffer,\
  /*int*/ indexLabel,  /*int*/ indexFetch, /*int*/ c_indexSeek,\
  /*G_STRING_STUFF* */ acvnt_gStringStuff,\
  /*const struct G_KEY* */ ccap_key) \
GStringsIndexFetch(handle,  nf_indexIteratorAutomaticBuffer,  indexLabel,\
  indexFetch,c_indexSeek,  acvnt_gStringStuff,  ccap_key)


// #REF GStringsVerifyIndexes <g-string set>
// #SEE GreenCollectionVerifyIndexes@c-ansi/green.h  <<g-string set>>
int GStringsVerifyIndexes(G_STRINGS_HANDLE handle);


typedef struct G_STRING *G_STRINGS_ARRAY;

// Passed:
// - gStringsArray:
// - cardinality:
// - element
#define GET_G_STRING_STUFF(/*G_STRINGS_ARRAY*/gStringsArray,/*int*/cardinality,/*int*/element) \
  ((gStringsArray) + ((cardinality)*(element)))


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
#define /*G_STRING_SET_STUFF*/ GET_G_STRING_SET_STUFF(/*char* */ u_greenArray,  /*int*/ u_entry,\
  /*int*/ u_gStringSetCardinality) \
( (G_STRING_SET_STUFF) r_GET_GREEN_ITEM_STUFF(u_greenArray,u_entry,\
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


// #REF GStringsDestroyInstance <g-string set> 
// Disengage and destroy <g-string set>s collection.
//
// Returned:
// - RETURNED: OK...
// - -1: unexpected problem ; anomaly is raised
int GStringsDestroyInstance(G_STRINGS_HANDLE xh_handle);


#endif // __C_ANSI_G_STRING_H_INCLUDED
