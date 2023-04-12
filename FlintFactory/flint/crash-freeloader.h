// flint/crash-freeloader.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: process recovery in case of crash - SEGV, BUS, ILL (...) - IMPLEMENTATION
// =======
// => This "freeloader" can be #included by unique module of process (typically the main() place).

#ifndef __FLINT_CRASH_FREELOADER_H_INCLUDED__
#define __FLINT_CRASH_FREELOADER_H_INCLUDED__

#include "flint/crash.h"

#include <setjmp.h>
#include <signal.h>


int b_crashRecovery = 0;
sigjmp_buf c_crashSigjmpBuf ;
int crashSignal = 0;

// sig_t / sighandler_t
static void CrashSigHandler (int sig) {
  if (b_crashRecovery) {
    siglongjmp(c_crashSigjmpBuf,sig);
  } else {
    fprintf(stderr,"UNRECOVERABLE CRASH, sig=%d\n",sig);
    exit(-1); // TODO: dedfinir et utiliser valeur exit "signature" (-69...)
  }
}

// To be called once during process init...
static void CrashRecoveryInit(void) {
  signal(SIGSEGV,CrashSigHandler);
  signal(SIGBUS,CrashSigHandler);
  signal(SIGILL,CrashSigHandler);
} // CrashRecoveryInit

#else

#error Already included

#endif //  __FLINT_CRASH_FREELOADER_H_INCLUDED__
