// c-posix/fetepe.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <errno.h>
#include <stdarg.h>
#include <ctype.h>

#include "c-ansi/g-string.h"
#include "c-ansi/c-string.h"
#include "c-ansi/eol.h"
#include "c-ansi/alloc.h"

#include "c-posix/fetepe.h"
#include "c-posix/erw.h"
#include "c-posix/rwn.h"
#include "c-posix/rwf.h"

#include "c-ansi/diggy.h"
//#include "c-posix/testy-diggy.h"


// Principal FTP commands (alla printf format string)
// String format arg(s), if present, simply correspond to FTP command arg(s)
//
// USER => %s : user name
#define USER__FTP_COMMAND__FMT_S	"USER %s"
// PASS => %s : password
#define PASS__FTP_COMMAND__FMT_S	"PASS %s"
// TYPE => %c : 'I' or 'A'
#define TYPE__FTP_COMMAND__FMT_C	"TYPE %c"
// NOOP => no arg
#define NOOP__FTP_COMMAND__FMT  	"NOOP"
// PWD => no arg
#define  PWD__FTP_COMMAND__FMT  	"PWD"
// SYST => no arg
#define SYST__FTP_COMMAND__FMT  	"SYST"
// CWD => %s : dir name
#define  CWD__FTP_COMMAND__FMT_S	"CWD %s"
// RNFR => %s : old name
#define RNFR__FTP_COMMAND__FMT_S	"RNFR %s"
// RNTO => %s : new name
#define RNTO__FTP_COMMAND__FMT_S	"RNTO %s"
// DELE => file name
#define DELE__FTP_COMMAND__FMT_S	"DELE %s"
// PASV => no arg
#define PASV__FTP_COMMAND__FMT  	"PASV"
// PORT => %u %u %u %u : IP address  ;  %u %u : TCP port
#define PORT__FTP_COMMAND__FMT_6U	"PORT %u,%u,%u,%u,%u,%u"
// QUIT => no arg
#define QUIT__FTP_COMMAND__FMT   	"QUIT"
// STOR => %s : file name
#define STOR__FTP_COMMAND__FMT_S	"STOR %s"
// RETR => %s : file name
#define RETR__FTP_COMMAND__FMT_S	"RETR %s"

// Generic format strings to generate FTP commands with "dynamic" number of arguments...
//
// When you want to generate a command without argument
// %s : FTP command name
#define  SIMPLE_FTP_COMMAND__FMT_S  	"%s"
// When you want to generate a command with one argument (passed as string)
// %s : FTP command name
// %s : FTP command argument (passed as string)
#define ONE_ARG_FTP_COMMAND__FMT_S_S	"%s %s"
// FTP commands strings usable with Generic format strings above
#define NLST__FTP_COMMAND	"NLST"
#define LIST__FTP_COMMAND	"LIST"

// Specific ftp response status format 
#define ENTERING_PASSIVE_MODE_STATUS__FMT_6U  "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u)"



// During a transfer, indicates whether it's the "source" that is local - and then the recipient
// is remote - or if it's the contrary (the "source" is remote...)
enum { // #REF S_BUTT-enum During operation implying data sucking, the "source" butt...
  S_BUTT_IS__UNKNOWN, // ...is unknown (the operation does not implies data sucking)
  S_BUTT_IS__LOCAL, // ...is a LOCAL file of ftp client ("ftp put"...)
  S_BUTT_IS__REMOTE, // ...is a REMOTE file, on ftp server ("ftp get"...)
} ;


struct FETEPE {
  char b_passive ;

  struct {
    struct {
      int connectErrno ;
      int rwStatus ;
      int ftpCommandStatus ;
      char b_ftpServerRefusal ;
    } outline ;
    struct {
      FETEPE_LOG_COMMAND_STATUS_FUNCTION n_logFunction ;
      void *cr_logFunctionHandle ;
      GREEN_COLLECTION_HANDLE h_linesPartitionHandle;
      G_STRING_STUFF h_line ;
    } status ;
    struct {
      ERW_HANDLE h_erwHandle ;
      int nh_connectedDescriptor ; // -1 special value: not initialized
      in_addr_t c_inetAddr ; // Only significant if descriptor is initialized
    } socket ;
  } command ;

  struct {
    struct {
      int bindOrListenOrConnectErrno;
      int rwnAcceptStatus;
      char b_closeConnectionLost;
      struct {
        int localOpenErrno;
        int suckerStatus;
        int r_flopCause;
        int sButtIs; // #SEE S_BUTT-enum
      } sucker ;
    } outline;
    PD_HANDLE h_listeningSocketPdHandle ;
    SUCKER_HANDLE h_suckerHandle ;
    int nonPosixOpenFlags;
    int nh_streamDescriptor; // >= 0 => open stream
    STREAM_BUTT_ADEPT_HANDLE h_streamButtAdeptHandle; 
    STREAM_BUTT_ADEPT_HANDLE h_localStreamButtAdeptHandle; 
    G_STRING_STUFF h_lsOutput ;
    G_STRING_BUTT_ADEPT_HANDLE h_lsOutputButtAdeptHandle; 
    GREEN_COLLECTION_HANDLE h_lsLines ;
  } data ;
} ;

// Reset all "outline"-related data
#define m_RESET_OUTLINE(handle) {\
  (handle)->command.outline.connectErrno = 0;\
  (handle)->command.outline.rwStatus = RW_STATUS__OK;\
  (handle)->command.outline.ftpCommandStatus = COMMAND_EXECUTED__FTP_COMMAND_STATUS;\
  (handle)->command.outline.b_ftpServerRefusal = b_FALSE0;\
  (handle)->data.outline.bindOrListenOrConnectErrno = 0;\
  (handle)->data.outline.rwnAcceptStatus = RWN_ACCEPT_STATUS__OK;\
  (handle)->data.outline.b_closeConnectionLost = b_FALSE0;\
  (handle)->data.outline.sucker.localOpenErrno = 0;\
  (handle)->data.outline.sucker.sButtIs = S_BUTT_IS__UNKNOWN;\
  (handle)->data.outline.sucker.r_flopCause = FLOP_CAUSE__NONE0;\
  (handle)->data.outline.sucker.suckerStatus = SUCKER_STATUS__OK;\
}


// custom function (see CHECK_READ_FUNCTION @ c-posix/erw.h)
static int CheckReadFtpCommandStatus (void * r_virtualHandle, const char *p_readBuffer,
  int readLength, int attemptsCount, int *ac_messageLength) {
  m_DIGGY_BOLLARD_S() ;
  GREEN_COLLECTION_HANDLE linesPartitionHandle = (GREEN_COLLECTION_HANDLE) r_virtualHandle ;
  int answer = ANSWER__NO; // a priori 

  m_ASSERT(readLength > 0)
  m_ASSIGN_LOCAL_STRING_PORTION(readPortion,p_readBuffer,readLength)
  int count = ParseAsciiLines(readPortion,0,ac_messageLength,linesPartitionHandle);
  m_TRACK_IF(count < 0)

  LINE_DELIMITOR_STUFF n_lastLineDelimitorStuff = (LINE_DELIMITOR_STUFF) UNDEFINED;
  m_TRACK_IF(LINES_PARTITION_GET_COUNT(linesPartitionHandle,&n_lastLineDelimitorStuff) < 0)

  if (n_lastLineDelimitorStuff != NULL) {
    int practicalLength = UNDEFINED;
    practicalLength = m_StringPortionLength(&n_lastLineDelimitorStuff->practicalLine);
    if (practicalLength >= 4 && isdigit(n_lastLineDelimitorStuff->practicalLine.string[0]) && 
      isdigit(n_lastLineDelimitorStuff->practicalLine.string[1]) && 
      isdigit(n_lastLineDelimitorStuff->practicalLine.string[2]) &&
      n_lastLineDelimitorStuff->practicalLine.string[3] == ' ')  answer = ANSWER__YES;
  } // if

  m_DIGGY_RETURN(answer)
} // CheckReadFtpCommandStatus 


static const struct WAITING_PLAN p_fetepeWaitingPlan = {
  .waitingPlan = DEEPLY_BLOCKING__WAITING_PLAN ,
  .c_deadline = 30 ,
} ;
  

// Public function: see .h
int FetepeCreateInstance (FETEPE_HANDLE *azh_handle, ALARM_SYSTEM_HANDLE nf_alarmSystemHandle,
  BROKEN_PIPE_FIX_HANDLE f_brokenPipeFixHandle, int transferSizeLimit,
  FETEPE_LOG_COMMAND_STATUS_FUNCTION n_logCommandStatusFunction,
  void *cfr_logCommandStatusFunctionHandle) {
  m_DIGGY_BOLLARD()
  m_MALLOC_INSTANCE(*azh_handle)
  FETEPE_HANDLE handle = *azh_handle;

  handle->b_passive = b_FALSE0;
  m_RESET_OUTLINE(handle)

  // command:
  if ((handle->command.status.n_logFunction = n_logCommandStatusFunction) != NULL) {
    handle->command.status.cr_logFunctionHandle = cfr_logCommandStatusFunctionHandle ;
  } // if
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&handle->command.status.h_line) != RETURNED)
  m_TRACK_IF(LINES_PARTITION_CREATE_INSTANCE(&handle->command.status.h_linesPartitionHandle,10,
    NULL,(void *)UNDEFINED) != RETURNED)
  handle->command.socket.nh_connectedDescriptor = -1;
  m_TRACK_IF(ErwCreateInstance(&handle->command.socket.h_erwHandle, f_brokenPipeFixHandle,
    nf_alarmSystemHandle, 256,NULL, CheckReadFtpCommandStatus,
    (void *) handle->command.status.h_linesPartitionHandle, NULL) != RETURNED)

  // data:
  handle->data.nh_streamDescriptor = -1;
  m_TRACK_IF(StreamButtAdeptCreateInstance(&handle->data.h_streamButtAdeptHandle,
    nf_alarmSystemHandle, f_brokenPipeFixHandle,
    p_fetepeWaitingPlan, BATEAU__FILE_BUTT_ADEPT__READ_BUFFER_SIZE, -1) != RETURNED)
  m_TRACK_IF(StreamButtAdeptCreateInstance(&handle->data.h_localStreamButtAdeptHandle,
    nf_alarmSystemHandle, f_brokenPipeFixHandle,
    p_fetepeWaitingPlan, BATEAU__FILE_BUTT_ADEPT__READ_BUFFER_SIZE, -1) != RETURNED)
  handle->data.nonPosixOpenFlags = 0;
  m_TRACK_IF(PdCreateInstance(&handle->data.h_listeningSocketPdHandle,nf_alarmSystemHandle) != RETURNED)
  m_TRACK_IF(SuckerCreateInstance(&handle->data.h_suckerHandle, transferSizeLimit) != RETURNED)
  m_TRACK_IF(LINES_PARTITION_CREATE_INSTANCE(&handle->data.h_lsLines,100, NULL,(void *)UNDEFINED)
    != RETURNED)
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&handle->data.h_lsOutput) != RETURNED)
  m_TRACK_IF(GStringButtAdeptCreateInstance(&handle->data.h_lsOutputButtAdeptHandle,
    handle->data.h_lsOutput) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // FetepeCreateInstance


// Public function: see .h
int FetepeGetLastFtpCommandStatus (FETEPE_HANDLE handle, int *a_ftpCommandStatus,
  int *na_localOpenErrno) {
  m_DIGGY_BOLLARD()
  *a_ftpCommandStatus = handle->command.outline.ftpCommandStatus ;

  if (na_localOpenErrno != NULL) {
    *na_localOpenErrno = handle->data.outline.sucker.localOpenErrno ;
  } // if

  m_DIGGY_RETURN((handle->command.outline.b_ftpServerRefusal ? COMPLETED__OK: COMPLETED__BUT))
} // FetepeGetLastFtpCommandStatus



// Indicates the "origin" of a transfer "flop"....
enum {
  FLOP_ORIGIN__UNKNOWN,
  FLOP_ORIGIN__LOCAL,
  FLOP_ORIGIN__REMOTE,
} ;

// Retrieve the origin (i.e "culprit") of the flop
//
// Passed:
// - suckerStatus
// - sButtIs
//
// Returned:
// - >= 0: flop origin
//   (FLOP_ORIGIN__UNKNOWN / LOCAL / REMOTE)
// - -1: anomaly is raised
static int GetFlopOrigin (int suckerStatus, int sButtIs) {
  m_DIGGY_BOLLARD_S()
  int flopOrigin = FLOP_ORIGIN__UNKNOWN ;

  switch (suckerStatus) {
  case SUCKER_STATUS__OK:
  case SUCKER_STATUS__LIMIT_BREACHED:
  break; case SUCKER_STATUS__S_FLOP:
    switch (sButtIs) {
    case S_BUTT_IS__UNKNOWN:
    break; case S_BUTT_IS__LOCAL:
      flopOrigin = FLOP_ORIGIN__LOCAL;
    break; case S_BUTT_IS__REMOTE:
      flopOrigin = FLOP_ORIGIN__REMOTE;
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,sButtIs)
    } // switch
  break; case SUCKER_STATUS__D_FLOP:
    switch (sButtIs) {
    case S_BUTT_IS__UNKNOWN:
    break; case S_BUTT_IS__LOCAL:
      flopOrigin = FLOP_ORIGIN__REMOTE;
    break; case S_BUTT_IS__REMOTE:
      flopOrigin = FLOP_ORIGIN__LOCAL;
    break; default:
      m_RAISE(ANOMALY__VALUE__FMT_D,sButtIs)
    } // switch
  break; default:
    m_RAISE(ANOMALY__VALUE__FMT_D,suckerStatus)
  } // switch

  m_DIGGY_RETURN(flopOrigin)
} // GetFlopOrigin

// Public function: see .h
const char *FetepeGetClue (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD()
  const char *clue = "n/a" ;

  if (handle->command.outline.connectErrno != 0) {
    clue = "Cannot connect to FTP server.";
  } else if (handle->command.outline.rwStatus == RW_STATUS__TRY_AGAIN) {
    clue = "Timeout while sending or receiving command on FTP server.";
  } else if (handle->command.outline.rwStatus == RW_STATUS__TERMINATING) {
    clue = "FTP server unexpectedly shut command channel.";
  } else if (handle->command.outline.rwStatus == RW_STATUS__CONNECTION_LOST) {
    clue = "Lost (command channel) connection with FTP server.";
  } else if (handle->command.outline.b_ftpServerRefusal) {
    clue = "Command refused by FTP server.";
  } else if (handle->data.outline.sucker.localOpenErrno != 0) {
    clue = "Cannot open LOCAL file.";
  } else if (handle->data.outline.rwnAcceptStatus != RWN_ACCEPT_STATUS__OK) {
    if (handle->data.outline.rwnAcceptStatus == RWN_ACCEPT_STATUS__TRY_AGAIN) {
      clue = "FTP server missed to open data channel.";
    } else { // RWN_ACCEPT_STATUS__INTERCEDED
      clue = "A firewall prevents from opening ftp data channel.";
    } // if
  } else if (handle->data.outline.bindOrListenOrConnectErrno != 0) {
    if (handle->b_passive) {
      clue = "Cannot create data channel; LOCAL host is out of TCP port.";
    } else {
      clue = "Cannot create data channel; could not reach DATA port on FTP server.";
    } // if

  } else {
    char b_clearClue = b_TRUE ; // a priori
    int flopOrigin = GetFlopOrigin(handle->data.outline.sucker.suckerStatus,
      handle->data.outline.sucker.sButtIs);
    m_RAISE_VERBATIM_IF_V(NULL, flopOrigin < 0)

    switch (handle->data.outline.sucker.suckerStatus) {
    case SUCKER_STATUS__OK:
      b_clearClue = b_FALSE0 ;
    break; case SUCKER_STATUS__LIMIT_BREACHED:
      clue = "Volume of copied data exceeds allowed limit.";
    break; case SUCKER_STATUS__S_FLOP:
    case SUCKER_STATUS__D_FLOP:
      if (handle->data.outline.sucker.r_flopCause == STREAM_FLOP_CAUSE__TIMEOUT) {
        if (flopOrigin == FLOP_ORIGIN__REMOTE) {
          clue = "Timeout while transferring data - REMOTE server hanged.";
        } else { // FLOP_ORIGIN__LOCAL
          clue = "Timeout while transferring data - LOCAL stream hanged.";
        } // if
      } else if (handle->data.outline.sucker.r_flopCause == STREAM_FLOP_CAUSE__BROKEN_PIPE) {
        if (flopOrigin == FLOP_ORIGIN__REMOTE) {
          clue = "FTP server unexpectedly closed the data channel.";
        } else { // FLOP_ORIGIN__LOCAL
          clue = "LOCAL stream closed unexpectedly; LOCAL file system may be full.";
        } // if
      } else if (handle->data.outline.sucker.r_flopCause ==  STREAM_FLOP_CAUSE__CONNECTION_LOST) {
        if (flopOrigin == FLOP_ORIGIN__REMOTE) {
          clue = "Data channel with FTP server broken.";
        } else { // FLOP_ORIGIN__LOCAL
          clue = "I/O failure on LOCAL data stream.";
        } // if
      } // if 
    break; default:
      m_RAISE_V(NULL,ANOMALY__VALUE__FMT_D,handle->data.outline.sucker.suckerStatus)
    } // switch

    if (!b_clearClue) {
      if (handle->data.outline.b_closeConnectionLost) {
        clue = "I/O failure reported while closing DATA channel; possible loss of data.";
      } else if (handle->command.socket.nh_connectedDescriptor < 0) {
        clue = "NOT connected to FTP server.";
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(clue)
} // FetepeGetClue


// Ret:
// - RETURNED: ok
// - -1: anomaly is raised
static int FetepeCloseCommandChannel (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD_S()
  if (handle->command.socket.nh_connectedDescriptor != -1) {

    m_PROTECTED_CLOSE(handle->command.socket.nh_connectedDescriptor,)
    handle->command.socket.nh_connectedDescriptor = -1;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // FetepeCloseCommandChannel


// Retrieve last (i.e actual) status line command response sent by FTP server.
// 
// Passed:
// - m_action: action to execute (with lastLineDelimitorStuff)
//
// Created (local):
// - commandStatusLinesCount: actual number of lines of the ftp command response 
#define m_ACTION_ON_LAST_COMMAND_STATUS_LINE(m_action) int commandStatusLinesCount = UNDEFINED;\
{\
  LINE_DELIMITOR_STUFF lastLineDelimitorStuff;\
  commandStatusLinesCount = LINES_PARTITION_GET_COUNT((handle)->command.status.h_linesPartitionHandle,\
    &lastLineDelimitorStuff) ;\
  m_TRACK_IF(commandStatusLinesCount < 0)\
  m_ASSERT(lastLineDelimitorStuff != NULL)\
  m_action \
}

// Update general FETEPE status according to FTP status answered by FTP server.
// 
// Passed: 
// - m_fetepeStatus: current FETEPE status (will not be modified if already != 'Ok')
// - n_successfulFtpCommandStatus: 
//   + -1 special value: DO nothing (DO NOT modify FETEPE status)
//   + >= 0: ftp response status corresponding to 'success'
// 
// Modified:
// - m_fetepeStatus: updated accordingly if ftp command is not 'successful' 
#define m_CHECK_COMMAND_STATUS(/* int */ m_fetepeStatus, /* int */ n_successfulFtpCommandStatus) \
if ((n_successfulFtpCommandStatus) != -1 && \
    (m_fetepeStatus) == FETEPE_STATUS__OK && \
    handle->command.outline.ftpCommandStatus != (n_successfulFtpCommandStatus)) { \
  handle->command.outline.b_ftpServerRefusal = b_TRUE; \
  (m_fetepeStatus) =  FETEPE_STATUS__REFUSED; \
}


// Passed:
// - handle: 
// - *a_fetepeStatus: current FETEPE status
//
// Changed:
// - *a_fetepeStatus: updated FETEPE status
//
// Returned :
// - RETURNED:
// - -1 : unexpected problem
static int FetepeReadCommandStatus (FETEPE_HANDLE handle, int *a_fetepeStatus, 
  int n_successfulFtpCommandStatus) {
  m_DIGGY_BOLLARD_S()
  if (handle->command.socket.nh_connectedDescriptor == -1) {
    *a_fetepeStatus = FETEPE_STATUS__NO_COMMAND_CHANNEL;
    m_DIGGY_RETURN(RETURNED)
  } // if

  GreenCollectionClear(handle->command.status.h_linesPartitionHandle);
  int commandStatusLength = UNDEFINED;
  char *commandStatusBufferPtr = (char *)UNDEFINED;

  switch (handle->command.outline.rwStatus = ErwRead(handle->command.socket.h_erwHandle,
    NULL,&commandStatusBufferPtr, &commandStatusLength)) {
  case RW_STATUS__OK : 
    { // Scan command status lines 

      LINE_DELIMITOR_STUFF t_lineDelimitorStuff;
      int commandStatusLinesCount = LINES_PARTITION_GET_COUNT(
        handle->command.status.h_linesPartitionHandle, &t_lineDelimitorStuff); 
      m_TRACK_IF(commandStatusLinesCount < 0)\
      m_ASSERT(t_lineDelimitorStuff != NULL)
      switch (SScanfStringPortion(t_lineDelimitorStuff->practicalLine,"%3d",
        &handle->command.outline.ftpCommandStatus))  {
      case 1:
      break; case 0:
        m_RAISE(ANOMALY__UNEXPECTED_CASE) // TODO: what else ??
      break; default:
        m_TRACK()
      } // if

      if (handle->command.status.n_logFunction != NULL) {
        int i = 0;
        for (; i < commandStatusLinesCount ; i++) {
          m_TRACK_IF(LINES_PARTITION_FETCH(handle->command.status.h_linesPartitionHandle,i,
            &t_lineDelimitorStuff) < 0)

          m_TRACK_IF(handle->command.status.n_logFunction(
            handle->command.status.cr_logFunctionHandle, t_lineDelimitorStuff->practicalLine,
            i == commandStatusLinesCount - 1) != RETURNED)
        } // for 
      } // if 
    } // Scan command status lines 

    m_CHECK_COMMAND_STATUS(*a_fetepeStatus, n_successfulFtpCommandStatus)

  break; case RW_STATUS__TRY_AGAIN:
  case RW_STATUS__TERMINATING : 
  case RW_STATUS__CONNECTION_LOST : 
    *a_fetepeStatus = FETEPE_STATUS__NO_COMMAND_CHANNEL;
    m_TRACK_IF(FetepeCloseCommandChannel(handle) != RETURNED)
  break; default : // Unexpected problem
    m_TRACK() ;
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // FetepeReadCommandStatus


// Passed:
// - handle: 
// - *a_fetepeStatus: current FETEPE status
// - p_commandFormat: 
// - commandArguments:
//
// Changed:
// - *a_fetepeStatus: updated FETEPE status
//
// Returned :
// - RETURNED:
// - -1 : unexpected problem
static int FetepeWriteCommandV (struct FETEPE *handle, int *a_fetepeStatus,
  const char *p_commandFormat, va_list commandArguments) {
  m_DIGGY_BOLLARD_S()
  if (handle->command.socket.nh_connectedDescriptor == -1) {
    *a_fetepeStatus = FETEPE_STATUS__NO_COMMAND_CHANNEL;
    m_DIGGY_RETURN(RETURNED)
  } // if

  int length = UNDEFINED;
  char command[256] ;

  vsnprintf(command,sizeof(command)-2,p_commandFormat,commandArguments) ;
  
  length = strlen(command) ;
  command[length++] = '\r' ;
  command[length++] = '\n' ;
  switch (handle->command.outline.rwStatus = ErwWrite2(handle->command.socket.h_erwHandle,
    NULL,command,length,NULL)) {
  case RW_STATUS__OK: 
  break; case RW_STATUS__TRY_AGAIN:
  case RW_STATUS__TERMINATING : 
  case RW_STATUS__CONNECTION_LOST : 
    *a_fetepeStatus = FETEPE_STATUS__NO_COMMAND_CHANNEL;
    m_TRACK_IF(FetepeCloseCommandChannel(handle) != RETURNED)
  break; default : // Unexpected problem
    m_TRACK() ;
  } // switch

  m_DIGGY_RETURN(RETURNED)
} // FetepeWriteCommandV

// Passed:
// - handle: 
// - *a_fetepeStatus: current FETEPE status
// - p_commandFormat: 
// - ...: variadic arguments of p_commandFormat
//
// Changed:
// - *a_fetepeStatus: updated FETEPE status
//
// Returned :
// - RETURNED:
// - -1 : unexpected problem
static int FetepeWriteCommand (struct FETEPE *handle, int *a_fetepeStatus,
  const char *p_commandFormat,...) {
  m_DIGGY_BOLLARD_S()
  va_list commandArguments ;
  va_start(commandArguments,p_commandFormat) ;
  m_TRACK_IF(FetepeWriteCommandV(handle,a_fetepeStatus,p_commandFormat,commandArguments) != RETURNED)
  va_end(commandArguments) ;
  
  m_DIGGY_RETURN(RETURNED)
} // FetepeWriteCommand 


// Public function ; see description in .h
int FetepeConnect (FETEPE_HANDLE handle, const char *p_hostIpAddress, const char *p_user,
  const char *np_password, int *na_connectErrno) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK ; // a priori
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeCloseCommandChannel(handle) != RETURNED)
  m_ASSERT(handle->command.socket.nh_connectedDescriptor == -1)

  switch (CreateAndConnect(&handle->command.socket.nh_connectedDescriptor,
    inet_addr(p_hostIpAddress), htons(21), &handle->command.outline.connectErrno)) {
  case ATTEMPT__SUCCESSFUL: 
  break; case ATTEMPT__TRY_AGAIN: 
    handle->command.socket.nh_connectedDescriptor = -1 ; // not connected
  break; default :
    m_TRACK() 
  } // switch  
  if (na_connectErrno != NULL) {
    *na_connectErrno = handle->command.outline.connectErrno;
  } // switch

  if (handle->command.socket.nh_connectedDescriptor == -1) {
    fetepeStatus = FETEPE_STATUS__NO_COMMAND_CHANNEL;
  } else {
    switch (ErwReset(handle->command.socket.h_erwHandle,
      handle->command.socket.nh_connectedDescriptor, &p_fetepeWaitingPlan)) {
    case COMPLETED__OK:
    case COMPLETED__BUT:
    break; default :
      m_TRACK()
    } // switch
  
    { struct sockaddr_in sockaddrIn ;
      socklen_t size ;

      size = sizeof(sockaddrIn) ;
      m_PERROR_VERBATIM_IF(getsockname(handle->command.socket.nh_connectedDescriptor,
        (struct sockaddr *)&sockaddrIn,&size) < 0)

      handle->command.socket.c_inetAddr = sockaddrIn.sin_addr.s_addr ;
    } // sockaddrIn 

    // Logical connection:

    // Pass welcome message
    m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus,SERVICE_IS_AVAILABLE__FTP_COMMAND_STATUS) != RETURNED)
  } // if
  
  if (fetepeStatus == FETEPE_STATUS__OK) {
    m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,USER__FTP_COMMAND__FMT_S,p_user) != RETURNED)
    m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus,-1) != RETURNED)
    if (fetepeStatus == FETEPE_STATUS__OK &&
        handle->command.outline.ftpCommandStatus == ASK_FOR_PASSWORD__FTP_COMMAND_STATUS) {
       m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,PASS__FTP_COMMAND__FMT_S,
         np_password != NULL? np_password: p_user) != RETURNED)
       m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus,-1) != RETURNED)
    } // if
    m_CHECK_COMMAND_STATUS(fetepeStatus, SESSION_IS_OPEN__FTP_COMMAND_STATUS)
  } // if
  
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeConnect


// Passed:
// - handle:
// - letter: 'A' / 'I'
//
// Returned:
// - FETEPE_STATUS__OK :
// - FETEPE_STATUS__REFUSED :
// - FETEPE_STATUS__NO_COMMAND_CHANNEL :
// - -1 : unexpected problem; anomaly is raised
static int FetepeSetType (FETEPE_HANDLE handle, char letter) {
  m_DIGGY_BOLLARD_S()
  int fetepeStatus = FETEPE_STATUS__OK ;

  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,TYPE__FTP_COMMAND__FMT_C,letter) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, COMMAND_EXECUTED__FTP_COMMAND_STATUS) != RETURNED)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeSetType

// Public function ; see description in .h
int FetepeAscii (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD()

  int fetepeStatus = FetepeSetType(handle,'A') ;
  m_TRACK_IF(fetepeStatus < 0)
  if (fetepeStatus == FETEPE_STATUS__OK) {
    handle->data.nonPosixOpenFlags = TEXT__NON_POSIX_OPEN_FLAG;
  } // if
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeAscii

// Public function ; see description in .h
int FetepeBinary (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD()

  int fetepeStatus = FetepeSetType(handle,'I') ;
  m_TRACK_IF(fetepeStatus < 0)
  if (fetepeStatus == FETEPE_STATUS__OK) {
    handle->data.nonPosixOpenFlags = BINARY__NON_POSIX_OPEN_FLAG;
  } // if
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeAscii


// Public function ; see description in .h
int FetepeNoop (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD() ;
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,NOOP__FTP_COMMAND__FMT) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, COMMAND_EXECUTED__FTP_COMMAND_STATUS) != RETURNED)
  
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeNoop


// Public function ; see description in .h
int FetepePwd (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD() ;
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,PWD__FTP_COMMAND__FMT) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, PATHNAME_CREATED__FTP_COMMAND_STATUS) != RETURNED)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepePwd


// Public function ; see description in .h
int FetepeSystem (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD() ;
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,SYST__FTP_COMMAND__FMT) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, COMMAND_EXECUTED__FTP_COMMAND_STATUS) != RETURNED)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeSystem


// Public function ; see description in .h
int FetepeCd (FETEPE_HANDLE handle, const char *directoryName) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,CWD__FTP_COMMAND__FMT_S,directoryName) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, FILE_SERVICE_OK__FTP_COMMAND_STATUS) != RETURNED)
  
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeCd


// Public function ; see description in .h
int FetepeRename (FETEPE_HANDLE handle, const char *oldName, const char *newName) {
  m_DIGGY_BOLLARD() ;
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,RNFR__FTP_COMMAND__FMT_S,oldName) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus,
    FILE_SERVICE_WAITING_FOR_COMPLEMENTARY_INFORMATION__FTP_COMMAND_STATUS) != RETURNED)

  if (fetepeStatus == FETEPE_STATUS__OK) {
    m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,RNTO__FTP_COMMAND__FMT_S,newName) != RETURNED)
    m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, FILE_SERVICE_OK__FTP_COMMAND_STATUS) != RETURNED)
  } // if

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeRename 


// Public function ; see description in .h
int FetepeDelete (FETEPE_HANDLE handle, const char *name) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK ;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,DELE__FTP_COMMAND__FMT_S,name) != RETURNED)
  m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus, FILE_SERVICE_OK__FTP_COMMAND_STATUS) != RETURNED)
  
  m_DIGGY_RETURN(fetepeStatus)
} // FetepeDelete


// Public function ; see description in .h
int FetepePassive (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD() ;

  handle->b_passive = ! handle->b_passive;

  m_DIGGY_RETURN(handle->b_passive? ANSWER__YES: ANSWER__NO)
} // FetepePassive


// "Active" mode :
// - Initialize listening socket for data transfert
// - Accept connection of FTP server
// "Passive" mode:
// - Directly connect to FTP server (using passive port) 
//
// Passed parameters : 
// - handle:
// - *a_fetepeStatus: current status
// - p_dataCommandFormat: the command that will open the "data stream" ("a la printf" format string)
// - ... : arguments of command's "a la printf" format string
//
// Changed:
// - *a_fetepeStatus: updated status
//
// Returned:
// - RETURNED:
// -1 : unexpected problem
static int FetepeOpenDataStream (FETEPE_HANDLE handle, int *a_fetepeStatus,
  const char *p_dataCommandFormat, ...) {
  m_DIGGY_BOLLARD_S()
  int nh_listeningSocketDescriptor = -1 ;

  m_ASSERT(handle->data.nh_streamDescriptor == -1)

  if (handle->b_passive) {
    m_TRACK_IF(FetepeWriteCommand(handle,a_fetepeStatus,PASV__FTP_COMMAND__FMT) != RETURNED)
    m_TRACK_IF(FetepeReadCommandStatus(handle,a_fetepeStatus, 
      ENTERING_PASSIVE_MODE__FTP_COMMAND_STATUS) != RETURNED)
    if (*a_fetepeStatus == FETEPE_STATUS__OK) {
      int ret = UNDEFINED;
      unsigned p1, p2 = UNDEFINED;
      unsigned h1, h2, h3, h4 = UNDEFINED;

      LINE_DELIMITOR_STUFF lastLineDelimitorStuff;
      int commandStatusLinesCount = LINES_PARTITION_GET_COUNT(
        handle->command.status.h_linesPartitionHandle, &lastLineDelimitorStuff);
      m_TRACK_IF(commandStatusLinesCount < 0)
      m_ASSERT(lastLineDelimitorStuff != NULL)
      ret = SScanfStringPortion(lastLineDelimitorStuff->practicalLine, 
        ENTERING_PASSIVE_MODE_STATUS__FMT_6U, &h1,&h2,&h3,&h4, &p1,&p2); 
      m_TRACK_IF(ret < 0)
      m_ASSERT(ret == 6)
      switch (CreateAndConnect(&(handle->data.nh_streamDescriptor), RWN_INET_ADDR(h1,h2,h3,h4),
        RWN_INET_PORT(p1,p2), &handle->data.outline.bindOrListenOrConnectErrno)) {
      case ATTEMPT__SUCCESSFUL: 
      break; case ATTEMPT__TRY_AGAIN: 
        handle->data.nh_streamDescriptor = -1 ; // not connected
        *a_fetepeStatus = FETEPE_STATUS__REFUSED;
      break; default :
        m_TRACK()
      } // switch
    } // if

  } else {
    handle->command.outline.b_ftpServerRefusal = b_FALSE0 ; // a priori

    switch (CreateBindAndListen(&nh_listeningSocketDescriptor,htons(0), RWN_BACKLOG,
      &handle->data.outline.bindOrListenOrConnectErrno)) {
    case ATTEMPT__SUCCESSFUL :
    break; case ATTEMPT__TRY_AGAIN :
      nh_listeningSocketDescriptor = -1 ;
      if (*a_fetepeStatus == FETEPE_STATUS__OK) {
        *a_fetepeStatus = FETEPE_STATUS__REFUSED;
      } // if
    break; default :
      m_TRACK()
    } // switch

    if (nh_listeningSocketDescriptor != -1) {
      in_port_t inetPort = (in_port_t) UNDEFINED;

      { struct sockaddr_in sockaddrIn ;
        socklen_t size = UNDEFINED;
    
        size = sizeof(sockaddrIn) ;
        m_PERROR_VERBATIM_IF(getsockname(nh_listeningSocketDescriptor,
          (struct sockaddr *)&sockaddrIn,&size) < 0)
        inetPort = sockaddrIn.sin_port ;
      } // sockaddrIn

#define U_ARG_BYTE_OF_INET(m_inet, i) \
(unsigned int ) RWN_BYTE_OF_INET(m_inet,i)

      m_TRACK_IF(FetepeWriteCommand(handle,a_fetepeStatus,PORT__FTP_COMMAND__FMT_6U,
        U_ARG_BYTE_OF_INET(handle->command.socket.c_inetAddr,0),
        U_ARG_BYTE_OF_INET(handle->command.socket.c_inetAddr,1),
        U_ARG_BYTE_OF_INET(handle->command.socket.c_inetAddr,2),
        U_ARG_BYTE_OF_INET(handle->command.socket.c_inetAddr,3),
        U_ARG_BYTE_OF_INET(inetPort,0), U_ARG_BYTE_OF_INET(inetPort,1)) != RETURNED)

#undef U_ARG_BYTE_OF_INET

      m_TRACK_IF(FetepeReadCommandStatus(handle,a_fetepeStatus,
        COMMAND_EXECUTED__FTP_COMMAND_STATUS) != RETURNED)
    } // if
  } // if

  if (*a_fetepeStatus == FETEPE_STATUS__OK) {
    { va_list arguments ;

      va_start(arguments,p_dataCommandFormat) ;
      m_TRACK_IF(FetepeWriteCommandV(handle,a_fetepeStatus,p_dataCommandFormat,arguments) != RETURNED)
      va_end(arguments);
    } // arguments

    m_TRACK_IF(FetepeReadCommandStatus(handle,a_fetepeStatus, OPEN_DATA_CHANNEL__FTP_COMMAND_STATUS) != RETURNED)
  } // if

  if ((!handle->b_passive) && *a_fetepeStatus == FETEPE_STATUS__OK) {
    switch (handle->data.outline.rwnAcceptStatus = ProtectedAccept(nh_listeningSocketDescriptor,
      handle->data.h_listeningSocketPdHandle, &p_fetepeWaitingPlan,
      &(handle->data.nh_streamDescriptor), NULL,NULL,NULL)) {
    case RWN_ACCEPT_STATUS__OK :
    break; case RWN_ACCEPT_STATUS__TRY_AGAIN :
    case RWN_ACCEPT_STATUS__INTERCEDED :
      handle->data.nh_streamDescriptor = -1 ;
      *a_fetepeStatus = FETEPE_STATUS__REFUSED;
    break; default :
      m_TRACK() ;
    } // switch
  } // if

  if (handle->data.nh_streamDescriptor >= 0) {
    m_TRACK_IF(StreamButtAdeptKick(handle->data.h_streamButtAdeptHandle,
      handle->data.nh_streamDescriptor) != RETURNED) 
  } // if
  if (nh_listeningSocketDescriptor >= 0) {
    m_PROTECTED_CLOSE(nh_listeningSocketDescriptor, m_RAISE(ANOMALY__CONNECTION_LOST))
  } // if

  m_DIGGY_RETURN(RETURNED)
} // FetepeOpenDataStream 


// Passed parameters : 
// - handle:
// - *a_fetepeStatus: current status
//
// Changed:
// - *a_fetepeStatus: updated status (in very specific case...)
//
// Returned value :
// - RETURNED:
// - -1 : unpexpected problem
static int FetepeCloseDataStream(struct FETEPE *handle, int *a_fetepeStatus) {
  m_DIGGY_BOLLARD_S()
  if (handle->data.nh_streamDescriptor == -1) {
    m_DIGGY_RETURN(RETURNED)
  } // if

  m_PROTECTED_CLOSE(handle->data.nh_streamDescriptor,
    if (*a_fetepeStatus == FETEPE_STATUS__OK) *a_fetepeStatus = FETEPE_STATUS__REFUSED;
    handle->data.outline.b_closeConnectionLost = b_TRUE;
  )
  handle->data.nh_streamDescriptor = -1; 

  m_TRACK_IF(FetepeReadCommandStatus(handle,a_fetepeStatus,
     DATA_CHANNEL_HAS_BEEN_CLOSED__FTP_COMMAND_STATUS) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // FetepeCloseDataStream


// Public function ; see description in .h
int FetepePut3Open (FETEPE_HANDLE handle, const char *p_remoteFileName,
  int *ac_remoteDescriptor) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeOpenDataStream(handle,&fetepeStatus, STOR__FTP_COMMAND__FMT_S,p_remoteFileName)
     == -1)

  if (handle->data.nh_streamDescriptor != -1) {
    m_ASSERT(fetepeStatus == FETEPE_STATUS__OK)
    *ac_remoteDescriptor = handle->data.nh_streamDescriptor;
  } else {
    m_ASSERT(fetepeStatus != FETEPE_STATUS__OK)
  } // if

  m_DIGGY_RETURN(fetepeStatus)
} // FetepePut3Open

// Public function ; see description in .h
int FetepePut3Close (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeCloseDataStream(handle,&fetepeStatus) < 0)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepePut3Close


// Public function ; see description in .h
int FetepePut2 (FETEPE_HANDLE handle, //const struct SUCKER_BUTT *ap_localButt,
  BUTT_ADEPT_SUCK_FUNCTION localButtAdeptSuckFunction, void *r_localButtAdeptSuckHandle,
  const char *p_remoteFileName, int *na_putLength) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK;
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeOpenDataStream(handle,&fetepeStatus, STOR__FTP_COMMAND__FMT_S,p_remoteFileName)
    == -1)

  if (handle->data.nh_streamDescriptor != -1) {
    m_ASSERT(fetepeStatus == FETEPE_STATUS__OK)
    m_TRACK_IF(SuckerPlugSDButts(handle->data.h_suckerHandle, 
      localButtAdeptSuckFunction,r_localButtAdeptSuckHandle,
      StreamButtAdeptFill, handle->data.h_streamButtAdeptHandle) != RETURNED)
    handle->data.outline.sucker.sButtIs = S_BUTT_IS__LOCAL;

    switch (handle->data.outline.sucker.suckerStatus = SuckerSuckOut(handle->data.h_suckerHandle,
      na_putLength, &handle->data.outline.sucker.r_flopCause)) {
    case SUCKER_STATUS__OK:
    break; case SUCKER_STATUS__S_FLOP:
    case SUCKER_STATUS__D_FLOP:
      if (fetepeStatus == FETEPE_STATUS__OK) fetepeStatus = FETEPE_STATUS__REFUSED;
    break; default :
      m_TRACK()
    } // switch
  } else if (na_putLength != NULL) {
    m_ASSERT(fetepeStatus != FETEPE_STATUS__OK)
    *na_putLength = 0;
  } // if

  m_TRACK_IF(FetepeCloseDataStream(handle,&fetepeStatus) < 0)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepePut2


// Public function ; see description in .h
int FetepePut (FETEPE_HANDLE handle, const char *p_localFilePathname, const char *p_remoteFileName,
  int *na_putLength) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__REFUSED ; // a priori
  int ch_localFileDescriptor = UNDEFINED;

  switch (ProtectedOpenFile(p_localFilePathname, READ__BASIC_OPEN_FILE_MODE |
    handle->data.nonPosixOpenFlags, 0, &ch_localFileDescriptor,
    &handle->data.outline.sucker.localOpenErrno)) {
  case ANSWER__YES: 
    m_TRACK_IF(StreamButtAdeptKick(handle->data.h_localStreamButtAdeptHandle,
      ch_localFileDescriptor) != RETURNED)
    fetepeStatus = FetepePut2(handle,StreamButtAdeptSuck,
      handle->data.h_localStreamButtAdeptHandle, p_remoteFileName,na_putLength);
    m_TRACK_IF(fetepeStatus < 0)
    m_PROTECTED_CLOSE(ch_localFileDescriptor,
      if (fetepeStatus == FETEPE_STATUS__OK) fetepeStatus = FETEPE_STATUS__REFUSED;
    )
  break; case ANSWER__NO: 
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(fetepeStatus)
} // FetepePut


// Return :
// - RETURNED: 
// - -1 : unexpected problem
static int CorrectFilename (G_STRING_STUFF fileName, unsigned int corrections) {
  m_DIGGY_BOLLARD_S()
  if (fileName->nhi_string == NULL) {
    m_DIGGY_RETURN(RETURNED)
  } // if

  if (corrections & VMS_VERSION_1_SUFFIX__FILE_NAME_CORRECTION) {
    // VAX : Suppress the ";1" suffix if present  
    // Do NOT correct files with ";n" [n>=2] extension
    if (fileName->c_copiedLength >= 2) {
      char *ptr ;

      ptr = fileName->nhi_string + fileName->c_copiedLength - 2 ;
      if (*ptr == ';' && *(ptr+1) == '1') {
        fileName->c_copiedLength -= 2 ;
        *ptr = '\0' ;
      } // if
    } // if
  } // if

  if (corrections & ASCII_UPPER_CASE__FILE_NAME_CORRECTION) {
    // Convert name to upper case
    m_TRACK_IF(GStringConvert(fileName,  (IS_CHAR_FUNCTION)NULL,toupper) < 0)
  } // if

  m_DIGGY_RETURN(RETURNED)
} // CorrectFileName 


// Passed:
// - handle:
// - p_cmd: "NLST" / "LIST"
// - n_filter:
// - c_corrections:
//
// Modified:
// - list :
//
// Returned:
// - FETEPE_STATUS__OK :
// - FETEPE_STATUS__REFUSED:
// - FETEPE_STATUS__NO_COMMAND_CHANNEL :
// - -1 : unexpected problem; anomaly is raised
static int FetepeNListDir (FETEPE_HANDLE handle, const char *p_cmd, const char *n_filter,
  unsigned int corrections, G_STRINGS_HANDLE list) {
  m_DIGGY_BOLLARD_S()
  int fetepeStatus = FETEPE_STATUS__OK; 
  m_RESET_OUTLINE(handle)

  if (n_filter != NULL) {
    m_TRACK_IF(FetepeOpenDataStream(handle,&fetepeStatus, ONE_ARG_FTP_COMMAND__FMT_S_S,p_cmd,
      n_filter) != RETURNED)
  } else {
    m_TRACK_IF(FetepeOpenDataStream(handle,&fetepeStatus, SIMPLE_FTP_COMMAND__FMT_S,p_cmd) !=
      RETURNED)
  } // if

  if (handle->data.nh_streamDescriptor != -1) {
    m_ASSERT(fetepeStatus == FETEPE_STATUS__OK)
    m_TRACK_IF(SuckerPlugSDButts(handle->data.h_suckerHandle, 
      StreamButtAdeptSuck, handle->data.h_streamButtAdeptHandle,
      GStringButtAdeptFill, handle->data.h_lsOutputButtAdeptHandle) != RETURNED)

    handle->data.outline.sucker.sButtIs = S_BUTT_IS__REMOTE;
    m_G_STRING_CLEAR(handle->data.h_lsOutput)
    switch (handle->data.outline.sucker.suckerStatus = SuckerSuckOut(handle->data.h_suckerHandle,
      NULL, &handle->data.outline.sucker.r_flopCause)) {
    case SUCKER_STATUS__OK:
    break; case SUCKER_STATUS__S_FLOP:
    case SUCKER_STATUS__D_FLOP:
      if (fetepeStatus == FETEPE_STATUS__OK) fetepeStatus = FETEPE_STATUS__REFUSED; 
    break; default :
      m_TRACK()
    } // switch

    m_TRACK_IF(GreenCollectionClear(handle->data.h_lsLines) != RETURNED)
    m_TRACK_IF(ParseAsciiLines(m_GStringGetLogicalStringPortion(handle->data.h_lsOutput), -1,NULL,
      handle->data.h_lsLines) < 0)
    int count = LINES_PARTITION_GET_COUNT(handle->data.h_lsLines,NULL);
    m_TRACK_IF(count < 0)
    GStringsClear(list, b_TRUE) ;
    int i ;

    for (i = 0 ; i < count ; i++) {
      LINE_DELIMITOR_STUFF t_lsLine ;
      G_STRING_STUFF t_item = (G_STRING_STUFF) UNDEFINED;

      m_TRACK_IF(LINES_PARTITION_FETCH(handle->data.h_lsLines,i,&t_lsLine) != i)
      m_TRACK_IF(GStringsFetch(list,-1,&t_item) != i)
      m_ASSERT(t_item != NULL)
      m_TRACK_IF(GStringCopy(t_item, 0, &t_lsLine->practicalLine) < 0)
      if (corrections != NO__FILE_NAME_CORRECTION) {
        m_TRACK_IF(CorrectFilename(t_item,corrections) != RETURNED)
      } // if
    } // for
  } // if

  m_TRACK_IF(FetepeCloseDataStream(handle,&fetepeStatus) != RETURNED)

  if (fetepeStatus != FETEPE_STATUS__OK) {
    GStringsClear(list, b_TRUE) ;
  } // if

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeNListDir


// Public function ; see description in .h
int FetepeNlist (FETEPE_HANDLE handle, const char *n_filter, unsigned int corrections,
  G_STRINGS_HANDLE filesNames) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FetepeNListDir(handle,NLST__FTP_COMMAND,n_filter,corrections,filesNames);
  m_TRACK_IF(fetepeStatus < 0)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeNlist



// Public function ; see description in .h
int FetepeDir (FETEPE_HANDLE handle, const char *n_filter, G_STRINGS_HANDLE dirList) {
  m_DIGGY_BOLLARD()

  int fetepeStatus = FetepeNListDir(handle,LIST__FTP_COMMAND,n_filter,NO__FILE_NAME_CORRECTION,dirList);
  m_TRACK_IF(fetepeStatus < 0)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeDir



// Public function ; see description in .h
int FetepeGet2 (FETEPE_HANDLE handle, const char *p_remoteFileName, //const struct SUCKER_BUTT *ap_localButt,
  BUTT_ADEPT_FILL_FUNCTION localButtAdeptFillFunction, void *r_localButtAdeptFillHandle,
  int *na_getLength) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK; 
  m_RESET_OUTLINE(handle)

  m_TRACK_IF(FetepeOpenDataStream(handle,&fetepeStatus, RETR__FTP_COMMAND__FMT_S,p_remoteFileName)
    != RETURNED)
 
  if (handle->data.nh_streamDescriptor != -1) {
    m_ASSERT(fetepeStatus == FETEPE_STATUS__OK)
    handle->data.outline.sucker.sButtIs = S_BUTT_IS__REMOTE;
    m_TRACK_IF(SuckerPlugSDButts(handle->data.h_suckerHandle, 
      StreamButtAdeptSuck, handle->data.h_streamButtAdeptHandle,
      localButtAdeptFillFunction, r_localButtAdeptFillHandle) != RETURNED)

    switch (handle->data.outline.sucker.suckerStatus = SuckerSuckOut(handle->data.h_suckerHandle,
      na_getLength, &handle->data.outline.sucker.r_flopCause)) {
    case SUCKER_STATUS__OK:
    break; case SUCKER_STATUS__S_FLOP:
    case SUCKER_STATUS__D_FLOP:
      if (fetepeStatus == FETEPE_STATUS__OK) fetepeStatus = FETEPE_STATUS__REFUSED;
    break; default :
      m_TRACK()
    } // switch
  } else if (na_getLength != NULL) {
    *na_getLength = 0;
  } // if

  m_TRACK_IF(FetepeCloseDataStream(handle,&fetepeStatus) != RETURNED)

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeGet2


// Public function ; see description in .h
int FetepeGet (FETEPE_HANDLE handle, const char *p_remoteFileName, const char *p_localFilePathname,
  int *na_getLength) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = UNDEFINED ;
  int ch_localFileDescriptor = UNDEFINED;

  switch (ProtectedOpenFile(p_localFilePathname, 
    COLD_READ_WRITE__BASIC_OPEN_FILE_MODE | handle->data.nonPosixOpenFlags, 0,
    &ch_localFileDescriptor, &handle->data.outline.sucker.localOpenErrno)) {
  case ANSWER__YES: 
    m_TRACK_IF(StreamButtAdeptKick(handle->data.h_localStreamButtAdeptHandle,
      ch_localFileDescriptor) != RETURNED)
    fetepeStatus = FetepeGet2(handle,p_remoteFileName, StreamButtAdeptFill,
      handle->data.h_localStreamButtAdeptHandle, na_getLength);
    m_TRACK_IF(fetepeStatus < 0)
    m_PROTECTED_CLOSE(ch_localFileDescriptor,
      if (fetepeStatus == FETEPE_STATUS__OK) fetepeStatus = FETEPE_STATUS__REFUSED;
    )
  break; case ANSWER__NO: 
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(fetepeStatus)
} // FetepeGet


// Public function ; see description in .h
int FetepeDisconnect (FETEPE_HANDLE handle) {
  m_DIGGY_BOLLARD()
  int fetepeStatus = FETEPE_STATUS__OK;

   m_RESET_OUTLINE(handle)
   m_TRACK_IF(FetepeWriteCommand(handle,&fetepeStatus,QUIT__FTP_COMMAND__FMT) != 0)
   m_TRACK_IF(FetepeReadCommandStatus(handle,&fetepeStatus,-1) != 0)
   // Note: 221 is the expected response status

  m_TRACK_IF(FetepeCloseCommandChannel(handle) != RETURNED)
 
  m_DIGGY_RETURN(RETURNED)
} // FetepeDisconnect


// Public function ; see description in .h
int FetepeDestroyInstance (FETEPE_HANDLE xh_handle) { 
  m_DIGGY_BOLLARD()
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(xh_handle->command.status.h_line) != RETURNED)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->command.status.h_linesPartitionHandle) !=
    RETURNED)

  m_TRACK_IF(ErwDestroyInstance(xh_handle->command.socket.h_erwHandle) != RETURNED)
  m_TRACK_IF(FetepeCloseCommandChannel(xh_handle) != RETURNED)

  m_TRACK_IF(PdDestroyInstance(xh_handle->data.h_listeningSocketPdHandle) != RETURNED)
  m_TRACK_IF(SuckerDestroyInstance(xh_handle->data.h_suckerHandle) != RETURNED)
  m_TRACK_IF(GreenCollectionDestroyInstance(xh_handle->data.h_lsLines) != RETURNED)
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(xh_handle->data.h_lsOutput) != RETURNED)
  m_TRACK_IF(GStringButtAdeptDestroyInstance(xh_handle->data.h_lsOutputButtAdeptHandle) != 
    RETURNED)
  m_TRACK_IF(StreamButtAdeptDestroyInstance(xh_handle->data.h_streamButtAdeptHandle) !=
     RETURNED)
  m_TRACK_IF(StreamButtAdeptDestroyInstance(xh_handle->data.h_localStreamButtAdeptHandle) !=
     RETURNED)

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // FetepeDestroyInstance
