// c-ansi/testy-diggy.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2024
//
// Purpose: include **TEMPORARILLY** that header when you really want to dig into a module... 
// =======

#ifndef __C_ANSI_TESTY_DIGGY_H_INCLUDED__
#define __C_ANSI_TESTY_DIGGY_H_INCLUDED__


#ifdef DIGGY
// When -DDIGGY compilation directive is present:
// ----------------------------------------------

// Enable DIGGY tracking of static functions:
#ifdef b_DIGGY_BOLLARD_S_ENABLED
#undef b_DIGGY_BOLLARD_S_ENABLED
#endif
#define b_DIGGY_BOLLARD_S_ENABLED 1

// Base m_DIGGY_*VAR() macros :
#include "diggy/vars.h"

// Extra m_DIGGY_*VAR() macros

// Need #include "c-ansi/types.h":
#define m_DIGGY_VAR_ANSWER(m_var) \
  m_DIGGY_VAR_VALUE(m_var,AnswerImage)

#define m_DIGGY_VAR_RESULT(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ResultImage)

#define m_DIGGY_VAR_COMPARISON(m_var) \
  m_DIGGY_VAR_VALUE(m_var,ComparisonImage)

// Need #include "c-ansi/p-string.h":
#define m_DIGGY_VAR_P_STRING(m_var) \
  m_DIGGY_VAR(m_var,\
  printf("[" FMT_P_STRING "]", m_P_STRING_2_FMT_ARGS(m_var));)

// Need #include "c-ansi/green-index.h":
#define m_DIGGY_VAR_INDEX_SEEK_FLAGS(m_var) \
  m_DIGGY_VAR_VALUE(m_var,m_IndexSeekFlagsImage)

#define m_DIGGY_VAR_INDEX_FETCH_FLAGS(m_var) \
  m_DIGGY_VAR_VALUE(m_var,m_IndexFetchFlagsImage)

// Need #include "c-ansi/blotcode.h":
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

#else // DIGGY
// When -DDIGGY compilation directive is absent:
// ---------------------------------------------
// RULE: ALL m_DIGGY_*() macros are placeholders - just do NOTHING...

#define m_DIGGY_INFO(p_info, ...)

// Base m_DIGGY_*VAR() placeholders for "diggy/vars.h" macros :

#define m_DIGGY_VAR(m_var, m_printImageAction) ;
#define m_DIGGY_VAR_GEN(m_var,m_specifier) ;
#define m_DIGGY_VAR_D(m_var) ;
#define m_DIGGY_VAR_P(m_var) ;
#define m_DIGGY_VAR_STRING(m_var) ;
#define m_DIGGY_VAR_DATA(m_var,length) ;
#define m_DIGGY_VAR_VALUE(m_var, m_valueImageInlineFunction) ;

// Extra m_DIGGY_*VAR() macros placeholders

#define m_DIGGY_VAR_ANSWER(m_var) ;
#define m_DIGGY_VAR_RESULT(m_var) ;
#define m_DIGGY_VAR_COMPARISON(m_var) ;
#define m_DIGGY_VAR_P_STRING(m_var) ;
#define m_DIGGY_VAR_INDEX_SEEK_FLAGS(m_var) ;
#define m_DIGGY_VAR_INDEX_FETCH_FLAGS(m_var) ;
#define m_DIGGY_VAR_BLOTINST(m_var) ;

#endif // DIGGY


#endif // __C_ANSI_TESTY_DIGGY_H_INCLUDED__

