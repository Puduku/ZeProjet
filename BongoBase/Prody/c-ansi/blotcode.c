// c-ansi/blotcode.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2002

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdint.h>

#include "c-ansi/types.h"
#include "c-ansi/alloc.h"
#include "c-ansi/stderr.h"
#include "c-ansi/c-stack.h"
#include "c-ansi/blotcode.h"
#include "c-ansi/p-string.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-token.h"

#include "c-ansi/testy-diggy.h"
#include "c-ansi/diggy.h"



// BLOTKEYWs (blot keywords)
// -------------------------

// Blot control statements enumeration
enum {
           NONE__BLOTKEYW_ID = 0,
           LOOP__BLOTKEYW_ID,
       END_LOOP__BLOTKEYW_ID,
         SWITCH__BLOTKEYW_ID,
           CASE__BLOTKEYW_ID,
   DEFAULT_CASE__BLOTKEYW_ID,
     END_SWITCH__BLOTKEYW_ID,
         BLOTKEYW_IDS_NUMBER
} ;

const struct TOKEN_DEFINITION p_blotkeywsTokenDefinitions[] = { 
  { "_NONE_",              NONE__BLOTKEYW_ID },
  { "loop",                LOOP__BLOTKEYW_ID },
  { "case",                CASE__BLOTKEYW_ID },
  { "switch",            SWITCH__BLOTKEYW_ID },
  { "endLoop",         END_LOOP__BLOTKEYW_ID },
  { "endSwitch",     END_SWITCH__BLOTKEYW_ID },
  { "defaultCase", DEFAULT_CASE__BLOTKEYW_ID },
} ;


// BLOTLIBs (blot libraries)
// -------------------------

// GREEN ITEM:
struct BLOTLIB {
  const char *np_prefix; 
  int localBlotfuncNamesNumber;
  const char **cp_localBlotfuncNames; // Significant when localBlotfuncNamesNumber > 0 

  BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION 
  blotlibExecutorFactoryCreateProductInstanceFunction;
  void *pr_blotlibExecutorFactoryHandle;
  BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION c_blotlibExecutorExecuteCFunction;
  BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION c_blotlibExecutorDestroyInstanceFunction;
} ;
typedef struct BLOTLIB *BLOTLIB_STUFF; 

// Assign blotlib  
// 
// Passed:
// - blotlibStuff: blotlib to assign
// 
// Changed:
// - blotlibStuff: assigned blotlib
static void o_Blotlib(BLOTLIB_STUFF blotlibStuff, const char* np_prefix,
  int localBlotfuncNamesNumber, const char** cp_localBlotfuncNames,
  BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION 
  blotlibExecutorFactoryCreateProductInstanceFunction,
  void* pr_blotlibExecutorFactoryHandle,
  BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION blotlibExecutorExecuteCFunction,
  BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION blotlibExecutorDestroyInstanceFunction) {
  blotlibStuff->np_prefix = np_prefix;
  blotlibStuff->localBlotfuncNamesNumber = localBlotfuncNamesNumber;
  blotlibStuff->cp_localBlotfuncNames = cp_localBlotfuncNames;
  blotlibStuff->blotlibExecutorFactoryCreateProductInstanceFunction =
    blotlibExecutorFactoryCreateProductInstanceFunction;
  blotlibStuff->pr_blotlibExecutorFactoryHandle = pr_blotlibExecutorFactoryHandle;
  blotlibStuff->c_blotlibExecutorExecuteCFunction = blotlibExecutorExecuteCFunction;
  blotlibStuff->c_blotlibExecutorDestroyInstanceFunction = blotlibExecutorDestroyInstanceFunction;
} // o_Blotlib

// BLOTFUNCs (blot functions)
// --------------------------

// Assign blotfunc call
//
// Passed:
// - a_blotfuncCall: 

// Changed:
// - *a_blotfuncCall: 
static void o_BlotfuncCall(struct BLOTFUNC_CALL *a_blotfuncCall,
  struct P_STRING referral, struct P_STRING arguments) {
  a_blotfuncCall->referral = referral;
  a_blotfuncCall->arguments = arguments;
} // o_BlotfuncCall

// Assign blotfunc entry 
// Passed:
// - 
static void o_BlotfuncEntry(struct BLOTFUNC_ENTRY *a_blotfuncEntry,
 int m_blotlibEntry, int m_localBlotfuncNameEntry) {
 a_blotfuncEntry->blotlibEntry = m_blotlibEntry;
 a_blotfuncEntry->localBlotfuncNameEntry = m_localBlotfuncNameEntry;
} // o_BlotfuncEntry

// "Ultra but dirty" GREEN ITEM:
typedef struct BLOTFUNC_ENTRY *BLOTFUNC_ENTRY_STUFF;
     
// blotfuncs handler pseudo instance:
struct BLOTFUNCS_HANDLER {
  int blotlibsPhysicalCount; // > 0 
  int blotlibsCount; 
  struct BLOTLIB *blotlibsArray; 
} ; 

// Retrieve blotlib stuff (from blotfuncs handler pseudo instance) 
//
// Passed:
// - p_handle: pointer to pseudo instance (protected against modification) 
// - entry:
//
// Changed:
// - m_blotlibStuff:
#define m_BLOTFUNCS_HANDLER_GET_BLOTLIB(/*struct BLOTFUNCS_HANDLER* */p_handle, /*int*/entry,\
  /*BLOTLIB_STUFF*/m_blotlibStuff) {\
  m_ASSERT((entry) < (p_handle)->blotlibsCount) \
  (m_blotlibStuff) = (p_handle)->blotlibsArray + (entry);\
}

// Pure key
struct BLOTFUNC_KEY_NAME {
  struct P_STRING prefix ;
  struct P_STRING name ;
} ;

// Assign "blotfunc key name"...
//
// Passed:
// - a_blotfuncKeyName:
// 
// Changed:
// - *a_blotfuncKeyName:
static void o_BlotfuncKeyName(struct BLOTFUNC_KEY_NAME *a_blotfuncKeyName, const char* n_prefix,
  const char* name) {
  a_blotfuncKeyName->prefix = o_PString(n_prefix == NULL? GOOD_OLD_EMPTY_C_STRING: n_prefix);
  a_blotfuncKeyName->name = o_PString(name);
} // o_BlotfuncKeyName 

// GREEN_HANDLER__COMPARE_FUNCTION
static int BlotfuncsHandlerCompare (void *cpr_handle,  char b_frozen, int indexLabel,  int keyRank,
  char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff, const void *cpr_bKeys) {
  //m_DIGGY_BOLLARD_S()
  struct BLOTFUNCS_HANDLER *p_handle = (struct BLOTFUNCS_HANDLER *)cpr_handle; // protected
    // instance (safe for freeze) 

  m_ASSERT(indexLabel == 0)
  m_ASSERT(keyRank == 0)

  BLOTFUNC_ENTRY_STUFF p_blotfuncEntryStuff = (BLOTFUNC_ENTRY_STUFF) pr_aGreenItemStuff;
  BLOTLIB_STUFF blotlibStuff = (BLOTLIB_STUFF) UNDEFINED;
  m_BLOTFUNCS_HANDLER_GET_BLOTLIB(p_handle, p_blotfuncEntryStuff->blotlibEntry, blotlibStuff)
  m_ASSERT(p_blotfuncEntryStuff->localBlotfuncNameEntry < blotlibStuff->localBlotfuncNamesNumber)
  struct BLOTFUNC_KEY_NAME aBlotfuncKeyName;
  o_BlotfuncKeyName(&aBlotfuncKeyName, blotlibStuff->np_prefix,
    blotlibStuff->cp_localBlotfuncNames[p_blotfuncEntryStuff->localBlotfuncNameEntry]);

  struct BLOTFUNC_KEY_NAME bBlotfuncKeyName ;
  if (npr_bGreenItemStuff != NULL) {
    p_blotfuncEntryStuff = (BLOTFUNC_ENTRY_STUFF) npr_bGreenItemStuff;
    m_BLOTFUNCS_HANDLER_GET_BLOTLIB(p_handle, p_blotfuncEntryStuff->blotlibEntry, blotlibStuff)
    m_ASSERT(p_blotfuncEntryStuff->localBlotfuncNameEntry < blotlibStuff->localBlotfuncNamesNumber)
    o_BlotfuncKeyName(&bBlotfuncKeyName, blotlibStuff->np_prefix,
      blotlibStuff->cp_localBlotfuncNames[p_blotfuncEntryStuff->localBlotfuncNameEntry]);
  } else bBlotfuncKeyName = *((struct BLOTFUNC_KEY_NAME*)cpr_bKeys); 

  int comparison = ComparePStrings(aBlotfuncKeyName.prefix, bBlotfuncKeyName.prefix,
    (IS_CHAR_FUNCTION)NULL,  (TO_CHAR_FUNCTION)NULL,!b_SUB_STRING_2);
  if (comparison == EQUAL_TO__COMPARISON) {
    comparison = ComparePStrings(aBlotfuncKeyName.name, bBlotfuncKeyName.name,
      (IS_CHAR_FUNCTION)NULL,  (TO_CHAR_FUNCTION)NULL,!b_SUB_STRING_2);
  } //if
  
  //m_DIGGY_RETURN(comparison)
  return comparison;
} // BlotfuncsHandlerCompare


// BLOTCODE (blot code)
// --------------------

struct BLOTCODE {
  g_G_TOKENS_HANDLE h_blotkeywsHandle; // Directly frozen
  GREEN_COLLECTION_HANDLE h_blotlibsHandle; // blotlibs collection
  char b_frozen ; // TRUE => blotlibs collection is frozen
  // two fields below are only significant when blotlibs collection is frozen
  struct BLOTFUNCS_HANDLER c_blotfuncsHandler; // pseudo instance 
  GREEN_COLLECTION_HANDLE ch_blotfuncsHandle; // blotfuncs collection (actually, a simple index on
    // blotlibs collection) - directly frozen
} ;

// Public function ; see .h 
int BlotcodeCreateInstance (BLOTCODE_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle) 
  BLOTCODE_HANDLE handle = *azh_handle;

  m_TRACK_IF(l_GTokensCreateInstance(&handle->h_blotkeywsHandle, BLOTKEYW_IDS_NUMBER) != RETURNED)
  m_ASSERT(gm_GTokensAddPlainLexicalIndex(handle->h_blotkeywsHandle, NULL, NULL) == INDEX_LABEL0)
  m_TRACK_IF(l_GTokensImport(handle->h_blotkeywsHandle, p_blotkeywsTokenDefinitions,
    BLOTKEYW_IDS_NUMBER) != RETURNED)
  m_ASSERT(g_GTokensFreeze(handle->h_blotkeywsHandle, NULL) >= BLOTKEYW_IDS_NUMBER)

  m_TRACK_IF(GreenCollectionCreateInstance(&(handle->h_blotlibsHandle),  10,
    sizeof(struct BLOTLIB),  NULL, NULL,NULL, (void *)UNDEFINED) != RETURNED)
  handle->b_frozen = b_FALSE0;

  m_DIGGY_RETURN(RETURNED)
} // BlotcodeCreateInstance 


// Find control statement 
//
// Passed:
// -p_handle: protected instance 
// -p_litteralKeyw:
// -litteralKeywLength:
//
// Changed:
// -*ac_blotkeywId: (with RESULT__FOUND)  
// 
// Returned:
// - RESULT__FOUND:
// - RESULT__NOT_FOUND:
// - -1: unexpected problem ; anomaly is raised
static int BlotcodeFindBlotkeyw (BLOTCODE_HANDLE p_handle,
  const struct P_STRING*  ap_litteralKeyw,  int *ac_blotkeywId) {
  m_DIGGY_BOLLARD_S()
  m_INDEX_REQUEST_AUTOMATIC_BUFFER(indexRequestAutomaticBuffer)
  G_STRING_STUFF t_blotkeywStuff = (G_STRING_STUFF) UNDEFINED;

  struct G_KEY gKey = m_GKey_PString(*ap_litteralKeyw); 

  int result = gm_GTokensIndexSingleFetch(p_handle->h_blotkeywsHandle, indexRequestAutomaticBuffer,
    INDEX_LABEL0,INDEX_SEEK_FLAGS__EQUAL,&gKey,INDEX_FETCH_FLAGS__READ_ONLY,&t_blotkeywStuff,
    NULL); 
  switch(result) { 
  case RESULT__FOUND:
    *ac_blotkeywId = t_blotkeywStuff->acolyt.cen_value ;
  break; case RESULT__NOT_FOUND:
  break; default: 
    m_TRACK()
  } // switch
     
  m_DIGGY_RETURN(result)
} // BlotcodeFindBlotkeyw 


// Public function ; see .h 
int BlotcodeLinkBlotlib (BLOTCODE_HANDLE ep_handle,  const char* nfp_blotlibPrefix,
  int localBlotfuncNamesNumber, const char** cfp_localBlotfuncNames,  
  BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION 
  blotlibExecutorFactoryCreateProductInstanceFunction,
  void *fpr_blotlibExecutorFactoryHandle,
  BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION blotlibExecutorExecuteCFunction,
  BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION blotlibExecutorDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()
  m_ASSERT(!ep_handle->b_frozen)
// TODO: vedrifier les noms des librairies et function (charset) 

  int blotlibEntry = UNDEFINED; 
  BLOTLIB_STUFF blotlibStuff = (BLOTLIB_STUFF)UNDEFINED;
  blotlibEntry = GreenCollectionFetch(ep_handle->h_blotlibsHandle, -1,  (char **) &blotlibStuff);
  m_TRACK_IF(blotlibEntry < 0)
  o_Blotlib(blotlibStuff, nfp_blotlibPrefix,localBlotfuncNamesNumber,cfp_localBlotfuncNames,
    blotlibExecutorFactoryCreateProductInstanceFunction,fpr_blotlibExecutorFactoryHandle,
    blotlibExecutorExecuteCFunction,blotlibExecutorDestroyInstanceFunction);

  m_DIGGY_RETURN(blotlibEntry)
} // BlotcodeLinkBlotlib 


// Public function ; see .h 
int BlotcodeFreeze(BLOTCODE_HANDLE ep_handle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(!ep_handle->b_frozen)

  ep_handle->c_blotfuncsHandler.blotlibsPhysicalCount = 
  GreenCollectionFreeze(ep_handle->h_blotlibsHandle,(char**)&ep_handle->c_blotfuncsHandler.blotlibsArray);
  m_TRACK_IF(ep_handle->c_blotfuncsHandler.blotlibsPhysicalCount < 0)  
  ep_handle->c_blotfuncsHandler.blotlibsCount = GreenCollectionGetCount(ep_handle->h_blotlibsHandle,NULL);
  m_TRACK_IF(ep_handle->c_blotfuncsHandler.blotlibsCount < 0)

  m_TRACK_IF(GreenCollectionCreateInstance(&(ep_handle->ch_blotfuncsHandle),
    BATEAU__EXPECTED_ITEMS_NUMBER,  sizeof(struct BLOTFUNC_ENTRY), NULL,
    BlotfuncsHandlerCompare, NULL, &(ep_handle->c_blotfuncsHandler)) != RETURNED)
  m_ASSERT(GreenCollectionAddIndex(ep_handle->ch_blotfuncsHandle,1) == INDEX_LABEL0)  

  BLOTFUNC_ENTRY_STUFF blotfuncEntryStuff = (BLOTFUNC_ENTRY_STUFF)UNDEFINED;
  int i = 0, j = UNDEFINED;
  struct BLOTLIB *blotlibPtr = ep_handle->c_blotfuncsHandler.blotlibsArray;
  for (; i < ep_handle->c_blotfuncsHandler.blotlibsCount; i++, blotlibPtr++) { 
    for (j = 0; j < blotlibPtr->localBlotfuncNamesNumber ; j++) {   
      int ret = GreenCollectionFetch(ep_handle->ch_blotfuncsHandle, -1, (char**)&blotfuncEntryStuff);
      m_TRACK_IF(ret < 0)
      o_BlotfuncEntry(blotfuncEntryStuff, i,j);
    } // for
  } // for 

  m_TRACK_IF(GreenCollectionFreeze(ep_handle->ch_blotfuncsHandle,NULL) < 0)

  ep_handle->b_frozen = b_TRUE;

  m_DIGGY_RETURN(RETURNED)
} // BlotcodeFreeze


// Only callable if the blotcode instance is frozen.
// 
// Passed:
// - p_handle: 
// - entry:
// - m_blotlibStuff:
#define m_BLOTCODE_GET_BLOTLIB(/*BLOTCODE_HANDLE*/p_handle, /*int*/entry,\
  /*BLOTLIB_STUFF*/m_blotlibStuff) {\
  m_ASSERT(p_handle->b_frozen) \
  m_BLOTFUNCS_HANDLER_GET_BLOTLIB(&p_handle->c_blotfuncsHandler, entry, blotlibStuff)\
}


// Only callable if the blotcode instance is frozen.
// Return the number of blotlibs
// 
// Passed:
// - p_handle: handle (protected instance)
// 
// Changed:
// - m_blotlibsCount
#define m_BLOTCODE_GET_BLOTLIBS_COUNT(/*BLOTCODE_HANDLE*/p_handle, m_blotlibsCount) {\
  m_ASSERT((p_handle)->b_frozen)\
  (m_blotlibsCount) = (p_handle)->c_blotfuncsHandler.blotlibsCount;\
} 

// Only callable if the blotcode instance is frozen.
//
// Passed:
// - p_handle: handle (protected instance)
// - a_referral: allows blotfunc identification 
//
// Changed:
// - *ac_blotfuncEntry: only significant with RESULT__FOUND 
// 
// Ret:
// - RESULT__FOUND:
// - RESULT__NOT_FOUND: 
// - -1: unexpected problem; anomaly is raised
static int BlotcodeFindBlotfunc(BLOTCODE_HANDLE p_handle, struct P_STRING referral,
  struct BLOTFUNC_ENTRY *ac_blotfuncEntry) {
  m_DIGGY_BOLLARD_S()
  struct BLOTFUNC_KEY_NAME blotfuncKeyName;
  m_ASSERT(p_handle->b_frozen) 
  m_INDEX_REQUEST_AUTOMATIC_BUFFER(indexRequestAutomaticBuffer)
m_DIGGY_VAR_P_STRING(referral)
  PParsePassChars(&referral,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'.',&(blotfuncKeyName.prefix));
  if (b_EMPTY_P_STRING(referral)) {
    blotfuncKeyName.name = blotfuncKeyName.prefix; 
    blotfuncKeyName.prefix = o_PString(GOOD_OLD_EMPTY_C_STRING);
  } else {
    PParseOffset(&referral,1,NULL);
    blotfuncKeyName.name = referral;
  } // if
m_DIGGY_VAR_P_STRING(blotfuncKeyName.prefix)
m_DIGGY_VAR_P_STRING(blotfuncKeyName.name)
     
  BLOTFUNC_ENTRY_STUFF blotfuncEntryStuff = (BLOTFUNC_ENTRY_STUFF)UNDEFINED;
  m_TRACK_IF(GreenCollectionIndexRequest(p_handle->ch_blotfuncsHandle,
    indexRequestAutomaticBuffer, 1, INDEX_LABEL0, INDEX_SEEK_FLAGS__EQUAL,
    (void *) &blotfuncKeyName) != RETURNED)
  int result = GreenCollectionIndexFetch(p_handle->ch_blotfuncsHandle,
    indexRequestAutomaticBuffer, INDEX_FETCH_FLAGS__READ_ONLY, (char **)&blotfuncEntryStuff, NULL);
  switch(result) {
  case RESULT__FOUND:
    *ac_blotfuncEntry = *blotfuncEntryStuff ;
  break; case RESULT__NOT_FOUND:
  break; default:
     m_TRACK()
  } // switch
 
  m_DIGGY_RETURN(result)
} // BlotcodeFindBlotfunc


// Public function ; see .h
int BlotcodeDestroyInstance (BLOTCODE_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  m_TRACK_IF(g_GTokensDestroyInstance(xh_handle->h_blotkeywsHandle) != RETURNED)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_blotlibsHandle) != RETURNED)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->ch_blotfuncsHandle) != RETURNED)

  free(xh_handle);
  m_DIGGY_RETURN(RETURNED) 
} // BlotcodeDestroyInstance



// BLOTINSTs (blot instructions) 
// -----------------------------

#define     MATCHED_SWITCH_CASE__MASTER__BLOTVAL0  TRUE__BLOTVAL0
#define NOT_MATCHED_SWITCH_CASE__MASTER__BLOTVAL  FALSE__BLOTVAL

// These fields are first filled  
// - during parsing stage by BlotcodeExecutorParseTemplate() function 
// and then completed  
// - during execution stage by BlotcodeExecutorConstructPage () function
// NOTICE1: execution stage(s) only possible after SUCCESSFUL parsing stage.
// NOTICE2: (theoritical) re-execution stage(s) MUST have SAME behavior than 1st execution stage.
struct BLOTINST {
  // Filled during parsing stage 
  int blotkeywId ;

  // Filled during parsing stage 
  char b_blotfunc; 
  struct BLOTFUNC c_blotfunc;

  // - if blot function provided:
  //   blot value established by blot function  
  //   (not set (b_blotval is FALSE) if blot function not yet called) 
  // - if no blot function provided:
  //   + instruction's static value
  //   + no set if no static value provided 
  // Either blot function or static blot instruction value must be present for 
  // "loop" control statement
  // "switch" control statement
  // "case" control statement
  char b_blotval; // true means significant blot value is set 
  gen_BLOTVAL c_blotval ; // blot value when significant 

  // Filled during parsing stage 
  // only significant for
  // "loop" "switch", "case" and "defaultCase" blot control statements;
  // - -1 special value: not present
  // - >= 0:
  //  + "loop" => points to "endLoop" statement 
  //  + "switch" / "case" / "defaultCase" => 
  //    points to next corresponding "case" / "defaultCase" / "endSwitch"
  int n_subEntry ;

  // No significant value set (b_masterBlotval is false) during parsing stage 
  // Used during execution stage:
  // - switch/case management:
  //   significant for each "switch" blot control statement 
  //   + not set: out of this switch/case blot instructions "block"
  //   + NOT_MATCHED_SWITCH_CASE__MASTER__BLOTVAL: no switch case yet matched
  //   + MATCHED_SWITCH_CASE__MASTER__BLOTVAL0: a switch case yet matched
  // - for loop/endLoop management:
  //   significant for each "loop" blot control statement 
  //   + not set: out of this loop blot instructions "block", or loop control statement is based on
  //     blot function 
  //   + >= 0: indicates the number of completed iterations (if loop control statement is based on
  //     static value) 
  // => Generic rule: must be unset (b_masterBlotval is false) as long as we are out of the blot
  //    instructions "block" of that master "loop" or "switch" control statement...
  char b_masterBlotval ; // true means significant master value is set
  gen_BLOTVAL c_masterBlotval ; // master value when significant

  // Filled during parsing stage 
  // - "loop" and "switch" blot control statements:
  //   Auto-reference to blot control statement 
  // - "case", "defaultCase" "endSwitch" and "endLoop" (slave) blot control statements;
  //   Refer to the corresponding "switch" or "loop" (master) blot control statement 
  //   (See b/c_masterBlotval above)
  int n_masterEntry ;
} ;

// "Ultra but dirty" GREEN ITEM:
struct TEMPLATE_PARTITION {
  char b_blotinst ;
  union {
    struct P_STRING c_decor; 
    struct BLOTINST c_blotinst; 
  } select;
} ;
typedef struct TEMPLATE_PARTITION *TEMPLATE_PARTITION_STUFF ;


// Passed:
// - a_templatePartition:
// 
// Changed:
// - *a_templatePartition:
static void o_DecorTemplatePartition(struct TEMPLATE_PARTITION *a_templatePartition,
  const struct P_STRING p_decor) {
  a_templatePartition->b_blotinst = b_FALSE0;
  a_templatePartition->select.c_decor = p_decor;
} // o_DecorTemplatePartition 


// Passed:
// - a_templatePartition:
// 
// Changed:
// - *a_templatePartition:
static void o_DefautlBlotinstTemplatePartition(struct TEMPLATE_PARTITION *a_templatePartition) {
  a_templatePartition->b_blotinst = b_TRUE;
  a_templatePartition->select.c_blotinst.blotkeywId =  NONE__BLOTKEYW_ID;
  a_templatePartition->select.c_blotinst.b_blotfunc = b_FALSE0;
  a_templatePartition->select.c_blotinst.b_blotval = b_FALSE0;
  a_templatePartition->select.c_blotinst.n_subEntry = -1;
  a_templatePartition->select.c_blotinst.b_masterBlotval = b_FALSE0;
  a_templatePartition->select.c_blotinst.n_masterEntry = -1;
} // o_DefautlBlotinstTemplatePartition 


// BLOTCODE executors
// ------------------

struct BLOTCODE_EXECUTOR {
  BLOTCODE_HANDLE frozenBlotcodeHandle ;
  int blotlibsCount ; 
  void **hshr_blotlibExecutorHandles ; 
  
  GREEN_COLLECTION_HANDLE h_templatePartitionsHandle;  
  // Only used by BlotcodeExecutorParseTemplate() :
  struct C_STACK h_flowControlStack;
  // Only used by BlotcodeExecutorConstructPage() : 
  G_STRING_STUFF h_blotfuncSurrogate ;
} ;


// Public function ; see .h
int BlotcodeExecutorCreateInstance(BLOTCODE_EXECUTOR_HANDLE *azh_handle,
  BLOTCODE_HANDLE f_frozenBlotcodeHandle) {

  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  BLOTCODE_EXECUTOR_HANDLE handle = *azh_handle;
  handle->frozenBlotcodeHandle = f_frozenBlotcodeHandle;

  m_BLOTCODE_GET_BLOTLIBS_COUNT(f_frozenBlotcodeHandle,handle->blotlibsCount)
  m_MALLOC_ARRAY(handle->hshr_blotlibExecutorHandles,handle->blotlibsCount)

  { BLOTLIB_STUFF blotlibStuff = (BLOTLIB_STUFF) UNDEFINED;
    int i = 0; void **shr_blotlibExecutorHandlePtr = handle->hshr_blotlibExecutorHandles;
    for (; i < handle->blotlibsCount; i++, shr_blotlibExecutorHandlePtr++) {
      m_BLOTCODE_GET_BLOTLIB(f_frozenBlotcodeHandle, i, blotlibStuff)
      m_TRACK_IF(blotlibStuff->blotlibExecutorFactoryCreateProductInstanceFunction(
        blotlibStuff->pr_blotlibExecutorFactoryHandle,
        shr_blotlibExecutorHandlePtr) != RETURNED)
    } // for
  } // blotlibStuff

  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_templatePartitionsHandle,
    BATEAU__EXPECTED_ITEMS_NUMBER,sizeof(struct TEMPLATE_PARTITION), NULL,NULL,NULL,
    (void*)UNDEFINED) != RETURNED)

  m_C_STACK_INIT(handle->h_flowControlStack)

  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&handle->h_blotfuncSurrogate) != RETURNED) 
  m_DIGGY_RETURN(RETURNED)
} // BlotcodeExecutorCreateInstance


// Public function ; see .h
int BlotcodeExecutorGetBlotlibExecutorHandle(BLOTCODE_EXECUTOR_HANDLE handle,  int blotlibEntry,
  void **acr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  int result = RESULT__NOT_FOUND; // a priori

  if (blotlibEntry < handle->blotlibsCount) {
    *acr_blotlibExecutorHandle = handle->hshr_blotlibExecutorHandles[blotlibEntry] ;
    result = RESULT__FOUND; 
  } // if
     
  m_DIGGY_RETURN(result) ;
} // BlotcodeExecutorGetBlotlibExecutorHandle


// IS_CHAR_FUNCTION 
static int IsBasicTokenChar(int c) {
  return (isalnum(c) || c == '-' || c == '.' || c == '_');
} // IsBasicTokenChar 


// Public function: see .h
// TODO: split in static local functions (see _k indicator)
int BlotcodeExecutorParseTemplate (BLOTCODE_EXECUTOR_HANDLE handle,
  struct P_STRING fp_template, struct P_STRING *nac_parsingErrorLocalization,
  G_STRING_STUFF nc_parsingErrorInfo) {
  m_DIGGY_BOLLARD()
  
  m_TRACK_IF(GreenCollectionClear(handle->h_templatePartitionsHandle) != RETURNED)

  m_C_STACK_CLEAR(handle->h_flowControlStack)

  struct P_STRING blotinstSequence; 
  blotinstSequence = o_PString(GOOD_OLD_EMPTY_C_STRING);

// Report error in some Blot instruction
//
// Passed:
// lexeme:
// p_format:
// ...: 
//
// Uses:
// - nac_parsingErrorLocalization
// - blotinstSequence 
#define m_REPORT_ERROR(/*const struct P_STRING */lexeme,\
  /*const char* */p_format, ...) {\
  if (nac_parsingErrorLocalization != NULL) {\
    *nac_parsingErrorLocalization = blotinstSequence;\
  }\
  if (nc_parsingErrorInfo != NULL) {\
    m_TRACK_IF(GStringPrintf(nc_parsingErrorInfo,0,p_format, ##__VA_ARGS__) < 0)\
    m_TRACK_IF(GStringPrintf(nc_parsingErrorInfo,-1," [" FMT_P_STRING "]",\
      m_P_STRING_2_FMT_ARGS(lexeme)) < 0)\
  }\
  m_DIGGY_RETURN(ANSWER__NO)\
}

#define DELIMITOR__S "**%s** delimitor"
#define DELIMITOR__C "**%c** delimitor"

  TEMPLATE_PARTITION_STUFF ti_templatePartitionStuff = (TEMPLATE_PARTITION_STUFF) UNDEFINED;
  int v_templatePartitionEntry = UNDEFINED; 
  struct BLOTINST *vc_blotinstPtr = (struct BLOTINST *) UNDEFINED;
  struct P_STRING decor; // UNDEFINED 
  struct P_STRING dummy; // UNDEFINED 
  dummy = o_PString("N/A");

  char b_blotblog = b_FALSE0;
  while (!b_EMPTY_P_STRING(fp_template)) {
    // Stage 1: Locate new blotinst "sequence" 
m_DIGGY_VAR_P_STRING(fp_template)
m_DIGGY_VAR_D(b_blotblog)
    if (!b_blotblog) {
      PParseTillMatch(&fp_template,o_PString("##<<"),NULL, &decor);
      if (!b_EMPTY_P_STRING(decor)) {
        v_templatePartitionEntry = GreenCollectionFetch(handle->h_templatePartitionsHandle, -1,
          (char**)&ti_templatePartitionStuff);
        m_TRACK_IF(v_templatePartitionEntry < 0)
        o_DecorTemplatePartition(ti_templatePartitionStuff, decor);
      } // if
      if (b_EMPTY_P_STRING(fp_template)) { // "##<<' NOT located 
        break;
      } // if
      PParseOffset(&fp_template,4, NULL);
    } // if 

    int c_delimitorEntry = UNDEFINED ;
m_DIGGY_VAR_P_STRING(fp_template)
    m_P_PARSE_TILL_FIRST_MATCH_C(fp_template,NULL, &c_delimitorEntry, &blotinstSequence,2,";;",">>")
    if (b_EMPTY_P_STRING(fp_template)) { // NO ending ";;" or ">>" located 
m_DIGGY_VAR_P_STRING(fp_template)
      m_REPORT_ERROR(dummy,"Missing " DELIMITOR__S, ";; or >>")
    } // if
m_DIGGY_VAR_P_STRING(fp_template)
m_DIGGY_VAR_D(c_delimitorEntry)
m_DIGGY_VAR_P_STRING(blotinstSequence)
    if ((b_blotblog = (c_delimitorEntry == 0))) { // ending ";;" located 
      PParseOffset(&fp_template,2, NULL);
    } else { // // ending ">>" located  
      m_ASSERT(c_delimitorEntry == 1)
      PParseOffset(&fp_template,2, NULL);
    } // if

    // blotinstSequence: complete blotinst "sequence"

    // Stage 2: Getting raw "tokens" within blotinst "sequence" 
    // Parsing <basic token 1> [ <basic token 2> ] [ '(' <arguments> ')' ]
    char b_arguments = b_FALSE0; // No arguments lexeme a priori
    struct P_STRING basicToken1, basicToken2, c_arguments ;
    m_PParsePassSpaces(&blotinstSequence, NULL);
    PParsePassChars(&blotinstSequence,b_REGULAR_SCAN, b_PASS_CHARS_WHILE,IsBasicTokenChar,
      (char)UNDEFINED, &basicToken1);

    m_PParsePassSpaces(&blotinstSequence, NULL);
    PParsePassChars(&blotinstSequence,b_REGULAR_SCAN, b_PASS_CHARS_WHILE,IsBasicTokenChar,
      (char)UNDEFINED, &basicToken2);

    m_PParsePassSpaces(&blotinstSequence, NULL);
    PParsePassChars(&blotinstSequence,b_REGULAR_SCAN, b_PASS_CHARS_TILL,NULL,'(', &dummy);
    if (!b_EMPTY_P_STRING(dummy)) {
      m_REPORT_ERROR(dummy,"Unexpected content before " DELIMITOR__C,'(')
    } // if
    if ((b_arguments = !b_EMPTY_P_STRING(blotinstSequence))) {
      PParseOffset(&blotinstSequence,1, NULL);
      PParsePassChars(&blotinstSequence,b_REVERTED_SCAN, b_PASS_CHARS_TILL,NULL,')', &c_arguments);
      if (b_EMPTY_P_STRING(blotinstSequence)) {
        m_REPORT_ERROR(c_arguments,"Missing " DELIMITOR__C, ')')
      } // if
      PParseOffset(&blotinstSequence,1, NULL);
      m_PParsePassSpaces(&blotinstSequence, NULL);
      if (!b_EMPTY_P_STRING(blotinstSequence)) {
        m_REPORT_ERROR(blotinstSequence,"Trailing content after " DELIMITOR__C, ')')
      } // if
    } // if
    // basicToken1: initialized (should not be empty) 
    // basicToken2: initilized (may be empty) 
    // b_arguments: TRUE => (arguments) lexeme located  
    // c_arguments: (when located) may be empty  

    // Stage 3: Finding actual "tokens" within blotinst "sequence" 
    struct P_STRING litteralKeyw, referral, litteralBlotval; 
    litteralKeyw = o_PString(GOOD_OLD_EMPTY_C_STRING);
    referral = o_PString(GOOD_OLD_EMPTY_C_STRING);
    litteralBlotval = o_PString(GOOD_OLD_EMPTY_C_STRING);

    if (b_arguments) { // Found '(' <arguments> ')' 
      if (!b_EMPTY_P_STRING(basicToken2)) { // two tokens 
        // <keyw> <blotfunct referral> '(' arguments ')' : 
        litteralKeyw = basicToken1;
        referral = basicToken2;
      } else {
        // <blotfunct referral> '(' <arguments> ')' :
        referral = basicToken1;
      } // if
    } else {
      // <keyw> [ <blotval> ] :
      litteralKeyw = basicToken1;
      if (!b_EMPTY_P_STRING(basicToken2)) {
        // <keyw> <blotval> :
        litteralBlotval = basicToken2;
      } // if
    } // if
    // litteralKeyw: initialized (may be empty)
    // referral: initialized (may be empty)
    // litteralBlotval: initialized (may be empty)

    // Stage 4: Fill-in blotinst with parsed tokens
    // Get new blotinst
    v_templatePartitionEntry = GreenCollectionFetch(handle->h_templatePartitionsHandle, -1,
      (char**)&ti_templatePartitionStuff) ;
    m_TRACK_IF(v_templatePartitionEntry < 0)
    o_DefautlBlotinstTemplatePartition(ti_templatePartitionStuff);
    vc_blotinstPtr = &ti_templatePartitionStuff->select.c_blotinst; 

    if (b_arguments) {
      switch (BlotcodeFindBlotfunc(handle->frozenBlotcodeHandle, referral,
        &vc_blotinstPtr->c_blotfunc.entry)) {
      case RESULT__FOUND: 
        vc_blotinstPtr->b_blotfunc = b_TRUE;
        o_BlotfuncCall(&vc_blotinstPtr->c_blotfunc.call, referral, c_arguments); 
      break; case RESULT__NOT_FOUND:
        m_REPORT_ERROR(referral, "Unknown blotfunc")
      break; default:
        m_TRACK()
      } // switch
    } // if

    if (!b_EMPTY_P_STRING(litteralBlotval)) {
      int parsedLength = UNDEFINED;
      switch (ReadGenericIntegerPString(litteralBlotval,  &vc_blotinstPtr->c_blotval,
        &parsedLength)) {
      case ANSWER__YES: 
        vc_blotinstPtr->b_blotval = b_TRUE;
        if (parsedLength < m_PStringLength(litteralBlotval)) {
          m_REPORT_ERROR(litteralBlotval, "Trailing chars in blotval")
        } // if
      break; case ANSWER__NO: 
        vc_blotinstPtr->b_blotval = b_FALSE0;
        m_REPORT_ERROR(litteralBlotval, "Invalid blotval")
      break; default:
        m_TRACK()
      } // switch
    } // if 

    if (!b_EMPTY_P_STRING(litteralKeyw)) { 
      switch (BlotcodeFindBlotkeyw(handle->frozenBlotcodeHandle, &litteralKeyw,
        &vc_blotinstPtr->blotkeywId)) {
      case RESULT__FOUND: 
      break; case RESULT__NOT_FOUND: 
        m_REPORT_ERROR(litteralKeyw, "Unknown blotkeyw")
      break; default:
        m_TRACK()
      } // switch
    } // if

    // *vc_blotinstPtr: "main" fields completed 

    // Stage 5: Verify blotinst's consistency

    switch (vc_blotinstPtr->blotkeywId) {
    case LOOP__BLOTKEYW_ID:
    case SWITCH__BLOTKEYW_ID:
    case CASE__BLOTKEYW_ID:
      if ((!vc_blotinstPtr->b_blotfunc) && (!vc_blotinstPtr->b_blotval)) {
        m_REPORT_ERROR(litteralKeyw, "Control statement without value")
      } // if
    break ; case NONE__BLOTKEYW_ID:
    case END_LOOP__BLOTKEYW_ID:
    case END_SWITCH__BLOTKEYW_ID:
    case DEFAULT_CASE__BLOTKEYW_ID:
    break; default:
      m_RAISE(ANOMALY__VALUE__D,vc_blotinstPtr->blotkeywId)
    } // switch

    switch (vc_blotinstPtr->blotkeywId) {
    case NONE__BLOTKEYW_ID:
    break; case LOOP__BLOTKEYW_ID:
    case SWITCH__BLOTKEYW_ID:
      vc_blotinstPtr->n_masterEntry = v_templatePartitionEntry; 
      m_C_STACK_PUSH(handle->h_flowControlStack,v_templatePartitionEntry)
    break; case CASE__BLOTKEYW_ID:                  
    case DEFAULT_CASE__BLOTKEYW_ID:                  
    case END_LOOP__BLOTKEYW_ID:
    case END_SWITCH__BLOTKEYW_ID:
      if (C_STACK_GET_COUNT(handle->h_flowControlStack) == 0) {
        m_REPORT_ERROR(litteralKeyw,"Orphan control statement (NO prior control statement)")

      } else { 
        struct BLOTINST *t_priorBlotinstPtr = (struct BLOTINST *)UNDEFINED; 
        { int priorEntry = UNDEFINED; 
          m_C_STACK_POP(handle->h_flowControlStack,priorEntry)
          TEMPLATE_PARTITION_STUFF t_priorTemplatePartitionStuff = 
          (TEMPLATE_PARTITION_STUFF) UNDEFINED;
          m_TRACK_IF(GreenCollectionFetch(handle->h_templatePartitionsHandle, priorEntry,
            (char**)&t_priorTemplatePartitionStuff) < 0);
          m_ASSERT(t_priorTemplatePartitionStuff->b_blotinst)
          t_priorBlotinstPtr = &t_priorTemplatePartitionStuff->select.c_blotinst; 
        } // priorEntry

        if (vc_blotinstPtr->blotkeywId == END_LOOP__BLOTKEYW_ID) {
          if (t_priorBlotinstPtr->blotkeywId != LOOP__BLOTKEYW_ID) { 
            m_REPORT_ERROR(litteralKeyw,
              "Orphan control statement (prior control statement is not a loop)")
          } // if 
        } else {
          switch (vc_blotinstPtr->blotkeywId) {
          case CASE__BLOTKEYW_ID:                  
          case DEFAULT_CASE__BLOTKEYW_ID:                  
            if (t_priorBlotinstPtr->blotkeywId == DEFAULT_CASE__BLOTKEYW_ID) {
              m_REPORT_ERROR(litteralKeyw,
                "Orphan control statement (prior control statement is a default case)")
            } // if 
          case END_SWITCH__BLOTKEYW_ID:
            if (t_priorBlotinstPtr->blotkeywId != SWITCH__BLOTKEYW_ID &&
                t_priorBlotinstPtr->blotkeywId != CASE__BLOTKEYW_ID &&
                t_priorBlotinstPtr->blotkeywId != 
                DEFAULT_CASE__BLOTKEYW_ID) { 
              m_REPORT_ERROR(litteralKeyw,"Orphan control statement "
                "(prior control statement is neither a switch, a case nor a default case)")
            } // if 
          break; default:
            m_RAISE(ANOMALY__VALUE__D,vc_blotinstPtr->blotkeywId)
          } // switch
        } // if 
           
        vc_blotinstPtr->n_masterEntry = t_priorBlotinstPtr->n_masterEntry;

        t_priorBlotinstPtr->n_subEntry = v_templatePartitionEntry;

        switch (vc_blotinstPtr->blotkeywId) {
        case END_LOOP__BLOTKEYW_ID:
        case END_SWITCH__BLOTKEYW_ID:
        break; default: // CASE / DEFAULT_CASE
          m_C_STACK_PUSH(handle->h_flowControlStack,v_templatePartitionEntry)
        } // switch
      } // if 
    break; default:
      m_RAISE(ANOMALY__VALUE__D,vc_blotinstPtr->blotkeywId)
    } // switch
  } // while

  int trailingBlotkeywsNumber = C_STACK_GET_COUNT(handle->h_flowControlStack);
  if (trailingBlotkeywsNumber > 0) {
    m_REPORT_ERROR(dummy, "Trailing %d loop or switch control statement(s)", trailingBlotkeywsNumber)
  } // if

#undef DELIMITOR__S
#undef DELIMITOR__C 
#undef m_REPORT_ERROR
  m_DIGGY_RETURN(ANSWER__YES)
} // BlotcodeExecutorParseTemplate 


// Public function ; see .h 
int BlotcodeExecutorConstructPage (BLOTCODE_EXECUTOR_HANDLE handle,
  SUCKER_HANDLE outputSuckerHandle,  struct BLOTFUNC *nac_abandonmentBlotfunc,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  int n_blotcodeConstructionStatus = -1; // Unknown for the moment
  
  int answer = UNDEFINED;
  int templatePartitionsCount = GreenCollectionGetCount(handle->h_templatePartitionsHandle,NULL) ;
  m_TRACK_IF(templatePartitionsCount < 0)

  struct TEMPLATE_PARTITION* templatePartitionsArray = (struct TEMPLATE_PARTITION*) UNDEFINED;
  m_TRACK_IF(GreenCollectionPullOut(handle->h_templatePartitionsHandle,
    (char **)&templatePartitionsArray) < 0)

  int i_entry = 0 ;
  struct TEMPLATE_PARTITION *v_templatePartitionPtr = templatePartitionsArray;
  for (; i_entry < templatePartitionsCount; i_entry++, v_templatePartitionPtr++) {
    if (v_templatePartitionPtr->b_blotinst) {
      v_templatePartitionPtr->select.c_blotinst.b_masterBlotval = b_FALSE0; 
    } // if
  } // for

  for (i_entry = 0, v_templatePartitionPtr = templatePartitionsArray; i_entry < 
    templatePartitionsCount && n_blotcodeConstructionStatus == -1; i_entry++,
    v_templatePartitionPtr++) {
    if (!v_templatePartitionPtr->b_blotinst) { // no instruction  
      switch(SuckerFillDButt(outputSuckerHandle,
        v_templatePartitionPtr->select.c_decor, NULL)) {
      case ANSWER__YES: // completed output successfully  
        // goto next partition:
      break; case ANSWER__NO: // flop while completing output
        n_blotcodeConstructionStatus = BLOTCODE_CONSTRUCTION_STATUS__OUTPUT_FLOP;
      break; default: 
        m_TRACK()
      } // switch
      continue ;
    } // if

    struct BLOTINST *blotinstPtr = &v_templatePartitionPtr->select.c_blotinst;

    if (blotinstPtr->b_blotfunc) {
      int blotlibEntry = blotinstPtr->c_blotfunc.entry.blotlibEntry; 
      int localBlotfuncNameEntry = blotinstPtr->c_blotfunc.entry.localBlotfuncNameEntry ;

      BLOTLIB_STUFF blotlibStuff = (BLOTLIB_STUFF) UNDEFINED;
      m_BLOTCODE_GET_BLOTLIB(handle->frozenBlotcodeHandle,  blotlibEntry,  blotlibStuff) 

      m_G_STRING_CLEAR(handle->h_blotfuncSurrogate)
      blotinstPtr->b_blotval = b_TRUE; 
      blotinstPtr->c_blotval = TRUE__BLOTVAL0; // default value 
      struct BLOTFUNC blotfunc, *a_blotfunc = &blotfunc; 
      if (nac_abandonmentBlotfunc != NULL) {
        a_blotfunc = nac_abandonmentBlotfunc; 
      } //
      a_blotfunc->entry.blotlibEntry = blotlibEntry ; 
      a_blotfunc->entry.localBlotfuncNameEntry = localBlotfuncNameEntry ;
      a_blotfunc->call = blotinstPtr->c_blotfunc.call; 
      m_ASSERT(blotlibStuff->localBlotfuncNamesNumber > 0)
      switch (answer = blotlibStuff->c_blotlibExecutorExecuteCFunction(
        handle->hshr_blotlibExecutorHandles[blotlibEntry],  a_blotfunc,
        handle->h_blotfuncSurrogate,  &blotinstPtr->c_blotval, nc_abandonmentInfo)) {
      case ANSWER__YES: // Blot function completed
        if (handle->h_blotfuncSurrogate->c_copiedLength > 0) {
          m_TRACK_IF(SuckerFillDButt(outputSuckerHandle, 
            m_GStringGetLogicalPString(handle->h_blotfuncSurrogate),  NULL) != 0)
        } // if
      break; case ANSWER__NO: // Abandon dynamic construction
        blotinstPtr->b_blotval = b_FALSE0;
        n_blotcodeConstructionStatus = BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED;
        continue;
      break; default: 
        m_TRACK()
      } // switch 
    } // if

    switch (blotinstPtr->blotkeywId) {
    case NONE__BLOTKEYW_ID : 
      // Goto next partition:

    break; case LOOP__BLOTKEYW_ID : 
      { // loop control
        char b_exit ;
        m_ASSERT(blotinstPtr->b_blotval)

        if (blotinstPtr->b_blotfunc) {
          // loop is function controlled;
          // exit when function is "false"
          b_exit = (blotinstPtr->c_blotval != ITERATE_LOOP__BLOTVAL0) ; 

        } else {
          // loop is value controlled;
          if (!blotinstPtr->b_masterBlotval) { // New loop "execution"
            // 0 iterations for the moment
            blotinstPtr->b_masterBlotval = b_TRUE ;
            blotinstPtr->c_masterBlotval = 0 ;
          } // if 
          // exit when number of iterations reach value 
          if ((b_exit = (blotinstPtr->c_masterBlotval >= blotinstPtr->c_blotval))) {
            // Leaving the loop:
            blotinstPtr->b_masterBlotval = b_FALSE0 ;
          } else {
            blotinstPtr->c_masterBlotval++ ;
          } 
        } // if

        if (b_exit) {
          // Goto partition following the "endLoop"
          m_ASSERT((i_entry = blotinstPtr->n_subEntry) != -1)
          v_templatePartitionPtr = templatePartitionsArray + i_entry; 
        } else {
          // Goto next partition:
        } // if
      } // loop control

    break; case END_LOOP__BLOTKEYW_ID : 
      // TBC: check also the jump partition correspond to "loop" statement 
      m_ASSERT(blotinstPtr->n_masterEntry != -1)
      v_templatePartitionPtr = templatePartitionsArray + 
      (i_entry = blotinstPtr->n_masterEntry - 1);

    break; case SWITCH__BLOTKEYW_ID : 
      m_ASSERT(blotinstPtr->b_blotval)
      // The master control value is to ensure only a single case will be executed
      if (!blotinstPtr->b_masterBlotval) {
        blotinstPtr->b_masterBlotval = b_TRUE;
        blotinstPtr->c_masterBlotval = NOT_MATCHED_SWITCH_CASE__MASTER__BLOTVAL;
      } // if 
      // Goto next partition:

    break; case CASE__BLOTKEYW_ID : 
    case DEFAULT_CASE__BLOTKEYW_ID : 
    case END_SWITCH__BLOTKEYW_ID : 
      { // Bloc
        struct BLOTINST *masterBlotinstPtr ;

        m_ASSERT(blotinstPtr->n_masterEntry != -1)
        masterBlotinstPtr = 
          &(templatePartitionsArray[blotinstPtr->n_masterEntry].select.c_blotinst);

        // TBC: check also the control instruction correspond to "switch" statement 
        m_ASSERT(masterBlotinstPtr->b_masterBlotval)

        if (blotinstPtr->blotkeywId == END_SWITCH__BLOTKEYW_ID) {
          masterBlotinstPtr->b_masterBlotval = b_FALSE0 ;
          // Goto next partition:

        } else {
          m_ASSERT(blotinstPtr->blotkeywId != CASE__BLOTKEYW_ID ||
            (blotinstPtr->b_blotval && masterBlotinstPtr->b_blotval))
          if (masterBlotinstPtr->c_masterBlotval == NOT_MATCHED_SWITCH_CASE__MASTER__BLOTVAL
            && (blotinstPtr->blotkeywId == DEFAULT_CASE__BLOTKEYW_ID ||
            blotinstPtr->c_blotval == masterBlotinstPtr->c_blotval)) {
            masterBlotinstPtr->c_masterBlotval = MATCHED_SWITCH_CASE__MASTER__BLOTVAL0 ;
            // Goto next partition:
          } else {
            // TBC: check also the next partition correspond to case, default 
            // or endSwitch instruction
            // Normally controlled at parsing stage...
            m_ASSERT(blotinstPtr->n_subEntry >= 0)
            v_templatePartitionPtr = templatePartitionsArray + 
            (i_entry = blotinstPtr->n_subEntry - 1);
          } // if
        } // if
      } // Bloc 

    break; default: 
      m_RAISE(ANOMALY__VALUE__D, blotinstPtr->blotkeywId)
    } // switch 
  } // for 
  if (n_blotcodeConstructionStatus == -1) {
    n_blotcodeConstructionStatus = BLOTCODE_CONSTRUCTION_STATUS__OK; 
  } // if
  m_DIGGY_RETURN(n_blotcodeConstructionStatus)
} // BlotcodeExecutorConstructPage


// Public function ; see .h
int BlotcodeExecutorDestroyInstance (BLOTCODE_EXECUTOR_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  { BLOTLIB_STUFF blotlibStuff = (BLOTLIB_STUFF) UNDEFINED;
    int i = 0; void **shr_blotlibExecutorHandlePtr = xh_handle->hshr_blotlibExecutorHandles;
    for (; i < xh_handle->blotlibsCount; i++, shr_blotlibExecutorHandlePtr++) {
      m_BLOTCODE_GET_BLOTLIB(xh_handle->frozenBlotcodeHandle, i, blotlibStuff)
      m_TRACK_IF(blotlibStuff->c_blotlibExecutorDestroyInstanceFunction(
        *shr_blotlibExecutorHandlePtr) != RETURNED)
    } // for
  } // blotlibStuff
  free(xh_handle->hshr_blotlibExecutorHandles);

  m_C_STACK_FREE(xh_handle->h_flowControlStack)

  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->h_templatePartitionsHandle) != RETURNED)

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(xh_handle->h_blotfuncSurrogate) != RETURNED)

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // BlotcodeExecutorDestroyInstance 

