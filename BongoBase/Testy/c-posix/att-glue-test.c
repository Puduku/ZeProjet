#include "c-posix/att-glue.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  ATT_GLUE_HANDLE h_handle ;

  m_TRACK_IF(AttGlueCreateInstance(&h_handle) != RETURNED)
  m_TRACK_IF(AttGlueDestroyInstance(h_handle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
