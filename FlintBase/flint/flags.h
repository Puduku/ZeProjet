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
#define om_FLAGS_SET_ON(mu_me,u_flags) (mu_me) |= (u_flags);

// Set state to "OFF"(disabled) for (some) flag(s) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to set "OFF""
//
// Modified:
// - mu_me: flag(s) is(are) OFF 
#define om_FLAGS_SET_OFF(mu_me,u_flags) (mu_me) &= ~(u_flags);

// Set state (ON/OFF - enabled/disabled) for (some) flag(s)
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to set
// - bu_on: TRUE => set to "ON" ; FALSE => set to "OFF"
//
// Modified:
// - mu_me: flag(s) is(are) set 
#define om_FLAGS_SET(mu_me,u_flags,bu_on) {\
  if (bu_on) om_FLAGS_SET_ON(mu_me,u_flags) else om_FLAGS_SET_OFF(mu_me,u_flags)\
}

// Toggle flag(s) state (OFF/ON) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to toggle 
//
// Modified:
// - m_me: flag(s) has(ve) been toggled 
#define om_FLAGS_FLIP(mu_me,u_flags)  (mu_me) ^= (u_flags);


// Check whether flag(s) is(are) ON(enabled) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) ON ; FALSE: flag(s) is(are) OFF
#define ob_FLAGS_ON(mu_me,u_flags) ((mu_me) & (u_flags))

// Check whether flag(s) is(are) OFF(disabled) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) OFF ; FALSE: flag(s) is(are) ON
#define ob_FLAGS_OFF(mu_me,u_flags) (!ob_FLAGS_ON(mu_me,u_flags)) 

// Check flag(s) state(ON/OFF - enabled/disabled) 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
// - bu_on: TRUE => check whether flag(s) is(are) ON ; FALSE => check whether flag(s) is(are) OFF
//
// Ret:
// -  TRUE: (ALL) flag(s) is(are) in the expected state
// -  FALSE: (SOME) flag(s) is(are) in the "opposite" state
#define ob_FLAGS_ARE(mu_me,u_flags,bu_on) ((bu_on)? ob_FLAGS_ON(mu_me,u_flags): ob_FLAGS_OFF(mu_me,\
  u_flags))


// Pseudo-flag (when off)
#define FLAG_OFF0 0

// "No flag"
#define ALL_FLAGS_OFF0 0




#endif //  __FLINT_FLAGS_H_INCLUDED__

