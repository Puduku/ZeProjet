// c-posix/rwf-kitchen.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// PURPOSE: Enhance handling of regular files (and FIFOS...) with POSIX standard functions...

#ifndef __C_POSIX_RWF_KITCHEN_H_INCLUDED__
#define __C_POSIX_RWF_KITCHEN_H_INCLUDED__

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/file.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"


// Support for non-posix open()'s flags : O_BINARY O_TEXT 

#ifdef O_BINARY 
// __CYGWIN__
#define K_BINARY__NON_POSIX_OPEN_FLAG	O_BINARY
#define   K_TEXT__NON_POSIX_OPEN_FLAG	O_TEXT

#else
#define K_BINARY__NON_POSIX_OPEN_FLAG	0
#define   K_TEXT__NON_POSIX_OPEN_FLAG	0

#endif


#endif // __C_POSIX_RWF_KITCHEN_H_INCLUDED__
