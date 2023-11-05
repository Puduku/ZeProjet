// c-ansi/testy-diggy.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: include that header when you really want to dig into a module 
// =======

#ifndef __C_ANSI_TESTY_DIGGY_H_INCLUDED__
#define __C_ANSI_TESTY_DIGGY_H_INCLUDED__


#ifdef DIGGY
// When -DDIGGY compilation directive is present:

// Enable DIGGY tracking of static functions:
#ifdef b_DIGGY_BOLLARD_S_ENABLED
#undef b_DIGGY_BOLLARD_S_ENABLED
#endif
#define b_DIGGY_BOLLARD_S_ENABLED 1

// Actual m_DIGGY_*VAR() macros :
#include "diggy/vars.h"
#include "testy/c-ansi-diggy-vars.h"

#else
// When -DDIGGY compilation directive is absent:
// m_DIGGY_<XXX>() placeholders - just do NOTHING...

#define m_DIGGY_INFO(p_info, ...)

// m_DIGGY_VAR_<XXX>() placeholders:

#define m_DIGGY_VAR(m_var, m_printImageAction)

#define m_DIGGY_VAR_GEN(m_var,m_specifier)
#define m_DIGGY_VAR_D(m_var)
#define m_DIGGY_VAR_P(m_var)

#define m_DIGGY_VAR_STRING(m_var)

#define m_DIGGY_VAR_DATA(m_var,length)

#define m_DIGGY_VAR_VALUE(m_var, m_valueImageInlineFunction)

// m_DIGGY_VAR_<XXX>() placeholders from Testy:

#define m_DIGGY_VAR_ANSWER(m_var)

#define m_DIGGY_VAR_COMPARISON(m_var)

#define m_DIGGY_VAR_STRING_PORTION(m_var)

#endif


#endif // __C_ANSI_TESTY_DIGGY_H_INCLUDED__

