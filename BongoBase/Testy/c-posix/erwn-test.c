#include "c-posix/erwn.h"
#include "c-posix/rwn.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


#include <stdio.h>

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  in_addr_t cs_hostInetAddr[5];
  int hErrno = 0;

  int cpt = 1;
  for (; cpt <= 2; cpt++) {
    m_DIGGY_VAR_D(cpt)
    if (cpt == 1) {
      ret = ProtectedGethostbynameR("users.skynet.be", cs_hostInetAddr, 5, &hErrno) ;
    } else {
      ret = ProtectedGethostbyname("users.skynet.be", cs_hostInetAddr, 5, &hErrno) ;
    } // if
    m_TRACK_IF(ret < 0)
    m_DIGGY_VAR_D(ret)
    m_DIGGY_VAR_D(hErrno)
    int i = 0;
    for (; i < 5 && i < ret; i++) {
      m_DIGGY_VAR_D(i)
      m_DIGGY_VAR_GEN((unsigned)RWN_BYTE_OF_INET(cs_hostInetAddr[i],0), u)
      m_DIGGY_VAR_GEN((unsigned)RWN_BYTE_OF_INET(cs_hostInetAddr[i],1), u)
      m_DIGGY_VAR_GEN((unsigned)RWN_BYTE_OF_INET(cs_hostInetAddr[i],2), u)
      m_DIGGY_VAR_GEN((unsigned)RWN_BYTE_OF_INET(cs_hostInetAddr[i],3), u)
    } // for

    if (cpt == 1) {
      ret = ProtectedGethostbynameR("toto.skynet.be", cs_hostInetAddr, 5, &hErrno) ;
    } else {
      ret = ProtectedGethostbyname("toto.skynet.be", cs_hostInetAddr, 5, &hErrno) ;
    } // if
    m_TRACK_IF(ret < 0)
    m_DIGGY_VAR_D(ret)
    m_DIGGY_VAR_D(hErrno)
  } // for

  m_DIGGY_RETURN(0)
} // main
