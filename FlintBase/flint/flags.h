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

// 1. Simple flags set
// -------------------

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
// - m_flagsArray: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag's entry to set
//
// Modified:
// - m_flagsArray: flag is set
#define m_SET_ENTRY_FLAG_ON(m_flagsArray,flagEntry) \
  m_SET_FLAG_ON(m_flagsArray[FLAGS_ARRAY_ENTRY(flagEntry)], FLAGS_ARRAY_FLAG(flagEntry))

// Set flag state to "OFF"
//
// Passed:
// - m_flagsArray: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag's entry to set
//
// Modified:
// - m_flagsArray: flag is set
#define m_SET_ENTRY_FLAG_OFF(m_flagsArray,flagEntry) \
  m_SET_FLAG_OFF(m_flagsArray[FLAGS_ARRAY_ENTRY(flagEntry)], FLAGS_ARRAY_FLAG(flagEntry))

// Set flag state
//
// Passed:
// - m_flagsArray: declared with FLAGS_ARRAY_SIZE() macro  
// - flagEntry: flag to set
// - b_on: TRUE => set flag ON ; FALSE => set flag OFF
//
// Modified:
#define m_SET_ENTRY_FLAG(m_flagsArray,flagEntry,b_on) if (b_on) m_SET_ENTRY_FLAG_ON(\
  m_flagsArray,flagEntry) else  m_SET_ENTRY_FLAG_OFF(m_flagsArray,flagEntry) 

// Check whether flag is ON 
//
// Passed:
// - flagsArray:
// - flagEntry: flag to check 
//
// Ret: TRUE: flag is ON ; FALSE: flag is OFF
#define b_ENTRY_FLAG_SET_ON(flagsArray,flagEntry) ((flagsArray[FLAGS_ARRAY_ENTRY(flagEntry)]) & \
  FLAGS_ARRAY_FLAG(flagEntry))

// Check whether flag is OFF 
//
// Passed:
// - flagsArray:
// - flagEntry: flag to check 
//
// Ret: TRUE: flag is OFF ; FALSE: flag is ON
#define b_ENTRY_FLAG_SET_OFF(flagsArray,flagEntry) (!b_ENTRY_FLAG_SET_ON(flagsArray,flagEntry)) 

// Check flag state 
//
// Passed:
// - flagsArray:
// - flagEntry: flag to check 
// - b_on: TRUE => check whether flag is ON ; FALSE => check whether flag is OFF
//
// Ret:
// -  TRUE: the flag is in the expected state
// -  FALSE: the flag is in the "opposite" state
#define b_ENTRY_FLAG_SET(flagsArray,flagEntry,b_on) ((b_on)? b_ENTRY_FLAG_SET_ON(flagsArray,\
  flagEntry): b_ENTRY_FLAG_SET_OFF(flagsArray,flag))

// Switch flag state (OFF/ON) 
//
// Passed:
// - m_flagsArray:
// - flagEntry: flag to switch
//
// Modified:
// - m_flagsArray: flag has been switched 
#define m_SWITCH_ENTRY_FLAG(m_flagsArray,flagEntry) if (b_ENTRY_FLAG_SET_ON(m_flagsArray,\
  flagEntry)) m_SET_ENTRY_FLAG_OFF(m_flagsArray,flagEntry) else m_SET_ENTRY_FLAG_ON(m_flagsArray,\
  flagEntry) 


// Set all flags to OFF
// - m_flagsArray:
// - entriesNumber:
#define m_RESET_FLAGS_ARRAY(m_flagsArray,entriesNumber) memset(m_flagsArray,0,FLAGS_ARRAY_SIZE(\
  entriesNumber);

#endif //  __FLINT_FLAGS_H_INCLUDED__

