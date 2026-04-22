// blotex-kitchen.c, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

#include "c-ansi/blotex-kitchen.h"
#include "c-ansi/alloc.h"
#include "c-ansi/p-parse.h"
#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"
#include "c-ansi/stderr.h"

#include "c-ansi/testy-diggy.h"

#include <limits.h>


// Main blotex "concepts":
// =======================

// BLOTREG:
// --------

// See .h
int BlotregCreateInstance(void **azhr_handle, struct P_STRING f_name, va_list arguments){
  m_DIGGY_BOLLARD()
  g_BLOTREG_HANDLE* azh_handle = (g_BLOTREG_HANDLE*) azhr_handle;
  m_TRACK_IF(l_GParamsCreateInstance(azh_handle,BATEAU__EXPECTED_ITEM_COUNT) !=
    RETURNED) 
  g_BLOTREG_HANDLE handle = *azh_handle;
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_NAME_ELEMENT,P_STRING__GS_KEYS_COMPARISON,NULL,NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == NAME__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_NAME_ELEMENT,ACOLYT_VALUE__GS_KEYS_COMPARISON,
    (IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == TOKEN_ID__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_VALUE_ELEMENT,ACOLYT_VALUE__GS_KEYS_COMPARISON,
    (IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
    INT_VALUE__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_VALUE_ELEMENT, P_STRING__GS_KEYS_COMPARISON,NULL,NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
    STR_VALUE__BLOTREG_INDEX_LABEL)
 m_DIGGY_RETURN(RETURNED)
} // BlotregCreateInstance 

// See .h
int BlotregDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD()
  g_BLOTREG_HANDLE xh_handle = (g_BLOTREG_HANDLE) xhr_handle;

  m_TRACK_IF(g_GParamsDestroyInstance(xh_handle) < 0)

  m_DIGGY_RETURN(RETURNED)
} // BlotregDestroyInstance  


// BLOTVAR:
// --------

// See .h
int FetchBlotvar(const struct BLOTVAR_REFERENCE *ap_blotvarReference, char b_lValue,
  g_BLOTVAR_STUFF *ant_blotvarStuff, int *navn_entry) {
  m_DIGGY_BOLLARD()

  *ant_blotvarStuff = (g_BLOTVAR_STUFF)UNDEFINED;
  switch (ap_blotvarReference->n_specifierFlag) {
  case NAME__SPECIFIER_FLAG:
  case TOKEN_ID__SPECIFIER_FLAG:
    { struct GS_KEY gsKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->n_specifierFlag == NAME__SPECIFIER_FLAG) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
m_DIGGY_VAR_P_STRING(ap_blotvarReference->c_select.c_name)
        gsKey = m_GsKey(ap_blotvarReference->c_select.c_name);
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        gsKey = m_GsKey3(ap_blotvarReference->c_select.c_tokenId);
      } // if
m_DIGGY_VAR_P(ap_blotvarReference->blotregHandle)
      switch (gm_BlotregIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,indexLabel,
        INDEX_SEEK_FLAGS__EQUAL,&gsKey,b_lValue?  INDEX_FETCH_FLAGS__FETCH:
        INDEX_FETCH_FLAGS__SEEK_ONLY, ant_blotvarStuff, navn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        if (b_lValue) {
          m_ASSERT(*ant_blotvarStuff != NULL)
          if (ap_blotvarReference->n_specifierFlag == NAME__SPECIFIER_FLAG)
            m_TRACK_IF(m_GParamNameCopy(*ant_blotvarStuff,0,ap_blotvarReference->c_select.c_name)
              < 0)
          else { // TOKEN_ID__SPECIFIER_FLAG
            m_TRACK_IF(GParamNameAssign(*ant_blotvarStuff,(struct P_STRING*)NULL,
              ap_blotvarReference->c_select.c_tokenId, ap_blotvarReference->blotregHandle)
              < 0)
          } // if
        } // if
      break; default:
        m_TRACK()
      } // switch
    } // gsKey 
  break; case ENTRY__SPECIFIER_FLAG:
    m_ASSERT(g_BlotregFetch(ap_blotvarReference->blotregHandle,
      ap_blotvarReference->c_select.c_entry, ant_blotvarStuff) ==
      ap_blotvarReference->c_select.c_entry)
  break; case -1: // current blotvar reference 
    switch (g_BlotregIndexFetch(ap_blotvarReference->blotregHandle,NULL,
      INDEX_FETCH_FLAGS__CURRENT, ant_blotvarStuff, navn_entry)){
    case RESULT__FOUND:
m_ASSERT(*ant_blotvarStuff != NULL)
    break; case RESULT__NOT_FOUND:
m_ASSERT(*ant_blotvarStuff == NULL)
    break; default: m_TRACK() 
    } // switch   
 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->n_specifierFlag)  
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // FetchBlotvar

// BLOTEX_VALUE:
// -------------

// Public function: see .h
int SetIntexValue(struct BLOTEX_VALUE *a_intexValue, gen_BLOTVAL blotval){
  m_DIGGY_BOLLARD()
  a_intexValue->b_strValue = b_INT_VALUE; 
  a_intexValue->select.c_blotval = blotval;
  m_DIGGY_RETURN(RETURNED)
} // SetIntexValue

// Public function: see .h
int SetStrexValue(G_STRINGS_HANDLE workingGStringsHandle, struct BLOTEX_VALUE *a_strexValue,
  struct P_STRING str, char b_fugaciousStr) {
  m_DIGGY_BOLLARD()
  a_strexValue->b_strValue = b_STR_VALUE; 
  G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF((a_strexValue->select.c_strex.workingGStringEntry =
    GStringsFetch(workingGStringsHandle,-1, &t_workingGStringStuff)) < 0)
  if (b_fugaciousStr) {
    m_TRACK_IF(GStringCopy(t_workingGStringStuff,0, str) < 0)
  } else {
    switch(GStringImport(t_workingGStringStuff, str)) {
    case COMPLETED__OK:
    break; case COMPLETED__BUT: 
    break; default:
      m_TRACK()
    } // switch
  } // if
  a_strexValue->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
  m_DIGGY_RETURN(RETURNED)
} // SetStrexValue 

// Public function: see .h
int ConcatenateStrexValue(G_STRINGS_HANDLE workingGStringsHandle,struct BLOTEX_VALUE *a_strexValue1,
  struct P_STRING p_str2) {
  m_DIGGY_BOLLARD()
  m_ASSERT(a_strexValue1->b_strValue)
  G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF(GStringsFetch(workingGStringsHandle,
    a_strexValue1->select.c_strex.workingGStringEntry, &t_workingGStringStuff) !=
    a_strexValue1->select.c_strex.workingGStringEntry)
  m_TRACK_IF(GStringCopy(t_workingGStringStuff,-1, p_str2) < 0)
  a_strexValue1->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
  m_DIGGY_RETURN(RETURNED)
} // ConcatenateStrexValue


// Parsing and computing functions: 
// =============================== 

// See .h 
char ob_EmptySequence(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  om_PParsePassSpaces(a_sequence,NULL);
  m_DIGGY_RETURN(b_EMPTY_P_STRING(*a_sequence))
} // ob_EmptySequence


// See .h 
int ParseExistingSequence(struct P_STRING *a_sequence,const char *p_sequenceType,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_PREPARE_ABANDON(a_sequence,p_sequenceType)
  if (ob_EmptySequence(a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseExistingSequence

// See .h 
int ParseEndOfSequence(struct P_STRING *a_sequence, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  if (!ob_EmptySequence(a_sequence)) m_ABANDON_S(NOT_PARSABLE__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseEndOfSequence


// IS_CHAR_FUNCTION:
// Recognize any character corresponding to <entity name>
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 

// See .h 
int o_ParseEntityName(struct P_STRING *a_sequence, struct P_STRING *a_entityName) {
  m_DIGGY_BOLLARD()
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassChars(a_sequence,PASS_CHARS_WHILE__SCAN_FLAGS,IsEntityNameChar,
    (char)UNDEFINED,a_entityName); // <entity name>
  m_DIGGY_RETURN(RETURNED)
} // o_ParseEntityName


#define  AS__ENTRY__XX "!#" 
#define  AS__NAME__XX           "!$"
#define  AS__VALUE_INT__X       "#" 
#define  AS__ID__X              "!"
#define  AS__VALUE_STR__X       "$"

// See .h 
int ParseAs(char b_lValue, struct P_STRING *a_sequence, int *a_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  om_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,a_as,&lexeme, 5,
    AS__ENTRY__XX,b_lValue? -1:AS__ENTRY,  AS__NAME__XX,AS__NAME,
    AS__VALUE_INT__X,AS__VALUE_INT,  AS__ID__X,AS__ID,  AS__VALUE_STR__X,AS__VALUE_STR)
  if (*a_as == -1) *a_as = AS__VALUE_INT;
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*a_as)
  m_DIGGY_RETURN(RETURNED)
} // ParseAs 

// See .h 
int ParseAsValue(struct P_STRING *a_sequence, signed char *anb_strValue) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  om_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  int n_as = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,&n_as,&lexeme, 2,
    AS__VALUE_INT__X,AS__VALUE_INT, AS__VALUE_STR__X,AS__VALUE_STR)
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(n_as)
  if (n_as == -1) *anb_strValue = -1; 
  else *anb_strValue = (n_as == AS__VALUE_STR);
  m_DIGGY_RETURN(RETURNED)
} // ParseAsValue 


// See .h 
int ParseSpecifier(struct P_STRING *a_sequence, int *a_specifierFlags, 
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  // Retrieve blotvar reference:
  PParseOffset(a_sequence,1,&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON_S(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  switch (lexeme.string[0]) {
  case '.' : // <name specifier> : 
    if (b_FLAG_SET_OFF(*a_specifierFlags,NAME__SPECIFIER_FLAG)) m_ABANDON_S(
      SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *a_specifierFlags = NAME__SPECIFIER_FLAG;
  break; case '[' : // <entry specifier>  :
    if (b_FLAG_SET_OFF(*a_specifierFlags,ENTRY__SPECIFIER_FLAG)) m_ABANDON_S(
      SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *a_specifierFlags = ENTRY__SPECIFIER_FLAG;
  break; case '{' : // <token id specifier> : 
    if (b_FLAG_SET_OFF(*a_specifierFlags,TOKEN_ID__SPECIFIER_FLAG)) m_ABANDON_S(
      SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *a_specifierFlags = TOKEN_ID__SPECIFIER_FLAG;
  break; default:
    m_ABANDON_S(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseSpecifier 

// See .h 
int ParseEndSpecifier(struct P_STRING *a_sequence, int specifierFlag, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  char singleChar = (char) UNDEFINED;
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  switch (specifierFlag) {
  case ENTRY__SPECIFIER_FLAG:
    singleChar = ']';
  break; case TOKEN_ID__SPECIFIER_FLAG:    
    singleChar = '}';
  break; default: 
    m_RAISE(ANOMALY__VALUE__D,specifierFlag)
  } // switch
  o_PParsePassSingleChar(a_sequence,NULL,singleChar,&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON_S(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES)
} // ParseEndSpecifier

// See .h 
int DelimitBlotregRequest(struct P_STRING *a_sequence,const char *np_sequenceType, 
  struct P_STRING *ac_blotregRequestSequence, G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, (np_sequenceType != NULL? np_sequenceType:"<blotreg request>"))

  m_TRACK_IF(PParseTillMatch(a_sequence,FLAG_OFF0,o_PString(":?"),NULL, ac_blotregRequestSequence)
    != RETURNED)
m_DIGGY_VAR_P_STRING(*ac_blotregRequestSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);

  m_DIGGY_RETURN(ANSWER__YES)
} // DelimitBlotregRequest

// See .h 
int DelimitCreationSequence(struct P_STRING *a_sequence, const char *p_sequenceType,
  struct P_STRING *ac_creationSequence, G_STRING_STUFF nc_abandonmentInfo){
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence,p_sequenceType)

  m_TRACK_IF(PParseTillMatch(a_sequence,FLAG_OFF0,o_PString("]?"),NULL, ac_creationSequence) !=
    RETURNED)
m_DIGGY_VAR_P_STRING(*ac_creationSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);

  m_DIGGY_RETURN(ANSWER__YES)
} // DelimitCreationSequence

// See .h 
char ob_ParseAnything(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  o_PParsePassSingleChar(a_sequence,NULL,'*',&lexeme); 
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // ob_ParseAnything

// See .h 
int ParseBlotregRequestAtom(struct P_STRING *a_sequence, int *ac_as, int *ac_indexSeekFlags,
  G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<blotreg request atom>")

  // <blotreg request atom int> | <blotreg request atom str> ...
  m_TRACK_IF(ParseAs(b_R_VALUE,a_sequence,ac_as) != RETURNED)
  if (ob_ParseAnything(a_sequence)) *ac_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
  else {  // select with actual criterion
    m_TRACK_IF(ParseRequestCompOp(a_sequence,*ac_as != AS__VALUE_INT,
      ac_indexSeekFlags) != RETURNED)
    if (*ac_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseBlotregRequestAtom

// Interpret as ...  blotreg index
//
// Passed:
// - as:
// - nap_blotexValue: NULL special pointer => NO actual criterion
// - cac_gsKey: only significant if actual criterion
// 
// Changed:
// - *ac_blotregIndexLabel: only significant if no abandon
// - *cac_gsKey: only significant if no abandon
// - nc_abandonmentInfo: only significant if abandon
// 
// Ret: parsed successfully ? 
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'str/int mismatch' error; abandon processing 
// - 1: unexpected problem; anomaly is raised
static inline int m_AsBlotregIndex(int as, const struct BLOTEX_VALUE* nap_blotexValue,
  int *ac_blotregIndexLabel, struct GS_KEY* cac_gsKey, G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()

  switch (as) {
  case AS__VALUE_INT: // [ '#' ]
    *ac_blotregIndexLabel = INT_VALUE__BLOTREG_INDEX_LABEL;
  break; case AS__ENTRY: // '!#' (r-value) 
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case AS__ID: // '!' 
    *ac_blotregIndexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
  break; case AS__VALUE_STR: // '$'
    *ac_blotregIndexLabel = STR_VALUE__BLOTREG_INDEX_LABEL;
  break; case AS__NAME:  // '!$'
    *ac_blotregIndexLabel = NAME__BLOTREG_INDEX_LABEL;
  break; default: m_RAISE(ANOMALY__VALUE__D,as)
  } // switch

  if (nap_blotexValue != NULL) {
    switch (as) {
    case AS__VALUE_INT: // [ '#' ]
      if (nap_blotexValue->b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
      *cac_gsKey = m_GsKey3(nap_blotexValue->select.c_blotval); 
    break; case AS__ENTRY: // '!#' (r-value) 
      if (nap_blotexValue->b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__ID: // '!' 
      *cac_gsKey = m_GsKey3(nap_blotexValue->select.c_blotval); 
    break; case AS__VALUE_STR: // '$'
      if (!nap_blotexValue->b_strValue) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
      *cac_gsKey = m_GsKey(nap_blotexValue->select.c_strex.v_str);
    break; case AS__NAME:  // '!$'
      if (!nap_blotexValue->b_strValue) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
      *cac_gsKey = m_GsKey(nap_blotexValue->select.c_strex.v_str);
    break; default: m_RAISE(ANOMALY__VALUE__D,as)
    } // switch
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // m_AsBlotregIndex 

// See .h 
int ParseBlotregRequestAtomEnd(struct P_STRING *a_sequence, int as, int indexSeekFlags,
  struct GS_REQUEST_CRITERIA *a_criteria52, 
  struct BLOTEX_VALUE c_blotexValue, G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
  int blotregIndexLabel = UNDEFINED;

struct GS_KEY c_gsKey = UNDEFINED_GS_KEY; // only significant if actual criterion 

  m_CHECK_ABANDON(m_AsBlotregIndex(as,indexSeekFlags != INDEX_SEEK_FLAGS__ANY?
    &c_blotexValue: NULL,&blotregIndexLabel,&c_gsKey, nc_abandonmentInfo))

  int criteriaOpFlags = UNDEFINED;
  m_TRACK_IF(ParseLogical2Op(a_sequence, &criteriaOpFlags) != RETURNED)

  m_TRACK_IF(GsRequestCriteriaAddCriterion(a_criteria52, blotregIndexLabel,
indexSeekFlags, c_gsKey,UNDEFINED_GS_KEY_PAR,criteriaOpFlags) < 0) 

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseBlotregRequestAtomEnd

// See .h 
char ob_ParseOpReadSet(struct P_STRING *a_sequence, int *an_indexFetchFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) {
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
    m_DIGGY_RETURN(b_TRUE)
  } // if
  m_DIGGY_RETURN(b_FALSE0)
} // ob_ParseOpReadSet 

// See .h 
char ob_ParseOpCreate(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'[',&lexeme); 
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme)) ;
} // ob_ParseOpCreate

// See .h 
char ob_ParseOpNext(struct P_STRING *a_sequence, int *an_indexFetchFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'+',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) {
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
    m_SET_FLAG_ON(*an_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
    m_DIGGY_RETURN(b_TRUE)
  } // if
  m_DIGGY_RETURN(b_FALSE0)
} // ob_ParseOpNext 

// See .h 
char ob_ParseOpSelect(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,':',&lexeme); 
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme)) ;
} // ob_ParseOpSelect

// See .h 
char ob_ParseOpReset(struct P_STRING *a_sequence, int *an_indexFetchFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'^',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) {
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
    m_SET_FLAG_ON(*an_indexFetchFlags,INDEX_FETCH_FLAG__RESET)
    m_DIGGY_RETURN(b_TRUE)
  } // if
  m_DIGGY_RETURN(b_FALSE0)
} // ob_ParseOpReset

// See .h 
char ob_ParseOpInsert(struct P_STRING *a_sequence, int *an_indexFetchFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'@',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) {
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
m_RAISE(ANOMALY__NOT_AVAILABLE)
    m_DIGGY_RETURN(b_TRUE)
  } // if
  m_DIGGY_RETURN(b_FALSE0)
} // ob_ParseOpInsert


// See . h
int GetBlotvarRValue(g_BLOTVAR_STUFF n_blotvarStuff, int n_as, int c_entry, 
  G_STRINGS_HANDLE workingGStringsHandle, struct BLOTEX_VALUE *a_blotexValue) {
  m_DIGGY_BOLLARD()

  char b_strValue = (char)UNDEFINED;
  gen_BLOTVAL c_blotval = UNDEFINED; 
  struct P_STRING str = UNDEFINED_P_STRING;
  char cb_fugaciousStr = (char)UNDEFINED;

  switch (n_as) {
  case -1:
    b_strValue = b_INT_VALUE;
    c_blotval = BOOLEAN_BLOTVAL(n_blotvarStuff != NULL); 
  break; case AS__VALUE_INT: // <blotvar as int>
    b_strValue = b_INT_VALUE;
    c_blotval = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value:
      FALSE__BLOTVAL;
  break; case AS__ID: // <blotvar id>
    b_strValue = b_INT_VALUE;
    c_blotval = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].acolyt.cen_value:
      FALSE__BLOTVAL;
  break; case AS__ENTRY: // <blotvar entry> (r-value) 
    b_strValue = b_INT_VALUE;
    c_blotval = n_blotvarStuff != NULL? c_entry: FALSE__BLOTVAL;
  break; case AS__VALUE_STR: // <blotvar strex> 
    b_strValue = b_STR_VALUE;
    str = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].cv_pString:
      *ap_aTrivialEmptyPString;
    cb_fugaciousStr = n_blotvarStuff != NULL; 
  break; case AS__NAME:  // <blotvar name> 
    b_strValue = b_STR_VALUE,
    str = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].cv_pString:
      *ap_aTrivialEmptyPString;
    cb_fugaciousStr = n_blotvarStuff != NULL;
  break; default:
    m_RAISE(ANOMALY__VALUE__D,n_as)
  } // switch

  if (b_strValue) m_TRACK_IF(SetStrexValue(workingGStringsHandle,a_blotexValue,str,cb_fugaciousStr)
    != RETURNED)
  else m_TRACK_IF(SetIntexValue(a_blotexValue,c_blotval) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GetBlotvarRValue

// See . h
int SetBlotvarLValue(g_BLOTVAR_STUFF blotvarStuff,int lValueAs, struct BLOTEX_VALUE blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  switch (lValueAs) {
  case AS__VALUE_INT: // [ '#' ]
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value = blotexValue.select.c_blotval;
  break; case AS__ID: // '!' 
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case AS__VALUE_STR: // '$'
    if (!blotexValue.b_strValue) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
    m_TRACK_IF(GStringCopy(blotvarStuff+g_BLOTVAR_VALUE_ELEMENT,0,
      blotexValue.select.c_strex.v_str) < 0)
  break; case AS__NAME:  // '!$'
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; default: m_RAISE(ANOMALY__VALUE__D,lValueAs) } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // SetBlotvarLValue


// See .h
int ParseCompOp(int compOpExtension, struct P_STRING *a_sequence, int *an_compOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  int n_matchedEntry = UNDEFINED;
  om_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_compOp,&lexeme, 9,
    "===", compOpExtension == STR__COMP_OP_EXTENSION? LIKE__STR__COMP_OP: -1,
    "!=",NOT_EQUAL__COMP_OP, ">=",GREATER_EQUAL__COMP_OP, "<=",LESS_EQUAL__COMP_OP,
    "==",EQUAL__COMP_OP, "<",LESS__COMP_OP, ">",GREATER__COMP_OP,
    "*",compOpExtension == FACT_OP__COMP_OP_EXTENSION? MULTIPLY__FACT_OP__COMP_OP: -1,
    "/",compOpExtension == FACT_OP__COMP_OP_EXTENSION? DIVIDE__FACT_OP__COMP_OP: -1)
  m_DIGGY_RETURN(RETURNED)
} // ParseCompOp

// See .h
int ComputeFactOp(gen_BLOTVAL *a_blotval1, int factOp, gen_BLOTVAL blotval2) {
  m_DIGGY_BOLLARD()
  switch (factOp) {
  case EQUAL__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 == blotval2);
  break; case LESS__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 < blotval2);
  break; case LESS_EQUAL__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 <= blotval2);
  break; case GREATER__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 > blotval2);
  break; case GREATER_EQUAL__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 >= blotval2);
  break; case NOT_EQUAL__COMP_OP:
    *a_blotval1 = BOOLEAN_BLOTVAL(*a_blotval1 != blotval2);
  break; case MULTIPLY__FACT_OP__COMP_OP:
    *a_blotval1 = *a_blotval1 * blotval2;
  break; case DIVIDE__FACT_OP__COMP_OP:
    *a_blotval1 = *a_blotval1 / blotval2;
  break; default:
    m_RAISE(ANOMALY__VALUE__D,factOp);
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // ComputeFactOp

// See .h
int ParseRequestCompOp(struct P_STRING *a_sequence, char b_str,
  int *an_indexSeekFlags) {
  m_DIGGY_BOLLARD()
  *an_indexSeekFlags = -1; // a priori
  om_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)

  int n_compOp = UNDEFINED;
  m_TRACK_IF(ParseCompOp(b_str? STR__COMP_OP_EXTENSION:
    NO__COMP_OP_EXTENSION, a_sequence, &n_compOp) != RETURNED)
m_DIGGY_VAR_D(n_compOp)
  switch (n_compOp) {
  case -1: 
    *an_indexSeekFlags = -1;
  break; case GREATER__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER; 
  break; case LESS__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS; 
  break; case GREATER_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__GREATER_EQUAL;
  break; case LESS_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LESS_EQUAL;
  break; case EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__EQUAL;
  break; case NOT_EQUAL__COMP_OP: 
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__NOT_EQUAL;
  break; case LIKE__STR__COMP_OP: 
    m_ASSERT(b_str)
    *an_indexSeekFlags = INDEX_SEEK_FLAGS__LIKE;
  break; default: m_RAISE(ANOMALY__VALUE__D,n_compOp)
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // ParseRequestCompOp 

// See .h
int ParseLogical2Op(struct P_STRING *a_sequence,
  int *a_criteriaOpFlags) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  om_PParsePassSpaces(a_sequence,NULL);
  PParseMatch(a_sequence,o_PString("and"),NULL,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__AND;
  else { 
    PParseMatch(a_sequence,o_PString("or"),NULL,&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__OR;
    else *a_criteriaOpFlags = ALL_FLAGS_OFF0;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ParseLogical2Op


// IS_CHAR_FUNCTION:
static int IsInt1Op(int c) {
  return (c == '+' || c == '-' || c == '!');
} // IsInt1Op

// See .h
int ParseInt1Op(struct P_STRING *a_sequence, int *an_int1Op) { 
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  *an_int1Op = -1; // a priori
  o_PParsePassSingleChar(a_sequence,IsInt1Op,(char)UNDEFINED,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : *an_int1Op = NOT__INT_1OP;
    break; case '+' : *an_int1Op = PLUS__INT_1OP;
    break; case '-' : *an_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    om_PParsePassSpaces(a_sequence,NULL);
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ParseInt1Op

// See .h
int ComputeInt1Op(int n_int1Op,struct BLOTEX_VALUE *ac_blotexValue,G_STRING_STUFF nc_abandonmentInfo){
  m_DIGGY_BOLLARD()
  if (n_int1Op >= 0) {
    if (ac_blotexValue->b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    switch (n_int1Op) {
    break; case NOT__INT_1OP:
      if (ac_blotexValue->select.c_blotval == TRUE__BLOTVAL0)
        ac_blotexValue->select.c_blotval = FALSE__BLOTVAL;
      else ac_blotexValue->select.c_blotval = TRUE__BLOTVAL0;
    break; case PLUS__INT_1OP:
    break; case MINUS__INT_1OP:
      ac_blotexValue->select.c_blotval = -(ac_blotexValue->select.c_blotval);
    break; default:
      m_RAISE(ANOMALY__VALUE__D,n_int1Op)
    } // switch
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ComputeInt1Op 

// See .h
int ParseIntConstant(struct P_STRING *a_sequence, char *ab_parsed,
  struct BLOTEX_VALUE *ac_blotexValue) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  gen_BLOTVAL c_blotval = UNDEFINED;
  PParseGenericInteger(a_sequence,&c_blotval,&lexeme);
  if ((*ab_parsed = !b_EMPTY_P_STRING(lexeme))) m_TRACK_IF(SetIntexValue(ac_blotexValue,c_blotval)
    != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // ParseIntConstant

// See .h
int ParseStrConstant(struct P_STRING *a_sequence, char *acb_parsed,
  G_STRINGS_HANDLE workingGStringsHandle, struct BLOTEX_VALUE *acc_blotexValue,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  m_PREPARE_ABANDON(a_sequence, "<str constant>")
  o_PParsePassSingleChar(a_sequence,NULL,'"',&lexeme);
  if ((*acb_parsed = !b_EMPTY_P_STRING(lexeme))) { // <str constant> ... 
    o_PParsePassChars(a_sequence,PASS_CHARS_TILL__SCAN_FLAGS,NULL,'"', &lexeme);
    if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    PParseOffset(a_sequence,1,NULL);
    m_TRACK_IF(SetStrexValue(workingGStringsHandle,acc_blotexValue,lexeme,!b_FUGACIOUS_STR) !=
      RETURNED) // TODO: handle permanancy : PROCESS / PARSER / VARIABLE
  } // if 
  m_DIGGY_RETURN(ANSWER__YES)
} // ParseStrConstant

// See .h
int ProbeBlotexAtom(struct P_STRING *a_sequence, int *acn_int1Op, int *ac_probedBlotexAtom,
  G_STRINGS_HANDLE workingGStringsHandle, struct BLOTEX_VALUE *acc_blotexAtomValue,
  G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_CHECK_ABANDON(ParseExistingSequence(a_sequence,"<intex atom> | <strex atom>",
    nc_abandonmentInfo))

  m_TRACK_IF(ParseInt1Op(a_sequence, acn_int1Op) != RETURNED)
m_DIGGY_VAR_D(*acn_int1Op)
  char b_parsed = (char) UNDEFINED; 
  m_TRACK_IF(ParseIntConstant(a_sequence, &b_parsed, acc_blotexAtomValue) != RETURNED)
  *ac_probedBlotexAtom = CONSTANT__PROBED_BLOTEX_ATOM; // a priori
  if (!b_parsed) {
    m_CHECK_ABANDON(ParseStrConstant(a_sequence,&b_parsed, workingGStringsHandle,
      acc_blotexAtomValue,nc_abandonmentInfo))
    if (!b_parsed) {
      struct P_STRING lexeme = UNDEFINED_P_STRING;
      o_PParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) *ac_probedBlotexAtom = BLOTEX__PROBED_BLOTEX_ATOM;
      else *ac_probedBlotexAtom = OTHER__PROBED_BLOTEX_ATOM; 
    } // if 
  } // if 
m_DIGGY_VAR_D(*ac_probedBlotexAtom)
  m_DIGGY_RETURN(ANSWER__YES) 
} // ProbeBlotexAtom

// See .h
int ParseBlotexAtomBlotexEnd(struct P_STRING *a_sequence,G_STRING_STUFF nc_abandonmentInfo){
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  m_PREPARE_ABANDON(a_sequence, "<intex atom> | <strex atom>")
  o_PParsePassSingleChar(a_sequence,NULL,')',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseBlotexAtomBlotexEnd


// See .h
char ob_ParseOpsIndicator(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  o_PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // ob_ParseOpsIndicator

// See .h
char ob_ParseStrexAtomOp(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  o_PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // ob_ParseStrexAtomOp

// See .h
int ParseTermOp(struct P_STRING *a_sequence, int *an_termOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  int c_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&c_matchedEntry,an_termOp,&lexeme, 4,
    "&&",AND__TERM_OP, "||",OR__TERM_OP, "+",ADD__TERM_OP, "-",SUBTRACT__TERM_OP)
  // TODO: FOIREUX !!!!???? "++" sera rejeted !!!!!!????
  m_DIGGY_RETURN(RETURNED)
} // ParseTermOp

// See .h
int ComputeTermOp(gen_BLOTVAL *a_blotval1, int termOp, gen_BLOTVAL blotval2) {
  m_DIGGY_BOLLARD()
  switch (termOp) {
  case ADD__TERM_OP:
    *a_blotval1 += blotval2;
  break; case SUBTRACT__TERM_OP:
    *a_blotval1 -= blotval2;
  break; case AND__TERM_OP:
    *a_blotval1 = *a_blotval1 && blotval2;
  break; case OR__TERM_OP:
    *a_blotval1 = *a_blotval1 || blotval2;
  break; default:
    m_RAISE(ANOMALY__VALUE__D,termOp)
  } // switch
  m_DIGGY_RETURN(RETURNED)
} // ComputeTermOp

// IS_CHAR_FUNCTION:
static int IsFormatSpecifierChar(int c) {
  return (c == 'd' || c == 's' || c == 'x' || c == 'X' || c == 'L' || c == 'B' || c == 'E');
} // IsFormatSpecifierChar


// See .h
int ParseFormatAndSeparator(struct P_STRING *a_sequence, int *ac_format,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED

  int n_format = -1; // a priori
  m_PREPARE_ABANDON(a_sequence, "<format>") 
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  o_PParsePassChars(a_sequence,PASS_CHARS_WHILE__SCAN_FLAGS,IsFormatSpecifierChar,(char)UNDEFINED,
    &lexeme);
  int length = m_PStringLength(lexeme);
  if (length > 0) {
    switch (lexeme.string[0]) {
    case 'd': 
      if (length == 1) n_format = D__FORMAT;
    break; case 's':
      if (length == 1) n_format = LS__FORMAT;
    break; case 'x':
      if (length == 1) n_format = LX__FORMAT;
    break; case 'X':
      if (length == 1) n_format = UX__FORMAT;
    break; case 'L':
      if (length == 2 && lexeme.string[0] == 'E') n_format = LE__FORMAT;
    break; case 'B':
      if (length == 2 && lexeme.string[0] == 'E') n_format = BE__FORMAT;
    } // switch
  } // if

  if (n_format < 0) m_ABANDON(INVALID_FORMAT__ABANDONMENT_CAUSE) 
  *ac_format = n_format;

  m_PRECISE_ABANDON(a_sequence, "','") 
  om_PParsePassSpaces(a_sequence,NULL);
  o_PParsePassSingleChar(a_sequence,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  om_PParsePassSpaces(a_sequence,NULL);

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseFormatAndSeparator

// See .h
int ComputeFormat(int format, struct BLOTEX_VALUE blotexValue, G_STRING_STUFF c_surrogate,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  switch (format) {
  case D__FORMAT:
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%d", blotexValue.select.c_blotval);
  break; case LS__FORMAT:
    if (!blotexValue.b_strValue) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,FMT_P_STRING,m_P_STRING_2_FMT_ARGS(
      blotexValue.select.c_strex.v_str));
  break; case LX__FORMAT:
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%x",blotexValue.select.c_blotval);
  break; case UX__FORMAT:
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%X", blotexValue.select.c_blotval);
  break; case LE__FORMAT:
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case BE__FORMAT:
    if (blotexValue.b_strValue) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; default:
    m_RAISE(ANOMALY__VALUE__D,format)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // ComputeFormat

int ParseBlottabsLabel(struct P_STRING *a_sequence, int *an_blottabsLabel) {
  m_DIGGY_BOLLARD()
  *an_blottabsLabel = -1; // a priori
  struct P_STRING lexeme; // UNDEFINED
  while (b_TRUE) { 
    o_PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) break;
    (*an_blottabsLabel)++; 
  } // while 
  m_DIGGY_RETURN(RETURNED)
} // ParseBlottabsLabel


// See .h
int ParseLValueBlotregOps( 
  struct P_STRING *a_sequence, struct P_STRING blotregName, g_BLOTREG_HANDLE n_blotregHandle, 
  struct BLOTVAR_REFERENCE *ac_blotvarReference, int *ac_lValueAs, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  m_PREPARE_ABANDON(a_sequence, "<blotreg ref op set int> | <blotreg ref op set str>")

  if (n_blotregHandle == NULL) m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE)
  ac_blotvarReference->blotregHandle = n_blotregHandle; 
  ac_blotvarReference->n_specifierFlag = -1; // current blotvar reference  
  *ac_lValueAs = UNDEFINED; // For the moment 

  om_PParsePassSpaces(a_sequence,NULL);

  int n_lValueAs = -1; // No blotreg read/set op a priori 
  int n_indexFetchFlags = -1;
  if (ob_ParseOpReadSet(a_sequence,&n_indexFetchFlags)) {
    // <blotreg ref op set int> | <blotreg ref op set str>... 
    m_ASSERT(n_indexFetchFlags == ALL_FLAGS_OFF0)
    m_TRACK_IF(ParseAs(b_L_VALUE,a_sequence,&n_lValueAs) != RETURNED)
  } // if
  
  if (n_lValueAs == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  *ac_lValueAs = n_lValueAs ;

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseLValueBlotregOps

// See .h
signed char b_ParseBlotexAssignation(struct P_STRING *a_sequence, struct P_STRING *ac_subSequence) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(PParseTillMatch(a_sequence,QUOTED__SCAN_FLAG,o_PString(":="),NULL,ac_subSequence) != RETURNED)
  char b_assignation = !b_EMPTY_P_STRING(*a_sequence);
  if (b_assignation) { 
    PParseOffset(a_sequence,2,NULL);
  } else { // NO assignation
    *a_sequence = *ac_subSequence; 
  } // if
  om_PParsePassSpaces(a_sequence,NULL); // TODO: ag virer ???
  
  m_DIGGY_RETURN(b_assignation)
} // b_ParseBlotexAssignation


// Public function; see .h
char ob_ParseStrPortion(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;
  om_PParsePassSpaces(a_sequence,NULL);

  o_PParsePassSingleChar(a_sequence,NULL,'{',&lexeme);
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // ob_ParseStrPortion

// IS_CHAR_FUNCTION:
static int IsPortionOpChar(int c) { 
  return (c == ':' || c == '-');
} // IsPortionOpChar 

// Public function; see .h
char ob_ParseStrPortionOffset(struct P_STRING *a_sequence, int *ac_portionOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  o_PParsePassSingleChar(a_sequence,IsPortionOpChar,(char)UNDEFINED,&lexeme);
  
  if (!b_EMPTY_P_STRING(lexeme)) {
    if (lexeme.string[0] == ':') *ac_portionOp = OFFSET__PORTION_OP; else *ac_portionOp =
      LENGTH__PORTION_OP; 
    m_DIGGY_RETURN(b_TRUE)
  } // if
  m_DIGGY_RETURN(b_FALSE0)
} // ob_ParseStrPortionOffset

// Public function; see .h
int ParseStrPortionEnd(struct P_STRING *a_sequence,G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence,"<str portion>")
  o_PParsePassSingleChar(a_sequence,NULL,'}',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES)
} // ParseStrPortionEnd



