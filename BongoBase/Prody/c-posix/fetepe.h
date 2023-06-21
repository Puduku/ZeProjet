// c-posix/fetepe.h, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// Description : FTP client module

#ifndef __C_POSIX_FETEPE_H_INCLUDED
#define __C_POSIX_FETEPE_H_INCLUDED

#include "c-ansi/types.h"
#include "c-ansi/c-string.h"
#include "c-ansi/g-string.h"
#include "c-posix/rw.h"
#include "c-posix/sucker.h"

// TODO: futupu (ftp SERVER)

enum { // #REF FTP_COMMAND_STATUS-small-enum
             OPEN_DATA_CHANNEL__FTP_COMMAND_STATUS = 150, // "Open data channel"
              COMMAND_EXECUTED__FTP_COMMAND_STATUS = 200, // => indicates a success ; it's also the
                // arbitrary status provided if no command status has yet been received.
          SERVICE_IS_AVAILABLE__FTP_COMMAND_STATUS = 220, // "Service is available"
  DATA_CHANNEL_HAS_BEEN_CLOSED__FTP_COMMAND_STATUS = 226, // "Data channel has been closed"
         ENTERING_PASSIVE_MODE__FTP_COMMAND_STATUS = 227, // "Entering Passive Mode"
               SESSION_IS_OPEN__FTP_COMMAND_STATUS = 230, // "Session is open"
               FILE_SERVICE_OK__FTP_COMMAND_STATUS = 250, // "File service OK"
              PATHNAME_CREATED__FTP_COMMAND_STATUS = 257, //  "PATHNAME" created."
              ASK_FOR_PASSWORD__FTP_COMMAND_STATUS = 331, // "Ask for password"
  FILE_SERVICE_WAITING_FOR_COMPLEMENTARY_INFORMATION__FTP_COMMAND_STATUS = 350, // "File service
    // waiting for complementary information"
           SESSION_IS_NOT_OPEN__FTP_COMMAND_STATUS = 530, // => may correspond to a server
             // "timeout" due to lack of activity...
              FILE_UNAVAILABLE__FTP_COMMAND_STATUS = 550, // => permission denied, no such
                //  directory, etc.
         FILE_NAME_NOT_ALLOWED__FTP_COMMAND_STATUS = 553, // => "File name not allowed"
} ; // ...other statuses: see FTP RFC 959...



// Internal structure (private!) :
struct FETEPE ;
// Internal structure pointer (=> public) :
typedef struct FETEPE *FETEPE_HANDLE ;

// #REF FETEPE_LOG_COMMAND_STATUS_FUNCTION Hook function to log command status
//
// Passed:
// - r_handle: private handle
// - commandStatusLine: command status line to log (non EOL terminated)
// - b_lastLine: 
//   + true : last line of commmand status
//   + false: other lines will follow
// 
// Return:
// - RETURNED: OK
// - -1: unexpected problem ; anomaly is raised
typedef int (*FETEPE_LOG_COMMAND_STATUS_FUNCTION) (void *r_handle,
  struct STRING_PORTION commandStatusLine, char b_lastLine) ;


// "FETEPE's FTP operations" : possible statuses...
enum {
  FETEPE_STATUS__OK, // command accepted and completed.
  FETEPE_STATUS__REFUSED, // #REF FETEPE_STATUS__REFUSED
  // "In general", it means command explicitly refused by server (for instance if
  // you are no more "logged in")... See FetepeGetLastFtpCommandStatus() function
  FETEPE_STATUS__NO_COMMAND_CHANNEL, // #REF FETEPE_STATUS__NO_COMMAND_CHANNEL
  // Not connected or failed to send command to or receive status from FTP server: probably a
  // "network" problem... or the FTP server has unexpectedly closed the connection...
} ;


// Create "FTP client" instance
//
// Passed:
// - azh_handle: handle's address to be initialized
// - nf_alarmSystemHandle: alarm system as formalized in c-posix/alarm.h
//   + NULL (not provided): fetepe cannot support timeout in communication
//   + non NULL (true handle): allows fetepe instance to support deadline during communication
// - f_brokenPipeFixHandle: 
// - transferSizeLimit: limit, in bytes, of the size of files that can be transferred to (from) 
//   FTP server (0 for no limit)
// - n_fetepeLogCommandStatusFunction:
// - cr_fetepeLogCommandStatusFunctionHandle:
//
// Modified:
// - *azh_handle : created "ftp client" handle 
//
// Returned:
// - RETURNED: ok
// - -1: unexpected problem; anomaly is raised  
int FetepeCreateInstance(FETEPE_HANDLE *azh_handle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle,
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle, int transferSizeLimit,
  FETEPE_LOG_COMMAND_STATUS_FUNCTION n_fetepeLogCommandStatusFunction,
  void *cfr_fetepeLogCommandStatusFunctionHandle);


// #REF FetepeGetLastFtpCommandStatus()
// Aim: understand why a command has been refused (when you get FETEPE_STATUS__REFUSED status)
// For commands involving a data transfer (like FtpGet() / Put()...) this function also allows
// to know whether it's really an explicit refusal of the ftp server or rather a "local permission
// problem" in relation with data transfer. #ENDREF
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - na_localOpenErrno: NULL pointer if not used
//
// Modified:
// - *a_ftpCommandStatus: last command status returned by FTP server.
//   #SEE FTP_COMMAND_STATUS-small-enum
// - *na_localOpenErrno: (if used): helps to understand the reason of a local problem :
//   + >0 : failed to open "local" file ; see b_OPEN_ERRNO_MEANS_...() macros defined in
//     c-posix/rwf.h for good interpretation...
//   + 0 (ENOERR) : this is NOT a classical problem (while trying to open file...) ; the issue that
//     occurred is quite infrequent and has another origin (examples: I/O problem on the file
//     system; firewall that blocks opening of data flow ; ...). See FetepeGetClue() for details...
//
// Returned :
// COMPLETED__OK: the refusal comes explicitly from FTP server, the status reported in
// *a_ftpCommandStatus is relevant to understand the problem...
// COMPLETED__BUT: the last command has not been refused by the FTP server, the refusal has another
// origin (like a problem of LOCAL permissions) ; *a_ftpCommandStatus is NOT relevant to understand
// the problem ; yet, *na_localOpenErrno should contain the explanation...
// - -1: unexpected problem; anomaly is raised
int FetepeGetLastFtpCommandStatus(FETEPE_HANDLE handle, int *a_ftpCommandStatus,
  int *na_localOpenErrno);


// #REF FetepeGetClue()
// Aim: understand, in last resort, the cause of a "sporadic" problem in some FTP operation.
// (i.e some Fetepe<Op>() function returns FETEPE_STATUS__NO_COMMAND_CHANNEL or
//  FETEPE_STATUS__REFUSED, but the cause is not explicit) #ENDREF
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned :
// - non NULL pointer: brief, but hopefully "humanely readable" description of the encountered
//   issue; purpose is solely to indicate the good direction for problem resolution...
// - NULL pointer: unexpected problem; anomaly is raised
const char *FetepeGetClue(FETEPE_HANDLE handle);


// Connect "physically" and log in to a FTP server:
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_hostIpAddress: server IP address
// - p_user: user name
// - np_password:
//   + NULL pointer: do not provide password to connect (public user)
//   + non NULL pointer: the password awaited by the FTP Server
// - na_connectErrno: NULL special address if not used
//
// Modified:
// - *na_connectErrno: (if used) meanfull in "NOT CONNECTED" case, i.e when status is
//   FETEPE_STATUS__NO_COMMAND_CHANNEL:
//   + > 0: actual connect() primitive failed because of some "NETWORK ERROR" ; corresponding
//     errno value ;
//   + 0 (ENOERR): succeeded physical connect() stage ; but then failed to send command to or
//     receive status from server (or possibly, the server directly disconnected...) ; see
//     FetepeGetClue() for details...
//
// Returned:
// - FETEPE_STATUS__OK : we are "physically" connected and "logged in" to the FTP server.
// - FETEPE_STATUS__REFUSED : "physically" connected, but FTP "log in" refused by
//   FTP Server; typical cause: wrong password, etc. See FetepeGetLastFtpCommandStatus() function
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
//   => see also *na_connectErrno (if relevant)
// - -1 : unexpected problem (an anomaly is raised)
int FetepeConnect (FETEPE_HANDLE handle, const char *p_hostIpAddress, const char *p_user,
  const char *np_password, int *na_connectErrno) ;


// Specify 'Ascii' file transfer type
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK : 
// - FETEPE_STATUS__REFUSED : #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeAscii (FETEPE_HANDLE handle) ;


// Specify 'Binary' file transfer type
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK :
// - FETEPE_STATUS__REFUSED : #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeBinary (FETEPE_HANDLE handle) ;


// "No operation"
// "No op" commands are usefull to avoid 'time out" of the ftp server.
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED : #SEE FETEPE_STATUS__REFUSED
//   TODO: verifier si le server envoie effectivement peter les noop hors session...
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem ; anomaly is raised
int FetepeNoop (FETEPE_HANDLE handle) ;



// "System" - display server's operating system
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED : #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem ; anomaly is raised
int FetepeSystem (FETEPE_HANDLE handle) ;


// "Print working Directory"
// The remote working directory is displayed within the command response...
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED : #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem ; anomaly is raised
int FetepePwd (FETEPE_HANDLE handle) ;


// Change remote directory
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_directoryName:
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: directory not found or not available
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeCd (FETEPE_HANDLE handle, const char *p_directoryName) ;


// Rename a remote file or directory
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_oldName:
// - p_newName:
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: file does not exist, file is protected, etc.
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeRename (FETEPE_HANDLE handle, const char *p_oldName, const char *p_newName) ;


// Delete a remote file or empty directory
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_name:
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: file does not exist or is protected...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #SEE FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeDelete (FETEPE_HANDLE handle, const char *p_name) ;


// "Passive" - toggle passive mode
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned: Are we now in Passive mode ?
// - ANSWER__YES : toggled to PASSIVE mode
// - ANWSER__NO : toggled to ACTIVE mode
// - -1 : unexpected problem ; anomaly is raised
int FetepePassive (FETEPE_HANDLE handle) ;


// Transfer a local (source) file to remote (destination) file
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_localFilePathname: local file's pathname (source)
// - p_remoteFileName:
// - na_putLength: NULL (special) pointer if not used
//
// Modified:
// - *na_putLength: (if asked) : length of data transferred so far...
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: FTP user does not have write access...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepePut (FETEPE_HANDLE handle, const char *p_localFilePathname, const char *p_remoteFileName,
  int *na_putLength) ;


// Same as FetepePut() above, but use a "sucker butt" as "local" source...
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - n_localButtSpotterSuckFunction:
// - r_localButtSpotterSuckHandle:
// - p_remoteFileName:
// - na_putLength: NULL (special) pointer if not used
//
// - Modified:
// - *na_putLength: (if asked) : length of data transfered so far...
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: FTP user does not have write access...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepePut2 (FETEPE_HANDLE handle, //const struct SUCKER_BUTT *ap_localButt,
  BUTT_SPOTTER_SUCK_FUNCTION localButtSpotterSuckFunction, void *r_localButtSpotterSuckHandle,
  const char *p_remoteFileName, int *na_putLength);

// Allow Transfer "local" content into "destination" file : provide "destination" stream
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_remoteFileName:
//
// Modified:
// - *ac_remoteDescriptor: (only sigificant with "OK" status) allows to feed "remote" file
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: FTP user does not have write access...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepePut3Open (FETEPE_HANDLE handle, const char *p_remoteFileName, 
  int *ac_remoteDescriptor) ;

// Transfer "local" content into "destination" file : terminate transfer 
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: failed to close (pathetic case)  
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepePut3Close (FETEPE_HANDLE handle);


// List of corrective operations (bit flags) applicable to the file names
// returned by the FTP server. Indeed, we may have to take into account the
// peculiarities of certain FTP servers concerning file name conventions. 

// Do not trick
#define NO__FILE_NAME_CORRECTION 0

// Working around non-respect of the case by certain FTP servers (such as NT) 
#define ASCII_UPPER_CASE__FILE_NAME_CORRECTION 1

// Suppres ";1" suffix returned by the VMS servers
#define VMS_VERSION_1_SUFFIX__FILE_NAME_CORRECTION 16

// Get remote directory's files list 
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - n_filter:
//   + NULL pointer: no filter applied
//   + not a NULL pointer: apply this filter
// - corrections: corrections to be applied to each file name returned by the FTP server, to work
//   around with some platform pecularities...
//   Recapitulation of known FTP server peculiarities:
//   + NT server :
//     Using FetepePut(), you firstly put the "TOTO.DAT" new file on the server, but then when 
//     you perform the FetepeList(), you will get "Toto.dat" instead.
//     The workaround we propose is to apply the UPPER_CASE__FILE_NAME_CORRECTION
//     flag when calling ftpLs() so that you'll get the "TOTO.DAT" as expected; of course, this
//     workaround only fits if you use file names in full uppercase.
//   + VAX VMS server :
//     You firstly put the "toto.dat" new file, but then when you perform the FetepeList(), you get
//     "toto.dat;1" instead.
//     It's because VAX VMS handles the file versions (version 2 of file above would be issued 
//     "toto.dat;2", etc.)
//     The workaround we propose is to apply the VMS_VERSION_1_SUFFIX__FILE_NAME_CORRECTION flag
//     when calling ftpLs() so that you'll get the "toto.dat" as expected.
//
// Modified:
// - fileNames : list of files updated
//   in case of FTP server refusal (FETEPE_STATUS__REFUSED) or disconnection 
//   (FETEPE_STATUS__NO_COMMAND_CHANNEL), the list is empty (0 item).
//
// Returned:
// - FETEPE_STATUS__OK : files list established. The list is **generally** not empty, but note
//   carefully that some FTP servers return an OK status for (existing but) empty directories.
// - FETEPE_STATUS__REFUSED: typical cause: directory does not exist, is not available or is empty
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeNlist(FETEPE_HANDLE handle, const char *n_filter, unsigned int corrections,
  G_STRINGS_HANDLE fileNames);


// Get remote directory's files list
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - n_filter:
//   + NULL pointer: no filter applied
//   + not a NULL pointer: apply this filter
//
// Modified:
// - dirList : listing of directory (includes system-dependent information)
//   in case of FTP server refusal (FETEPE_STATUS__REFUSED) or disconnection
//   (FETEPE_STATUS__NO_COMMAND_CHANNEL), the list is empty (0 item).
//
// Returned:
// - FETEPE_STATUS__OK : files list established. The list is **generally** not empty, but note
//   carefully that some FTP servers return an OK status for (existing but) empty directories.
// - FETEPE_STATUS__REFUSED: typical cause: directory does not exist, is not available or is empty
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeDir(FETEPE_HANDLE handle, const char *n_filter, G_STRINGS_HANDLE dirList);


// Transfer a remote (source) file to local (dest) 
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_sourceFileName:
// - p_destinationFileName:
// - na_getLength: NULL address if NOT used...
//
// Modified:
// - *na_getLength: (if used)
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: remote file does not exist or is not available...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeGet (FETEPE_HANDLE handle, const char *p_remoteFileName, const char *p_localFilePathname,
  int *na_getLength);


// Same as FetepeGet() function above, but use "local" (dest) sucker butt...
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
// - p_remoteFileName:
// - localButtSpotterFillFunction:
// - r_localButtSpotterFillHandle:
//
// Modified:
//
// Returned:
// - FETEPE_STATUS__OK : OK
// - FETEPE_STATUS__REFUSED: typical cause: remote file does not exist or is not available...
//   #SEE FETEPE_STATUS__REFUSED
// - FETEPE_STATUS__NO_COMMAND_CHANNEL : #see FETEPE_STATUS__NO_COMMAND_CHANNEL
// - -1 : unexpected problem; anomaly is raised
int FetepeGet2(FETEPE_HANDLE handle, const char *p_remoteFileName, //const struct SUCKER_BUTT *ap_localButt,
  BUTT_SPOTTER_FILL_FUNCTION localButtSpotterFillFunction, void *r_localButtSpotterFillHandle,
  int *na_getLength);


// Log off and disconnect "physically".
//
// Passed:
// - handle: connection handle, see FetepeCreateInstance()
//
// Returned:
// - RETURNED: you are no more connected, whatever !
// - -1 : unexpected problem; anomaly is raised
int FetepeDisconnect (FETEPE_HANDLE handle) ;


// Destroy fetepe instance.
//
// Passed:
// - xh_handle: handle to liquidate...
//
// Modified:
// - xh_handle: this handle is no more valid 
//
// Returned:
// - RETURNED :
// - -1 : unexpected problem; anomaly is raised
int FetepeDestroyInstance(FETEPE_HANDLE xh_handle);


#endif // __C_POSIX_FETEPE_H_INCLUDED
