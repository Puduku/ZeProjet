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
  G_STRINGS_HANDLE hp_fieldsHandle;
} ; 
typedef struct BLOTTAB* BLOTTAB_HANDLE;


// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
// Public function; see .h
int BlottabCreateInstance(BLOTTAB_HANDLE *azh_handle, int fieldsNumber,
  struct P_STRING* s_names, int* s_blottabIndexTypes) {
  m_DIGGY_BOLLARD_S()

  m_MALLOC_INSTANCE(*azh_handle)
  BLOTTAB_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(BLOTTAB_HANDLE,handle)

  m_TRACK_IF(GStringsCreateInstance(&handle->h_tableHandle,BATEAU__EXPECTED_ITEMS_NUMBER,
    fieldsNumber, VALUED_STRING__G_STRING_CONVEYANCE, (const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION) UNDEFINED) != RETURNED)

  m_TRACK_IF(GStringsCreateInstance(&handle->hp_fieldsHandle,10,2,
    VALUED_STRING__G_STRING_CONVEYANCE, (const int *)UNDEFINED,
    (NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION)UNDEFINED) != RETURNED)
  m_ASSERT(GStringsAddIndex(handle->hp_fieldsHandle,1,
    0,P_STRING__G_KEYS_COMPARISON,(IS_CHAR_FUNCTION)NULL,(TO_CHAR_FUNCTION)NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == INDEX_LABEL0)

  G_STRING_SET_STUFF fieldStuff = (G_STRING_SET_STUFF)UNDEFINED; 
  int c_indexLabel = UNDEFINED;
  int i = 0; for (; i < fieldsNumber; i++) {
    if (s_blottabIndexTypes[i] != NONE__BLOTTAB_INDEX_TYPE) m_TRACK_IF((c_indexLabel =
      GStringsAddIndex(handle->h_tableHandle,1,i,s_blottabIndexTypes[i] == STR__BLOTTAB_INDEX_TYPE?
      P_STRING__G_KEYS_COMPARISON: ACOLYT_VALUE__G_KEYS_COMPARISON ,NULL,NULL,
      (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED)) < 0)
    m_TRACK_IF(GStringsFetch(handle->hp_fieldsHandle, -1, &fieldStuff) != i)
    m_ASSERT(fieldStuff != NULL)
    m_TRACK_IF(GStringCopy(fieldStuff,0, s_names[i]) < 0) 
    fieldStuff->acolyt.cen_value = s_blottabIndexTypes[i] ; 
    fieldStuff[1].acolyt.cen_value = c_indexLabel ;
  } // for

  m_TRACK_IF(GStringsFreeze(handle->hp_fieldsHandle,NULL) < 0)

  m_DIGGY_RETURN(RETURNED)
} // BlottabCreateInstance

// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int BlottabDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD_S()

  BLOTTAB_HANDLE xh_handle = (BLOTTAB_HANDLE) xhr_handle;
  m_CHECK_MAGIC_FIELD(BLOTTAB_HANDLE,xh_handle)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->h_tableHandle) != RETURNED)
  m_TRACK_IF(GStringsDestroyInstance(xh_handle->hp_fieldsHandle) != RETURNED)
  
  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // BlottabDestroyInstance



// Blot expressions parsing:

#define NO_BLOTTAB_INDEX__ABANDONMENT_CAUSE "No index in table"
#define EXPECT_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE "Integer index expected in table"
#define EXPECT_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE "String index expected in table"
#define UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE "Unknown blottab"
#define UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE "Unknown blottab field"
#define ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE "Blottab already exists"

// Passed:
// - np_fieldsHandle: != NULL: seek for table...
// - cnac_tableIndexLabel: (significant when seek for table)
//  + NULL special value: seek for table element 
//  + != NULL: seek for table index label
//
// Changed:
// - *a_sequence: after parsing
// - *cnac_tableIndexLabel: (when seek for table index label; only significant if no abandon)
// - *ccac_element: (when seek for table element; only significant if no abandon) 
// - *cac_asValue: (when seek for table; only significant if no abandon) default to "as value int"
// - *cac_fieldName: (when NO seek for table; only significant if no abandon)
// - *cac_blottabIndexType: (when NO seek for table; only significant if no abandon)
// - nc_abandonmentInfo: 
//
// Ret:
// - ANSWER__YES: Ok
// - ANSWER__NO: parsing abandoned
static int ParseBlottabElement(struct P_STRING *a_sequence, G_STRINGS_HANDLE np_fieldsHandle,
  int *cnac_tableIndexLabel, int *ccac_element, int *cac_asValue, struct P_STRING *cac_fieldName,
  int *cac_blottabIndexType, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  G_STRING_SET_STUFF p_fieldStuff; // UNDEFINED
  struct P_STRING fieldName; //UNDEFINED
  int n_asValue = UNDEFINED;
 
  m_PREPARE_ABANDON(a_sequence,"<entity> [ <<as value int>> | <<as value str>> ]")
  ParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,&fieldName); // <entity>

  if (np_fieldsHandle != NULL) { // Seek for table
    const struct G_KEY p_fieldKey = m_GKey_PString(fieldName);
    int result = UNDEFINED;
    int entry = UNDEFINED;
    switch (result = m_GStringsIndexSingleFetch(np_fieldsHandle,(INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
      INDEX_LABEL0, INDEX_SEEK_FLAGS__EQUAL, &p_fieldKey, INDEX_FETCH_FLAGS__READ_ONLY, 
      &p_fieldStuff, &entry)) {
    case RESULT__FOUND:
      m_ASSERT(p_fieldStuff != NULL)
      m_ASSERT(entry > 0)
    break; case RESULT__NOT_FOUND:
      m_ABANDON(UNKNOWN_BLOTTAB_FIELD__ABANDONMENT_CAUSE) 
    break; default: m_TRACK()
    } // switch 
    if (cnac_tableIndexLabel != NULL) {
      if (p_fieldStuff->acolyt.cen_value == NONE__BLOTTAB_INDEX_TYPE) m_ABANDON(
        NO_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      *cnac_tableIndexLabel = p_fieldStuff[1].acolyt.cen_value ;
    } else *ccac_element = entry;
  } // if

  m_TRACK_IF(ParseAsValue(a_sequence, &n_asValue) != RETURNED)  
m_DIGGY_VAR_D(n_asValue)
  if (np_fieldsHandle != NULL) { // Seek for table
    if (n_asValue == -1) n_asValue = AS__VALUE_INT;
    *cac_asValue = n_asValue ;
m_DIGGY_VAR_D(n_asValue)
    if (cnac_tableIndexLabel != NULL) {
      switch (n_asValue) {
      case AS__VALUE_INT: // '#'
        if (p_fieldStuff->acolyt.cen_value != INT__BLOTTAB_INDEX_TYPE) m_ABANDON(
          EXPECT_INT_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      break; case AS__VALUE_STR: // '$'
        if (p_fieldStuff->acolyt.cen_value != STR__BLOTTAB_INDEX_TYPE) m_ABANDON(
          EXPECT_STR_BLOTTAB_INDEX__ABANDONMENT_CAUSE) 
      break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue)
      } // switch
    } // if
  } else {
    switch (n_asValue) {
    case -1:  *cac_blottabIndexType = NONE__BLOTTAB_INDEX_TYPE; 
    break; case AS__VALUE_INT: *cac_blottabIndexType = INT__BLOTTAB_INDEX_TYPE; 
    break; case AS__VALUE_STR: *cac_blottabIndexType = STR__BLOTTAB_INDEX_TYPE; 
    break; default: m_RAISE(ANOMALY__VALUE__D,n_asValue)
    } // switch
    *cac_fieldName = fieldName;
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseBlottabElement 

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
// - p_fieldsHandle: 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int ml_BlotexlibExecutorComputeBlottabRequest(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, G_STRINGS_HANDLE tableHandle,
  G_STRINGS_HANDLE p_fieldsHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab request>") 

  int criteriaNumber = 0;
  struct G_REQUEST_CRITERIUM criteria5[5] ;  
  struct P_STRING lexeme;
  struct P_STRING subSequence; 

  ParseTillMatch(a_sequence,m_PString(":?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParseOffset(a_sequence,2,NULL);
  m_PRECISE_ABANDON(&subSequence, "<blottab request atom>") 
  struct BLOTEX_VALUE blotexValue; // UNDEFINED
  struct G_KEY gKey; // UNDEFINED
  do {
    int asValue = UNDEFINED;  
    int tableIndexLabel = UNDEFINED ;
    // <blottab request atom int> | <blottab request atom str> ...
    switch (ParseBlottabElement(&subSequence, p_fieldsHandle, &tableIndexLabel, (int*)UNDEFINED,
      &asValue,(struct P_STRING*)UNDEFINED,(int*)UNDEFINED,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
m_DIGGY_VAR_P_STRING(subSequence)
    
    int n_indexSeekFlags = UNDEFINED;
    ParsePassSingleChar(&subSequence,NULL,'*',&lexeme);
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
    m_ParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

m_DIGGY_INFO("Before GStringsIndexRequestR(tableHandle=%p)...",tableHandle)
  switch (GStringsIndexRequestR(tableHandle,NULL,criteriaNumber,criteria5)) {
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
// - *ac_tableHandle: (only significant when success) CREATED  
// - *acp_fieldsHandle: (only significant when success) CREATED 
// - nc_abandonmentInfo: only significant if "computing abandoned"
//
// Ret: Parsed and computed successfully ? 
// - ANSWER__YES: success
// - ANSWER__NO: 'syntax' error; abandon processing 
// - -1: unexpected problem
static inline int ml_BlotexlibExecutorComputeBlottabCreation(BLOTEXLIB_EXECUTOR_HANDLE handle,
  struct P_STRING *a_sequence, struct P_STRING blottabName, G_STRINGS_HANDLE *ac_tableHandle,
  G_STRINGS_HANDLE *acp_fieldsHandle, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blottab creation>") 

  struct P_STRING subSequence; 

  ParseTillMatch(a_sequence,m_PString("]?"),NULL, &subSequence);
m_DIGGY_VAR_P_STRING(subSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  ParseOffset(a_sequence,2,NULL);

  struct P_STRING s_names10[10];
  int s_blottabIndexTypes10[10];
  int i = -1;
  do {
    m_ASSERT(++i < 10)
    switch (ParseBlottabElement(&subSequence, NULL, (int*)UNDEFINED, (int*)UNDEFINED,
       (int*)UNDEFINED, s_names10 + i, s_blottabIndexTypes10 + i,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    m_ParsePassSpaces(&subSequence,NULL);
  } while (!b_EMPTY_P_STRING(subSequence)) ; 

  BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE) blottabHandle;
  switch(l_BlotexlibExecutorCreateBlottab(handle, blottabName, i+1, s_names10, s_blottabIndexTypes10,
    &blottabHandle)) {
    //ac_tableHandle, acp_fieldsHandle)) {
  case COMPLETED__OK:
    *ac_tableHandle = blottabHandle->h_tableHandle;
    *acp_fieldsHandle = blottabHandle->hp_fieldsHandle; 
  break; case COMPLETED__BUT:
    m_RAISE(ANOMALY__VALUE__D,COMPLETED__BUT)
  break; default: m_TRACK() 
  } // switch 

  m_DIGGY_RETURN(ANSWER__YES)
} // ml_BlotexlibExecutorComputeBlottabCreation

// Public function: see .h
int l_BlotexlibExecutorComputeBlottabOps(BLOTEXLIB_EXECUTOR_HANDLE handle,
  char b_lValue, struct P_STRING *a_sequence, struct P_STRING blottabName,
  struct BLOTEX_VALUE *cac_blotexValue, struct BLOTVAR_REFERENCE *cac_blotvarReference,
  int *cac_asValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, b_lValue? "<blottab ref op set int> | <blottab ref op set str>":
    "<int blottab ops> | <str blottab ops>") 

  G_STRINGS_HANDLE n_tableHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  G_STRINGS_HANDLE cp_fieldsHandle = (G_STRINGS_HANDLE)UNDEFINED; 
  BLOTTAB_HANDLE blottabHandle = (BLOTTAB_HANDLE)UNDEFINED;
m_DIGGY_VAR_P_STRING(blottabName)
  switch (l_BlotexlibExecutorGetBlottab(handle,blottabName,&blottabHandle)) {
  case RESULT__FOUND:
    n_tableHandle = blottabHandle->h_tableHandle; 
    cp_fieldsHandle = blottabHandle->hp_fieldsHandle;
  break; case RESULT__NOT_FOUND:
    n_tableHandle = NULL; 
  break; default:
    m_TRACK()
  } // switch

  if (!b_lValue) {
    cac_blotexValue->b_strex = b_FALSE0; // a priori 
    cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; // a priori
  } // if
  int n_indexFetchFlags = -1; // a priori 
  m_ParsePassSpaces(a_sequence,NULL);

  if (!b_lValue) {
    ParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op create> ...
      if (n_tableHandle != NULL) m_ABANDON(ALREADY_EXISTS_BLOTTAB__ABANDONMENT_CAUSE)
      switch (ml_BlotexlibExecutorComputeBlottabCreation(handle,a_sequence,blottabName,
        &n_tableHandle, &cp_fieldsHandle, nc_abandonmentInfo)) {
      case ANSWER__YES:
      break; case ANSWER__NO:
        m_DIGGY_RETURN(ANSWER__NO)
      break; default:
        m_TRACK()
      } // switch

    } else {
      if (n_tableHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
      ParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op select> ...
        switch (ml_BlotexlibExecutorComputeBlottabRequest(handle,a_sequence,n_tableHandle,
          cp_fieldsHandle, nc_abandonmentInfo)) {
        case ANSWER__YES:
        break; case ANSWER__NO:
          m_DIGGY_RETURN(ANSWER__NO)
        break; default:
          m_TRACK()
        } // switch
      } // if
      ParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op reset>
        n_indexFetchFlags = INDEX_FETCH_FLAG__RESET; 
      } // if
      ParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op next>
        if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0;
        m_SET_FLAG_ON(n_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
      } // if
    } // if
  } // if
  int n_asValue = -1; // No blottab read/set op a priori 
  ParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  int c_element = UNDEFINED; // Only significant with blottab read/set op
  if (!b_EMPTY_P_STRING(lexeme)) { // <blottab op read int> | <blottab op read str> (R-value)
    // <blottab ref op set int> | <blottab ref op set str> (L-value)... 
    switch (ParseBlottabElement(a_sequence, cp_fieldsHandle, NULL,&c_element, &n_asValue,
      (struct P_STRING*)UNDEFINED,(int*)UNDEFINED,nc_abandonmentInfo)) {
    case ANSWER__YES:
    break; case ANSWER__NO:
      m_DIGGY_RETURN(ANSWER__NO)
    break; default:
      m_TRACK()
    } // switch
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
  if (b_lValue) {
    if (n_asValue == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    if (n_tableHandle == NULL) m_ABANDON(UNKNOWN_BLOTTAB__ABANDONMENT_CAUSE) 
    cac_blotvarReference->in_blottabElement = c_element;
    cac_blotvarReference->select.c_blottab.tableHandle = n_tableHandle; 
    cac_blotvarReference->select.c_blottab.p_fieldsHandle = cp_fieldsHandle; 
    cac_blotvarReference->cv_blotvarReference = SET_CURRENT__L_VALUE__BLOTVAR_REFERENCE;
    *cac_asValue = n_asValue ;
  } else {
    if (n_indexFetchFlags < 0) {
      cac_blotexValue->b_strex = b_FALSE0; 
      cac_blotexValue->select.c_blotval = TRUE__BLOTVAL0; 
    } else {
      G_STRING_SET_STUFF ct_blotsetStuff = (G_STRING_SET_STUFF)UNDEFINED;
      int c_entry = UNDEFINED;
  
  m_DIGGY_VAR_INDEX_FETCH_FLAGS(n_indexFetchFlags)
      m_ASSERT(n_tableHandle != NULL)
      switch (GStringsIndexFetch(n_tableHandle,NULL,n_indexFetchFlags,
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


// Public function; see .h
int l_BlotexlibExecutorGetBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  BLOTTAB_HANDLE *ac_blottabHandle) {
  //G_STRINGS_HANDLE *ac_tableHandle, G_STRINGS_HANDLE *acp_fieldsHandle) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF ct_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED; 
  struct G_KEY gKey = m_GKey_PString(blottabName);
  
  G_STRINGS_HANDLE blottabsHandle = (G_STRINGS_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotexlibExecutorGetBlottabsHandle(handle,blottabName,&blottabsHandle) != RETURNED)

  int result = m_GStringsIndexSingleFetch(blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__SEEK_ONLY,&ct_namedBlottabStuff, NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*ac_blottabHandle = (BLOTTAB_HANDLE)ct_namedBlottabStuff->acolyt.cnhr_handle) != NULL);
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // l_BlotexlibExecutorGetBlottab

// Public function; see .h
int l_BlotexlibExecutorCreateBlottab(BLOTEXLIB_EXECUTOR_HANDLE handle, struct P_STRING blottabName,
  int fieldsNumber, struct P_STRING* s_names, int* s_blottabIndexTypes,
  BLOTTAB_HANDLE *a_blottabHandle) {
  //G_STRINGS_HANDLE *na_tableHandle, G_STRINGS_HANDLE *nap_fieldsHandle) {
  m_DIGGY_BOLLARD()

  G_STRINGS_HANDLE blottabsHandle = (G_STRINGS_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotexlibExecutorGetBlottabsHandle(handle,blottabName,&blottabsHandle) != RETURNED)

  int completed = COMPLETED__OK; // a priori
  G_STRING_STUFF t_namedBlottabStuff = (G_STRING_STUFF)UNDEFINED;
  //BLOTTAB_HANDLE ch_blottabHandle = (BLOTTAB_HANDLE)UNDEFINED; // ch_ => transitory "head" handle 
  struct G_KEY gKey = m_GKey_PString(blottabName);
  int result = m_GStringsIndexSingleFetch(blottabsHandle,NULL,INDEX_LABEL0,
    INDEX_SEEK_FLAGS__EQUAL,&gKey, INDEX_FETCH_FLAGS__FETCH,&t_namedBlottabStuff,NULL);
  switch (result) {
  case RESULT__FOUND:
    m_ASSERT((*a_blottabHandle = (BLOTTAB_HANDLE) t_namedBlottabStuff->acolyt.cnhr_handle) != NULL)
    completed = COMPLETED__BUT;
  break; case RESULT__NOT_FOUND:
    m_TRACK_IF(BlottabCreateInstance(a_blottabHandle,fieldsNumber, s_names,s_blottabIndexTypes) !=
      RETURNED)
    t_namedBlottabStuff->acolyt.cnhr_handle = *a_blottabHandle;
  break; default:
    m_TRACK()
  } // switch
//  if (na_tableHandle != NULL) *na_tableHandle = ch_blottabHandle->h_tableHandle;
//  if (nap_fieldsHandle != NULL) *nap_fieldsHandle = ch_blottabHandle->hp_fieldsHandle;

  m_DIGGY_RETURN(completed)
} // l_BlotexlibExecutorCreateBlottab


