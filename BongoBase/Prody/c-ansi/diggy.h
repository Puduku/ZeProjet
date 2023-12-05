// c-ansi/diggy.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: integration of "base" diggy directives in production code... 
// =======

#ifndef __C_ANSI_DIGGY_H_INCLUDED__
#define __C_ANSI_DIGGY_H_INCLUDED__

// Diggy code: placeholders

// Principle: prepare the ground for "diggy" code...
// For each PUBLIC function:
// - place m_DIGGY_BOLLARD() macro at begin of function body.
// - replace any return statement with m_DIGGY_RETURN()
// For each PRIVATE (i.e static) function:
// - place m_DIGGY_BOLLARD_S() macro at begin of function body.
// - replace any return statement with m_DIGGY_RETURN()
// => These manips must NOT alter the "production" code.

// Add -DDIGGY compilation directive to enable diggy code...

#ifdef DIGGY
// When -DDIGGY compilation directive is present:

#include "diggy/diggy.h"

#else
// When -DDIGGY compilation directive is absent:

// m_DIGGY_BOLLARD() macro is just a place holder
#define m_DIGGY_BOLLARD() ;

// m_DIGGY_BOLLARD_S() macro is just a place holder
#define m_DIGGY_BOLLARD_S() ;


// m_DIGGY_RETURN() macro just returns value
//
// Passed:
// u_ret: value to return
#define m_DIGGY_RETURN(u_ret) return u_ret;

#endif

#endif // __C_ANSI_DIGGY_H_INCLUDED__

