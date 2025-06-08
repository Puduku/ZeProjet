// testy/c-ansi-diggy-vars.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED
#define __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED

#include "diggy/diggy.h"
#include "diggy/vars.h"
#include "diggy/vars-fields.h"

#include "c-ansi/types.h"
#include "c-ansi/p-string.h"
#include "c-ansi/green.h"
#include "c-ansi/blotcode.h"


// "c-ansi/types.h" :

// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h

// "answer" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_ANSWER(m_var) \
  m_DIGGY_VAR_VALUE(m_var,AnswerImage)


// "result" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_RESULT(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ResultImage)


// "comparison" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_COMPARISON(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ComparisonImage)


// "c-ansi/p-string.h" :

// "String portion" var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_P_STRING(m_var) \
  m_DIGGY_VAR(m_var,\
  printf("[" FMT_P_STRING "]", m_P_STRING_2_FMT_ARGS(m_var));)


// "c-ansi/green.h" :

// "index seek flags" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_INDEX_SEEK_FLAGS(m_var) \
  m_DIGGY_VAR_VALUE(m_var,m_IndexSeekFlagsImage)

// "index fetch flags" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_INDEX_FETCH_FLAGS(m_var) \
  m_DIGGY_VAR_VALUE(m_var,m_IndexFetchFlagsImage)


// "c-ansi/blotcode.h" :

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
