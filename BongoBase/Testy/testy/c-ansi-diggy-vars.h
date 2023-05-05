// testy/c-ansi/diggy-vars.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED
#define __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED


#include "diggy/diggy.h"
#include "diggy/vars.h"
#include "c-ansi/c-string.h"


// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h
// "String portion" var display
//
// Passed
// - m_var: common var expression to display
// - nm_entry: explicit entry for array (useful when var expression contains a variable array
//   entry)
//   + -1 special value: not used
//   + >= 0: explicit array entry present in the var expression
#define m_DIGGY_VAR_STRING_PORTION(m_var) \
  m_DIGGY_VAR(m_var,\
  printf("[" FMT_STRING_PORTION "]", m_STRING_PORTION_2_FMT_ARGS(m_var));)


// #SEE DIGGY_VAR_XXX_MACRO@diggy/vars.h
// "comparison" (enum) var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_COMPARISON(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ComparisonImage)


#endif // __TESTY_C_ANSI_DIGGY_VARS_H_INCLUDED 
