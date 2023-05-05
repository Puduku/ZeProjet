// c-ansi/stack.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_C_STACK_H_INCLUDED
#define __C_ANSI_C_STACK_H_INCLUDED

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"


// "C" STACK 
// ---------

struct C_STACK {
  int *hsc_stack ; // - Physical size given by itemsPhysicalNumber
                   // - 'count' first items are significant
  int physicalNumber;
  int count ;
} ;

// Must be at least 2
#define C_STACK_BASE_PHYSICAL_NUMBER 20


// Passed:
// - mz_stack:
#define m_C_STACK_INIT(mz_stack) {\
  m_MALLOC_ARRAY((mz_stack).hsc_stack,\
    ((mz_stack).physicalNumber = C_STACK_BASE_PHYSICAL_NUMBER))\
  (mz_stack).count = 0;\
}

// Passed:
// - m_stack:
// - m_poppedEntry:
#define m_C_STACK_POP(m_stack,m_poppedEntry) {\
  m_ASSERT((m_stack).count >= 1)\
  m_poppedEntry = (m_stack).hsc_stack[--((m_stack).count)];\
}

// Passed:
// - m_stack:
//
// Ret: >= 0: number of items in stack (always even number)
#define C_STACK_GET_COUNT(m_stack) ((m_stack).count)

// Passed:
// - m_stack:
// - pushedEntry:
#define m_C_STACK_PUSH(m_stack, pushedEntry) {\
  if ((m_stack).count + 1 > (m_stack).physicalNumber) {\
    (m_stack).physicalNumber += C_STACK_BASE_PHYSICAL_NUMBER;\
    m_REALLOC_ARRAY((m_stack).hsc_stack, (m_stack).physicalNumber)\
  }\
  (m_stack).hsc_stack[((m_stack).count)++] = pushedEntry;\
}

// Passed:
// - m_stack:
#define m_C_STACK_CLEAR(m_stack) \
m_stack.count = 0;

// Passed:
// - ax_index:
#define m_C_STACK_FREE(mx_stack) \
free((mx_stack).hsc_stack);

#endif // __C_ANSI_C_STACK_H_INCLUDED
