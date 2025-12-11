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

// Blot expressions parsing:

// IS_CHAR_FUNCTION:
int IsEntityNameChar(int c) {
  return (c == '_' || isalnum(c));
} // IsEntityNameChar 


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
  PParseMatch(a_sequence,m_PString("and"),NULL,&lexeme);
  if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__AND;
  else { 
    PParseMatch(a_sequence,m_PString("or"),NULL,&lexeme); 
    if (!b_EMPTY_P_STRING(lexeme)) *a_criteriaOpFlags = CRITERIA_OP_FLAGS__OR;
    else *a_criteriaOpFlags = ALL_FLAGS_OFF0;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ParseLogical2Op


// See .h
// IS_CHAR_FUNCTION:
int IsInt1Op(int c) {
  return (c == '+' || c == '-' || c == '!');
} // IsInt1Op


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


// IS_CHAR_FUNCTION:
static int IsFormatSpecifierChar(int c) {
  return (c == 'd' || c == 's' || c == 'x' || c == 'X' || c == 'L' || c == 'B' || c == 'E');
} // IsFormatSpecifierChar


// See .h
int ParseFormat(struct P_STRING *a_sequence, int *avn_format,
  G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme; // UNDEFINED

  *avn_format = -1;
  m_PREPARE_ABANDON(a_sequence, "<format>") 
  m_PParsePassSpaces(a_sequence,NULL);
  PParsePassSingleChar(a_sequence,NULL,'%',&lexeme);
  if (b_EMPTY_P_STRING(lexeme)) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
  PParsePassChars(a_sequence,b_REGULAR_SCAN,b_PASS_CHARS_WHILE,IsFormatSpecifierChar,(char)UNDEFINED,
    &lexeme);
  int length = m_PStringLength(lexeme);
  switch (lexeme.string[0]) {
  case 'd': 
    if (length == 1) *avn_format = D__FORMAT;
  break; case 's':
    if (length == 1) *avn_format = LS__FORMAT;
  break; case 'x':
    if (length == 1) *avn_format = LX__FORMAT;
  break; case 'X':
    if (length == 1) *avn_format = UX__FORMAT;
  break; case 'L':
    if (length == 2 && lexeme.string[0] == 'E') *avn_format = LE__FORMAT;
  break; case 'B':
    if (length == 2 && lexeme.string[0] == 'E') *avn_format = BE__FORMAT;
  } // switch

  if (*avn_format < 0) m_ABANDON(INVALID_FORMAT__ABANDONMENT_CAUSE) 

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseFormat

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
  struct BLOTVAR_REFERENCE *ac_blotvarReference, int *ac_as, G_STRING_STUFF nc_abandonmentInfo) {
  m_DIGGY_BOLLARD()
  struct P_STRING lexeme;

  m_PREPARE_ABANDON(a_sequence, "<blotreg ref op set int> | <blotreg ref op set str>")

  if (n_blotregHandle == NULL) m_ABANDON(UNKNOWN_BLOTREG__ABANDONMENT_CAUSE)
  ac_blotvarReference->blotregHandle = n_blotregHandle; 
  ac_blotvarReference->blotvarReference = CURRENT__BLOTVAR_REFERENCE;
  *ac_as = UNDEFINED; // For the moment 

  int n_indexFetchFlags = -1; // a priori
  m_PParsePassSpaces(a_sequence,NULL);

  int n_as = -1; // No blotreg read/set op a priori 
  PParsePassSingleChar(a_sequence,NULL,'=',&lexeme); 
  if (!b_EMPTY_P_STRING(lexeme)) { // <blotreg ref op set int> | <blotreg ref op set str>... 
    m_TRACK_IF(ParseAs(b_L_VALUE,a_sequence,&n_as) != RETURNED)
    if (n_indexFetchFlags < 0) n_indexFetchFlags = ALL_FLAGS_OFF0; 
  } // if
  
    if (n_as == -1) m_ABANDON(SYNTAX_ERROR__ABANDONMENT_CAUSE)
    *ac_as = n_as ;

  m_DIGGY_RETURN(ANSWER__YES)
} // ParseAndComputeLValueBlotregOps


// See .h
int FetchBlotvar(const struct BLOTVAR_REFERENCE *ap_blotvarReference, char cb_lValue,
  g_G_PARAM_STUFF *ant_blotvarStuff, int *navn_entry) {
  m_DIGGY_BOLLARD()

  int ret = UNDEFINED;
  *ant_blotvarStuff = (g_G_PARAM_STUFF)UNDEFINED;
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
