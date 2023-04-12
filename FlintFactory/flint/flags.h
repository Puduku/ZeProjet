// flint/flags.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: flags "management" ...
// =======

#ifndef __FLINT_FLAGS_H_INCLUDED__
#define __FLINT_FLAGS_H_INCLUDED__

// Technical convention: 
// Flag <----> Bit
// OFF  <----> 0    
// ON   <----> 1    

// Set flag state to "ON"
//
// Passed:
// - m_flags:
// - flag: flag to set
//
// Modified:
// - m_flags: flag is set
#define m_SET_FLAG_ON(m_flags,flag) (m_flags) |= (flag);

// Set flag state to "OFF"
//
// Passed:
// - m_flags:
// - flag: flag to set
//
// Modified:
// - m_flags: flag is set
#define m_SET_FLAG_OFF(m_flags,flag) (m_flags) &= ~(flag);

// Set flag state
//
// Passed:
// - m_flags:
// - flag: flag to set
// - b_on: TRUE => set flag ON ; FALSE => set flag OFF
//
// Modified:
// - m_flags: flag is set
#define m_SET_FLAG(m_flags,flag,b_on) if (b_on) m_SET_FLAG_ON(m_flags,flag) else \
  m_SET_FLAG_OFF(m_flags,flag) 

// Check whether flag is ON 
//
// Passed:
// - flags:
// - flag: flag to check 
//
// Ret: TRUE: flag is ON ; FALSE: flag is OFF
#define b_FLAG_SET_ON(flags,flag) ((flags) & (flag))

// Check whether flag is OFF 
//
// Passed:
// - flags:
// - flag: flag to check 
//
// Ret: TRUE: flag is OFF ; FALSE: flag is ON
#define b_FLAG_SET_OFF(flags,flag) (!b_FLAG_SET_ON(flags,flag)) 

// Check flag state 
//
// Passed:
// - flags:
// - flag: flag to check 
// - b_on: TRUE => check whether flag is ON ; FALSE => check whether flag is OFF
//
// Ret:
// -  TRUE: the flag is in the expected state
// -  FALSE: the flag is in the "opposite" state
#define b_FLAG_SET(flags,flag,b_on) ((b_on)? b_FLAG_SET_ON(flags,flag): b_FLAG_SET_OFF(flags,flag))

// Switch flag state (OFF/ON) 
//
// Passed:
// - m_flags:
// - flag: flag to switch
//
// Modified:
// - m_flags: flag has been switched 
#define m_SWITCH_FLAG(m_flags,flag) if (b_FLAG_SET_ON(m_flags,flag)) m_SET_FLAG_OFF(m_flags,flag)\
  else m_SET_FLAG_ON(m_flags,flag) 

// "No flag"
#define ALL_FLAGS_OFF0 0

// Check whether ALL flags are in expected state...
//
// Passed:
// - flags:
// - allExpectedFlags: all expected flags (constructed by bitwise "or-ization" of "ON" flags)
//
// Ret:
// - TRUE: all flags is in the expected state
// - FALSE: at least one flag is in the "opposite" state
#define b_ALL_FLAGS_OK(flags,allExpectedFlags) ((flags) == (allExpectedFlags))

// Set all flags
// - m_flags:
// - allFlags: all flags (constructed by bitwise "or-ization" of "ON" flags)
#define m_SET_ALL_FLAGS(m_flags,allFlags) m_flags = (allFlags); \


#endif //  __FLINT_FLAGS_H_INCLUDED__

