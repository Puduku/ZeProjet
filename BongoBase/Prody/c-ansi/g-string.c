// c-ansi/g-string.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdarg.h>
#include <errno.h>


#include "c-ansi/stderr.h"
#include "c-ansi/p-string.h"
#include "c-ansi/g-string.h"
#include "c-ansi/alloc.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// G_STRING
// ========


// Ensure presence of "physical" buffer 
// POST: stuff in copied state
// POST: stuff's "logical" value NOT changed
#define m_G_STRING_SET_INITIAL_BUFFER(/*G_STRING_STUFF*/stuff, /*int*/u_minimalInitialBufferSize) {\
  if (stuff->nhi_string == NULL) {\
    int em_initialBufferSize = u_minimalInitialBufferSize; \
    int em_stringPortionLength = m_StringPortionLength(&stuff->cv_stringPortion);\
    if (em_initialBufferSize < OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(em_stringPortionLength)) \
      em_initialBufferSize = OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(em_stringPortionLength);\
    m_MALLOC((stuff)->nhi_string, (stuff)->c_bufferSize = em_initialBufferSize)\
    stuff->c_copiedLength = CopyStringPortion(stuff->nhi_string,(stuff)->c_bufferSize,\
      &(stuff)->cv_stringPortion);\
    m_TRACK_IF(stuff->c_copiedLength < 0) \
  } \
}

// PRE: stuff in copied state (implies "logical" value == "physical" value)
// POST: offset is adjusted (mn_offset >= 0)
#define m_G_STRING_ADJUST_COPY_OFFSET(/*G_STRING_STUFF*/stuff,mn_offset) {\
  if (mn_offset == -1 || mn_offset > (stuff)->c_copiedLength) {\
    mn_offset = (stuff)->c_copiedLength;\
  } \
}


// Public function : see .h
int GStringCopy(G_STRING_STUFF stuff, int n_offset, const struct STRING_PORTION *ap_stringPortion) {
  m_DIGGY_BOLLARD()

  int requiredBufferSize = OPTIMAL_BUFFER_SIZE_4_STRING_PORTION_COPY(m_StringPortionLength(
    ap_stringPortion));
  m_G_STRING_SET_INITIAL_BUFFER(stuff, requiredBufferSize)
  m_G_STRING_ADJUST_COPY_OFFSET(stuff,n_offset) 
  requiredBufferSize += n_offset;
  if (requiredBufferSize >= stuff->c_bufferSize) {
    m_REALLOC(stuff->nhi_string, stuff->c_bufferSize = requiredBufferSize)
  } 
  stuff->c_copiedLength = CopyStringPortion(stuff->nhi_string + (n_offset),
    stuff->c_bufferSize - (n_offset),  ap_stringPortion) + (n_offset);

  m_ASSIGN_STRING_PORTION(stuff->cv_stringPortion,stuff->nhi_string,stuff->c_copiedLength)

  m_DIGGY_RETURN(stuff->c_copiedLength)
} // GStringCopy

// Public function : see .h
int GStringImport(G_STRING_STUFF stuff, const struct STRING_PORTION *afp_stringPortion) {
  m_DIGGY_BOLLARD()
  
  if (stuff->nhi_string == NULL) {
    stuff->cv_stringPortion = *afp_stringPortion;
    m_DIGGY_RETURN(COMPLETED__OK)
  } // if
  int ret = GStringCopy(stuff,0,afp_stringPortion);
  m_TRACK_IF(ret < 0) 
  m_DIGGY_RETURN(COMPLETED__BUT)
} // GStringImport

#define PRINTF_MINIMAL_INITIAL_G_STRING_BUFFER_SIZE 8

// Public function : see .h
int GStringPrintf(G_STRING_STUFF stuff,  int n_offset,  const char *p_format, ...) {
  m_DIGGY_BOLLARD()

  m_G_STRING_SET_INITIAL_BUFFER(stuff,PRINTF_MINIMAL_INITIAL_G_STRING_BUFFER_SIZE)
  m_G_STRING_ADJUST_COPY_OFFSET(stuff,n_offset)

  int ret = -1 ;
  int try = 1 ;
  for (; try == 1 || ret >= stuff->c_bufferSize - n_offset; try++) {
    m_ASSERT(try <= 2)
    if (try == 2) {
      m_REALLOC(stuff->nhi_string, stuff->c_bufferSize = ret + n_offset + 1)
    } // if
    { va_list arguments;
      va_start(arguments,p_format);
      ret = vsnprintf(stuff->nhi_string + n_offset,stuff->c_bufferSize - n_offset,
        p_format,arguments);
      va_end(arguments);
    } // arguments
    m_ASSERT(ret >= 0) // To bad for compiler versions prior to C99...
  } // for
  stuff->c_copiedLength = n_offset + ret;

  m_ASSIGN_STRING_PORTION(stuff->cv_stringPortion,stuff->nhi_string,stuff->c_copiedLength)
  m_DIGGY_RETURN(stuff->c_copiedLength)
} // GStringPrintf


// Public function : see .h
int GStringConvert(G_STRING_STUFF stuff,  IS_CHAR_FUNCTION n_isNeutralCharFunction,
  TO_CHAR_FUNCTION toCharFunction) {
  m_DIGGY_BOLLARD()

  m_G_STRING_SET_INITIAL_BUFFER(stuff, 1)
  stuff->c_copiedLength = ConvertStringPortion(&stuff->cv_stringPortion,b_C_TERMINATED,
    n_isNeutralCharFunction,toCharFunction);
  m_TRACK_IF(stuff->c_copiedLength < 0) 
  m_ASSERT(stuff->c_copiedLength < stuff->c_bufferSize) 
  m_DIGGY_RETURN(stuff->c_copiedLength);
} // GStringConvert


// G_STRING_SET
// ============

// Public function : see .h
int GStringSetCreateInstance(G_STRING_SET_STUFF *azh_stuff,  int cardinality) {
  m_DIGGY_BOLLARD()
  m_CALLOC( *azh_stuff, cardinality,sizeof(struct G_STRING) )

  m_DIGGY_RETURN(RETURNED)
} // GStringSetCreateInstance 


// Passed:
// - notNamedObjectStuff:
// - cardinality: 
static int GStringSetDisengage(G_STRING_SET_STUFF notNamedObjectStuff,  int cardinality) {
  m_DIGGY_BOLLARD()
  int i = 0;

  for (; i < cardinality; i++, notNamedObjectStuff++) {
    if (notNamedObjectStuff->nhi_string != NULL) {
      free(notNamedObjectStuff->nhi_string) ;
      notNamedObjectStuff->nhi_string = NULL;
    } // if
  } // for

  m_DIGGY_RETURN(RETURNED)
} // GStringSetDisengage


// Public function : see .h
int GStringSetDestroyInstance(G_STRING_SET_STUFF xh_notNamedObjectStuff,  int cardinality) {
  m_DIGGY_BOLLARD()

  m_TRACK_IF(GStringSetDisengage(xh_notNamedObjectStuff,cardinality) != RETURNED)
  free(xh_notNamedObjectStuff) ;

  m_DIGGY_RETURN(RETURNED)
} // GStringSetDestroyInstance


// G_STRINGS
// =========


struct KEY_SETTINGS {
  int gStringSetElement;
  int gKeysComparison;
  union {
    // Only significant with STRING_PORTION__G_KEYS_COMPARISON:
    struct {
      IS_CHAR_FUNCTION cn_isNeutralCharFunction;
      TO_CHAR_FUNCTION cn_toCharFunction;
    } stringPortionComparison ;
    // Only significant with INTRINSIC_VALUE__G_KEYS_COMPARISON:
    struct {
      STRING_PORTION_INTRINSIC_VALUE_FUNCTION stringPortionIntrinsicValueFunction;
      void *pr_stringPortionIntrinsicValueFunctionHandle;
    } intrinsicValueComparison ;
  } select ; 
};


struct INDEX_PROPERTIES {
  int keysNumber;
  struct KEY_SETTINGS *hs_keysSettings;
};


struct G_STRINGS {
  GREEN_COLLECTION_HANDLE h_greenCollectionHandle;
  int gStringSetCardinality;
  int n_gStringConveyance; 
  const int *cps_gStringConveyances; // not significant if general conveyance is specified
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION c_namedObjectDestroyInstanceFunction;
  int indexesNumber;
  struct INDEX_PROPERTIES *vnhs_indexesProperties;
  m_DECLARE_MAGIC_FIELD(G_STRINGS_HANDLE)
};


// #see GREEN_ITEM_HANDLER__DISENGAGE_FUNCTION@c-ansi/green.h <gString>
static int GStringsDisengage(void *r_handle,  char *r_greenItemStuff) {
  G_STRINGS_HANDLE handle = (G_STRINGS_HANDLE) r_handle;
  m_CHECK_MAGIC_FIELD(G_STRINGS_HANDLE,handle)
  G_STRING_SET_STUFF gStringSetStuff = (G_STRING_SET_STUFF) r_greenItemStuff;
  
  int i = 0;
  for (; i < handle->gStringSetCardinality ; i++) {
    if ((handle->n_gStringConveyance >= 0? handle->n_gStringConveyance:
      (handle->cps_gStringConveyances)[i]) == NAMED_OBJECT__G_STRING_CONVEYANCE &&
      gStringSetStuff->acolyt.cnhr_handle != NULL) {
      m_TRACK_IF(handle->c_namedObjectDestroyInstanceFunction(gStringSetStuff[i].acolyt.cnhr_handle)
      != RETURNED)
    } // if
  } // for 
  m_TRACK_IF(GStringSetDisengage(gStringSetStuff, handle->gStringSetCardinality) != RETURNED)

  return RETURNED;
} // GStringsDisengage


// Passed:
// - m_bareGKey:
// - m_gKeysComparison:
// - ap_gKey:
#define m_ASSIGN_BARE_G_KEY__G_KEY(/*union BARE_G_KEY*/ m_bareGKey,\
  /*int*/ m_gKeysComparison,  /*const struct G_KEY* */ ap_gKey) {\
  m_ASSERT((ap_gKey)->gKeysComparison == m_gKeysComparison)\
  m_bareGKey = ap_gKey->bare;\
} 

// Passed:
// - m_bareGKey:
// - m_gKeysComparison:
// - p_gStringStuff:
// - c_stringPortionIntrinsicValueFunction:
// - cpr_stringPortionIntrinsicValueFunctionHandle:
#define m_ASSIGN_BARE_G_KEY__G_STRING(/*union BARE_G_KEY*/m_bareGKey, /*int*/gKeysComparison,\
  /*G_STRING_STUFF*/ p_gStringStuff,\
  /*STRING_PORTION_INTRINSIC_VALUE_FUNCTION*/c_stringPortionIntrinsicValueFunction,\
  /*void* */cpr_stringPortionIntrinsicValueFunctionHandle) {\
  switch (gKeysComparison) {\
  case STRING_PORTION__G_KEYS_COMPARISON: \
    (m_bareGKey).cp_stringPortion = (p_gStringStuff)->cv_stringPortion;\
  break; case INTRINSIC_VALUE__G_KEYS_COMPARISON:\
    (m_bareGKey).cen_intrinsicValue = (c_stringPortionIntrinsicValueFunction)\
      (cpr_stringPortionIntrinsicValueFunctionHandle,&(p_gStringStuff)->cv_stringPortion);\
  break; case ACOLYT_TOKEN_ID__G_KEYS_COMPARISON:\
    (m_bareGKey).c_acolytTokenId = (p_gStringStuff)->acolyt.c_tokenId;\
  break; case ACOLYT_VALUE__G_KEYS_COMPARISON:\
    (m_bareGKey).cen_acolytValue = (p_gStringStuff)->acolyt.cen_value;\
  break; case ACOLYT_HANDLE__G_KEYS_COMPARISON:\
    (m_bareGKey).cnr_acolytHandle = (p_gStringStuff)->acolyt.cnhr_handle;\
  break; default:\
    m_RAISE(ANOMALY__VALUE__FMT_D,gKeysComparison)\
  }\
}


// #SEE GREEN_ITEM_HANDLER__KEYS_COMPARE_FUNCTION @ c-ansi/green.h
static int GStringsKeysCompare(void *cpr_handle,  char b_frozen, int indexLabel,  int keyRank,
  char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff,  void *cpr_bKeys) {
  m_DIGGY_BOLLARD()
  G_STRINGS_HANDLE p_handle = (G_STRINGS_HANDLE) cpr_handle;
  m_CHECK_MAGIC_FIELD(G_STRINGS_HANDLE,p_handle)
  m_ASSERT(indexLabel < p_handle->indexesNumber)

  // Key settings:
  struct KEY_SETTINGS *ap_keySettings = (struct KEY_SETTINGS *)UNDEFINED;
  { struct INDEX_PROPERTIES *ap_indexProperties = p_handle->vnhs_indexesProperties + indexLabel;

    m_ASSERT(keyRank < ap_indexProperties->keysNumber)
    ap_keySettings = ap_indexProperties->hs_keysSettings + keyRank;
  } // struct INDEX_PROPERTIES

  // Bare key 'a':
  union BARE_G_KEY aBareGKey ;
  { G_STRING_STUFF p_aGStringStuff = (G_STRING_STUFF) pr_aGreenItemStuff +
      ap_keySettings->gStringSetElement;
    m_ASSIGN_BARE_G_KEY__G_STRING(aBareGKey,  ap_keySettings->gKeysComparison, p_aGStringStuff,
      ap_keySettings->select.intrinsicValueComparison.stringPortionIntrinsicValueFunction,
      ap_keySettings->select.intrinsicValueComparison.pr_stringPortionIntrinsicValueFunctionHandle);
  } // G_STRING_STUFF

  // Bare key 'b':
  union BARE_G_KEY bBareGKey ;
  if (npr_bGreenItemStuff != NULL) {
    G_STRING_STUFF p_bGStringStuff = (G_STRING_STUFF) npr_bGreenItemStuff + 
      ap_keySettings->gStringSetElement;
    m_ASSIGN_BARE_G_KEY__G_STRING(bBareGKey,  ap_keySettings->gKeysComparison, p_bGStringStuff,
      ap_keySettings->select.intrinsicValueComparison.stringPortionIntrinsicValueFunction,
      ap_keySettings->select.intrinsicValueComparison.pr_stringPortionIntrinsicValueFunctionHandle);
  } else {
    const struct G_KEY *ap_bGKey = ((const struct G_KEY **)cpr_bKeys)[keyRank];
    m_ASSIGN_BARE_G_KEY__G_KEY(bBareGKey,  ap_keySettings->gKeysComparison,  ap_bGKey)
  } // if
  
  // Compare bare keys:
  int comparison = UNDEFINED;
  switch (ap_keySettings->gKeysComparison) {
  case STRING_PORTION__G_KEYS_COMPARISON:
    comparison = CompareStringPortions(&aBareGKey.cp_stringPortion,  &bBareGKey.cp_stringPortion,
      ap_keySettings->select.stringPortionComparison.cn_isNeutralCharFunction,
      ap_keySettings->select.stringPortionComparison.cn_toCharFunction); 
  break; case INTRINSIC_VALUE__G_KEYS_COMPARISON:
    comparison = GET_COMPARISON(aBareGKey.cen_intrinsicValue,bBareGKey.cen_intrinsicValue);
  break; case ACOLYT_TOKEN_ID__G_KEYS_COMPARISON:
    comparison = GET_COMPARISON(aBareGKey.c_acolytTokenId,bBareGKey.c_acolytTokenId);
  break; case ACOLYT_VALUE__G_KEYS_COMPARISON:
    comparison = GET_COMPARISON(aBareGKey.cen_acolytValue,bBareGKey.cen_acolytValue);
  break; case ACOLYT_HANDLE__G_KEYS_COMPARISON:
    comparison = GET_COMPARISON(aBareGKey.cnr_acolytHandle,bBareGKey.cnr_acolytHandle);
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,ap_keySettings->gKeysComparison)
  } // switch
  
m_DIGGY_VAR_COMPARISON(comparison)
  m_DIGGY_RETURN(comparison)
} // GStringsKeysCompare


// Public function : see .h
int GStringsCreateInstance(G_STRINGS_HANDLE* azh_handle,  int expectedItemsNumber,
  int gStringSetCardinality, int n_gStringConveyance, const int *cnfps_gStringConveyances,
  NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION c_namedObjectDestroyInstanceFunction) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  G_STRINGS_HANDLE handle = *azh_handle;

  handle->gStringSetCardinality = gStringSetCardinality;
  if (n_gStringConveyance == -1 && cnfps_gStringConveyances == NULL) n_gStringConveyance =
    TOKEN__G_STRING_CONVEYANCE ;
  handle->n_gStringConveyance = n_gStringConveyance; 
  handle->cps_gStringConveyances = cnfps_gStringConveyances; 
  handle->c_namedObjectDestroyInstanceFunction = c_namedObjectDestroyInstanceFunction;
 
  handle->indexesNumber = 0;
  handle->vnhs_indexesProperties = NULL;

  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_greenCollectionHandle,  expectedItemsNumber,
    sizeof(struct G_STRING),  GStringsDisengage,  GStringsKeysCompare, NULL, handle) != RETURNED)

  m_ASSIGN_MAGIC_FIELD(G_STRINGS_HANDLE,handle)

  m_DIGGY_RETURN(RETURNED)
} // GStringsCreateInstance


// Public function : see .h
int GStringsFetch(G_STRINGS_HANDLE cp_handle,  int n_entry,
      G_STRING_SET_STUFF* acnt_gStringSetStuff) {
  m_DIGGY_BOLLARD()
  int entry = GreenCollectionFetch(cp_handle->h_greenCollectionHandle,n_entry,
    (char **)acnt_gStringSetStuff) ;
  m_TRACK_IF(entry < 0)
  m_DIGGY_RETURN(entry)
} // GStringsFetch


// Public function : see .h
int GStringsGetCount(G_STRINGS_HANDLE cp_handle,
      G_STRING_SET_STUFF *navnt_gStringSetStuff) {
  m_DIGGY_BOLLARD()
  int count = GreenCollectionGetCount(cp_handle->h_greenCollectionHandle,
    (char **)navnt_gStringSetStuff) ;
  m_TRACK_IF(count < 0)
  m_DIGGY_RETURN(count)
} // GStringsGetCount


// Public function : see .h
int GStringsAddIndex (G_STRINGS_HANDLE handle,  int keysNumber,
  int key1GStringSetElement,  int key1GKeysComparison,
  IS_CHAR_FUNCTION cn_key1IsNeutralCharFunction,  TO_CHAR_FUNCTION cn_key1ToCharFunction,
  STRING_PORTION_INTRINSIC_VALUE_FUNCTION c_key1StringPortionIntrinsicValueFunction,
  void *cfpr_key1StringPortionIntrinsicValueFunctionHandle,  ...) {
  m_DIGGY_BOLLARD()

  m_REALLOC_ARRAY(handle->vnhs_indexesProperties,++(handle->indexesNumber))

  struct INDEX_PROPERTIES *a_indexProperties =
    handle->vnhs_indexesProperties + handle->indexesNumber-1 ;

  m_MALLOC_ARRAY(a_indexProperties->hs_keysSettings,a_indexProperties->keysNumber = keysNumber)

  va_list ap ;
  va_start(ap,cfpr_key1StringPortionIntrinsicValueFunctionHandle) ;

  int i = 0;
  struct KEY_SETTINGS *s_keysSettings = a_indexProperties->hs_keysSettings;
  for ( ; i < keysNumber ; i++, s_keysSettings++) {
    if (i > 0) { 
      key1GStringSetElement = va_arg(ap,int);
      key1GKeysComparison = va_arg(ap,int);
      cn_key1IsNeutralCharFunction = va_arg(ap,IS_CHAR_FUNCTION);
      cn_key1ToCharFunction = va_arg(ap,TO_CHAR_FUNCTION);
      c_key1StringPortionIntrinsicValueFunction = va_arg(ap,STRING_PORTION_INTRINSIC_VALUE_FUNCTION);
      cfpr_key1StringPortionIntrinsicValueFunctionHandle = va_arg(ap,void *);
      
    } // if
    m_ASSERT(key1GStringSetElement < handle->gStringSetCardinality)
    s_keysSettings->gStringSetElement = key1GStringSetElement;
    int gStringConveyance = (handle->n_gStringConveyance >= 0? handle->n_gStringConveyance:
      handle->cps_gStringConveyances[key1GStringSetElement]);
    switch (s_keysSettings->gKeysComparison = key1GKeysComparison) {
    case STRING_PORTION__G_KEYS_COMPARISON:
    case INTRINSIC_VALUE__G_KEYS_COMPARISON:
    break; case ACOLYT_TOKEN_ID__G_KEYS_COMPARISON:
      m_ASSERT(gStringConveyance == TOKEN__G_STRING_CONVEYANCE)
    break; case ACOLYT_VALUE__G_KEYS_COMPARISON:
      m_ASSERT(gStringConveyance == VALUED_STRING__G_STRING_CONVEYANCE)
    break; case ACOLYT_HANDLE__G_KEYS_COMPARISON:
      m_ASSERT(gStringConveyance == NAMED_OBJECT__G_STRING_CONVEYANCE)
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,key1GKeysComparison)
    } // switch
    s_keysSettings->select.stringPortionComparison.cn_isNeutralCharFunction = cn_key1IsNeutralCharFunction;
    s_keysSettings->select.stringPortionComparison.cn_toCharFunction = cn_key1ToCharFunction;
    s_keysSettings->select.intrinsicValueComparison.stringPortionIntrinsicValueFunction =
      c_key1StringPortionIntrinsicValueFunction;
    s_keysSettings->select.intrinsicValueComparison.pr_stringPortionIntrinsicValueFunctionHandle =
      cfpr_key1StringPortionIntrinsicValueFunctionHandle;
   } // for
  va_end(ap) ;

  int indexLabel =
  GreenCollectionAddIndex(handle->h_greenCollectionHandle, keysNumber) ;
  m_TRACK_IF(indexLabel < 0) ;

  m_DIGGY_RETURN(indexLabel)
} // GStringsAddIndex


// Public function : see .h
int GStringsIndexRequest(G_STRINGS_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, int criteriaNumber,
  int indexLabel1, unsigned int indexSeekFlags1, const struct G_KEY *cfps_keys1, ...) {
  m_DIGGY_BOLLARD()

  //  va_list ap ;
  //  va_start(ap,cafp_key1_1) ;
  //      cap_key1 = va_arg(ap,const struct G_KEY*) ;
  //  va_end(ap) ;

  m_TRACK_IF(GreenCollectionIndexRequest(cp_handle->h_greenCollectionHandle,
    nf_indexRequestAutomaticBuffer, criteriaNumber, indexLabel1, indexSeekFlags1,
    (void*)cfps_keys1) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // GStringsIndexRequest


// Public function : see .h
int GStringsIndexFetch(G_STRINGS_HANDLE cp_handle,
  INDEX_REQUEST_AUTOMATIC_BUFFER nf_indexRequestAutomaticBuffer, unsigned int indexFetchFlags,
  G_STRING_SET_STUFF *acvnt_gStringSetStuff, int *nacvn_entry) {
  m_DIGGY_BOLLARD()

  int result = GreenCollectionIndexFetch(cp_handle->h_greenCollectionHandle,
    nf_indexRequestAutomaticBuffer, indexFetchFlags, (char **)acvnt_gStringSetStuff,nacvn_entry);
  m_TRACK_IF(result < 0)
  m_DIGGY_RETURN(result)
} // GStringsIndexFetch


// Public function : see .h
int GStringsVerifyIndexes(G_STRINGS_HANDLE handle) {
  m_DIGGY_BOLLARD()

  int completed = GreenCollectionVerifyIndexes(handle->h_greenCollectionHandle) ;
  m_TRACK_IF(completed < 0)
  
  m_DIGGY_RETURN(completed)
} // GStringsVerifyIndexes


// Public function : see .h
int GStringsPullOut(G_STRINGS_HANDLE handle,  G_STRINGS_ARRAY *at_gStringsArray) {
  m_DIGGY_BOLLARD()
  int count = GreenCollectionPullOut(handle->h_greenCollectionHandle, (char**)at_gStringsArray) ;
  m_ASSERT(count > 0)

  m_DIGGY_RETURN(count)
} // GStringsPullOut


// Public function : see .h
int GStringsFreeze(G_STRINGS_HANDLE handle, G_STRINGS_ARRAY **nap_gStringsArray) {
  m_DIGGY_BOLLARD()
  int count = GreenCollectionFreeze(handle->h_greenCollectionHandle, (char**)nap_gStringsArray);
  m_TRACK_IF(count <= 0)

  m_DIGGY_RETURN(count)
} // GStringsFreeze


// Public function : see .h
int GStringsClear(G_STRINGS_HANDLE handle, char b_fullClear) {
  m_DIGGY_BOLLARD()

  if (b_fullClear) {
    int count = GreenCollectionGetCount(handle->h_greenCollectionHandle,NULL);
    m_TRACK_IF(count < 0)
    
    int i = 0;
    for ( ; i < count ; i++) {
      G_STRING_STUFF n_gStringStuff = (G_STRING_STUFF)UNDEFINED;
      m_TRACK_IF(GreenCollectionFetch(handle->h_greenCollectionHandle,i, (char**)&n_gStringStuff) !=
        RETURNED)
      if (n_gStringStuff != NULL) {
        m_G_STRING_CLEAR(n_gStringStuff)   
      } // if
    } // for
  } // if
      
  m_TRACK_IF(GreenCollectionClear(handle->h_greenCollectionHandle) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GStringsClear

// Public function : see .h
int GStringsGetConveyance(G_STRINGS_HANDLE handle, int element) {
  m_DIGGY_BOLLARD()
  m_ASSERT(element < handle->gStringSetCardinality)
  if (handle->n_gStringConveyance >= 0) m_DIGGY_RETURN(handle->n_gStringConveyance)
  else m_DIGGY_RETURN(handle->cps_gStringConveyances[element])
} // GStringsGetConveyance 

// Public function : see .h
int GStringsDestroyInstance(G_STRINGS_HANDLE  xh_handle) {
  m_DIGGY_BOLLARD()
  if (xh_handle->indexesNumber > 0) {
    free(xh_handle->vnhs_indexesProperties);
  } // if

  int ret = GreenCollectionDestroyInstance(xh_handle->h_greenCollectionHandle) ;
  m_TRACK_IF(ret != RETURNED)

  m_DIGGY_RETURN(ret)
} // GStringsDestroyInstance
