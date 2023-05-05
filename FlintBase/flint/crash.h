// flint/crash.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: process recovery in case of crash - SIGSEGV, SIGBUS, SIGILL (...)
// =======

#ifndef __FLINT_CRASH_H_INCLUDED__
#define __FLINT_CRASH_H_INCLUDED__

#include "flint/types.h"

#include <setjmp.h>


// TODO: thread safety
// TODO: permettre ag plusieurs modules de partager le medcanisme (qq chose de simple
//       si conflit => juste reporter "bad luck"...)

extern int b_crashRecovery ;
extern sigjmp_buf c_crashSigjmpBuf ;
extern int crashSignal ;


// Allows to recover from crash when executing some
// potentially "unsafe" action... 
// See CrashRecoveryInit @ flint/crash-freeloader.h
// NOTICE: the macro is NOT thread safe
// NOTICE2: "nesting" of unsafe actions is NOT supported
//
// Passed:
// - m_crashableAction: the action that may provoke process crash... 
// - m_recoveryAction: the recovery action in case of crash;
//   crashSignal indicates the exact signal...
#define m_CRASH_RECOVERY_SET(m_crashableAction,m_recoveryAction) {\
  b_crashRecovery = b_TRUE;\
  crashSignal = sigsetjmp(c_crashSigjmpBuf,1);\
  if (crashSignal == 0) {\
    m_crashableAction \
  } else {\
    m_recoveryAction \
  }\
  b_crashRecovery = b_FALSE0;\
}



#endif //  __FLINT_CRASH_H_INCLUDED__
