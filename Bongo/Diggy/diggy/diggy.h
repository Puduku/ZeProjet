// diggy/diggy.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// Aim: simple code digger; vademecum for:
// - debugging
// - testing framework

#ifndef __DIGGY_DIGGY_H_INCLUDED
#define __DIGGY_DIGGY_H_INCLUDED


///////////////////////////
#include "diggy/diggy.topo"
///////////////////////////

#include <stdio.h>
#include <string.h>

#include "diggy/diggy-kitchen.h"
#include "flint/crash.h"

// "ordinary" bollard ; to be placed at the begin of each PUBLIC function body you want to dig in.
// (For the main() function, rather use the m_DIGGY_INIT_BOLLARD() macro below)
#define m_DIGGY_BOLLARD() \
int diggyOriginVar = UNDEFINED;\
DiggyBollard((void *)&diggyOriginVar,__func__,__LINE__,__FILE__,1);


// Bollard of static function is not enabled "a priori"
// (but can be enabled here-after)
#ifndef b_DIGGY_BOLLARD_S_ENABLED
#define b_DIGGY_BOLLARD_S_ENABLED 0
#endif

// "special" bollard ; to be placed at the begin of each STATIC function body you want to dig in.
// (For the main() function, rather use the m_DIGGY_INIT_BOLLARD() macro below)
#define m_DIGGY_BOLLARD_S() \
int diggyOriginVar = UNDEFINED;\
DiggyBollard((void *)&diggyOriginVar,__func__,__LINE__,__FILE__,b_DIGGY_BOLLARD_S_ENABLED);



// Formally initialize the diggy system.
// Put preferably this "bollard" in main() function
// 
// Passsed:
// - dgFlags: #see dg-flags @ diggy/diggy-wings.h
#define m_DIGGY_INIT_BOLLARD(/*unsigned int*/ dgFlags) \
DiggyMetaInit(dgFlags); \
m_DIGGY_BOLLARD()


// Formally replace the "return" instruction.
// Helps the "diggy system" to track the stack of functions calls & returns
// To be used with each function in which you have placed a diggy bollard...
//
// Passed:
// - u_ret: this param is evaluated once ;
//   you can pass safely a function call - or any param with "side effect" (++, etc.)
#define m_DIGGY_RETURN(u_ret) {\
  typeof(u_ret) _diggyRet = (u_ret) ;\
  DiggyReturn((void *)&diggyOriginVar,__func__,__LINE__,__FILE__,(long)_diggyRet);\
  return _diggyRet ;\
}


// Display generic information line "alla printf"
// - p_info:
// - ... :
// NOTE: you are invited to construct PRINTABLE content (basic ASCII)...
// NOTE2: the process is protected against crash (SEGV...) during info printing. That is,
// when info refers an invalid data address, a generic "crash / invalid data" message is displayed
// instead of data content, but then the process is resumed...
#define m_DIGGY_INFO(p_info, ...) {\
  DiggyBrickStart((void *)&diggyOriginVar,__func__,__LINE__,__FILE__); \
  m_DIGGY_BRICK_CRASHABLE_PRINT(printf(p_info, ##__VA_ARGS__);) \
  DiggyBrickEnd();\
}

#endif // __DIGGY_DIGGY_H_INCLUDED
