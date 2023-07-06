// testy/c-ansi/diggy-vars.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED
#define __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED

#include "diggy/diggy.h"
#include "diggy/vars.h"
#include "diggy/vars-fields.h"

#include "c-ansi/types.h"
#include "c-ansi/c-string.h"
#include "c-ansi/blotcode.h"


// "c-ansi/types.h" :

// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h
// "answer" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_ANSWER(m_var) \
  m_DIGGY_VAR_VALUE(m_var,AnswerImage)


// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h
// "comparison" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_COMPARISON(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ComparisonImage)


// "c-ansi/c-string.h" :

// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h
// "String portion" var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_STRING_PORTION(m_var) \
  m_DIGGY_VAR(m_var,\
  printf("[" FMT_STRING_PORTION "]", m_STRING_PORTION_2_FMT_ARGS(m_var));)


// "c-ansi/blotcode.h" :

// #SEE DIGGY_VAR_XXX_MACRO @ diggy/vars.h
// "blotinst" struct var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_BLOTINST(m_var) \
  m_DIGGY_VAR(m_var,\
    m_DIGGY_PRINT_FIELD_VALUE((m_var),blotkeywId,BlotkeywIdImage);\
    m_DIGGY_PRINT_FIELD_D((m_var),b_blotfunc);\
    m_DIGGY_PRINT_FIELD_D((m_var),b_blotval);\
    if ((m_var).b_blotval) m_DIGGY_PRINT_FIELD_D((m_var),c_blotval);\
    m_DIGGY_PRINT_FIELD_D((m_var),n_subEntry);\
    m_DIGGY_PRINT_FIELD_D((m_var),b_masterBlotval);\
    if ((m_var).b_masterBlotval) m_DIGGY_PRINT_FIELD_D((m_var),c_masterBlotval);\
  )

#endif // __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED 
