// c-posix/rwn.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: 
// --------
// -facilitate usage of TCP/IP-related, socket-based system calls.(=> IPv4 <=)

#ifndef __C_POSIX_RWN_H_INCLUDED__
#define __C_POSIX_RWN_H_INCLUDED__

#include <errno.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "c-ansi/stderr.h"
#include "c-posix/tryagain.h"
#include "c-ansi/types.h"


// 1. Some "utils" (bah!)...

// Create a "good old" TCP/IP socket (IPv4)...
//
// Returned (int value) :
// - >= 0 : created socket
// - -1 : could not create socket ; see errno for details
#define RWN_SOCKET() socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)


// Construct raw inet address from explicit bytes... 
//
// Passed:
// - msb: most significant byte
// - mib: most significant intermediary byte
// - lib: less significant intermediary byte
// - lsb: less significant byte
//
// Ret: inet address
#define /*in_addr_t*/ RWN_INET_ADDR(/*uint8_t*/ msb, /*uint8_t*/ mib,\
                                    /*uint8_t*/ lib, /*uint8_t*/ lsb) \
(in_addr_t) htonl( ((msb)<<24) + ((mib)<<16) + ((lib)<<8) + (lsb) )


// Construct raw inet port from explicit bytes... 
//
// Passed:
// - msb: most significant byte
// - lsb: less significant byte
//
// Ret: inet port
#define /*in_port_t*/ RWN_INET_PORT(/*uint8_t*/ msb, /*uint8_t*/ lsb) \
(in_port_t) htons( ((msb)<<8) + (lsb) )


// Retrieve specific byte of raw inet address or port
//
// Passed
// - m_inet : raw inet address or port (=> in network byte order)
// - i : byte offset :
//   + inet address: 0 => msb .. 3 => lsb
//   + inet port: 0 => msb ; 1 => lsb
#define /*uint8_t*/ RWN_BYTE_OF_INET(m_inet, i) \
( (uint8_t *) &(m_inet) )[i]


// inet_ntop(AF_INET,...) wrapper => IPv4 network
// Convert raw IP address in textual form ("IPV4 dotted-decimal" notation)
//
// Passed:
// - m_inetAddr:
// - ipAddress: "textual ip address" string buffer
//
// Ret:
// - "textual ip address"
#define /*const char* */ RWN_INET_NTOP(/*in_addr_t*/ m_inetAddr, /*char* */ ipAddress) \
inet_ntop(AF_INET, &(m_inetAddr), ipAddress, INET_ADDRSTRLEN)


// => That macro is meant when you continuously try to connect() within a loop...
// The purpose is simply to avoid your program hogging the cpu...
// Note: same purpose than "generic" macro b_ERRNO_MAY_CORRESPOND_TO_CPU_HOGGING_CONDITION() ;
// (but more accurate vis-a-vis connect() system call)...
//
// Passed:
// - errno: after some connect() / ProtectedConnect() failure.
//
// Macro value:
// - true: need to preserve CPU ! that is, add a small delay before new connect() attempt
// - false: the connect() actually "blocked" before failing... hence you need NOT to add delay
//   before trying to connect() again...
#define b_CONNECT_ERRNO_CORRESPONDS_TO_CPU_HOGGING_CONDITION(errno) \
  ((errno) != EINTR && ((errno) != ETIMEDOUT))

  
// 2. The functions..

// ProtectedConnect() function return status: 
enum {
  RWN_CONNECT_STATUS__OK,
  RWN_CONNECT_STATUS__CURRENTLY_NOT_POSSIBLE,
  RWN_CONNECT_STATUS__IN_PROGRESS,
}  ;
 
// This function wraps connect() system call. 
// Attempt to connect to a server.
//
// Passed:
// - tcpIpSocketDescriptor: socket descriptor (see RWN_SOCKET())
// - hostInetAddr: IP address (network byte order)
// - inetPort: tcp port (NETWORK BYTE ORDER - see htons() / RWN_INET_PORT())
// - na_connectErrno: NULL address if not used
//
// Modified:
// - *na_connectErrno: (if used)
//
// Returned:
// - RWN_CONNECT_STATUS__OK: connected!
// - RWN_CONNECT_STATUS__CURRENTLY_NOT_POSSIBLE: either the remote host is not reachable via the network
//   or the remote host is available, but no-one is listening on the port.
//   => you can consult "errno" to get the details on that "network error"...
//      Note: EINTR case (which means underlying connect() has been interrupted by a signal) ; this
//      case is quite "borderline" vis-a-vis the notion of "network error"... 
//      The simplest for you is to treat that case AS THOUGH it was really a network artifact.
//      But keep in mind the following:
//      Interrupting connect() with a signal may be a rudimentary technique to "scan" the presence
//      of a server (without blocking the program too much)...
//      However, the technique is absolutely not portable (e.g on Linux, connect() is immunized
//      against signal interruptions...)
//      Nevertheless, if you want to exploit the technique - when it works, then EINTR indeed means
//      "the seeked server is not responding" (and the signal interruption has simply allowed to 
//      speed up somewhat the obtaining of a response...)
//   => IMPORTANT RECOMMENDATIONS: if you are in a "while-not-connected-try-to-connect" scheme...
//      (I): depending on the platform, the socket may or may NOT remain in a "good state" for
//      trying new connection ; so if you want to be "platform independent" the only way is to
//      close() the socket and to create a new socket() before trying new connection...
//      See also CreateAndConnectTcpIpSocket()
//      (II): be careful not hogging the cpu!!! : see
//      b_CONNECT_ERRNO_CORRESPONDS_TO_CPU_HOGGING_CONDITION() macro.
// - RWN_CONNECT_STATUS__IN_PROGRESS: connection in progress; ONLY possible with "non-blocking"
//   socket descriptor (see O_NONBLOCK flag). You are now supposed to supervise the course of the
//   connection with the ad hoc system calls (select() and getsockopt()).
//   Reminder: by default, a socket is "blocking" (hence, that situation cannot occur unless you
//   "trick" the mode of the socket descriptor...)
// - -1: unexpected problem ; anomaly is raised
int ProtectedConnect (int tcpIpSocketDescriptor,
                      in_addr_t hostInetAddr, in_port_t inetPort,
                      int *na_connectErrno);

// If you get RWN_CONNECT_STATUS__IN_PROGRESS whereas you are certain not using an non blocking
// socket, you can raise that anomaly...
#define ANOMALY__CONNECTION_IN_PROGRESS "Connection in progress ???"


// This function wraps ProtectedConnect() function below. 
// - Create tcp ip socket (using  RWN_SOCKET())
// - Attempt to connect that socket to a server (in BLOCKING mode, using ProtectedConnect()).
// - in case of connection failure, the socket is destroyed (using ProtectedClose()) 
// - na_connectErrno:
//   + NULL address:
//   + != NULL:
//
// Passed:
// - aczh_connectedTcpIpSocketDescriptor: address of socket descriptor to be created
// - hostInetAddr: IP address (network byte order)
// - inetPort: tcp port (NETWORK BYTE ORDER - see htons() / RWN_INET_PORT())
//
// Modified:
// - *aczh_connectedTcpIpSocketDescriptor: only significant in case of success
// - *na_connectErrno:
//
// Returned:
// - ATTEMPT__SUCCESSFUL: successfully created and connected 
// - ATTEMPT__TRY_AGAIN: cannot connect (ProtectedConnect() returned "NOT POSSIBLE") ; created
//   socket has been automatically destroyed...
// - -1: unexpected problem ; anomaly is raised
int CreateAndConnect (int *aczh_connectedTcpIpSocketDescriptor,
                      in_addr_t hostInetAddr, in_port_t inetPort,
                      int *na_connectErrno) ;


#define RWN_BACKLOG 16

// Prepare socket for listening (wraps bind() and listen())
//
// Passed:
// - tcpIpSocketDescriptor: socket descriptor (see RWN_SOCKET())
// - inetPort: tcp port (NETWORK BYTE ORDER - see htons() / RWN_INET_PORT())
// - backlog: type "man 2 listen" for details ; if you have no better idea, take RWN_BACKLOG...
// - na_bindOrListenErrno: want details if failed attempt ?
//   + NULL special pointer: no thank you...
//   + != NULL: address of variable to get errno's copy
//
// Modified:
// - *na_bindOrListenErrno:
//
// Returned: Succeeded to bind ?
// - ATTEMPT__SUCCESSFUL: our socket is ready to accept TCP/IP client connections.
// - ATTEMPT__TRY_AGAIN: impossible to bind socket on that port ;
//   listening port already in use OR port is protected (requires root privilege)...
//   => you can consult "errno" to know the exact cause...
//   => OUR GOOD ADVICES:
//      (I) it is not clear whether the socket remains in a "good state" after a failure ; so if
//      you want to remain "platform independent" DO NOT re-use that socket descriptor to try a new
//      "binding"...
//      (II) in order to avoid hogging cpu, always observe a small delay if you want to undertake a
//      new attempt (on that port)...
// - -1: unexpected problem ; anomaly is raised
int BindAndListen (int tcpIpSocketDescriptor,
                   in_port_t inetPort, int backlog,
                   int *na_bindOrListenErrno);


// Wraps BindAndListen() function above.
// - Create tcp ip socket (using  RWN_SOCKET())
// - Attempt to bind that socket to a server (using BindAndListen()).
// - in case of binding failure, the socket is destroyed (using ProtectedClose()) 
//
// Passed:
// - *aczh_listeningTcpIpSocketDescriptor: UNINITIALIZED descriptor
// - inetPort: tcp port (NETWORK BYTE ORDER - see htons() / RWN_INET_PORT())
// - backlog: type "man 2 listen" for details ; if you have no better idea, take RWN_BACKLOG...
// - na_bindOrListenErrno: want details if failed attempt ?
//   + NULL special pointer: no thank you...
//   + != NULL: address of variable to get errno's copy
//
// Modified:
// - *aczh_listeningTcpIpSocketDescriptor: new socket descriptor
// - *na_bindOrListenErrno:
//
// Returned:
// - ATTEMPT__SUCCESSFUL: our socket is ready to accept TCP/IP client connections.
// - ATTEMPT__TRY_AGAIN: impossible to bind socket on that port ;
//   listening port already in use OR port is protected (requires root privilege)...
//   => you can consult "errno" to know the exact cause...
//   => OUR GOOD ADVICES:
//      (II) in order to avoid hogging cpu, always observe a small delay if you want to undertake a
//      new attempt (on that port)...
// - -1: unexpected problem ; anomaly is raised
int CreateBindAndListen (int *aczh_listeningTcpIpSocketDescriptor,
                         in_port_t inetPort , int backlog,
                         int *na_bindOrListenErrno) ;


// Maximun length (in characters) of IP addresses based on "IPV4 dotted-decimal" notation:
#define RWN_IP_ADDRESS_MAX_LENGTH (INET_ADDRSTRLEN-1)


// ProtectedAccept() function return status:
enum {
  RWN_ACCEPT_STATUS__OK,
  RWN_ACCEPT_STATUS__TRY_AGAIN,
  RWN_ACCEPT_STATUS__INTERCEDED,
}  ;

// accept() wrapper, i.e wait for a new connection on listening socket.
// Note: the function may modify the O_NONBLOCK flag of the (listening) socket descriptor.
//
// Passed:
// - listeningTcpIpSocketPdHandle: see above BindAndListen() / CreateBindAndListen()
// - pdHandle: "pretty descriptor" (PD) handle - see PdCreateInstance()
// - ap_waitingPlan: #SEE struct-WAITING_PLAN@c-posix/tryagain.h <client>
// - nac_clientInetAddr:
//   + NULL pointer: you don't care about the IP address of the client
//   + non NULL: you want to know the IP address of the client
// - nac_clientInetPort:
//   + NULL address: you don't care about client's port
//   + non NULL: you want to know the port of the client
// - na_acceptErrno: NULL address if not used
//
// Changed:
// - *aczh_slaveTcpIpSocketDescriptor: only significant if a connection is accepted 
// - *nac_clientInetAddr: (if asked) only significant if a connection is accepted;
//   IP address of the client ; see  RWN_INET_NTOP() macro above,
// - *nac_clientInetPort: (if asked) only significant if a connection is accepted ;
//   in NETWORK BYTE ORDER - see ntohs()
// - *na_acceptErrno: (if used) : exact errno set by accept() primitive...
//
// Returned:
// - RWN_ACCEPT_STATUS__OK: new TCP/IP client connected on slave socket
// - RWN_ACCEPT_STATUS__TRY_AGAIN: deadline expired or process has been interrupted by signal 
//   before a client has connected.
// - RWN_ACCEPT_STATUS__INTERCEDED: some network device interceded between client and server and
//   impeded connection to complete.
//   In simpler networking infrastructures, "accept" procedure should never fail; in nowadays
//   infrastructures, however, such problems are to arise sometimes (for instance, when a client
//   tries to connect behind a firewall).
//   => you can still consult "errno" to get the details on that "error"...
// - -1: unexpected problem ; anomaly is raised
int ProtectedAccept (int listeningTcpIpSocketDescriptor,
                     PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                     int *aczh_slaveTcpIpSocketDescriptor,
                     in_addr_t *nac_clientInetAddr, in_port_t *nac_clientInetPort,
                     int *na_acceptErrno) ;


// Same function but DO NOT synchronize O_NONBLOCK flag of the descriptor...
int ProtectedAccept2 (int listeningTcpIpSocketDescriptor,
                      PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                      int *aczh_slaveTcpIpSocketDescriptor,
                      in_addr_t *nac_clientInetAddr, in_port_t *nac_clientInetPort,
                      int *na_acceptErrno) ;




#endif // __C_POSIX_RWN_H_INCLUDED__
