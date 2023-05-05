// c-ansi/alloc.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __C_ANSI_ALLOC_H_INCLUDED
#define __C_ANSI_ALLOC_H_INCLUDED

#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"


#define BLAME__MALLOC	"malloc()"
#define BLAME__CALLOC	"calloc()"
#define BLAME__REALLOC	"realloc()"


// Very general malloc() wrapper
// (added value: manage "perror" triggering)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - muzh_ptr: the pointer to initialize
// - u_size: the memory size to allocate.
#define m_MALLOC(muzh_ptr, /*int*/ u_size)  m_PERROR_IF((muzh_ptr = malloc(u_size)) == NULL,\
  BLAME__MALLOC)


// Very general realloc() wrapper
// (added value: manage "perror" triggering)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - mnh_ptr: the pointer to initialize or to reinitialize;
//   + NULL special value: pure initialization (same result than m_MALLOC() macro)
//   + non NULL: pointer to current memory block (to resize)
// - u_size: the memory size to allocate or to reallocate
#define m_REALLOC(mnh_ptr, /*int*/ u_size)  m_PERROR_IF((mnh_ptr = realloc(mnh_ptr, u_size)) ==\
  NULL, BLAME__REALLOC)


// Very general calloc() wrapper
// (added value: manage "perror" triggering)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - muzh_ptr: the pointer to initialize
// - u_number: the number of elements .
// - u_size: the size of each element.
#define m_CALLOC(muzh_ptr, /*int*/ u_number, /*int*/ u_size)  m_PERROR_IF((muzh_ptr = \
 calloc(u_number,u_size)) == NULL, BLAME__CALLOC)


// malloc() wrapper for arrays (of typed elements)
// (added value: manage "perror" triggering AND establish the size of memory to allocate
// according to type of the array's elements)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - mzhs_array: the (array) pointer to initialize
// - u_number: the number of elements of the array
#define m_MALLOC_ARRAY(mzhs_array, /*int*/ u_number)  m_PERROR_IF(( (mzhs_array) = \
  malloc((u_number) * sizeof(*(mzhs_array))) ) == NULL,  BLAME__MALLOC)


// realloc() wrapper for arrays (of typed elements)
// (added value: manage "perror" triggering AND establish the size of memory to (re-)allocate
// according to type of the array's elements)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - mnhs_array: the (array) pointer to initialize or to re-initialize
//   + NULL special value: pure initialization (same result than m_MALLOC_ARRAY() macro)
//   + non NULL: pointer to current memory block (to resize)
// - u_number: the (new) number of elements of the array
#define m_REALLOC_ARRAY(mnhs_array, /*int*/ u_number)  m_PERROR_IF(( (mnhs_array) = \
  realloc(mnhs_array, (u_number) * sizeof(*(mnhs_array))) ) == NULL,  BLAME__REALLOC)


// malloc() wrapper for "object" instances
// (added value: manage "perror" triggering AND establish the size of memory to allocate
// according to type of the object instance)
// #SEE CURRENT_TRACKING_VALUE @c-ansi/stderr.h
//
// Passed:
// - mzh_handle: the instance pointer to initialize
#define m_MALLOC_INSTANCE(mzh_handle)  m_PERROR_IF(( (mzh_handle) = malloc(sizeof(*(mzh_handle))) )\
  == NULL,  BLAME__MALLOC)


#endif // __C_ANSI_ALLOC_H_INCLUDED
