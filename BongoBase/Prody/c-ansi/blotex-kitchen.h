// blotex-kitchen.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2023

#ifndef __C_ANSI_BLOTEX_KITCHEN_H_INCLUDED__
#define __C_ANSI_BLOTEX_KITCHEN_H_INCLUDED__

/////////////////////////////
#include "c-ansi/blotex.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/g-string.h"


// Parsing blot expressions: helpers
// ---------------------------------

// Prepare parsing function for potential abandonment.   
//
// Passed:
// - a_sequence: parsed sequence address
// - p_sequenceType: expected sequence type 
#define m_PREPARE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  struct P_STRING *ema_sequence = (a_sequence);\
  const char *emp_sequenceType = (p_sequenceType);\
  
// Precise sequence description of parsing function.
//
// Passed:
// - a_sequence: parsed sequence address 
// - p_sequenceType: expected sequence type 
#define m_PRECISE_ABANDON(/*struct P_STRING* */a_sequence,\
  /*const char* */p_sequenceType) \
  ema_sequence = (a_sequence);\
  emp_sequenceType = (p_sequenceType);\


#define SYNTAX_ERROR__ABANDONMENT_CAUSE "Syntax error"
#define NOT_PARSABLE__ABANDONMENT_CAUSE "Not parsable"
#define VALUE_ERROR__ABANDONMENT_CAUSE "Value error"
#define EXPECT_STREX__ABANDONMENT_CAUSE "Expect strex value"
#define EXPECT_INTEX__ABANDONMENT_CAUSE "Expect strex value"
#define UNKNOWN_BLOTVAR__ABANDONMENT_CAUSE "Unknown blotvar"
#define UNKNOWN_BLOTREG__ABANDONMENT_CAUSE "Unknown blotreg"
#define INVALID_FORMAT__ABANDONMENT_CAUSE "Invalid format"
#define NOT_EXISTING_L_VALUE__ABANDONMENT_CAUSE "Not existing l-value"
#define NOT_SUPPORTED__ABANDONMENT_CAUSE "Not supported"

// Make function abandon parsing (return ANSWER__NO)
//
// Passed:
// - p_cause : parsing abandonment cause format 
// - ... : parsing abandonment cause:
//  + mandatory const char* : parsing abandonment cause format
//  + cause format's optional variable arguments
// - ema_sequence : implicit variable 
// - emp_sequenceType: implicit variable
//
// Changed (Implicit) variables:
// - nc_abandonmentInfo:
#define m_ABANDON(...) {\
  if (nc_abandonmentInfo != NULL) {\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,0,"In %s [" FMT_P_STRING "] : ",\
      emp_sequenceType, m_P_STRING_2_FMT_ARGS((*ema_sequence))) < 0)\
    m_TRACK_IF(GStringPrintf(nc_abandonmentInfo,-1, __VA_ARGS__) < 0)\
  } \
  m_DIGGY_RETURN(ANSWER__NO)\
}
 
// IS_CHAR_FUNCTION:
// Recognize any character corresponding to <entity>
int IsEntityNameChar(int c) ;

#define b_L_VALUE b_TRUE
#define b_R_VALUE b_FALSE0 

enum {
  AS__ENTRY,    // '!#' (ONLY R-value)
  AS__NAME,     // '!$'
  AS__VALUE_INT,// [ '#' ]
  AS__ID,       // '!'
  AS__VALUE_STR,// '$'
} ;

static inline int mb_AsValue(int as) {
  return as == AS__VALUE_INT || as == AS__VALUE_STR;
} // mb_AsValue


// Parse "as" specifier (not present : default to '#" value int) 
//
// Passed:
// - *a_sequence: before parsing
// - b_lValue: TRUE => "as entry" not accepted 
//
// Changed:
// - *a_sequence: after parsing 
// - *a_as: (>=0) corresponding "as" specifier 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseAs(char b_lValue, struct P_STRING *a_sequence, int *a_as) ;

// Parse "as" "value" specifier if present
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_asValue: (>=0) corresponding "as" "value" specifier (-1 if not present) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseAsValue(struct P_STRING *a_sequence, int *an_asValue) ;


// Enumeration of <comp op> | <str comp op> | <fact op> terminal symbols 
enum {        
             EQUAL__COMP_OP,
              LESS__COMP_OP,
         LESS_EQUAL__COMP_OP,
            GREATER__COMP_OP,
      GREATER_EQUAL__COMP_OP,
          NOT_EQUAL__COMP_OP, 
         LIKE__STR__COMP_OP, // only <str comp op>
 MULTIPLY__FACT_OP__COMP_OP, // only <fact op>
   DIVIDE__FACT_OP__COMP_OP, // only <fact op>
} ;

enum {
     NO__COMP_OP_EXTENSION,
     STR__COMP_OP_EXTENSION,
 FACT_OP__COMP_OP_EXTENSION,
} ;

// Parse comparison operator, if present...
//
// Passed:
// - compOpExtension: + NO__COMP_OP_EXTENSION: parse <comp op>
//   + STR__COMP_OP_EXTENSION: parse <str comp op>
//   + FACT_OP__COMP_OP_EXTENSION: parse <fact op>
// - *a_sequence: expected (eventually) : <comp op> | <str comp op> | <fact op>  
//
// Changed:
// - *a_sequence: after parsing 
// - *an_compOp: + -1: special value: comparison operator NOT present
//   + >=0 : corresponding comparison operator 
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem
int ParseCompOp(int compOpExtension, struct P_STRING *a_sequence, int *an_compOp) ;

// Parse comparison operator in register/table request, if present... 
//
// Passed:
// - *a_sequence: expect <str comp op> | <comp op> 
// - b_str : TRUE => expect <str comp op> ; FALSE => expect <comp op>
//
// Changed:
// - *a_sequence: after parsing 
// - *an_indexSeekFlags: + -1 special value: comparison operator not present
//   + >= 0: actual seek flags corresponding to comparison operator ; may be NOT 
//     INDEX_SEEK_FLAG__ANY
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseRequestCompOp(struct P_STRING *a_sequence, char b_str, int *an_indexSeekFlags);

// Parse <logical 2op>  
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *a_criteriaFlags: corresponding op. flags (ALL_FLAG_OFF0 special value if not found) 
// 
// Ret:
// - RETURNED: Ok
// - 1: unexpected problem; anomaly is raised
int ParseLogical2Op(struct P_STRING *a_sequence, int *a_criteriaOpFlags);


// IS_CHAR_FUNCTION:
int IsInt1Op(int c) ;

// Terminal symbols (of <int 1op> terminal symbol)
enum {
  NOT__INT_1OP,
  PLUS__INT_1OP,
  MINUS__INT_1OP,
} ;

// Terminal symbols (of <int 2op> terminal symbol)
enum {
            ADD__TERM_OP,
       SUBTRACT__TERM_OP,
            AND__TERM_OP,
             OR__TERM_OP,
} ;

// Parse <term op> if present
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_termOp:
//   + -1: special value: <int 2op> NOT present
//   + >=0 : corresponding int 2op 
int ParseTermOp(struct P_STRING *a_sequence, int *an_termOp);



// Terminal symbols (of <format> terminal symbol)
enum {
             D__FORMAT,
             LS__FORMAT,
             LX__FORMAT,
            UX__FORMAT,
            LE__FORMAT,
            BE__FORMAT,
} ;

// Parse <format> 
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *avn_format:
//   + -1 special value: when abandonned (invalid format) 
//   + >=0 : corresponding format 
// 
// Ret:
// - ANSWER__YES: Ok,
// - ANSWER__NO: 'invalid format' error; abandon processing 
int ParseFormat(struct P_STRING *a_sequence, int *avn_format,
  G_STRING_STUFF nc_abandonmentInfo) ;



// blottabs:
// ---------

// Parse blottabs label  
//
// Passed:
// - *a_sequence: before parsing
//
// Changed:
// - *a_sequence: after parsing 
// - *an_blottabsLabel: -1 special value => NO blottabs label
//   + 0 (GENUINE_BLOTTABS_LABEL0) : parsed "^"
//   + 1 ("super" blottabs label) : parsed "^^" 
//   + 2 ("hyper" blottabs label) : parsed "^^^" 
//
// Ret: RETURNED (Ok)
int ParseBlottabsLabel(struct P_STRING *a_sequence, int *an_blottabsLabel);

#endif // __C_ANSI_BLOTEX_KITCHEN_H_INCLUDED__
