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
  m_itemType *hsc_stack ;\
  int physicalCount;\
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
// - m_poppedItem:
#define m_C_STACK_POP(m_me,m_poppedItem) {\
  m_ASSERT((m_me).count >= 1)\
  m_poppedItem = (m_me).hsc_stack[--((m_me).count)];\
}

// Passed:
// - m_me:
//
// Ret: >= 0: number of items in stack 
#define o_C_STACK_GET_COUNT(m_me) ((m_me).count)

// Passed:
// - m_me:
//
// changed:
// - um_item:
#define o_C_STACK_GET_STACK(m_me) ((m_me).hsc_stack) 

// Passed:
// - m_me:
// - pushedItem:
#define m_C_STACK_PUSH(m_me, pushedItem) {\
  if ((m_me).count + 1 > (m_me).physicalCount) {\
    (m_me).physicalCount *= 2;\
    m_REALLOC_ARRAY((m_me).hsc_stack, (m_me).physicalCount)\
  }\
  m_ASSERT((m_me).count < (m_me).physicalCount)\
  (m_me).hsc_stack[((m_me).count)++] = pushedItem;\
}

// Passed:
// - m_me:
#define m_C_STACK_CLEAR(m_me) {\
  m_me.count = 0;\
}

// Passed:
// - mx_me:
//
// Changed
// - mx_me: no more usable - MUST BE FREED 
#define m_C_STACK_FREE(mx_me) {\
  free((mx_me).hsc_stack);\
}

#endif // __C_ANSI_C_STACK_H_INCLUDED
