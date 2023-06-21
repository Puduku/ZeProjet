// c-posix/rwf.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
//
// PURPOSE: Enhance handling of regular files (and FIFOS...) with POSIX standard functions...

#ifndef __C_POSIX_RWF_H_INCLUDED__
#define __C_POSIX_RWF_H_INCLUDED__

///////////////////////////
#include "c-posix/rwf.topo"
///////////////////////////

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/file.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"

#include "c-posix/tryagain.h"
#include "c-posix/rwf-kitchen.h"


// I. Opening a regular file
// ========================= 

// Corresponds to fopen()'s "r" mode:
#define            READ__BASIC_OPEN_FILE_MODE	(O_RDONLY)

// Corresponds to fopen()'s "w" mode: 
#define      COLD_WRITE__BASIC_OPEN_FILE_MODE	(O_WRONLY | O_CREAT | O_TRUNC)
#define       HOT_WRITE__BASIC_OPEN_FILE_MODE	(O_WRONLY)
#define      WARM_WRITE__BASIC_OPEN_FILE_MODE	(O_WRONLY | O_CREAT)

// Corresponds to fopen()'s "w+" mode: 
#define COLD_READ_WRITE__BASIC_OPEN_FILE_MODE	(O_RDWR | O_CREAT | O_TRUNC)
// Corresponds to fopen()'s "r+" mode:
#define  HOT_READ_WRITE__BASIC_OPEN_FILE_MODE	(O_RDWR)
#define WARM_READ_WRITE__BASIC_OPEN_FILE_MODE	(O_RDWR | O_CREAT)

#define      HOT_APPEND__BASIC_OPEN_FILE_MODE	(O_WRONLY | O_APPEND) 
// Corresponds to fopen()'s "a" mode:
#define     WARM_APPEND__BASIC_OPEN_FILE_MODE 	(O_WRONLY | O_APPEND | O_CREAT) 

// TBC: what about "a+" (O_RDWR | O_CREAT | O_APPEND) ??? 


// => EXTENDED principle: these two open flag sets are meant to open "named pipes" (alias "fifos") 
// Reminder: those flags do NOT allow to create a fifo (see mkfifo()).
#define       FIFO_READ__BASIC_OPEN_FILE_MODE	READ__BASIC_OPEN_FILE_MODE  
#define      FIFO_WRITE__BASIC_OPEN_FILE_MODE	HOT_WRITE__BASIC_OPEN_FILE_MODE  


// Support for non-posix open()'s flags : O_BINARY / O_TEXT 
#define BINARY__NON_POSIX_OPEN_FLAG		K_BINARY__NON_POSIX_OPEN_FLAG
#define   TEXT__NON_POSIX_OPEN_FLAG		K_TEXT__NON_POSIX_OPEN_FLAG


// This is the typical creation mode used in conjunction with O_CREAT flag
#define BATEAU__FILE_CREATION_MODE	0644


// "b_OPEN_ERRNO_MEANS_..." macros: 
// -------------------------------

// NOTICE: the following classification is purely empiric ; the aim is only to
// provide a basic, understandable interpretation to the ordinary mortal...


// This macro maps the following open() errno values as a "not found" case
// - ENOENT: file entry does not exist 
// - ENOTDIR: a component of path prefix is not a directory 
//
// Macro value:
// - True : errno means: the specified file does not "exist" on the file system
// - False : errno "means" something else...
#define b_OPEN_ERRNO_MEANS_NOT_FOUND(errno) \
  ((errno) == ENOENT || (errno) == ENOTDIR)

// This macro maps the following open() errno values as a "forbidden" case
// - EROFS: write access for file on read-only file system 
// - EEXIST: file yet exists 
// - ETXTBSY: file is open for execution 
// - EACCES: no access right on file 
// - EDQUOT: user's quota of disk blocks 
//
// Macro value:
// - True : errno means: the system explicitly forbids to open that file in the specified mode
// - False : errno "means" something else...
#define b_OPEN_ERRNO_MEANS_FORBIDDEN(errno) \
  ((errno) == EACCES || (errno) == EROFS || (errno) == EEXIST || (errno) == ETXTBSY || (errno) == EDQUOT)

// This macro maps the following open() errno values as a "bad candidate" case; explanation:
// - EINTR or EWOULDBLOCK/EAGAIN: file is "refractory" to opening 
//   with O_TRUNC flag, the named file already exists and is locked with an enforced lock,
//   OR, (eventually) named file is a terminal device or a FIFO
//   OR (eventually) a NFS / USB stick mount unusually deficient ?
// - EISDIR: file is a directory 
// - ENXIO: may occur when file a special file or a FIFO 
//
// Macro value:
// - True : the specified file is apparently not a good candidate for opening 
// - False : errno "means" something else...
#define b_OPEN_ERRNO_MEANS_BAD_CANDIDATE(errno) \
  ((errno) == EINTR || (errno) == EAGAIN  || (errno) == EWOULDBLOCK || \
   (errno) == ENXIO || (errno) == EISDIR)

// This macro maps the errno values that do not enter easily in one of the three
// categories defined below...
//
// Macro value:
// - True : open() failed for non "obvious" reason.  
// - False : errno "means" something else...
#define b_OPEN_ERRNO_MEANS_SOMETHING_ELSE(errno) \
 !(b_OPEN_ERRNO_MEANS_NOT_FOUND(errno) || b_OPEN_ERRNO_MEANS_FORBIDDEN(errno) || \
 b_OPEN_ERRNO_MEANS_BAD_CANDIDATE(errno))
   
   
// Function...
// -----------

// The function (which wraps open() system call) is intended to open a file, supposedly being a
// "good candidate" - as explained in c-ansi/rwf.topo .
// 
// Passed:
// - p_filePathname: file path name in the file system.
// - flags: your combination of O_... flags (type "man 2 open" for details)
//   Cf "basic open flags" (defined above) to help you passing the "good" flags... 
//   To choose a "BASIC_OPEN_FILE_MODE": combine the mode 
//   (READ / WRITE / READ & WRITE / APPEND) with the good temperature 
//   (HOT / WARM  / COLD)
//   EXTENDED principle: for fifo's, choose a "FIFO_..." basic open flag mode 
// - creationMode: (significant with O_CREAT flag): permissions on file creation (type "man 2 open"
//   for details). If you have no clear idea, take BATEAU__FILE_CREATION_MODE...
// - na_openErrno: 
//   + NULL pointer: don't need open()'s errno
//   + not NULL pointer: you need the details when the opening is not possible...
//
// Modified:
// - *aczh_fileDescriptor: only significant with ANSWER__YES status;
// - *na_openErrno: (if asked) : use b_OPEN_ERRNO_MEANS_...() macros to retrieve errno
//   interpretation.
//
// Returned: Is file open ?
// - ANSWER__YES: file has been opened => seems "good candidate"...but don't hesitate to verify
//   by yourself that the file is indeed regular.
// - ANSWER__NO: not possible ; use b_OPEN_ERRNO_MEANS_...(errno) macros to retrieve
//   errno interpretation.
//   When errno is EINTR (interrupted by a signal) (or EAGAIN / ENONBLOCK if you added the
//   O_NBLOCK open flag) (that we assimilate as "bad candidate")...
//   In "COLD" modes (i.e the O_TRUNC flag is set), it COULD mean the named file already exists and
//   is locked with an enforced lock
//   => consider using "HOT" or "WARM" modes instead...
// - -1 : anomaly is raised (unexpected errno value)
int ProtectedOpenFile (const char *p_filePathname, int flags, mode_t creationMode,
  int *aczh_fileDescriptor, int *na_openErrno) ;


// Typical waiting plan 
// --------------------

// (for regular files)
extern const struct WAITING_PLAN p_fileStreamSimpleWaitingPlan;


// II. Locking a regular file
// ==========================

// II.1 POSIX Locks

// "POSIX Lock"-related functions return codes...
enum {
  RWF_FSET_LK_STATUS__OK,
  RWF_FSET_LK_STATUS__TRY_AGAIN,
  RWF_FSET_LK_STATUS__DEADLOCK,
} ;


#define ANOMALY__CANNOT_UNLOCK "Cannot unlock ?!?"


// This function wraps fcntl(F_SETLK / F_SETLKW cmd) system call ; added value:
// - protect process / thread against signal interruptions
// - support for temporizations...
// Thus the function allows the PROCESS to set (or remove...) a lock on a file (and hence
// controlling concurrent access on the file).
// Reminder: The lock protection is warranted to work correctly only if all the processes accessing
// the files share a common lock policy.
// Otherwise, if one process directly performs file access without caring about the lock policy,
// the lock action depends on the nature ("advisory", "mandatory") of the lock positioned for the
// file in the file system.
// Usage of "mandatory" lock allows to protect files against indelicate process, but, since the
// nature of the lock is out of strict POSIX definition, we have no grasp on it...
// NOTE: it seems you can equally use that mechanism to lock "named pipes" (though we've not found
// platform that warrants that to work...) ... However, from functional perpective, we don't see
// the point to lock a segment in a fifo...
//  
// Passed:
// - fileDescriptor : file to be locked
// - pdHandle : see PdCreateInstance() in  c-posix/tryagain.h
// - ap_waitingPlan: specify behavior if currently locked by another process ;
//   #SEE struct-WAITING_PLAN@c-posix/tryagain.h <file lock>
// - aflock: see also fcntl() doc (F_SETLK / F_SETLKW cmd) for details;
//   + a_flock->l_type:
//     . F_RDLCK: "shared" lock type is meant for protecting files you've  opened in the "read" mode.
//     . F_WRLCK: "exclusive" lock type is meant for protecting files you've opened in "write" or "r/w"
//       mode.
//     . F_UNLCK: remove a lock
//   + a_flock->l_whence: How to interpret l_start: SEEK_SET, SEEK_CUR or SEEK_END
//     . fileLock->l_start: Starting offset for lock
//     . fileLock->l_len: Number of bytes to lock
// 
// Returned:
// - RWF_FSET_LK_STATUS__OK: OK, your lock is set (or removed...) on the file
//   Note: whether you put twice the same lock on the file, that situation is not detected!
// - RWF_FSET_LK_STATUS__TRY_AGAIN: there is a conflict with a lock set by another process 
//   Note: if your action was to remove a lock (F_UNLCK), that status could mean that you want to
//   unlock a file you haven't locked before (but was CASUALLY locked by another process in the
//   meanwhile...) => consider raising ANOMALY__CANNOT_UNLOCK
// - RWF_FSET_LK_STATUS__DEADLOCK: refused by the system (deadlock detected)...
//   Note: if your action is to remove a lock (F_UNLCK), that case seems weird ; consider raising
//   ANOMALY__CANNOT_UNLOCK
// - -1 : unexpected problem; anomaly is raised
int ProtectedFcntlFSetlk (int fileDescriptor,
                          PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                          struct flock *a_flock) ;


// II.2 "BSD" (advisory) locks

// This function wraps flock() system call ; added value: 
// - protect process / thread against signal interruptions
// - support for temporizations...
//
// Passed:
// - fileDescriptor : file to be locked
// - operation: type man 2 flock for details (note: LOCK_NB extra flag is managed by the
//   wrapping function)
//   + LOCK_SH: place a share lock
//   + LOCK_EX: place an exclusive lock
//   + LOCK_UN: remove an existing lock
// - pdHandle : see PdCreateInstance() in  c-posix/tryagain.h
// - ap_waitingPlan: specify behavior if currently locked by another process ;
//   #SEE struct-WAITING_PLAN@c-posix/tryagain.h <file lock>
//
// Returned:
// - ATTEMPT__SUCCESS:
// - ATTEMPT__TRY_AGAIN: if your action is to remove (LOCK_UN) a lock you are certain to have
//   locked (LOCK_SH / LOCK_EX) before, consider raising ANOMALY__CANNOT_UNLOCK 
// - -1 : unexpected problem; anomaly is raised
int ProtectedFlock (int fileDescriptor,
                    PD_HANDLE pdHandle, const struct WAITING_PLAN *ap_waitingPlan,
                    int operation) ;



#endif // __C_POSIX_RWF_H_INCLUDED__
