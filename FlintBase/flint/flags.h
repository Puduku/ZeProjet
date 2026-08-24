// flint/flags.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023
//
// Purpose: flags "management" ...
// =======

#ifndef __FLINT_FLAGS_H_INCLUDED__
#define __FLINT_FLAGS_H_INCLUDED__

#include <string.h>

// Technical convention: 
// Flag <----> Bit
// OFF  <----> 0    
// ON   <----> 1    

// TODO: REAL once arguments evaluation: static inline + _Generic (C11)  
#if 0
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static inline bool test_all_bits_u8 (uint8_t  v, uint8_t  m, bool a) { return a ? (~v & m) == 0 : (v & m) == 0; }
static inline bool test_all_bits_u16(uint16_t v, uint16_t m, bool a) { return a ? (~v & m) == 0 : (v & m) == 0; }
static inline bool test_all_bits_u32(uint32_t v, uint32_t m, bool a) { return a ? (~v & m) == 0 : (v & m) == 0; }
static inline bool test_all_bits_u64(uint64_t v, uint64_t m, bool a) { return a ? (~v & m) == 0 : (v & m) == 0; }

#define TEST_ALL_BITS(var, mask, allOn) _Generic((var), \
    uint8_t:  test_all_bits_u8,                         \
    uint16_t: test_all_bits_u16,                        \
    uint32_t: test_all_bits_u32,                        \
    uint64_t: test_all_bits_u64,                        \
    default:  test_all_bits_u64                         \
)((var), (mask), (allOn)) 
#endif

// Combine two flags together
//
// Passed:
// - u_flag1:
// - u_flag2:
//
// Ret: flag combination
#define o_FLAGS2(u_flag1,u_flag2) ((u_flag1)|(u_flag2))

// Combine three flags together
//
// Passed:
// - u_flag1:
// - u_flag2:
// - u_flag3:
//
// Ret: flag combination
#define o_FLAGS3(u_flag1,u_flag2,u_flag3) ((u_flag1)|(u_flag2)|(u_flag3))

// Set state to "ON"(enabled) for (some) flag(s) 
//
// Passed:
// - mu_me: 
// - u_flags: flag(s) to set "ON""
//
// Modified:
// - mu_me: flag(s) is(are) ON
#define m_FLAGS_SET_ON(mu_me,u_flags) (mu_me) |= (u_flags);

// Set state to "OFF"(disabled) for (some) flag(s) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to set "OFF""
//
// Modified:
// - mu_me: flag(s) is(are) OFF 
#define m_FLAGS_SET_OFF(mu_me,u_flags) (mu_me) &= ~(u_flags);

// Set state (ON/OFF - enabled/disabled) for (some) flag(s)
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to set
// - bu_on: TRUE => set to "ON" ; FALSE => set to "OFF"
//
// Modified:
// - mu_me: flag(s) is(are) set 
#define m_FLAGS_SET(mu_me,u_flags,bu_on) {\
  if (bu_on) m_FLAGS_SET_ON(mu_me,u_flags) else m_FLAGS_SET_OFF(mu_me,u_flags)\
}

// Toggle flag(s) state (OFF/ON) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to toggle 
//
// Modified:
// - mu_me: flag(s) has(ve) been toggled 
#define m_FLAGS_FLIP(mu_me,u_flags)  (mu_me) ^= (u_flags);


// Check whether flag(s) is(are) ON(enabled) 
//
// Passed:
// - u_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) ON ; FALSE: (some) flag(s) is(are) OFF
#define b_FLAGS_ON(u_me,u_flags) ((~(u_me) & (u_flags)) == 0)

// Check whether flag(s) is(are) OFF(disabled) 
//
// Passed:
// - u_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) OFF ; FALSE: flag(s) is(are) ON
#define b_FLAGS_OFF(u_me,u_flags) (((u_me) & (u_flags)) == 0) 

// Check flag(s) state(ON/OFF - enabled/disabled) 
// NOTICE: once evaluation of args. not 100% warranted.
//
// Passed:
// - u_me: 
// - u_flags: flag(s) to check 
// - bu_on: TRUE => check whether flag(s) is(are) ON ; FALSE => check whether flag(s) is(are) OFF
//
// Ret:
// -  TRUE: (ALL) flag(s) is(are) in the expected state
// -  FALSE: (SOME) flag(s) is(are) in the "opposite" state
#define b_FLAGS_ARE(u_me,u_flags,bu_on) ((((bu_on) ? ~(u_me) : (u_me)) & (u_flags)) == 0)


// Pseudo-flag (when off)
#define FLAG_OFF0 0

// "No flag"
#define ALL_FLAGS_OFF0 0


#endif //  __FLINT_FLAGS_H_INCLUDED__

