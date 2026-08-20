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

// TODO: general renaming e.g: m_SET_FLAG_OFF(m_flags) => om_FLAGS_OFF(m_me)

// 1. Simple flags set
// -------------------

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

// Set state to "ON" for (several) flags 
//
// Passed:
// - mu_me: 
// - u_flags: single flag / flag combination to set "ON""
//
// Modified:
// - mu_me: flags are ON
#define om_FLAGS_SET_ON(mu_me,u_flags) (mu_me) |= (u_flags);

// Set state to "OFF" for (several) flags
//
// Passed:
// - mu_me:
// - u_flags: single flag / flag combination to set "OFF""
//
// Modified:
// - mu_me: flags are OFF 
#define om_FLAGS_SET_OFF(mu_me,u_flags) (mu_me) &= ~(u_flags);

// Set state (ON/OFF) for (several) flags
//
// Passed:
// - mu_me:
// - u_flags: single flag / flag combination to set
// - bu_on: TRUE => set flag ON ; FALSE => set flag OFF
//
// Modified:
// - mu_me: flag is set
#define om_FLAGS_SET(mu_me,u_flags,bu_on) {\
  if (bu_on) om_FLAGS_SET_ON(mu_me,u_flags) else om_FLAGS_SET_OFF(mu_me,u_flags)\
}

// Check whether flag(s) is(are) ON 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) ON ; FALSE: flag(s) is(are) OFF
#define ob_FLAGS_ON(mu_me,u_flags) ((mu_me) & (u_flags))

// Check whether flag(s) is(are) OFF 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
//
// Ret: TRUE: (all) flag(s) is(are) OFF ; FALSE: flag(s) is(are) ON
#define ob_FLAGS_OFF(mu_me,u_flags) (!ob_FLAGS_ON(mu_me,u_flags)) 

// Check flag(s) state 
//
// Passed:
// - mu_me:
// - u_flags: flag(s) to check 
// - bu_on: TRUE => check whether flag(s) is(are) ON ; FALSE => check whether flag(s) is(are) OFF
//
// Ret:
// -  TRUE: (all) flag(s) is(are) in the expected state
// -  FALSE: (all) flag(s) is(are) in the "opposite" state
#define ob_FLAGS_ARE(mu_me,u_flags,bu_on) ((bu_on)? ob_FLAGS_ON(mu_me,u_flags): ob_FLAGS_OFF(mu_me,\
  u_flags))

// Toggle flag(s) state (OFF/ON) 
//
// Passed:
// - m_me:
// - flag: flag SINGLE flag to switch
//
// Modified:
// - m_me: flag has been switched 
//#define m_FLAGS_SWITCH(m_me,flag) if (ob_FLAGS_ON(m_me,flag)) om_FLAGS_SET_OFF(m_me,flag)\
//  else om_FLAGS_SET_ON(m_me,flag) 
m_FLAGS_FLIP(m_me,flag)  (mu_me) ^= (u_flags);

// Pseudo-flag (when off)
#define FLAG_OFF0 0

// "No flag"
#define ALL_FLAGS_OFF0 0



// 2. Flags "arrays"
// -----------------

// 2.a The kitchen...

// Passed
// - flagEntry:
//
// Ret: actual entry in flags array
#define FLAGS_ARRAY_ENTRY(flagEntry) (flagEntry >> 3)

// Passed
// - flagEntry:
//
// Ret: actual flag corresponding to flag entry
#define FLAGS_ARRAY_FLAG(flagEntry) (1 << ((flagEntry) % 0x7)) 

// 2.b The menu...

// Get flags array size 
//
// Passed:
// - entriesNumber: 
#define FLAGS_ARRAY_SIZE(entriesNumber) ((((entriesNumber)-1) >> 3) + 1)


// Set flag state to "ON"
//
// Passed:
// - m_me: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag's entry to set
//
// Modified:
// - m_me: flag is set
#define m_FLAGS_ARRAY_SET_ON(m_me,flagEntry) \
  om_FLAGS_SET_ON(m_me[FLAGS_ARRAY_ENTRY(flagEntry)], FLAGS_ARRAY_FLAG(flagEntry))

// Set flag state to "OFF"
//
// Passed:
// - m_me: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag's entry to set
//
// Modified:
// - m_me: flag is set
#define m_FLAGS_ARRAY_SET_OFF(m_me,flagEntry) \
  om_FLAGS_SET_OFF(m_me[FLAGS_ARRAY_ENTRY(flagEntry)], FLAGS_ARRAY_FLAG(flagEntry))

// Set flag state
//
// Passed:
// - m_me: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag to set
// - b_on: TRUE => set flag ON ; FALSE => set flag OFF
//
// Modified:
#define m_FLAGS_ARRAY_SET(m_me,flagEntry,b_on) if (b_on) m_FLAGS_ARRAY_SET_ON(\
  m_me,flagEntry) else  m_FLAGS_ARRAY_SET_OFF(m_me,flagEntry) 

// Check whether flag is ON 
//
// Passed:
// - me:
// - flagEntry: flag to check 
//
// Ret: TRUE: flag is ON ; FALSE: flag is OFF
#define b_ENTRY_FLAG_SET_ON(me,flagEntry) ((me[FLAGS_ARRAY_ENTRY(flagEntry)]) & \
  FLAGS_ARRAY_FLAG(flagEntry))

// Check whether flag is OFF 
//
// Passed:
// - me:
// - flagEntry: flag to check 
//
// Ret: TRUE: flag is OFF ; FALSE: flag is ON
#define b_ENTRY_FLAG_SET_OFF(me,flagEntry) (!b_ENTRY_FLAG_SET_ON(me,flagEntry)) 

// Check flag state 
//
// Passed:
// - me:
// - flagEntry: flag to check 
// - b_on: TRUE => check whether flag is ON ; FALSE => check whether flag is OFF
//
// Ret:
// -  TRUE: the flag is in the expected state
// -  FALSE: the flag is in the "opposite" state
#define b_ENTRY_FLAG_SET(me,flagEntry,b_on) ((b_on)? b_ENTRY_FLAG_SET_ON(me,\
  flagEntry): b_ENTRY_FLAG_SET_OFF(me,flag))

// Switch flag state (OFF/ON) 
//
// Passed:
// - m_me:
// - flagEntry: flag to switch
//
// Modified:
// - m_me: flag has been switched 
#define m_SWITCH_ENTRY_FLAG(m_me,flagEntry) if (b_ENTRY_FLAG_SET_ON(m_me,\
  flagEntry)) m_FLAGS_ARRAY_SET_OFF(m_me,flagEntry) else m_FLAGS_ARRAY_SET_ON(m_me,\
  flagEntry) 


// Set all flags to OFF
// - m_me:
// - entriesNumber:
#define m_RESET_FLAGS_ARRAY(m_me,entriesNumber) memset(m_me,0,FLAGS_ARRAY_SIZE(\
  entriesNumber);

#endif //  __FLINT_FLAGS_H_INCLUDED__

