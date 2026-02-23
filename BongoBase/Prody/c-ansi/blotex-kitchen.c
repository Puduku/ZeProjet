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
  m_TRACK_IF(l_GParamsCreateInstance(azh_handle,BATEAU__EXPECTED_ITEMS_NUMBER) !=
    RETURNED) 
  g_BLOTREG_HANDLE handle = *azh_handle;
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_NAME_ELEMENT,P_STRING__G_KEYS_COMPARISON,NULL,NULL,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == NAME__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_NAME_ELEMENT,ACOLYT_VALUE__G_KEYS_COMPARISON,
    (IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) == TOKEN_ID__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_VALUE_ELEMENT,ACOLYT_VALUE__G_KEYS_COMPARISON,
    (IS_CHAR_FUNCTION)UNDEFINED,(TO_CHAR_FUNCTION)UNDEFINED,
    (P_STRING_INTRINSIC_VALUE_FUNCTION)UNDEFINED,(void*)UNDEFINED) ==
    INT_VALUE__BLOTREG_INDEX_LABEL)
  m_ASSERT(g_GParamsAddIndex(handle,1,G_PARAM_VALUE_ELEMENT, P_STRING__G_KEYS_COMPARISON,NULL,NULL,
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
int FetchBlotvar(const struct BLOTVAR_REFERENCE *ap_blotvarReference, char cb_lValue,
  g_BLOTVAR_STUFF *ant_blotvarStuff, int *navn_entry) {
  m_DIGGY_BOLLARD()

  int ret = UNDEFINED;
  *ant_blotvarStuff = (g_BLOTVAR_STUFF)UNDEFINED;
  switch (ap_blotvarReference->blotvarReference) {
  case NAME__BLOTVAR_REFERENCE:
  case TOKEN_ID__BLOTVAR_REFERENCE:
    { struct G_KEY gKey ;
      int indexLabel = UNDEFINED;
      if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE) {
        indexLabel = NAME__BLOTREG_INDEX_LABEL;
m_DIGGY_VAR_P_STRING(ap_blotvarReference->c_select.c_name)
        gKey = m_GKey_PString(ap_blotvarReference->c_select.c_name);
      } else {
        indexLabel = TOKEN_ID__BLOTREG_INDEX_LABEL;
        gKey = m_GKey_AcolytValue(ap_blotvarReference->c_select.c_tokenId);
      } // if
m_DIGGY_VAR_P(ap_blotvarReference->blotregHandle)
      switch (gm_BlotregIndexSingleFetch(ap_blotvarReference->blotregHandle,NULL,indexLabel,
        INDEX_SEEK_FLAGS__EQUAL,&gKey,cb_lValue?  INDEX_FETCH_FLAGS__FETCH:
        INDEX_FETCH_FLAGS__SEEK_ONLY, ant_blotvarStuff, navn_entry)) {
      case RESULT__FOUND:
      break; case RESULT__NOT_FOUND:
        if (cb_lValue) { 
          m_ASSERT(*ant_blotvarStuff != NULL)
          if (ap_blotvarReference->blotvarReference == NAME__BLOTVAR_REFERENCE)
            m_TRACK_IF(m_GParamNameCopy(*ant_blotvarStuff,0,ap_blotvarReference->c_select.c_name)
              < 0)
          else { // TOKEN_ID__BLOTVAR_REFERENCE 
            m_TRACK_IF(GParamNameAssign(*ant_blotvarStuff,(struct P_STRING*)NULL,
              ap_blotvarReference->c_select.c_tokenId, ap_blotvarReference->blotregHandle)
              < 0)
          } // if
        } // if
      break; default:
        m_TRACK()
      } // switch
    } // gKey 
  break; case ENTRY__BLOTVAR_REFERENCE:
    ret = g_BlotregFetch(ap_blotvarReference->blotregHandle,
      ap_blotvarReference->c_select.c_entry, ant_blotvarStuff);
    m_TRACK_IF(ret < 0)
m_ASSERT(ret == ap_blotvarReference->c_select.c_entry)
  break; case CURRENT__BLOTVAR_REFERENCE:
    switch (g_BlotregIndexFetch(ap_blotvarReference->blotregHandle,NULL,
      INDEX_FETCH_FLAGS__CURRENT, ant_blotvarStuff, navn_entry)){
    case RESULT__FOUND:
m_ASSERT(*ant_blotvarStuff != NULL)
    break; case RESULT__NOT_FOUND:
m_ASSERT(*ant_blotvarStuff == NULL)
    break; default: m_TRACK() 
    } // switch   
 
  break; default:
    m_RAISE(ANOMALY__VALUE__D,ap_blotvarReference->blotvarReference)  
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // FetchBlotvar

// BLOTEX_VALUE:
// -------------

// Public function: see .h
int SetBlotexValue(G_STRINGS_HANDLE c_workingGStringsHandle,struct BLOTEX_VALUE *a_blotexValue,
  int asValue, gen_BLOTVAL c_blotval, const struct P_STRING* cap_str, char cb_fugaciousStr) {
  m_DIGGY_BOLLARD()
  switch ((a_blotexValue->asValue = asValue)) {
  case AS__VALUE_STR:
    { G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
      m_TRACK_IF((a_blotexValue->select.c_strex.workingGStringEntry =
        GStringsFetch(c_workingGStringsHandle,-1, &t_workingGStringStuff)) < 0)
      if (cb_fugaciousStr) {
        m_TRACK_IF(GStringCopy(t_workingGStringStuff,0, *cap_str) < 0)
      } else {
        switch(GStringImport(t_workingGStringStuff, *cap_str)) {
        case COMPLETED__OK:
        break; case COMPLETED__BUT: 
        break; default:
          m_TRACK()
        } // switch
      } // if
      a_blotexValue->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
    } // t_workingGStringStuff
  break; case AS__VALUE_INT:
    a_blotexValue->select.c_blotval = c_blotval;
  break; default: m_RAISE(ANOMALY__VALUE__D,asValue) } // switch  
  m_DIGGY_RETURN(RETURNED)
} // SetBlotexValue

// Public function: see .h
int ConcatenateStrexValue(G_STRINGS_HANDLE workingGStringsHandle,
  struct BLOTEX_VALUE *a_strexValue1, struct P_STRING p_str2) {
  m_DIGGY_BOLLARD()
  m_ASSERT(a_strexValue1->asValue == AS__VALUE_STR)
  G_STRING_STUFF t_workingGStringStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF(GStringsFetch(workingGStringsHandle,
    a_strexValue1->select.c_strex.workingGStringEntry, &t_workingGStringStuff) !=
    a_strexValue1->select.c_strex.workingGStringEntry)
  m_TRACK_IF(GStringCopy(t_workingGStringStuff,-1, p_str2) < 0)
  a_strexValue1->select.c_strex.v_str = t_workingGStringStuff->cv_pString;
  m_DIGGY_RETURN(RETURNED)
} // ConcatenateStrexValue


// Parsing blot expressions: helpers 
// ================================= 

// See .h 
int ParseExistingSequence(struct P_STRING *a_sequence,const char *p_sequenceType,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_PREPARE_ABANDON(a_sequence,p_sequenceType)
  m_PParsePassSpaces(a_sequence,NULL);
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseExistingSequence

// See .h 
int ParseEndOfSequence(struct P_STRING *a_sequence, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  m_PParsePassSpaces(a_sequence,NULL);
  if (!b_EMPTY_P_STRING(*a_sequence)) m_ABANDON_S(NOT_PARSABLE__ABANDONMENT_CAUSE)
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseEndOfSequence


// IS_CHAR_FUNCTION:
// Recognize any character corresponding to <entity>
static int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 

// See .h 
int o_ParseEntity(struct P_STRING *a_sequence, struct P_STRING *a_entityName) {
  m_DIGGY_BOLLARD()
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
    (char)UNDEFINED,a_entityName); // <entity>
  m_DIGGY_RETURN(RETURNED)
} // o_ParseEntity




#define  AS__ENTRY__XX "!#" 
#define  AS__NAME__XX           "!$"
#define  AS__VALUE_INT__X       "#" 
#define  AS__ID__X              "!"
#define  AS__VALUE_STR__X       "$"

// See .h 
int ParseAs(char b_lValue, struct P_STRING *a_sequence, int *a_as) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PParsePassSpaces(a_sequence,NULL);
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
int ParseAsValue(struct P_STRING *a_sequence, int *an_asValue) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  m_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)
  int n_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_asValue,&lexeme, 2,
    AS__VALUE_INT__X,AS__VALUE_INT, AS__VALUE_STR__X,AS__VALUE_STR)
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P_STRING(lexeme)
m_DIGGY_VAR_D(*an_asValue)
  m_DIGGY_RETURN(RETURNED)
} // ParseAsValue 

// See .h 
int DelimitBlotregRequest(struct P_STRING *a_sequence, struct P_STRING *ac_blotregRequestSequence,
  G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<blotreg request>")

  PParseTillMatch(a_sequence,o_PString(":?"),NULL, ac_blotregRequestSequence);
m_DIGGY_VAR_P_STRING(*ac_blotregRequestSequence)
m_DIGGY_VAR_P_STRING(*a_sequence)
  if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParseOffset(a_sequence,2,NULL);

  m_DIGGY_RETURN(ANSWER__YES)
} // DelimitBlotregRequest


// See .h 
int ParseBlotregRequestAtom(struct P_STRING *a_sequence, int *ac_as, int *ac_indexSeekFlags,
  G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  m_PREPARE_ABANDON(a_sequence, "<blotreg request atom>")
  struct P_STRING lexeme;

  // <blotreg request atom int> | <blotreg request atom str> ...
  m_TRACK_IF(ParseAs(b_R_VALUE,a_sequence,ac_as) != RETURNED)
  PParsePassSingleChar(a_sequence,NULL,'*',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) *ac_indexSeekFlags = INDEX_SEEK_FLAGS__ANY;
  else {  // select with actual criterion
    m_TRACK_IF(ParseRequestCompOp(a_sequence,*ac_as != AS__VALUE_INT,
      ac_indexSeekFlags) != RETURNED)
    if (*ac_indexSeekFlags < 0) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseBlotregRequestAtom


// See .h 
char b_ParseRValueBlotregOpSelect(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassSingleChar(a_sequence,NULL,':',&lexeme);
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // b_ParseRValueBlotregOpSelect 

// See .h 
int ParseRValueBlotregFetchOps(struct P_STRING *a_sequence, int *an_indexFetchFlags, int *acn_as) {
  m_DIGGY_BOLLARD()
  *an_indexFetchFlags = -1; // No blotreg fetch op a priori 
  struct P_STRING lexeme;
  PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op reset>...
    *an_indexFetchFlags = INDEX_FETCH_FLAG__RESET;
  } // if
  PParsePassSingleChar(a_sequence,NULL,'+',&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op next>...
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
    m_SET_FLAG_ON(*an_indexFetchFlags,INDEX_FETCH_FLAG__NEXT)
  } // if
  
  *acn_as = -1; // No blotreg read/set op a priori  
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg op read int> | <blotreg op read str> (R-value)
    // <blotreg ref op set int> | <blotreg ref op set str> (L-value)... 
    m_TRACK_IF(ParseAs(!b_L_VALUE,a_sequence,acn_as) != RETURNED)
    if (*an_indexFetchFlags < 0) *an_indexFetchFlags = ALL_FLAGS_OFF0;
  } // if
  m_DIGGY_RETURN(RETURNED)
} // ParseRValueBlotregFetchOps


// See .h 
int AsBlotregIndex(int as, struct BLOTEX_VALUE* na_blotexValue, int *ac_blotregIndexLabel,
  struct G_KEY* ac_gKey, G_STRING_STUFF nc_abandonmentInfo) { 
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

  if (na_blotexValue != NULL) {
    switch (as) {
    case AS__VALUE_INT: // [ '#' ]
      if (na_blotexValue->asValue == AS__VALUE_STR) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
      *ac_gKey = m_GKey_AcolytValue(na_blotexValue->select.c_blotval); 
    break; case AS__ENTRY: // '!#' (r-value) 
      if (na_blotexValue->asValue == AS__VALUE_STR) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
    break; case AS__ID: // '!' 
      *ac_gKey = m_GKey_AcolytValue(na_blotexValue->select.c_blotval); 
    break; case AS__VALUE_STR: // '$'
      if (na_blotexValue->asValue == AS__VALUE_INT) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
      *ac_gKey = m_GKey_PString(na_blotexValue->select.c_strex.v_str);
    break; case AS__NAME:  // '!$'
      if (na_blotexValue->asValue == AS__VALUE_INT) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
      *ac_gKey = m_GKey_PString(na_blotexValue->select.c_strex.v_str);
    break; default: m_RAISE(ANOMALY__VALUE__D,as)
    } // switch
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // AsBlotegIndex 

// See . h
int BlotvarAs2BlotexValue(g_BLOTVAR_STUFF n_blotvarStuff, int n_as, int c_entry, 
  G_STRINGS_HANDLE workingGStringsHandle, struct BLOTEX_VALUE *a_blotexValue) {
  m_DIGGY_BOLLARD()

  int asValue = UNDEFINED;
  gen_BLOTVAL c_blotval = UNDEFINED; 
  const struct P_STRING* cap_str = (const struct P_STRING*)UNDEFINED;
  char cb_fugaciousStr = (char)UNDEFINED;

  switch (n_as) {
  case -1:
    asValue = AS__VALUE_INT;
    c_blotval = BOOLEAN_BLOTVAL(n_blotvarStuff != NULL); 
  break; case AS__VALUE_INT: // <blotvar as int>
    asValue = AS__VALUE_INT;
    c_blotval = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value:
      FALSE__BLOTVAL;
  break; case AS__ID: // <blotvar id>
    asValue = AS__VALUE_INT;
    c_blotval = n_blotvarStuff != NULL? n_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].acolyt.cen_value:
      FALSE__BLOTVAL;
  break; case AS__ENTRY: // <blotvar entry> (r-value) 
    asValue = AS__VALUE_INT;
    c_blotval = n_blotvarStuff != NULL? c_entry: FALSE__BLOTVAL;
  break; case AS__VALUE_STR: // <blotvar strex> 
    asValue = AS__VALUE_STR;
    cap_str = n_blotvarStuff != NULL? &n_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].cv_pString:
      ap_aTrivialEmptyPString;
    cb_fugaciousStr = n_blotvarStuff != NULL; // TODO: really FUGACIOUS????
  break; case AS__NAME:  // <blotvar name> 
    asValue = AS__VALUE_STR,
    cap_str = n_blotvarStuff != NULL? &n_blotvarStuff[g_BLOTVAR_NAME_ELEMENT].cv_pString:
      ap_aTrivialEmptyPString;
    cb_fugaciousStr = n_blotvarStuff != NULL; // TODO: really FUGACIOUS????
  break; default:
    m_RAISE(ANOMALY__VALUE__D,n_as)
  } // switch

  m_TRACK_IF(SetBlotexValue(workingGStringsHandle,a_blotexValue,asValue,c_blotval,
      cap_str,cb_fugaciousStr) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // BlotvarAs2BlotexValue

// See . h
int BlotexValue2BlotvarLValueAs(struct BLOTEX_VALUE blotexValue, g_BLOTVAR_STUFF c_blotvarStuff,
  int lValueAs, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  switch (lValueAs) {
  case AS__VALUE_INT: // [ '#' ]
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    c_blotvarStuff[g_BLOTVAR_VALUE_ELEMENT].acolyt.cen_value = blotexValue.select.c_blotval;
  break; case AS__ID: // '!' 
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case AS__VALUE_STR: // '$'
    if (blotexValue.asValue != AS__VALUE_STR) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
    m_TRACK_IF(GStringCopy(c_blotvarStuff+g_BLOTVAR_VALUE_ELEMENT,0,
      blotexValue.select.c_strex.v_str) < 0)
  break; case AS__NAME:  // '!$'
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; default: m_RAISE(ANOMALY__VALUE__D,lValueAs) } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // BlotexValue2BlotvarLValueAs


// See .h
int ParseCompOp(int compOpExtension, struct P_STRING *a_sequence, int *an_compOp) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED
  int n_matchedEntry = UNDEFINED;
  m_P_PARSE_MATCH_AMONG_C(*a_sequence,NULL,&n_matchedEntry,an_compOp,&lexeme, 9,
    "===", compOpExtension == STR__COMP_OP_EXTENSION? LIKE__STR__COMP_OP: -1,
    "!=",NOT_EQUAL__COMP_OP, ">=",GREATER_EQUAL__COMP_OP, "<=",LESS_EQUAL__COMP_OP,
    "==",EQUAL__COMP_OP, "<",LESS__COMP_OP, ">",GREATER__COMP_OP,
    "*",compOpExtension == FACT_OP__COMP_OP_EXTENSION? MULTIPLY__FACT_OP__COMP_OP: -1,
    "/",compOpExtension == FACT_OP__COMP_OP_EXTENSION? DIVIDE__FACT_OP__COMP_OP: -1)
  m_DIGGY_RETURN(RETURNED)
} // ParseCompOp

// See .h
int ApplyFactOp(gen_BLOTVAL *a_blotval1, int factOp, gen_BLOTVAL blotval2) {
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
} // ApplyFactOp

// See .h
int ParseRequestCompOp(struct P_STRING *a_sequence, char b_str,
  int *an_indexSeekFlags) {
  m_DIGGY_BOLLARD()
  *an_indexSeekFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);
m_DIGGY_VAR_P_STRING(*a_sequence)

  int n_compOp = UNDEFINED;
  m_TRACK_IF(ParseCompOp(b_str? STR__COMP_OP_EXTENSION:
    NO__COMP_OP_EXTENSION, a_sequence, &n_compOp) != RETURNED)

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
  m_PParsePassSpaces(a_sequence,NULL);
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
  PParsePassSingleChar(a_sequence,IsInt1Op,(char)UNDEFINED,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) { // <int 1op>
    switch (lexeme.string[0]) {
    case '!' : *an_int1Op = NOT__INT_1OP;
    break; case '+' : *an_int1Op = PLUS__INT_1OP;
    break; case '-' : *an_int1Op = MINUS__INT_1OP;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,lexeme.string[0])
    } // switch
    m_PParsePassSpaces(a_sequence,NULL);
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ParseInt1Op

// See .h
int ApplyInt1Op(int n_int1Op,struct BLOTEX_VALUE *ac_blotexValue,G_STRING_STUFF nc_abandonmentInfo){
  m_DIGGY_BOLLARD()
  if (n_int1Op >= 0) {
    if (ac_blotexValue->asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
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
} // ApplyInt1Op 

// See .h
int ParseIntConstant(struct P_STRING *a_sequence, char *ab_parsed,
  struct BLOTEX_VALUE *ac_blotexValue) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  gen_BLOTVAL c_blotval = UNDEFINED;
  PParseGenericInteger(a_sequence,&c_blotval,&lexeme);
  if ((*ab_parsed = !b_EMPTY_P_STRING(lexeme))) m_TRACK_IF(SetBlotexValue(
    (G_STRINGS_HANDLE)UNDEFINED,ac_blotexValue,AS__VALUE_INT, c_blotval, (const struct P_STRING*) UNDEFINED,
    (char)UNDEFINED) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // ParseIntConstant

// See .h
int ParseStrConstant(struct P_STRING *a_sequence, G_STRINGS_HANDLE workingGStringsHandle,
  char *acb_parsed, struct BLOTEX_VALUE *acc_blotexValue, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  m_PREPARE_ABANDON(a_sequence, "<str constant>")
  PParsePassSingleChar(a_sequence,NULL,'"',&lexeme);
  if ((*acb_parsed = !b_EMPTY_P_STRING(lexeme))) { // <str constant> ... 
    PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_TILL,NULL,'"', &lexeme);
    if (b_EMPTY_P_STRING(*a_sequence)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    PParseOffset(a_sequence,1,NULL);
    m_TRACK_IF(SetBlotexValue(workingGStringsHandle,acc_blotexValue,AS__VALUE_STR,UNDEFINED,&lexeme,
      !b_FUGACIOUS_STR) != RETURNED) // TODO: FUGACIOUS???
  } // if 
  m_DIGGY_RETURN(ANSWER__YES)
} // ParseStrConstant

// See .h
int ProbeBlotexAtom(struct P_STRING *a_sequence,G_STRINGS_HANDLE workingGStringsHandle,
  int *acn_int1Op, int *ac_probedBlotexAtom, struct BLOTEX_VALUE *acc_blotexAtomValue,
  G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()

  //m_PREPARE_ABANDON(a_sequence, "<intex atom> | <strex atom>")
  m_CHECK_ABANDON(ParseExistingSequence(a_sequence,"<intex atom> | <strex atom>",
    nc_abandonmentInfo))

  m_TRACK_IF(ParseInt1Op(a_sequence, acn_int1Op) != RETURNED)

  char b_parsed = (char) UNDEFINED; 
  m_TRACK_IF(ParseIntConstant(a_sequence, &b_parsed, acc_blotexAtomValue) != RETURNED)
  *ac_probedBlotexAtom = CONSTANT__PROBED_BLOTEX_ATOM; // a priori
  if (!b_parsed) {
    m_CHECK_ABANDON(ParseStrConstant(a_sequence,workingGStringsHandle, &b_parsed,
      acc_blotexAtomValue,nc_abandonmentInfo))
    if (!b_parsed) {
      struct P_STRING lexeme = UNDEFINED_P_STRING;
      PParsePassSingleChar(a_sequence,NULL,'(',&lexeme);
      if (!b_EMPTY_P_STRING(lexeme)) *ac_probedBlotexAtom = BLOTEX__PROBED_BLOTEX_ATOM;
      else *ac_probedBlotexAtom = OTHER__PROBED_BLOTEX_ATOM; 
    } // if 
  } // if 
  m_DIGGY_RETURN(ANSWER__YES) 
} // ProbeBlotexAtom

// See .h
int ParseBlotexAtomBlotexEnd(struct P_STRING *a_sequence,G_STRING_STUFF nc_abandonmentInfo){
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  m_PREPARE_ABANDON(a_sequence, "<intex atom> | <strex atom>")
  PParsePassSingleChar(a_sequence,NULL,')',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  m_DIGGY_RETURN(ANSWER__YES) 
} // ParseBlotexAtomBlotexEnd


// See .h
char b_ParseOpsIndicator(struct P_STRING *a_sequence) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme = UNDEFINED_P_STRING;
  PParsePassSingleChar(a_sequence,NULL,'?',&lexeme);
  m_DIGGY_RETURN(!b_EMPTY_P_STRING(lexeme))
} // b_ParseOpsIndicator


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
int ApplyTermOp(gen_BLOTVAL *a_blotval1, int termOp, gen_BLOTVAL blotval2) {
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
} // ApplyTermOp

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
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassSingleChar(a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsFormatSpecifierChar,(char)UNDEFINED,
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
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassSingleChar(a_sequence,NULL,',',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  m_PParsePassSpaces(a_sequence,NULL);

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseFormatAndSeparator

// See .h
int ApplyFormat(int format, struct BLOTEX_VALUE blotexValue, G_STRING_STUFF c_surrogate,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()

  switch (format) {
  case D__FORMAT:
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%d", blotexValue.select.c_blotval);
  break; case LS__FORMAT:
    if (blotexValue.asValue != AS__VALUE_STR) m_ABANDON_S(EXPECT_STREX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,FMT_P_STRING,m_P_STRING_2_FMT_ARGS(
      blotexValue.select.c_strex.v_str));
  break; case LX__FORMAT:
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%x",blotexValue.select.c_blotval);
  break; case UX__FORMAT:
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
    GStringPrintf(c_surrogate,0,"%X", blotexValue.select.c_blotval);
  break; case LE__FORMAT:
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; case BE__FORMAT:
    if (blotexValue.asValue != AS__VALUE_INT) m_ABANDON_S(EXPECT_INTEX__ABANDONMENT_CAUSE)
m_RAISE(ANOMALY__NOT_AVAILABLE)
  break; default:
    m_RAISE(ANOMALY__VALUE__D,format)
  } // switch

  m_DIGGY_RETURN(ANSWER__YES)
} // ApplyFormat

int ParseBlottabsLabel(struct P_STRING *a_sequence, int *an_blottabsLabel) {
  m_DIGGY_BOLLARD()
  *an_blottabsLabel = -1; // a priori
  struct P_STRING lexeme; // UNDEFINED
  while (b_TRUE) { 
    PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) break;
    (*an_blottabsLabel)++; 
  } // while 
  m_DIGGY_RETURN(RETURNED)
} // ParseBlottabsLabel


// See .h
int ParseAndComputeSimpleBlotvarReference(struct P_STRING *a_sequence, 
  g_BLOTREG_HANDLE blotregHandle, struct BLOTVAR_REFERENCE *ac_blotvarReference,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_P(blotregHandle)
  ac_blotvarReference->blotregHandle = blotregHandle;
  struct P_STRING lexeme = UNDEFINED_P_STRING;

  m_PREPARE_ABANDON(a_sequence,"<blotvar>")

  m_PParsePassSpaces(a_sequence,NULL);

  // Retrieve blotvar reference:
  PParseOffset(a_sequence,1,&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  GENERIC_INTEGER genericInteger = UNDEFINED;
  switch (lexeme.string[0]) {
  case '.' : // '.' <entity> : 
    ac_blotvarReference->blotvarReference = NAME__BLOTVAR_REFERENCE;
    PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsEntityNameChar,
      (char)UNDEFINED,&lexeme);
    ac_blotvarReference->c_select.c_name = lexeme;
  break; case '[' : // '[' <intex> ']' :
    ac_blotvarReference->blotvarReference = ENTRY__BLOTVAR_REFERENCE;
    PParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    // TODO: support empty or -1 entry when l-value (for smart fetch) 
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_entry = genericInteger;
    PParsePassSingleChar(a_sequence,NULL,']',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; case '{' : // '{' <intex> '}' : 
    ac_blotvarReference->blotvarReference = TOKEN_ID__BLOTVAR_REFERENCE;
    PParseGenericInteger(a_sequence,&genericInteger,&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    else if (genericInteger > INT_MAX || genericInteger < 0) m_ABANDON(
      VALUE_ERROR__ABANDONMENT_CAUSE)
    else ac_blotvarReference->c_select.c_tokenId = genericInteger;
    PParsePassSingleChar(a_sequence,NULL,'}',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  break; default:
    m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE) 
  } // switch

  m_DIGGY_RETURN(ANSWER__YES) ;
} // ParseAndComputeSimpleBlotvarReference 


// See .h
int ParseAndComputeLValueBlotregOps( 
  struct P_STRING *a_sequence, struct P_STRING blotregName, g_BLOTREG_HANDLE n_blotregHandle, 
  struct BLOTVAR_REFERENCE *ac_blotvarReference, int *ac_lValueAs, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<blotreg ref op set int> | <blotreg ref op set str>")

  if (n_blotregHandle == NULL) m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE)
  ac_blotvarReference->blotregHandle = n_blotregHandle; 
  ac_blotvarReference->blotvarReference = CURRENT__BLOTVAR_REFERENCE;
  *ac_lValueAs = UNDEFINED; // For the moment 

  m_PParsePassSpaces(a_sequence,NULL);

  int n_lValueAs = -1; // No blotreg read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg ref op set int> | <blotreg ref op set str>... 
    m_TRACK_IF(ParseAs(b_L_VALUE,a_sequence,&n_lValueAs) != RETURNED)
  } // if
  
  if (n_lValueAs == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  *ac_lValueAs = n_lValueAs ;

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseAndComputeLValueBlotregOps

// See .h
char b_ParseBlotexAssignation(struct P_STRING *a_sequence, struct P_STRING *ac_subSequence) {
  m_DIGGY_BOLLARD()
  PParseTillMatch(a_sequence,o_PString(":="),NULL,ac_subSequence); // TODO: improve; i.e: "joker" sequences???...
  char b_assignation = !b_EMPTY_P_STRING(*a_sequence);
  if (b_assignation) { 
    PParseOffset(a_sequence,2,NULL);
  } else { // NO assignation
    *a_sequence = *ac_subSequence; 
  } // if
  m_PParsePassSpaces(a_sequence,NULL); // TODO: ag virer ???
  
  m_DIGGY_RETURN(b_assignation)
} // b_ParseBlotexAssignation


