// c-ansi/stack.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_C_STACK_H_INCLUDED
#define __C_ANSI_C_STACK_H_INCLUDED

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"


// "C" STACK 
// ---------

#define m_STRUCT_C_STACK(m_NAME, m_itemType) \
struct m_NAME {\
  int physicalCount;\
  m_itemType *hsc_stack ;\
  int count ;\
} ;

#define BATEAU__C_STACK_BASE_PHYSICAL_COUNT 20


// Passed:
// - mz_me:
// - m_basePhysicalCount: if no idea, specify BATEAU__C_STACK_BASE_PHYSICAL_COUNT
#define m_C_STACK_INIT(mz_me, /*int*/u_basePhysicalCount) {\
  m_MALLOC_ARRAY((mz_me).hsc_stack,\
    (mz_me).physicalCount = (u_basePhysicalCount))\
  (mz_me).count = 0;\
}

// Passed:
// - m_me:
//
// changed:
// - mu_pop:
#define m_C_STACK_POP(m_me,mu_pop) {\
  m_ASSERT((m_me).count >= 1)\
  (mu_pop) = (m_me).hsc_stack[--((m_me).count)];\
}

// Passed:
// - m_me:
//
// changed:
// - mu_peek:
#define m_C_STACK_PEEK(m_me,mu_peek) {\
  m_ASSERT((m_me).count >= 1)\
  (mu_peek) = (m_me).hsc_stack[(m_me).count-1];\
}

// Passed:
// - m_me:
// - mu_poke:
#define m_C_STACK_POKE(m_me,mu_poke) {\
  m_ASSERT((m_me).count >= 1)\
  (m_me).hsc_stack[(m_me).count-1] = (mu_poke) ;\
}

// Passed:
// - m_me:
//
// Ret: empty stack ? (true/false) 
#define ob_C_STACK_EMPTY(m_me) ((m_me).count == 0)


// Passed:
// - m_me:
// - mu_push:
#define m_C_STACK_PUSH(m_me, mu_push) {\
  if ((m_me).count + 1 > (m_me).physicalCount) {\
    (m_me).physicalCount *= 2;\
    m_REALLOC_ARRAY((m_me).hsc_stack, (m_me).physicalCount)\
  }\
  m_ASSERT((m_me).count < (m_me).physicalCount)\
  (m_me).hsc_stack[((m_me).count)++] = (mu_push);\
}

// Passed:
// - m_me:
#define m_C_STACK_CLEAR(m_me) {\
  (m_me).count = 0;\
}

// Passed:
// - mx_me:
//
// Changed
// - mx_me: no more usable - MUST BE FREED 
#define m_C_STACK_FREE(mx_me) {\
  free((mx_me).hsc_stack);\
}

// ARRAYS (TODO: put in more ad hoc header) 
// ------

// Add an item to an array
//
// Passed:
// - u_me: array 
// - u_countMax:
// - m_count:
// - m_item
// 
// Changed:
// - m_me:
// - m_count:
#define m_ARRAY_ADD_ITEM(u_me, /*int*/u_countMax, /*int*/m_count, m_item) { \
  m_ASSERT((m_count) < (u_countMax))\
  (u_me)[(m_count)++] = (m_item);\
}




#endif // __C_ANSI_C_STACK_H_INCLUDED
