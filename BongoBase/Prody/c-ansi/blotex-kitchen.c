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

// blotex library executor 
// =======================


// Blotregs: 



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

//
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

int ParseBlottabLabel(struct P_STRING *a_sequence, int *an_blottabLabel) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(*a_sequence)
  *an_blottabLabel = -1; // a priori
  struct P_STRING lexeme; // UNDEFINED
  while (b_TRUE) { 
    PParsePassSingleChar(a_sequence,NULL,'^',&lexeme);
    if (b_EMPTY_P_STRING(lexeme)) break;
    (*an_blottabLabel)++; 
  } // while 
m_DIGGY_VAR_P_STRING(*a_sequence)
m_DIGGY_VAR_D(*an_blottabLabel)
  m_DIGGY_RETURN(RETURNED)
} // ParseBlottabLabel
