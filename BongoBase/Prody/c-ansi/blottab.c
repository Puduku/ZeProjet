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



// Blottab expressions parsing:

#define MISSING_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Missing string index in table"
#define MISSING_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Missing integer index in table"
#define UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE "Unknown blottab"
#define UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE "Unknown blottab field"
#define ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE "Blottab already exists"


// Parse and retrieve blottab element
//
// Passed:
// - *a_sequence: before parsing
// - fieldAttributesHandle: blottab fields definition 
// - nac_tableIndexLabel: != NULL special value if seek for created table index
// - cac_element: only significant if not seek for created table index
//
// Changed:
// - *a_sequence: after parsing
// - *nac_tableIndexLabel: (when seek for created table index) only significant if no abandon
// - *cac_element: (when not seek for created table index) only significant if no abandon
// - *ac_asValue: only significant if no abandon; default to "as value int" (must correspond to 
//   existing index when seek for created table index)
// - nc_abandonmentInfo: only significant when parsing abandoned 
//
// Ret:
// - ANSWER__YES: Ok
// - ANSWER__NO: parsing abandoned
static int RetrieveBlottabElement(struct P_STRING *a_sequence, G_STRINGS_HANDLE fieldAttributesHandle,
  int *nac_tableIndexLabel, int *cac_element, int *ac_asValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  G_STRING_SET_STUFF fieldAttributeStuff = (G_STRING_SET_STUFF)UNDEFINED;
  struct P_STRING fieldName; //UNDEFINED
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
} // RetrieveBlottabElement 

// Public function; see .h
int l_BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexFlags,
  BLOTTAB_HANDLE *a_blottabHandle) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(blottabName)
  G_STRINGS_HANDLE blottabsHandle = (G_STRINGS_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotexlibExecutorGetBlottabsHandle(handle,&blottabsHandle) != RETURNED)
m_DIGGY_VAR_P(blottabsHandle)
  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  struct G_KEY gKey = m_GKey_PString(blottabName);
  int result = m_GStringsIndexSingleFetch(blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL);
m_DIGGY_VAR_RESULT(result)
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*a_blottabHandle = (BLOTTAB_HANDLE) t_namedBlottabStuff->acolyt.cnhr_handle) != NULL)
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(BlottabCreateInstance(a_blottabHandle,fieldsNumber, s_names,s_blottabIndexFlags) !=
      RETURNED)
    m_TRACK_IF(GStringCopy(t_namedBlottabStuff,0,blottabName) < 0)
    m_TRACK_IF(m_GStringAsNamedObject(t_namedBlottabStuff,*a_blottabHandle,
      blottabsHandle) != RETURNED)
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(completed)
} // l_BlotexlibExecutorCreateBlottab


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
static inline int ml_BlotexlibExecutorComputeBlottabRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, BLOTTAB_HANDLE blottabHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme;
  struct P_STRING subSequence; 

  PParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blottab request atom>") 
  struct BLOTEX_VALUE blotexValue; // UNDEFINED
  struct G_KEY gKey; // UNDEFINED
  do {
    int asValue = UNDEFINED;  
    int tableIndexLabel = UNDEFINED;
    // <blottab request atom int> | <blottab request atom str> ...
    switch (RetrieveBlottabElement(&subSequence, blottabHandle->hp_fieldAttributesHandle,
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

      switch(BlotexlibExecutorComputeBlotex(handle,&subSequence,&blotexValue,nc_abandonmentInfo)) {
      case ANSWER__YES:
        switch (asValue) {
        case AS__VALUE_INT: // [ '#' ]
          if (blotexValue.b_strex) m_ABANDON(EXPECT_INTEX__ABANDONMENT_CAUSE)
          gKey = m_GKey_AcolytValue(blotexValue.select.c_blotval); 
        break; case AS__VALUE_STR: // '$'
          if (!blotexValue.b_strex) m_ABANDON(EXPECT_STREX__ABANDONMENT_CAUSE)
m_DIGGY_VAR_P_STRING(blotexValue.select.c_str)
          gKey = m_GKey_PString(blotexValue.select.c_str);
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
} // ml_BlotexlibExecutorComputeBlottabRequest

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
static inline int ml_BlotexlibExecutorComputeBlottabCreation(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING nonExistingBlottabName,
  BLOTTAB_HANDLE *ac_blottabHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab creation>") 

  struct P_STRING subSequence; 

  PParseTillMatch(a_sequence,m_PString("]?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);

  struct P_STRING s_names10[10];
  int s_blottabIndexFlags10[10] ;
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

  switch(l_BlotexlibExecutorCreateBlottab(handle, nonExistingBlottabName, i+1, s_names10,
    s_blottabIndexFlags10, ac_blottabHandle)) {
  case COMPLETED__OK:
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__VALUE__D,COMPLETED__BUT)
  break; default: m_TRACK() 
  } // switch 

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorComputeBlottabCreation

// Public function: see .h
int l_BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTTAB_FIELD_REFERENCE *cac_blottabFieldReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, b_lValue? "<blottab ref op set int> | <blottab ref op set str>":
    "<int blottab ops> | <str blottab ops>") 

  BLOTTAB_HANDLE n_blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (BlotexlibExecutorGetBlottab(handle,blottabName,&n_blottabHandle)) {
  case RESULT__FOUND:
    m_ASSERT(n_blottabHandle != NULL)
  break; case RESULT__NOT_FOUND:
    n_blottabHandle = NULL;
  break; default:
    m_TRACK()
  } // switch

  if (!b_lValue) {
    cac_blotexValue->b_strex = b_FALSE0; // a priori 
    cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  } // if
  int n_indexFetchFlags = -1; // a priori 
  m_PParsePassSpaces(a_sequence,NULL);

  if (!b_lValue) {
    PParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op create> ...
      if (n_blottabHandle != NULL) m_ABANDON(ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE)
      switch (ml_BlotexlibExecutorComputeBlottabCreation(handle,a_sequence,blottabName,
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
        switch (ml_BlotexlibExecutorComputeBlottabRequest(handle,a_sequence,
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
  } // if
  int n_asValue = -1; // No blottab read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  int c_element = UNDEFINED; // Only significant with blottab read/set op
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op read int> | <blottab op read str> (R-value)
    // <blottab ref op set int> | <blottab ref op set str> (L-value)... 
    switch (RetrieveBlottabElement(a_sequence, n_blottabHandle->hp_fieldAttributesHandle, NULL,&c_element,
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
  
  if (b_lValue) {
    if (n_asValue == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    if (n_blottabHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
    cac_blottabFieldReference->element = c_element;
    cac_blottabFieldReference->blottabHandle = n_blottabHandle; 
    *cac_asValue = n_asValue ;
  } else {
    if (n_indexFetchFlags < 0) {
      cac_blotexValue->b_strex = b_FALSE0; 
      cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
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
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
        break; case AS__VALUE_INT:
          cac_blotexValue->b_strex = b_FALSE0; 
          cac_blotexValue->select.c_blotval = ct_blotsetStuff[c_element].acolyt.cen_value;
        break; case AS__VALUE_STR:
          cac_blotexValue->b_strex = b_TRUE; 
          cac_blotexValue->select.c_str = ct_blotsetStuff[c_element].cv_pString ; 
        break; default: 
          m_TRACK()
        } // switch
  
      break; case RESULT__NOT_FOUND:
        if ((cac_blotexValue->b_strex = (n_asValue == AS__VALUE_STR)))
          cac_blotexValue->select.c_str = m_PString(GOOD_OLD_EMPTY_C_STRING) ;
        else cac_blotexValue->select.c_blotval = FALSE__BLOTVAL; 
      break; default: m_TRACK()
      } // switch
    } // if
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // l_BlotexlibExecutorComputeBlottabOps


// See .h
int UpdateBlottabField(const struct BLOTTAB_FIELD_REFERENCE* ap_blottabFieldReference, int as,
  struct BLOTEX_VALUE blotexValue) {
  m_DIGGY_BOLLARD()
#if 0
  //ap_blottabReference->blottabHandle 
  //G_STRINGS_HANDLE h_tableHandle;
  //G_STRINGS_HANDLE hp_fieldAttributesHandle;
  //ap_blottabReference->element 
  G_STRING_SET_STUFF nt_gStringSetStuff;
  int n_entry;
  switch (m_GStringsIndexiSingleFetch(ap_blottabReference->blottabHandle.h_tableHandle,NULL,
    INDEX_LABEL0,
    INDEX_FETCH_FLAGS__CURRENT, G_STRING_SET_STUFF &nt_gStringSetStuff, &n_entry)) {
  case RESULT__FOUND:
    switch (as) {
    case AS__VALUE_INT: // [ '#' ]
    break; case AS__ID: // '!' 
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__VALUE_STR: // '$'
    break; case AS__NAME:  // '!$'
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; default: m_RAISE(ANOMALY__VALUE__D,c_as)
    } // switch

  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch
#endif 

  m_DIGGY_RETURN(ANSWER__NO)
} // UpdateBlottabField

