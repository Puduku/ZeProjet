// diggy/diggy-kitchen.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#ifndef __DIGGY_DIGGY_KITCHEN_H_INCLUDED
#define __DIGGY_DIGGY_KITCHEN_H_INCLUDED

#include <stdio.h>
#include <string.h>

#include "flint/crash.h"

// TODO: support "pure C-ansi" compilation:
// - -DPTHREAD enabled =>
//   + support MONOTHREAD_DISPLAY__DG_FLAG / MONOTHREAD_NICE_DISPLAY__DG_FLAG
// - -DPTHREAD DISabled =>
//   + comptible with plain C-ANSI compiler
//   + implicitly MONOTHREAD_DISPLAY__DG_FLAG NO ; support MONOTHREAD_NICE_DISPLAY__DG_FLAG


// #REF diggy-flags
// "diggy" configuation flags
// to be combined together with logical or operator (|)
// date & time display:
// display the time (HH:MI:SS)
#define      TIME_DISPLAY__DG_FLAG	0x1 //
// display the date (YYYY/MM/DD) :
#define DATE_TIME_DISPLAY__DG_FLAG	0x2 //
// display the millseconds (.mmm) :
#define MSEC_TIME_DISPLAY__DG_FLAG	0x4 //
// display short stack origin :
#define SHORT_STACK_ORIGIN_DISPLAY__DG_FLAG 0x8 //
// display process id (pid) :
#define PID_DISPLAY__DG_FLAG 0x10 //
// display thread id (tid) :
#define TID_DISPLAY__DG_FLAG 0x20 //
// display adapted to monothread process (condensed form) :
#define MONOTHREAD_DISPLAY__DG_FLAG 0x40 //
// display adapted to monothread process (airy form) :
#define MONOTHREAD_NICE_DISPLAY__DG_FLAG 0x80 //


#define MULTITHREAD_DEFAULT_DG_FLAGS \
(DATE_TIME_DISPLAY__DG_FLAG  | MSEC_TIME_DISPLAY__DG_FLAG | SHORT_STACK_ORIGIN_DISPLAY__DG_FLAG)

#define MONOTHREAD_DEFAULT_DG_FLAGS \
(MULTITHREAD_DEFAULT_DG_FLAGS | MONOTHREAD_DISPLAY__DG_FLAG)


// Initialize the diggy system
//
// Passed:
// - dgFlags:
void DiggyMetaInit (unsigned int dgFlags);


// Indicates new function call to the diggy system.
//  TODO: display functions publiques [P] statiques [S]
// Passed:
// - stackOrigin:
// - p_func:
// - line:
// - p_file:
void DiggyBollard (void *stackOrigin, const char *p_func, int line, const char *p_file, int b_enabled) ;


// Indicates end of function call to the diggy system.
//
// Passed:
// - stackOrigin:
// - p_func:
// - line: > 0
// - p_file:
// - returnValue:
void DiggyReturn (void *stackOrigin, const char *p_func, int line, const char *p_file, long returnValue) ;

///////////////////////////

// Building diggy functions and macros with the
// "diggy bricks"....

// 1st diggy brick ; MANDATORY to build any diggy function / macro
//
// Passed:
// - stackOrigin:
// - p_func:
// - line: > 0
// - p_file:
void DiggyBrickStart (void *stackOrigin, const char *p_func, int line, const char *p_file) ;

// Brick display that may crash...
//
// Passed:
// - m_crashablePrintAction: printing instructions
#define m_DIGGY_BRICK_CRASHABLE_PRINT(m_crashablePrintAction) \
  m_CRASH_RECOVERY_SET(m_crashablePrintAction, \
  printf("(Recovered crash with signal %d)",crashSignal);)


// Last diggy brick ; MUST complete any diggy function / macro
void DiggyBrickEnd (void) ;


#endif // __DIGGY_DIGGY_KITCHEN_H_INCLUDED

