#include "c-posix/tid-glue.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  TID_GLUE_HANDLE handle ;

  m_TRACK_IF(TidGlueCreateInstance(&handle,b_PORTABLE) != RETURNED)
  pthread_t selfPthread = pthread_self(), pthread2 = (pthread_t) UNDEFINED;
  tid_t tid = (tid_t) UNDEFINED ; 
  int ret = TidGlueSelfPthread2Tid(handle, &tid) ;
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == RETURNED)
  m_DIGGY_VAR_D(tid)
  ret = TidGlueTid2Pthread(handle, tid, &pthread2) ;
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == RETURNED)
  m_ASSERT(pthread_equal(selfPthread,pthread2))
  m_TRACK_IF(TidGlueDestroyInstance(handle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
