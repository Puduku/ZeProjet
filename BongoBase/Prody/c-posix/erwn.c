// c-posix/erwn.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"
#include "c-posix/erwn.h"

#include "c-ansi/diggy.h"
//#include "c-posix/testy-diggy.h"


#if 0
TODO: use this function ?
  struct addrinfo hints = {
    .ai_family = AF_INET;SOCK_STREAM
    .ai_ai_socktype = SOCK_STREAM

ai_family   This  field  specifies  the  desired address family for the returned addresses.  Valid values for this field include AF_INET and AF_INET6
       int getaddrinfo(const char *node, const char *service,
                       const struct addrinfo *hints,
                       struct addrinfo **res);

       void freeaddrinfo(struct addrinfo *res);

       const char *gai_strerror(int errcode);
#endif



// Public function : see in .h
int ProtectedGethostbynameR (const char *p_host,
                             in_addr_t cs_hostInetAddr[], int maxNumber,
                             int *na_hErrno) {
  m_DIGGY_BOLLARD()

  int n_count = -1; // unknown status for the moment

#ifdef __APPLE__
  m_RAISE(ANOMALY__NOT_AVAILABLE)
#else
  struct hostent hostent;
  struct hostent *hostentPtr = (struct hostent *) UNDEFINED;
  int hErrno = 0;

  int bufSize = 1024;
  char *h_buf = (char *) UNDEFINED;
  m_MALLOC(h_buf,bufSize)

  while (n_count == -1) {
    int ret = gethostbyname_r(p_host,
                              &hostent, h_buf, bufSize,
                              &hostentPtr,
                              &hErrno);
    switch (ret) {
    case 0: // function call "succeeded"...
      n_count = 0;
      if (hostentPtr != NULL) { // Hostname found
        m_ASSERT(sizeof(*cs_hostInetAddr) == hostentPtr->h_length)
        m_ASSERT(hostentPtr == &hostent)
        for (; n_count < maxNumber && hostentPtr->h_addr_list[n_count] != NULL; n_count++) {
          memcpy(cs_hostInetAddr + n_count, hostentPtr->h_addr_list[n_count], sizeof(*cs_hostInetAddr)) ;
        } // for
      } // if
    break; case ERANGE: // buffer is too small
      bufSize *= 2 ;
      m_ASSERT(bufSize <= 0x4000) // TODO: what reasonable max size ???
      // Let's go for new try:
      m_REALLOC(h_buf,bufSize)
    break; default:
      m_RAISE(ANOMALY__VALUE__D,ret)
    } // switch
  } // while

  if (na_hErrno != NULL) {
    *na_hErrno = hErrno;
  } // if

  free(h_buf);
#endif 
  m_DIGGY_RETURN(n_count);
} // ProtectedGethostbynameR



// Public function : see in .h
int ProtectedGethostbyname (const char *p_host,
                            in_addr_t cs_hostInetAddr[], int maxNumber,
                            int *na_hErrno) {
  m_DIGGY_BOLLARD()
  int count = 0; // no item found for the moment...
  struct hostent *hostentPtr = (struct hostent *) UNDEFINED;

  hostentPtr = gethostbyname(p_host);
  if (hostentPtr != NULL) { // function call "succeeded"...
    m_ASSERT(sizeof(*cs_hostInetAddr) == hostentPtr->h_length)
    for (; count < maxNumber && hostentPtr->h_addr_list[count] != NULL; count++) {
      memcpy(cs_hostInetAddr + count, hostentPtr->h_addr_list[count], sizeof(*cs_hostInetAddr)) ;
    } // for
  } // if

  if (na_hErrno != NULL) {
    *na_hErrno = h_errno;
  } // if

  m_DIGGY_RETURN(count);
} // ProtectedGethostbyname

