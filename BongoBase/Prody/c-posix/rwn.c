// c-posix/rwn.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"

#include "c-posix/tryagain.h"
#include "c-posix/rwn.h"
#include "c-posix/rw.h"

#include "c-ansi/diggy.h"


#define BLAME__SOCKET  "socket()"
#define BLAME__CONNECT "connect()"
#define BLAME__LISTEN  "listen()"
#define BLAME__BIND    "bind()"
#define BLAME__ACCEPT  "accept()"


// Public function : see description in .h
int ProtectedConnect (int tcpIpSocketDescriptor, in_addr_t hostInetAddr, in_port_t inetPort,
  int *na_connectErrno) {
  m_DIGGY_BOLLARD()
  struct sockaddr_in tcpIpSocketPeerAddr ;
  int rwnConnectStatus = RWN_CONNECT_STATUS__CURRENTLY_NOT_POSSIBLE ; // not possible a priori

  if (na_connectErrno == NULL) *na_connectErrno = 0 ;

  memset(&tcpIpSocketPeerAddr,0,sizeof(tcpIpSocketPeerAddr)) ;
  tcpIpSocketPeerAddr.sin_family = AF_INET ;
  tcpIpSocketPeerAddr.sin_port = inetPort ;
  tcpIpSocketPeerAddr.sin_addr.s_addr = hostInetAddr ;

  if (connect(tcpIpSocketDescriptor, (struct sockaddr *)&tcpIpSocketPeerAddr,
    sizeof(tcpIpSocketPeerAddr)) == -1) {
    if (na_connectErrno != NULL) *na_connectErrno = errno ;

    switch (errno) {
    case EINPROGRESS:  // The socket is non-blocking and the connection cannot be completed 
                       // immediately.
    case EALREADY:     // The socket is non-blocking and a previous connection attempt has not yet
                       // been completed.
      rwnConnectStatus = RWN_CONNECT_STATUS__IN_PROGRESS ;
    break; case EINTR:        // Interrupted by signal
    case ETIMEDOUT:    // Connection timed out; the server may be too busy to accept new connections
    case EAGAIN:       // No more free local ports or insufficient entries in the routing cache
#if EAGAIN != EWOULDBLOCK
    case EWOULDBLOCK:
#endif
    case EACCES:       // Cx failed because of local firewall rule
    case EPERM:        // idem
    case ECONNREFUSED: // No one listening on the remote address
    case ENETUNREACH:  // Network is unreachable.
    case EHOSTUNREACH: // No route to host
      rwnConnectStatus = RWN_CONNECT_STATUS__CURRENTLY_NOT_POSSIBLE ;
    break; case EISCONN:  // The socket is already connected.
    default :
      m_PERROR(BLAME__CONNECT)
     } // switch

  } else {
    rwnConnectStatus = RWN_CONNECT_STATUS__OK ;
  } // if

  m_DIGGY_RETURN(rwnConnectStatus)
} // ProtectedConnect


// Public function : see description in .h
int CreateAndConnect (int *aczh_connectedTcpIpSocketDescriptor, in_addr_t hostInetAddr, 
  in_port_t inetPort, int *na_connectErrno) {
  m_DIGGY_BOLLARD()
  int h_descriptor = UNDEFINED;
  int attempt = ATTEMPT__SUCCESSFUL ; // a priori

  m_PERROR_IF((h_descriptor = RWN_SOCKET()) == -1, BLAME__SOCKET)

  switch (ProtectedConnect(h_descriptor,hostInetAddr,inetPort,na_connectErrno)) {
  case RWN_CONNECT_STATUS__OK : // Connected
    *aczh_connectedTcpIpSocketDescriptor = h_descriptor;
  break; case RWN_CONNECT_STATUS__CURRENTLY_NOT_POSSIBLE:
    attempt = ATTEMPT__TRY_AGAIN ;
    switch (ProtectedClose(h_descriptor)) {
    case RW_CLOSE_STATUS__OK:
    break; case RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST:
      m_RAISE(ANOMALY__CONNECTION_LOST) // Connection lost without being connected...
    break; case RW_CLOSE_STATUS__CANNOT_HANG_UP:
      m_RAISE(ANOMALY__CANNOT_HANG_UP)
    break; default :
      m_TRACK()
    } // switch
  break; case RWN_CONNECT_STATUS__IN_PROGRESS:
    m_RAISE(ANOMALY__CONNECTION_IN_PROGRESS)
  break; default :
    m_TRACK()
  } // switch  

  m_DIGGY_RETURN(attempt)
} // CreateAndConnect


// Public function : see description in .h
int BindAndListen (int tcpIpSocketDescriptor, in_port_t inetPort, int backlog,
  int *na_bindOrListenErrno) {
  m_DIGGY_BOLLARD()
  struct sockaddr_in tcpIpSocketLocalAddr ;

  memset(&tcpIpSocketLocalAddr,0,sizeof(tcpIpSocketLocalAddr)) ;
  tcpIpSocketLocalAddr.sin_family = AF_INET ;
  tcpIpSocketLocalAddr.sin_port = inetPort ;
  tcpIpSocketLocalAddr.sin_addr.s_addr = INADDR_ANY ;

  if (bind(tcpIpSocketDescriptor,(struct sockaddr *) &tcpIpSocketLocalAddr,
    sizeof(tcpIpSocketLocalAddr)) == -1) {
    if (na_bindOrListenErrno != NULL) *na_bindOrListenErrno = errno;
    switch (errno) {
    case EADDRINUSE: // The given address is already in use 
    case EACCES: // The address is protected, and the user is not the superuser 
      m_DIGGY_RETURN(ATTEMPT__TRY_AGAIN)
    break; case EINVAL: // The socket is already bound to an address.
    default :
      m_PERROR(BLAME__BIND)
    } // switch
  } // if

  if (listen(tcpIpSocketDescriptor,backlog) < 0) {
    if (na_bindOrListenErrno != NULL) *na_bindOrListenErrno = errno;
    switch (errno) {
    case EADDRINUSE: // Another socket is already listening on the same port 
      // TBC: <=> bind() ???? 
      m_DIGGY_RETURN(ATTEMPT__TRY_AGAIN)
    break; default:
      m_PERROR(BLAME__LISTEN)
    } // switch
  } // if

  m_DIGGY_RETURN(ATTEMPT__SUCCESSFUL)
} // BindAndListen


// Public function : see description in .h
int CreateBindAndListen (int *aczh_listeningTcpIpSocketDescriptor, in_port_t inetPort,
  int backlog, int *na_bindOrListenErrno) {
  m_DIGGY_BOLLARD()
  int h_descriptor = UNDEFINED;

  m_PERROR_IF((h_descriptor = RWN_SOCKET()) == -1, BLAME__SOCKET)

  int attempt = BindAndListen(h_descriptor,inetPort,backlog,na_bindOrListenErrno);
  switch (attempt) {
  case ATTEMPT__SUCCESSFUL:
    *aczh_listeningTcpIpSocketDescriptor = h_descriptor;
  break; case ATTEMPT__TRY_AGAIN:
    switch (ProtectedClose(h_descriptor)) {
    case RW_CLOSE_STATUS__OK:
    break; case RW_CLOSE_STATUS__BEWARE_CONNECTION_LOST:
      m_RAISE(ANOMALY__CONNECTION_LOST)
    break; case RW_CLOSE_STATUS__CANNOT_HANG_UP:
      m_RAISE(ANOMALY__CANNOT_HANG_UP)
    break; default :
      m_TRACK()
    } // switch
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(attempt)
} // CreateBindAndListen


// Public function : see description in .h
int ProtectedAccept (int listeningTcpIpSocketDescriptor, PD_HANDLE pdHandle, 
  const struct WAITING_PLAN *ap_waitingPlan, int *aczh_slaveTcpIpSocketDescriptor,
  in_addr_t *nac_clientInetAddr, in_port_t *nac_clientInetPort, int *na_acceptErrno) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(SynchronizeONonblock(listeningTcpIpSocketDescriptor,ap_waitingPlan) != RETURNED)

  struct sockaddr_in slaveTcpIpSocketPeerAddr ;
  socklen_t size = sizeof(slaveTcpIpSocketPeerAddr) ;
  int n_rwnAcceptStatus = -1; // Unknown

  m_TRACK_IF(PdSetDeadline(pdHandle,ap_waitingPlan) < 0)

  while (n_rwnAcceptStatus == -1) {
    m_TRACK_IF(PdSetAlarm(pdHandle) != RETURNED)

    if ((*aczh_slaveTcpIpSocketDescriptor =  accept(listeningTcpIpSocketDescriptor,
      (struct sockaddr*)&slaveTcpIpSocketPeerAddr,&size)) == -1) {
      // System call failed or interrupted
      if (na_acceptErrno != NULL) {
        *na_acceptErrno = errno;
      } // if
      switch (errno) {
      case EINTR: // Interrupted by signal
      case EAGAIN: // NON BLOCKING would block...
#if EAGAIN != EWOULDBLOCK
      case EWOULDBLOCK:
#endif
        switch (PdCheckTimeout(pdHandle)) {
        case ANSWER__YES: // timeout
          n_rwnAcceptStatus = RWN_ACCEPT_STATUS__TRY_AGAIN ;
        break; case ANSWER__NO: // NO timeout
          if (b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION(errno)) {
            SLEEP_JUST_TO_AVOID_CPU_HOGGING();
          } // if
        break; default:
          m_TRACK()
        } // switch
      break; case EPERM: // Firewall rules forbid connection.
      case ECONNABORTED: 
      case ETIMEDOUT: 
      case ENETDOWN: 
      case EHOSTDOWN: 
#ifndef __APPLE__
      case ENONET: 
#endif
      case EHOSTUNREACH: 
      case ENETUNREACH:
        // TBC: check that list...
        n_rwnAcceptStatus = RWN_ACCEPT_STATUS__INTERCEDED ;
      break; default :
        m_PERROR(BLAME__ACCEPT)
      } // switch

    } else {
      n_rwnAcceptStatus = RWN_ACCEPT_STATUS__OK ; // New client connection 
      if (nac_clientInetAddr != NULL) {
        *nac_clientInetAddr = slaveTcpIpSocketPeerAddr.sin_addr.s_addr;
      } // if
      if (nac_clientInetPort != NULL) {
        *nac_clientInetPort = slaveTcpIpSocketPeerAddr.sin_port ;
      } // if
    } // if
  } // while 

  m_TRACK_IF(PdClearAlarm(pdHandle) != RETURNED)

  m_DIGGY_RETURN(n_rwnAcceptStatus)
} // ProtectedAccept 
