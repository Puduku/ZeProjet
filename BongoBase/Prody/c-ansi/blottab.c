// blottab.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2024

#include "c-ansi/blottab.h"
#include "c-ansi/blotcode.h"
#include "c-ansi/blotex.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/named-object.h"
#include "c-ansi/stderr.h"

#include "c-ansi/testy-diggy.h"

#include <limits.h>



struct BLOTTAB {
  m_DECLARE_MAGIC_FIELD(BLOTTAB_HANDLE)
  G_STRINGS_HANDLE h_tableHandle;
  G_STRINGS_HANDLE hp_fieldAttributesHandle;
} ; 
typedef struct BLOTTAB* BLOTTAB_HANDLE;

//#define BLOTTAB_FIELDS_MAX_NUMBER 10


enum {
  BLOTTAB_FIELD_ATTRIBUTE_CORE_ELEMENT,
  BLOTTAB_FIELD_ATTRIBUTE_STR_INDEX_ELEMENT,
  BLOTTAB_FIELD_ATTRIBUTE_INT_INDEX_ELEMENT,
  BLOTTAB_FIELD_ATTRIBUTE_ELEMENTS_NUMBER 
} ;

// Public function; see .h
int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexFlags) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  BLOTTAB_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(BLOTTAB_HANDLE,handle)

  m_TRACK_IF(GStringsCreateInstance(&handle->h_tableHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
    fieldsNumber, VALUED_STRING__G_STRING_CONVEYANCE, (const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION) UNDEFINED) != RETURNED)

  m_TRACK_IF(GStringsCreateInstance(&handle->hp_fieldAttributesHandle,fieldsNumber,
    BLOTTAB_FIELD_ATTRIBUTE_ELEMENTS_NUMBER, VALUED_STRING__G_STRING_CONVEYANCE,
    (const int *)UNDEFINED, (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_ASSERT(GStringsAddIndex(handle->hp_fieldAttributesHandle,1,
    0,P_STRING__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)NULL,(TO_CHAR_FUNCTION)NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == INDEX_LABEL0)

  G_STRING_SET_STUFF fieldAttributeStuff = (G_STRING_SET_STUFF)UNDEFINED; 
  int c_indexLabel = UNDEFINED;
  int i = 0; for (; i < fieldsNumber; i++) {
    m_TRACK_IF(GStringsFetch(handle->hp_fieldAttributesHandle, -1, &fieldAttributeStuff) != i)
    m_ASSERT(fieldAttributeStuff != NULL)
    m_TRACK_IF(GStringCopy(fieldAttributeStuff,0, s_names[i]) < 0) 
    fieldAttributeStuff->acolyt.cen_value = s_blottabIndexFlags[i] ; 
    if (b_FLAG_SET_ON(s_blottabIndexFlags[i],STR__BLOTTAB_INDEX_FLAG)) m_TRACK_IF((c_indexLabel =
      GStringsAddIndex(handle->h_tableHandle,1,i, P_STRING__G_KEYS_COMPARISON,NULL,NULL,
      (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED, (void*)UNDEFINED)) < 0)
    fieldAttributeStuff[BLOTTAB_FIELD_ATTRIBUTE_STR_INDEX_ELEMENT].acolyt.cen_value = c_indexLabel ;
    if (b_FLAG_SET_ON(s_blottabIndexFlags[i],INT__BLOTTAB_INDEX_FLAG)) m_TRACK_IF((c_indexLabel =
      GStringsAddIndex(handle->h_tableHandle,1,i,ACOLYT_VALUE__G_KEYS_COMPARISON ,NULL,NULL,
      (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED)) < 0)
    fieldAttributeStuff[BLOTTAB_FIELD_ATTRIBUTE_INT_INDEX_ELEMENT].acolyt.cen_value = c_indexLabel ;
  } // for

  m_DIGGY_RETURN(RETURNED)
} // BlottabCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int BlottabDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  BLOTTAB_HANDLE xh_handle = (BLOTTAB_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTTAB_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_tableHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->hp_fieldAttributesHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // BlottabDestroyInstance

// Public function; see .h
int l_BlotexlibExecutorCreateGenuineBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexFlags,
  BLOTTAB_HANDLE *a_blottabHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blottabName)

  m_TRACK_IF(BlottabCreateInstance(a_blottabHandle,fieldsNumber, s_names,s_blottabIndexFlags) !=
    RETURNED)

  m_TRACK_IF(BlotexlibExecutorAddBlottab(handle,GENUINE_BLOTTAB_LABEL0,blottabName,
    *a_blottabHandle) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // l_BlotexlibExecutorCreateGenuineBlottab

// Blottab expressions parsing:

#define MISSING_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Missing string index in table"
#define MISSING_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Missing integer index in table"
#define UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE "Unknown blottab"
#define NOT_EXISTING_BLOTSET__ABANDONMENT_CAUSE "Not existing blotset"
#define UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE "Unknown blottab field"
#define ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE "Blottab already exists"

// Parse and retrieve blottab referenced element or index label
//
// Passed:
// - *a_sequence: before parsing
// - fieldAttributesHandle: blottab fields definition 
// - nac_tableIndexLabel: != NULL special value if seek for created table index
// - cac_element: only significant if not seek for created table index
//
// Changed:
// - *a_sequence: after parsing
// - *nac_tableIndexLabel: (when seek for table index) only significant if no abandon
// - *cac_element: (when not seek for table index) only significant if no abandon
// - *ac_asValue: only significant if no abandon; either "as value int" or "as value str"; default
//   to "as value int" (must correspond to existing index when seek for table index)
// - nc_abandonmentInfo: only significant when parsing abandoned 
//
// Ret:
// - ANSWER__YES: Ok
// - ANSWER__NO: parsing abandoned
static int ParseAndRetrieveBlottabElement(struct P_STRING *a_sequence, G_STRINGS_HANDLE fieldAttributesHandle,
  int *nac_tableIndexLabel, int *cac_element, int *ac_asValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  G_STRING_SET_STUFF fieldAttributeStuff = (G_STRING_SET_STUFF)UNDEFINED;
  struct P_STRING fieldName = UNDEFINED_P_STRING;
  *ac_asValue = UNDEFINED;
 
  m_PREPARE_ABANDON(a_sequence,"<entity> [ <<as value int>> | <<as value str>> ]")
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&fieldName); // <entity>
  m_TRACK_IF(ParseAsValue(a_sequence, ac_asValue) != RETURNED)  
  if (*ac_asValue == -1) *ac_asValue = AS__VALUE_INT;

  const struct G_KEY p_fieldKey = m_GKey_PString(fieldName);
  int result = UNDEFINED;
  int entry = UNDEFINED;
  switch (result = m_GStringsIndexSingleFetch(fieldAttributesHandle,(INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_LABEL0, INDEX_SEEK_FLAGS__EQUAL, &p_fieldKey, INDEX_FETCH_FLAGS__READ_ONLY, 
    &fieldAttributeStuff, &entry)) {
  case RESULT__FOUND:
    m_ASSERT(fieldAttributeStuff != NULL)
    m_ASSERT(entry >= 0)
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE) 
  break; default: m_TRACK()
  } // switch 

  if (nac_tableIndexLabel != NULL) { 
    switch (*ac_asValue) { // blottabIndexSingleFlag
    case AS__VALUE_INT: // '#'
      if (!b_FLAG_SET_ON(fieldAttributeStuff->acolyt.cen_value,INT__BLOTTAB_INDEX_FLAG)) m_ABANDON(
        MISSING_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE)
      *nac_tableIndexLabel =
        fieldAttributeStuff[BLOTTAB_FIELD_ATTRIBUTE_STR_INDEX_ELEMENT].acolyt.cen_value;
    break; case AS__VALUE_STR: // '$'
      if (!b_FLAG_SET_ON(fieldAttributeStuff->acolyt.cen_value,STR__BLOTTAB_INDEX_FLAG)) m_ABANDON(
        MISSING_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      *nac_tableIndexLabel =
        fieldAttributeStuff[BLOTTAB_FIELD_ATTRIBUTE_INT_INDEX_ELEMENT].acolyt.cen_value;
    break; default: m_RAISE(ANOMALY__VALUE__D,*ac_asValue)
    } // switch
  } else *cac_element = entry;

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseAndRetrieveBlottabElement 


// Parse and compute 'l-value' blottab operations:
// expect <blottab ref op set int> | <blottab ref op set str> 
static inline int ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSetOp(
  BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTTAB_FIELD_REFERENCE *ac_blottabFieldReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  m_PREPARE_ABANDON(a_sequence, "<blottab ref op set int> | <blottab ref op set str>") 

  BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,GENUINE_BLOTTAB_LABEL0,blottabName,
    (void**)&blottabHandle)) {
  case RESULT__FOUND:
    m_ASSERT(blottabHandle != NULL)
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE)
  break; default:
    m_TRACK()
  } // switch

  int n_indexFetchFlags = -1; // a priori 
  m_PParsePassSpaces(a_sequence,NULL);

  int n_asValue = -1; // No blottab read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  int c_element = UNDEFINED; // Only significant with blottab read/set op
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op read int> | <blottab op read str> (R-value)
    // <blottab ref op set int> | <blottab ref op set str> (L-value)... 
    switch (ParseAndRetrieveBlottabElement(a_sequence, blottabHandle->hp_fieldAttributesHandle, NULL,&c_element,
      &n_asValue, nc_abandonmentInfo)) {
    case ANSWER__YES:
m_ASSERT(n_asValue != -1)
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (n_asValue == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ac_blottabFieldReference->r_field = (void*)(GENERIC_INTEGER)c_element;
  ac_blottabFieldReference->r_blottabHandle = blottabHandle; 
  ac_blottabFieldReference->asValue = n_asValue;

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSetOp

// Passed:
// - handle:
// - *a_sequence:
// - blottabHandle:
// 
// Changed:
// - *act_blotsetStuff: only significant if not abandoned
// - *ac_element: only significant if not abandoned
// - *ac_asValue: only significant if not abandoned
// - nc_abandonmentInfo: only significant if abandoned:
// 
// Ret: Parsed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static int l_BlotexlibExecutorParseAndRetrieveBlottabSpot(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, BLOTTAB_HANDLE blottabHandle, G_STRING_SET_STUFF* act_blotsetStuff,
  int *ac_element, int *ac_asValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
  m_PParsePassSpaces(a_sequence,NULL);

  m_PREPARE_ABANDON(a_sequence, "<blottab spot>") 
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  PParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 

  struct BLOTEX_VALUE entryBlotexValue = {UNDEFINED};
  switch (BlotexlibExecutorParseAndComputeBlotex(handle,a_sequence,&entryBlotexValue,
    nc_abandonmentInfo)) {
  case ANSWER__YES: 
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default: m_TRACK() } // switch 

  int entry = entryBlotexValue.select.c_blotval;
  if (entry < 0) m_ABANDON(VALUE_ERROR__ABANDONMENT_CAUSE)
  *act_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
  m_TRACK_IF(GStringsFetch(blottabHandle->h_tableHandle, entry, act_blotsetStuff) < 0)
  if (*act_blotsetStuff == NULL) m_ABANDON(NOT_EXISTING_BLOTSET__ABANDONMENT_CAUSE)

  PParsePassSingleChar(a_sequence,NULL,']',&lexeme); 
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParsePassSingleChar(a_sequence,NULL,'.',&lexeme); 
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

  *ac_asValue = UNDEFINED; 
  *ac_element = UNDEFINED;
  switch (ParseAndRetrieveBlottabElement(a_sequence,blottabHandle->hp_fieldAttributesHandle,NULL,ac_element,
    ac_asValue, nc_abandonmentInfo)) { 
  case ANSWER__YES:
  break; case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO)
  break; default: m_TRACK() } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // l_BlotexlibExecutorParseAndRetrieveBlottabSpot

// Parse and compute 'l-value' blottab spot:
// expect <int blottab spot> | <str blottab spot> 
static inline int ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSpot(
  BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTTAB_FIELD_REFERENCE *ac_blottabFieldReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<int blottab spot> | <str blottab spot>") 

  BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,GENUINE_BLOTTAB_LABEL0,blottabName,
    (void**)&blottabHandle)) {
  case RESULT__FOUND:
    m_ASSERT(blottabHandle != NULL)
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  int asValue = UNDEFINED; 
  int element = UNDEFINED;
  G_STRING_SET_STUFF t_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (l_BlotexlibExecutorParseAndRetrieveBlottabSpot(handle,a_sequence,blottabHandle,
    &t_blotsetStuff,&element, &asValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break;  case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO) 
  break; default: m_TRACK() } // switch

  ac_blottabFieldReference->r_field = (void*)(GENERIC_INTEGER)element;
  ac_blottabFieldReference->r_blottabHandle = blottabHandle; 
  ac_blottabFieldReference->asValue = asValue;
  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSpot

// Public function: see .h
int l_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSetOp(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTTAB_FIELD_REFERENCE *ac_blottabFieldReference, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  int answer = UNDEFINED ;
  if (b_spot) {
    switch (answer = ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSpot(handle,a_sequence,
      blottabName,ac_blottabFieldReference,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch
  } else {
    switch (answer = ml_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSetOp(handle,a_sequence,
      blottabName,ac_blottabFieldReference,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch
  } // if
     
  m_DIGGY_RETURN(answer)
} // l_BlotexlibExecutorParseAndComputeLValueGenuineBlottabSetOp

// Parse and compute blottab request. 
//
// Passed:
// - handle:
// - *a_sequence: expect <blottab request>  
// - tableHandle: table for request 
//
// Changed:
// - *a_sequence: after parsing 
// - tableHandle: 
// - p_fieldAttributesHandle: 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int ml_BlotexlibExecutorParseAndComputeBlottabRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, BLOTTAB_HANDLE blottabHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  struct P_STRING subSequence = UNDEFINED_P_STRING; 

  PParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blottab request atom>") 
  struct BLOTEX_VALUE blotexValue = UNDEFINED_BLOTEX_VALUE; 
  struct G_KEY gKey = { UNDEFINED };
  do {
    int asValue = UNDEFINED;  
    int tableIndexLabel = UNDEFINED;
    // <blottab request atom int> | <blottab request atom str> ...
    switch (ParseAndRetrieveBlottabElement(&subSequence, blottabHandle->hp_fieldAttributesHandle,
      &tableIndexLabel, NULL, &asValue,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
m_DIGGY_VAR_P_STRING(subSequence)
    
    int n_indexSeekFlags = UNDEFINED;
    m_PParsePassSpaces(&subSequence,NULL);
    PParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
m_DIGGY_VAR_P_STRING(lexeme)
    if (!b_EMPTY_P_STRING(lexeme)) n_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
    else {  // select with actual criterium
m_DIGGY_VAR_P_STRING(lexeme)
      m_TRACK_IF(ParseRequestCompOp(&subSequence,asValue != AS__VALUE_INT,
        &n_indexSeekFlags) != RETURNED)
      if (n_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)

      switch(BlotexlibExecutorParseAndComputeBlotex(handle,&subSequence,&blotexValue,
        nc_abandonmentInfo)) {
      case ANSWER__YES:
        switch (asValue) {
        case AS__VALUE_INT: // [ '#' ]
          if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__VALUE_STR: // '$'
          if (blotexValue.asValue != AS__VALUE_STR) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
m_DIGGY_VAR_P_STRING(blotexValue.select.c_strex.v_str)
          gKey = m_GKey_PString(blotexValue.select.c_strex.v_str);
        break; default: m_RAISE(ANOMALY__VALUE__D,asValue)
        } // switch
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default: m_TRACK()
      } // switch
    } // if
m_DIGGY_VAR_INDEX_SEEK_FLAGS(n_indexSeekFlags)
    
    int criteriaOpFlags = UNDEFINED;
    m_TRACK_IF(ParseLogical2Op(&subSequence, &criteriaOpFlags) != RETURNED)
m_DIGGY_VAR_P_STRING(subSequence)

    m_ASSERT(criteriaNumber < 5)
    criteria5[criteriaNumber++] = m_GRequestCriterium_GKeys(tableIndexLabel,
      n_indexSeekFlags,&gKey, criteriaOpFlags);
    m_PParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch (GStringsIndexRequestR(blottabHandle->h_tableHandle,NULL,criteriaNumber,criteria5)) {
  case COMPLETED__OK:
  break ; case COMPLETED__BUT: // Request rectified
    m_RAISE(ANOMALY__UNEXPECTED_CASE)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorParseAndComputeBlottabRequest


// Parse and compute blottab creation. 
//
// Passed:
// - handle:
// - *a_sequence: expect <blottab creation>  
// - blottabName: name of NON EXISTING blottab
//
// Changed:
// - *a_sequence: after parsing 
// - *ac_blottabHandle: (only significant when success) CREATED  
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int ml_BlotexlibExecutorParseAndComputeBlottabCreation(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING nonExistingBlottabName,
  BLOTTAB_HANDLE *ac_blottabHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab creation>") 

  struct P_STRING subSequence = UNDEFINED_P_STRING; 

  PParseTillMatch(a_sequence,m_PString("]?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);

  struct P_STRING s_names10[10]; // UNDEFINED
  int s_blottabIndexFlags10[10]; // UNDEFINED
  int n_asValue = UNDEFINED, n_asValue2 = UNDEFINED;
  int i = -1;
  do {
    m_ASSERT(++i < 10)
    s_blottabIndexFlags10[i] = ALL_FLAGS_OFF0;
    m_PParsePassSpaces(&subSequence,NULL);
    PParsePassChars(&subSequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
      (char)UNDEFINED,s_names10 + i); // <entity>
m_DIGGY_VAR_P_STRING(s_names10[i])
    m_TRACK_IF(ParseAsValue(&subSequence, &n_asValue) != RETURNED)  
m_DIGGY_VAR_D(n_asValue)
    m_TRACK_IF(ParseAsValue(a_sequence, &n_asValue2) != RETURNED)  
m_DIGGY_VAR_D(n_asValue2)
    switch (n_asValue) {
    case -1: ;
    break; case AS__VALUE_INT:
      m_SET_FLAG_ON(s_blottabIndexFlags10[i],INT__BLOTTAB_INDEX_FLAG)
    break; case AS__VALUE_STR:
      m_SET_FLAG_ON(s_blottabIndexFlags10[i],STR__BLOTTAB_INDEX_FLAG)
    break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue)
    } // switch
    switch (n_asValue2) {
    case -1: ;
    break; case AS__VALUE_INT:
      m_SET_FLAG_ON(s_blottabIndexFlags10[i],INT__BLOTTAB_INDEX_FLAG)
    break; case AS__VALUE_STR:
      m_SET_FLAG_ON(s_blottabIndexFlags10[i],STR__BLOTTAB_INDEX_FLAG)
    break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue2)
    } // switch
    m_PParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  switch(l_BlotexlibExecutorCreateGenuineBlottab(handle, nonExistingBlottabName, i+1, s_names10,
    s_blottabIndexFlags10, ac_blottabHandle)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__VALUE__D,COMPLETED__BUT)
  break; default: m_TRACK() 
  } // switch 

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorParseAndComputeBlottabCreation


// Parse and compute 'r-value' blottab OPERATIONS:
// expect <int blottab ops> | <str blottab ops>
static inline int ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blottabName, struct BLOTEX_VALUE *ac_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  m_PREPARE_ABANDON(a_sequence, "<int blottab ops> | <str blottab ops>") 

  BLOTTAB_HANDLE n_blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,GENUINE_BLOTTAB_LABEL0,blottabName,
    (void**)&n_blottabHandle)) {
  case RESULT__FOUND:
    m_ASSERT(n_blottabHandle != NULL)
  break; case RESULT__NOT_FOUND:
    n_blottabHandle = NULL;
  break; default:
    m_TRACK()
  } // switch

  m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
    (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED) // a priori

  int n_indexFetchFlags = -1; // a priori 
  m_PParsePassSpaces(a_sequence,NULL);

  PParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op create> ...
    if (n_blottabHandle != NULL) m_ABANDON(ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE)
    switch (ml_BlotexlibExecutorParseAndComputeBlottabCreation(handle,a_sequence,blottabName,
      &n_blottabHandle, nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch

  } else {
    if (n_blottabHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
    PParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op select> ...
      switch (ml_BlotexlibExecutorParseAndComputeBlottabRequest(handle,a_sequence,
        n_blottabHandle, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch
    } // if
    PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op reset>
      n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
    } // if
    PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op next>
      if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
      m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
    } // if
  } // if

  int n_asValue = -1; // No blottab read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  int c_element = UNDEFINED; // Only significant with blottab read/set op
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op read int> | <blottab op read str> (R-value)
    // <blottab ref op set int> | <blottab ref op set str> (L-value)... 
    switch (ParseAndRetrieveBlottabElement(a_sequence, n_blottabHandle->hp_fieldAttributesHandle, NULL,&c_element,
      &n_asValue, nc_abandonmentInfo)) {
    case ANSWER__YES:
m_ASSERT(n_asValue != -1)
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (n_indexFetchFlags < 0) {
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
      (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
  } else {
    G_STRING_SET_STUFF ct_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
    int c_entry = UNDEFINED;

m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags)
    m_ASSERT(n_blottabHandle != NULL)
    switch (GStringsIndexFetch(n_blottabHandle->h_tableHandle,NULL,n_indexFetchFlags,
      &ct_blotsetStuff, &c_entry)) {
    case RESULT__FOUND:
m_DIGGY_VAR_D(n_asValue)
      switch (n_asValue) {
      case -1: 
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,TRUE__BLOTVAL0,
          (struct P_STRING*)UNDEFINED,(char)UNDEFINED,ac_blotexValue) != RETURNED)
      break; case AS__VALUE_INT:
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,
          ct_blotsetStuff[c_element].acolyt.cen_value,(struct P_STRING*)UNDEFINED,
          (char)UNDEFINED,ac_blotexValue) != RETURNED)
      break; case AS__VALUE_STR:
        m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
          &ct_blotsetStuff[c_element].cv_pString,b_FUGACIOUS_STR,ac_blotexValue) !=
          RETURNED) // TODO: really FUGACIOUS????
      break; default: 
        m_TRACK()
      } // switch

    break; case RESULT__NOT_FOUND:
      m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,n_asValue == AS__VALUE_STR,FALSE__BLOTVAL,
        ap_aTrivialEmptyPString,!b_FUGACIOUS_STR,ac_blotexValue) != RETURNED)
    break; default: m_TRACK()
    } // switch
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps

// Parse and compute 'r-value' blottab spot:
// expect <int blottab spot> | <str blottab spot>
static inline int ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottabSpot(
  BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<int blottab spot> | <str blottab spot>") 

  BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,GENUINE_BLOTTAB_LABEL0,blottabName,
    (void**)&blottabHandle)) {
  case RESULT__FOUND:
    m_ASSERT(blottabHandle != NULL)
  break; case RESULT__NOT_FOUND:
    m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
  break; default:
    m_TRACK()
  } // switch

  int asValue = UNDEFINED; 
  int element = UNDEFINED;
  G_STRING_SET_STUFF t_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
  switch (l_BlotexlibExecutorParseAndRetrieveBlottabSpot(handle,a_sequence,blottabHandle,
    &t_blotsetStuff,&element, &asValue,nc_abandonmentInfo)) {
  case ANSWER__YES:
  break;  case ANSWER__NO:
    m_DIGGY_RETURN(ANSWER__NO) 
  break; default: m_TRACK() } // switch
  m_PParsePassSpaces(a_sequence,NULL);

  switch (asValue) {
  case AS__VALUE_INT:
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle, AS__VALUE_INT,
      t_blotsetStuff[element].acolyt.cen_value,(struct P_STRING*)UNDEFINED,
      (char)UNDEFINED,ac_blotexValue) != RETURNED)
  break; case AS__VALUE_STR:
    m_TRACK_IF(BlotexlibExecutorSetBlotexValue(handle,AS__VALUE_STR,UNDEFINED,
      &t_blotsetStuff[element].cv_pString,b_FUGACIOUS_STR,ac_blotexValue) !=
      RETURNED) // TODO: really FUGACIOUS????
  break; default: m_TRACK() } // switch

  m_DIGGY_RETURN(ANSWER__YES) 
} // l_BlotexlibExecutorParseAndComputeRValueGenuineBlottabSpot

// Public function: see .h
int l_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_spot, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *ac_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  int answer = UNDEFINED;
  if (b_spot) {
    switch (answer = ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottabSpot(handle,a_sequence,
      blottabName,ac_blotexValue,nc_abandonmentInfo)) {  
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch 
  } else {
    switch (answer = ml_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps(handle,a_sequence,
      blottabName,ac_blotexValue,nc_abandonmentInfo)) {  
    case ANSWER__YES:
    break; case ANSWER__NO:
    break; default: m_TRACK() } // switch 
  } // if

  m_DIGGY_RETURN(answer)
} // l_BlotexlibExecutorParseAndComputeRValueGenuineBlottabOps


// Passed:
// - handle: supposed to be positoned on the "current" blotset
// - element:
// blotexValue 
static int BlottabUpdateCurrentBlotsetField(BLOTTAB_HANDLE handle, int element,
  struct BLOTEX_VALUE blotexValue) {
  m_DIGGY_BOLLARD()
  G_STRING_SET_STUFF nt_fieldAttributeStuff = (G_STRING_SET_STUFF)UNDEFINED;
  int fieldsNumber = GStringsGetCount(handle->hp_fieldAttributesHandle,
    &nt_fieldAttributeStuff);
  m_TRACK_IF(fieldsNumber < 0)
  m_ASSERT(nt_fieldAttributeStuff != NULL)

  m_TRACK_IF(fieldsNumber < 0)
  m_ASSERT(element < fieldsNumber)

  G_STRING_SET_STUFF vnt_blotsetStuff;
  int vn_entry = UNDEFINED;
  int result = GStringsIndexFetch(handle->h_tableHandle,NULL, INDEX_FETCH_FLAGS__CURRENT,
    &vnt_blotsetStuff, &vn_entry); 
  switch (result) {
  case RESULT__FOUND:
m_ASSERT(vnt_blotsetStuff != NULL)
    switch (blotexValue.asValue) {
    case AS__VALUE_STR: m_TRACK_IF(GStringCopy(vnt_blotsetStuff+element,0,
      blotexValue.select.c_strex.v_str) < 0)
    break; case AS__VALUE_INT: vnt_blotsetStuff[element].acolyt.cen_value =
      blotexValue.select.c_blotval;
    break; default: m_RAISE(ANOMALY__VALUE__D,blotexValue.asValue) } // switch
  break; case RESULT__NOT_FOUND:
m_ASSERT(vnt_blotsetStuff == NULL)
  break; default: m_TRACK() } // switch

  m_DIGGY_RETURN(result)
} // BlottabUpdateCurrentBlotsetField


// Public function; see .h
int UpdateBlottabCurrentBlotsetField(struct BLOTTAB_FIELD_REFERENCE blottabFieldReference,
  struct BLOTEX_VALUE blotexValue) {
  m_DIGGY_BOLLARD()
  m_ASSERT(blottabFieldReference.asValue == blotexValue.asValue);
  int result = BlottabUpdateCurrentBlotsetField(blottabFieldReference.r_blottabHandle,
    (int)(GENERIC_INTEGER)blottabFieldReference.r_field, blotexValue); 
  m_TRACK_IF(result < 0) 

  m_DIGGY_RETURN(result)
} // UpdateBlottabCurrentBlotsetField

