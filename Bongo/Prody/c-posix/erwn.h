// c-posix/erwn.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// Purpose: Enhance functionalities of rwn (focus: hostnames translation into inet (IPv4)
// -------  addresses)

#ifndef __C_POSIX_ERWN_H_INCLUDED__
#define __C_POSIX_ERWN_H_INCLUDED__

#include <netinet/in.h>
#include <arpa/inet.h>

#include "c-ansi/stderr.h"
#include "c-posix/tryagain.h"
#include "c-ansi/types.h"
#include "c-posix/rwn.h"


// This function wraps gethostbyname_r() (*) (**)
// => Retrieve raw inet (IPv4) address(es) corresponding to some hostname.
// (*) thread safe variant of gethostbyname().
// (**) not available on all platforms  
//
// Passed;
// - cs_hostInetAddr: only significant if maxNumber (see below) is > 0
// - maxNumber: >=0 : the max number of items
// - na_hErrno: NULL pointer if not used
//
// Changed:
// - cs_hostInetAddr[]: cs_hostInetAddr[0] -> 1st address found , etc.
// - *na_hErrno: (if used) : usefull to understand reason when NO address is found...
//   Typical reported error is HOST_NOT_FOUND, but other cases like TRY_AGAIN, NO_DATA/NO_ADDRESS
//   are possible (type man gethostbyname for details...)
//
// Ret:
// - >= 0: total number of inet addresses (0 => "not found")
// - -1: unexpected problem ; anomaly is raised
int ProtectedGethostbynameR (const char *p_host,
                             in_addr_t cs_hostInetAddr[], int maxNumber,
                             int *na_hErrno) ;


// This function wraps gethostbyname() (*) (**)
// => Retrieve raw inet (IPv4) address(es) corresponding to some hostname.
// (*) legacy NON thread-safe function.
// (**) available on all POSIX platforms
//
// Passed;
// - cs_hostInetAddr: only significant if maxNumber (see below) is > 0
// - maxNumber: >=0 : the max number of items
// - na_hErrno: NULL pointer if not used
//
// Changed:
// - cs_hostInetAddr[]: cs_hostInetAddr[0] -> 1st address found , etc.
// - *na_hErrno: (if used) : usefull to understand reason when NO address is found...
//   Typical reported error is HOST_NOT_FOUND, but other cases like TRY_AGAIN, NO_DATA/NO_ADDRESS
//   are possible (type man gethostbyname for details...)
//
// Ret:
// - >= 0: total number of inet addresses (0 => "not found")
// - -1: unexpected problem ; anomaly is raised
int ProtectedGethostbyname (const char *p_host,
                            in_addr_t cs_hostInetAddr[], int maxNumber,
                            int *na_hErrno) ;



#endif // __C_POSIX_ERWN_H_INCLUDED__
