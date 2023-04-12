// diggy/diggy-kitchen.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "diggy/diggy-kitchen.h"

#include "flint/types.h"
#include "flint/crash-freeloader.h"

#include <pthread.h>
#include "flint/tid.h"
#include <sys/types.h>

#include <signal.h>
#include <errno.h>


////  /////

// "Rudimentary" functions names convention:
// - "Diggy..." pattern      => function displays (or potentially displays) complete "diggy line(s)" 
//                              and/or "diggy META line(s)" ;
// - "DiggyBrick..." pattern => function displays some part of "diggy line" (header, body, etc.)
// - "DiggyMeta..." pattern  => functions displays (or potentially displays) complete "diggy META
//   line(s)" ;
// - no "Diggy..." pattern   => util function that DO NOT display "diggy line" ;
// - "...Geo..."             => function linked to "geometry"


//// UTILS (BAH!) //////////

// Display (hex/dec) (max 10 chars) pid (tid)  
// - b_hexDisplay: hex ?
//   + true : (hex) display length is 10
//   + false : (dec) display length is 8
static char *PTidDisplay10String (char *s11, pid_t ptid, int b_hexDisplay) {
  if (b_hexDisplay) {
    if (ptid <= 0xFFFFFFFF) {
      snprintf(s11,11,"0x%08x",ptid);
    } else {
      snprintf(s11,11,"0x%07x!",ptid & 0xFFFFFFF);
    } // if
  } else {
    if (ptid <= 99999999) {
      snprintf(s11,11,"%08d",ptid);
    } else {
      snprintf(s11,11,"%07d!",ptid % 10000000);
    } // if
  } // if
  return s11;
} // PTidDisplay10String


#define m_SECSTR_DECL(m_size) \
static char strBuffer ##m_size [m_size] ;\
strBuffer ##m_size [(m_size)-1] = '\0';

//
#define m_SECSTR_MEM(m_size,offset,length) \
if ((offset < 0) || (length) < 0 || sizeof(strBuffer ##m_size) + 1 < (length) + (offset)) {\
  memset(strBuffer ##m_size, '!', sizeof(strBuffer ##m_size)) ;\
} else {

//
#define m_SECSTR_MEMSET(m_size,offset,byte,length) {\
  m_SECSTR_MEM(m_size,offset,length)\
    memset(strBuffer ##m_size + (offset),byte,length);\
  }\
}

//
#define m_SECSTR_MEMCPY(m_size,offset,src,length) {\
  m_SECSTR_MEM(m_size,offset,length)\
    memcpy(strBuffer ##m_size + (offset),src,length);\
  }\
}

//
#define m_SECSTR_BYTESET(m_size,offset,byte) {\
  m_SECSTR_MEM(m_size,offset,1)\
    strBuffer ##m_size [offset] = (byte) ;\
  }\
}


//// RAW DIGGY DISPLAY //////////////


// pid/tid display in diggy line HEADER: 
static int b_pidDisplay = 1; 
static int b_hexPidDisplay = 0;
static int b_tidDisplay = 1; 
static int b_hexTidDisplay = 0;

enum { 
  NO__DATETIME_DISPLAY,
  TIME__DATETIME_DISPLAY,
  DATETIME__DATETIME_DISPLAY,
} ;
static int datetimeDisplay = DATETIME__DATETIME_DISPLAY ;
static int cb_msec = 0;


// Display line "prefix"
//
// - n_line:
//   + -1 special value: prefix for meta information
//   + >= 0: source code n_line number (0 if "anonymous")
static void DiggyBrickPrefix(int n_line) {
  m_SECSTR_DECL(256) 

  char s11[11] ; 
  if (b_pidDisplay) {
    printf("[%s] ",PTidDisplay10String(s11,getpid(),b_hexPidDisplay));
  } // if
  if (b_tidDisplay) {
    printf("[%s] ",PTidDisplay10String(s11,gettid(),b_hexTidDisplay));
  } // if
  if (datetimeDisplay != NO__DATETIME_DISPLAY) {
    struct timespec tp;
    struct tm tm ;

    if (clock_gettime(CLOCK_REALTIME,&tp) < 0) {
      tp.tv_sec = time(NULL);
      tp.tv_nsec = 0 ;
    } // if
    localtime_r(&(tp.tv_sec),&tm);

    putchar('[') ;
    if (datetimeDisplay == DATETIME__DATETIME_DISPLAY) {
      printf("%04d/%02d/%02d-%02d:%02d:%02d",tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) ;
    } else {
      printf("%02d:%02d:%02d",tm.tm_hour, tm.tm_min, tm.tm_sec);
    } // if
    if (cb_msec) {
      printf(".%03ld",tp.tv_nsec / 1000000) ;
    } // if
    fputs("] ",stdout);
  } // if

  if (n_line < 0) {
    fputs("(i)    ",stdout) ;
  } else if (n_line == 0) {
    fputs("       ",stdout) ;
  } else {
    snprintf(strBuffer256,sizeof(strBuffer256),"%d",n_line);
    int len = strlen(strBuffer256);
    if (len < 4) {
      len = 4 - len;
    } else {
      len = 0;
    }
    printf("[%d]%*s ",n_line,len,"") ;
  } // if
} // DiggyBrickPrefix


// Display some "meta" information 
static void DiggyMetaInfo (const char *p_info) {
  DiggyBrickPrefix(-1);

  fputs(p_info,stdout);
  putchar('\n');
}  // DiggyMetaInfo

// Display some "meta bug"...
#define DIGGY_META_BUG(m_bugInfo) \
DiggyMetaInfo("!!!! (BUG) " m_bugInfo " !!!!")

// Display "out of limits" notice  ...
#define DIGGY_META_LIMIT(m_limitInfo) \
DiggyMetaInfo("!!!! (OUT OF LIMITS) " m_limitInfo " !!!!")


//// GEO ////////////////////////////////////

static char b_geoOverflow ;

#define MAX_CALLS_DEPTH 256 


struct D_INSTANCE {
  int callsDepth ;
  const char *sp_funcs[MAX_CALLS_DEPTH] ;
  char sb_enabled[MAX_CALLS_DEPTH] ;
  void *s_origins[MAX_CALLS_DEPTH] ;
  int visibleCallsDepth; 
  int maxVisibleDepthDisplay ; // useful with multithread display
} ;


#define MAX_THREADS_NUMBER 50

static tid_t s_tids[MAX_THREADS_NUMBER] ;
static int s_tidsIndex[MAX_THREADS_NUMBER] ;

static struct D_INSTANCE s_dInstances[MAX_THREADS_NUMBER];
static int i_threadsNumber = 0 ; // when > 1, implies multithread display

static int cvb_multithreadDisplay = 0; // multithread display ? implicit when i_threadsNumber > 1 
static int b_niceMonothreadDisplay = 1;


int totalThreadsVisibleDisplayLength = 0; // useful with multithread display 


#define VISIBLE_DEPTH_DISPLAY_AUGMENTATION_STEP 5

enum {
  BEGIN__FUNCTION_TRACKING,
  INFO__FUNCTION_TRACKING,
  POP__FUNCTION_TRACKING,
} ;

// Display stack calls representation
static void DiggyBrickGeoVisibleDepth (int threadEntry, int functionTracking) {
  m_SECSTR_DECL(1024) 
  int i = UNDEFINED;
  int offset = UNDEFINED;

  int visibleCallsDepth = s_dInstances[0].visibleCallsDepth;

  if (!cvb_multithreadDisplay) {
    if (functionTracking == BEGIN__FUNCTION_TRACKING) {
      visibleCallsDepth-- ;
    } // if
    if (b_niceMonothreadDisplay) {
      for (i = 0; i < visibleCallsDepth; i++) {
        m_SECSTR_BYTESET(1024,(i*2),'|')
        m_SECSTR_BYTESET(1024,(i*2)+1,' ')
      } // for
      m_SECSTR_BYTESET(1024,visibleCallsDepth*2 ,'\0')
    } else {
      m_SECSTR_MEMSET(1024,0,'|',visibleCallsDepth)
      m_SECSTR_BYTESET(1024,visibleCallsDepth,'\0')
    } // if
    fputs(strBuffer1024,stdout);
    return;
  } // if

  m_SECSTR_MEMSET(1024,0,' ',totalThreadsVisibleDisplayLength)
  m_SECSTR_BYTESET(1024,totalThreadsVisibleDisplayLength,'\0') ;

  for (offset = 0, i = 0 ; i < i_threadsNumber ; i++) {
    visibleCallsDepth = s_dInstances[i].visibleCallsDepth;
    m_SECSTR_MEMSET(1024,offset,(i == threadEntry? '|': ':'),visibleCallsDepth)
    if (i == threadEntry) {
      switch (functionTracking) {
      case BEGIN__FUNCTION_TRACKING:
        m_SECSTR_BYTESET(1024,offset+visibleCallsDepth-1,'\\')
      break; case POP__FUNCTION_TRACKING:
        m_SECSTR_BYTESET(1024,offset+visibleCallsDepth,'/')
      break; default: ;
      } // switch
    } // if
    offset += s_dInstances[i].maxVisibleDepthDisplay;
  } // for
  fputs(strBuffer1024,stdout) ;
  if (functionTracking == INFO__FUNCTION_TRACKING) {
    putchar(' ');
  } // if
} // DiggyBrickGeoVisibleDepth


static int b_shortStackOriginDisplay = 0;

// Display diggy line representing new function call
// Passed:
// - n_line:
//   + -1 special value: display as meta information
//   + >= 0: line number (0 => anonymous)
static void DiggyGeoPush (int threadEntry, void *stackOrigin, 
                          const char *p_func, int n_line, const char *p_file, int b_enabled) {
  m_SECSTR_DECL(1024) 
  struct D_INSTANCE *a_dInstance = s_dInstances + threadEntry;

  if (a_dInstance->callsDepth >= MAX_CALLS_DEPTH) {
    if (!b_geoOverflow) {
      DIGGY_META_LIMIT("Too many calls") ;
    } // if
    b_geoOverflow = b_TRUE;
  } else {
    a_dInstance->sp_funcs[a_dInstance->callsDepth] = p_func;
    if ((a_dInstance->sb_enabled[a_dInstance->callsDepth] = b_enabled)) {
      ++(a_dInstance->visibleCallsDepth) ;
    } // if
    a_dInstance->s_origins[(a_dInstance->callsDepth)++] = stackOrigin ;
  } // if 

  if (!b_enabled) {
    return;
  } // if

  if ((!b_geoOverflow) &&  // NO calls overflow
      (!cvb_multithreadDisplay) && b_niceMonothreadDisplay) { 
    DiggyBrickPrefix(0);
    DiggyBrickGeoVisibleDepth(threadEntry,BEGIN__FUNCTION_TRACKING);
    putchar('\n') ;
  } // if

  DiggyBrickPrefix(n_line);

  if (!b_geoOverflow) {
    DiggyBrickGeoVisibleDepth(threadEntry,BEGIN__FUNCTION_TRACKING);
  } // if

  void *displayedStackOrigin = stackOrigin;
  if (b_shortStackOriginDisplay) {
    displayedStackOrigin = (void *)((long)stackOrigin & 0xFFFFFF);
  } // if
  printf("+%s(%p) @ %s\n",p_func,displayedStackOrigin,p_file);

  if (b_geoOverflow) { // calls overflow
    return;
  } // if

  if (a_dInstance->visibleCallsDepth < a_dInstance->maxVisibleDepthDisplay - 1) {
    return ; // No shift
  } // if

  int shift = VISIBLE_DEPTH_DISPLAY_AUGMENTATION_STEP;
  totalThreadsVisibleDisplayLength += VISIBLE_DEPTH_DISPLAY_AUGMENTATION_STEP;
  a_dInstance->maxVisibleDepthDisplay += VISIBLE_DEPTH_DISPLAY_AUGMENTATION_STEP;

  if (!cvb_multithreadDisplay) return;
 
  int i = UNDEFINED;
  int offset = UNDEFINED;

  m_SECSTR_MEMSET(1024,0,' ',totalThreadsVisibleDisplayLength + 1)
  m_SECSTR_BYTESET(1024,totalThreadsVisibleDisplayLength + 1,'\0')

  for (offset = 0, i = 0 ; i <= threadEntry ; i++) {
    m_SECSTR_MEMSET(1024,offset,':',s_dInstances[i].visibleCallsDepth)
    offset += s_dInstances[i].maxVisibleDepthDisplay;
  } // for

  for ( ; i < i_threadsNumber + 1 ; i++) {
    int len = (i >= i_threadsNumber? 1: s_dInstances[i].visibleCallsDepth) ;
    m_SECSTR_MEMSET(1024,offset - shift,'^',len)
    m_SECSTR_MEMSET(1024,offset - shift+len, '-', shift-len)
    m_SECSTR_MEMSET(1024,offset, 'v', len)
    if (i < i_threadsNumber) {
      offset += s_dInstances[i].maxVisibleDepthDisplay;
    } // if
  } // for

  DiggyMetaInfo(strBuffer1024);
} // DiggyGeoPush


// Pop last registered function call (and print function return)
// - n_line:
//   + -1 special value: display as meta information
//   + >= 0: line number (0 => anonymous)
// - c_returnValue: ONLY significant when line number is well known (> 0)
static void DiggyGeoPop (int threadEntry, int n_line, long c_returnValue) {
  struct D_INSTANCE *a_dInstance = s_dInstances + threadEntry ;

  if (a_dInstance->callsDepth > 0) {
    (a_dInstance->callsDepth)--;
    if (a_dInstance->sb_enabled[a_dInstance->callsDepth]) {
      if (a_dInstance->visibleCallsDepth > 0) {
        (a_dInstance->visibleCallsDepth)--;
      } else  {
        DIGGY_META_BUG("Out of visible calls");
      } // if
    } // if
  } else {
    DIGGY_META_BUG("Out of calls");
  } // if

  if (b_geoOverflow) { // calls overflow
    DiggyBrickPrefix(n_line);
    fputs("-(return)\n",stdout) ;
    return;
  } // if

  if (a_dInstance->sb_enabled[a_dInstance->callsDepth]) {
    DiggyBrickPrefix(n_line);
    DiggyBrickGeoVisibleDepth(threadEntry,POP__FUNCTION_TRACKING);
    if (n_line > 0) {
      printf("-%s() <0x%lx>\n",a_dInstance->sp_funcs[a_dInstance->callsDepth],c_returnValue);
    } else {
      printf("-%s()\n",a_dInstance->sp_funcs[a_dInstance->callsDepth]);
    } // if
  } // if
} // DiggyGeoPop


#define INIT_MAX_VISIBLE_DEPTH_DISPLAY 10 

// Binary search of some thread id...
static int BSearchByTid (tid_t tid, int *a_top) {
  int difference;
  int i;
  int n_indexEntry = -1;
  int bottom, top;
  top = i_threadsNumber ; bottom = -1;
  while (top - bottom > 1) {
    i =  (top + bottom) >> 1 ;
    difference = (tid - s_tids[s_tidsIndex[i]]);
    if (difference > 0) {
      top = i ;
    } else if (difference < 0) {
      bottom = i ;
    } else {
      n_indexEntry = i ;
      break ;
    } // if
  } // while
  *a_top = top;
  return n_indexEntry;
} // BSearchByTid


static int lastDisplayedThreadsNumber = 0;

//
static int DiggyMetaGeoGetDInstance (tid_t tid,
                                     const char *n_initInfos1, const char *c_initInfos2,
                                     const char *c_initInfos3,
                                     int *nacb_newDInstance) {
  char b_switchedToMultithread = 0;
  int tidsIndexTop = UNDEFINED;
  int n_tidsIndexEntry = BSearchByTid(tid,&tidsIndexTop);
  char b_newThread = (n_tidsIndexEntry == -1); // Not found => new thread 
  if (b_newThread) { 
    if (i_threadsNumber >=  MAX_THREADS_NUMBER) {
      DIGGY_META_LIMIT("Too many threads");
      return -1;
    } // if
    
    n_tidsIndexEntry = tidsIndexTop;
    memmove(s_tidsIndex + n_tidsIndexEntry + 1,
            s_tidsIndex + n_tidsIndexEntry,
            sizeof(int) * (i_threadsNumber - n_tidsIndexEntry));
    s_tidsIndex[n_tidsIndexEntry] = i_threadsNumber;
    s_tids[i_threadsNumber] = tid;
    s_dInstances[i_threadsNumber].callsDepth = 0;
    s_dInstances[i_threadsNumber].visibleCallsDepth = 0;
    s_dInstances[i_threadsNumber].maxVisibleDepthDisplay = INIT_MAX_VISIBLE_DEPTH_DISPLAY;
    i_threadsNumber++ ;

    if (i_threadsNumber >= 2 && !cvb_multithreadDisplay) {
      cvb_multithreadDisplay = 1; 
      b_switchedToMultithread = 1;
    } // if
    totalThreadsVisibleDisplayLength += INIT_MAX_VISIBLE_DEPTH_DISPLAY; 
  } // if

  int threadEntry = s_tidsIndex[n_tidsIndexEntry];
  if (n_initInfos1 != NULL) {
    DiggyMetaInfo(n_initInfos1);
    DiggyMetaInfo(c_initInfos2);
    DiggyMetaInfo(c_initInfos3);
    DiggyMetaInfo("DIGGY init. OK");
  } // if

  if (nacb_newDInstance != NULL) {
    *nacb_newDInstance = b_newThread;
  } // if

  if (!b_newThread) { 
    return threadEntry;
  } // if

  m_SECSTR_DECL(32) 
  m_SECSTR_DECL(1024) 
  int i = UNDEFINED;
  int offset = UNDEFINED;

  m_SECSTR_MEMSET(1024,0,' ',totalThreadsVisibleDisplayLength + 1)
  m_SECSTR_BYTESET(1024,totalThreadsVisibleDisplayLength + 1,'\0')

  if (!cvb_multithreadDisplay) { 
    PTidDisplay10String(strBuffer32,s_tids[0],0);
    strcat(strBuffer32,"*") ;
    DiggyMetaInfo(strBuffer32);
    lastDisplayedThreadsNumber = i_threadsNumber;
  } else {
    if (b_switchedToMultithread) {
      m_SECSTR_MEMSET(32,0,'>',INIT_MAX_VISIBLE_DEPTH_DISPLAY + 1) 
      DiggyMetaInfo(strBuffer32);
    } // if
    for (offset = 0, i = 0; i < lastDisplayedThreadsNumber; i++) {
      m_SECSTR_MEMSET(32,0,':',s_dInstances[i].visibleCallsDepth)
      m_SECSTR_MEMCPY(1024,offset,strBuffer32,s_dInstances[i].visibleCallsDepth)
      offset += s_dInstances[i].maxVisibleDepthDisplay;
    } // for
    m_SECSTR_BYTESET(1024,offset,'^')
    m_SECSTR_MEMSET(1024,offset + 1,'-',s_dInstances[i].maxVisibleDepthDisplay-1)
    m_SECSTR_BYTESET(1024,totalThreadsVisibleDisplayLength,'v')
    DiggyMetaInfo(strBuffer1024);

    m_SECSTR_MEMSET(1024,0,' ',totalThreadsVisibleDisplayLength + 1)
    for (i = 0, offset = 0; i < i_threadsNumber; i++) {
      if (i == threadEntry) {
        PTidDisplay10String(strBuffer32,s_tids[i],0);
        strcat(strBuffer32,"*") ;
      } else {
        m_SECSTR_MEMSET(32,0,':',s_dInstances[i].visibleCallsDepth)
        m_SECSTR_BYTESET(32,s_dInstances[i].visibleCallsDepth,'\0')
      } // if
      m_SECSTR_MEMCPY(1024,offset,strBuffer32,strlen(strBuffer32))
      offset += s_dInstances[i].maxVisibleDepthDisplay;
    } // for
    m_SECSTR_BYTESET(1024,totalThreadsVisibleDisplayLength,'v')
    DiggyMetaInfo(strBuffer1024);
    lastDisplayedThreadsNumber = i_threadsNumber;
  } // if 
  return threadEntry; 
} // DiggyMetaGeoGetDInstance 


/////////// GLUE between GEO and PUBLIC functions ///////

enum {
  UPWARDS__GROWING_STACK,
  DOWNWARDS__GROWING_STACK,
};

static int n_growingStack = -1 ;

// Dummy call to check how the stack is growing...
static void DummyCallee (void *callerStackOrigin) {
  int dummy = UNDEFINED;
  if ((void*)&dummy > callerStackOrigin) {
    n_growingStack = UPWARDS__GROWING_STACK;
  } else {
    n_growingStack = DOWNWARDS__GROWING_STACK;
  } // if
} // DummyCallee


// Get the stack direction
static void GetGrowingStackDirection(void) {
  int dummy = UNDEFINED;
  if (n_growingStack == -1) {
    DummyCallee(&dummy);
  } // if
} // GetGrowingStackDirection


// Indicates whether the specified call is below the top call currently referenced
//
// Ret:
// - true : strictly below the top call ; probably earlier function(s) return(s) has(have) not
//   been tracked ; a fake function "pop" could rectify the situation...
// - false : specified call corresponds to or is "above" top call currently referenced
//           (or calls depth overflow...)
static int b_StrictlyBelowCall (int threadEntry, void *stackOrigin) {
  GetGrowingStackDirection();
  int b_stricklyBelow = 0;
  struct D_INSTANCE *a_dInstance = s_dInstances + threadEntry;

  if (a_dInstance->callsDepth == 0) {
    b_stricklyBelow = 0;
  } else if (n_growingStack == UPWARDS__GROWING_STACK) {
    b_stricklyBelow = stackOrigin < a_dInstance->s_origins[a_dInstance->callsDepth-1];
  } else {
    b_stricklyBelow = stackOrigin > a_dInstance->s_origins[a_dInstance->callsDepth-1];
  } // if

  return b_stricklyBelow;
} // b_StrictlyBelowCall


// Indicates whether the passed call exactly correspond to the top call registered in the diggy
// system.
//
// Ret:
// - true: passed call does not correspond to top call registered ; a fake function "push" 
//         could rectify the situation...
// - false: passed call exactly corresponds to top call registered
//          (or calls depth overflow...)
static int b_NotTopCall (int threadEntry, void *stackOrigin, const char *p_func) {
  struct D_INSTANCE *a_dInstance = s_dInstances + threadEntry;

  return a_dInstance->callsDepth == 0 ||
         strcmp(a_dInstance->sp_funcs[a_dInstance->callsDepth-1],p_func) != 0 ||
         a_dInstance->s_origins[a_dInstance->callsDepth-1] != stackOrigin;
}  // b_NotTopCall


// Retrieve which registered function call the current call corresponds to.
static void DiggyRetrieve (int threadEntry, void *stackOrigin, 
                           const char *p_func, const char *p_file) {
  int cpt = 0 ;
  while (++cpt <= MAX_CALLS_DEPTH && b_StrictlyBelowCall(threadEntry,stackOrigin)) {
    DiggyGeoPop(threadEntry,-1,UNDEFINED) ;
  } // while
  if (b_NotTopCall(threadEntry,stackOrigin,p_func)) {
    DiggyGeoPush(threadEntry,stackOrigin, p_func,-1,p_file,1);
  } // if
} // DiggyRetrieve


static pthread_mutex_t h_mutex = PTHREAD_MUTEX_INITIALIZER;

// Entry point for public functions (handle multi-threading)
// "DiggyMeta..." function: possibly displays some [META] lines.
//
// Ret: thread entry
static int DiggyMetaGetIn (const char *n_initInfos1, const char *c_initInfos2, const char *c_initInfos3,
                           int *nacb_newDInstance) {
  int oldstate = UNDEFINED;
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);
  pthread_mutex_lock(&h_mutex);
  tid_t tid = gettid();
  int n_threadEntry =  DiggyMetaGeoGetDInstance(tid,n_initInfos1,c_initInfos2,c_initInfos3,nacb_newDInstance);

  return (n_threadEntry == -1 ? 0: n_threadEntry); 
} // DiggyMetaGetIn


// "Sortie" point for public functions (handle multi-threading)
static void GetOut (void) {
  pthread_mutex_unlock(&h_mutex);
  int oldstate = UNDEFINED; // TODO: call pthread_setcancelstate(PTHREAD_CANCEL_ENABLE only if necessary
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate);
} // GetOut



////// PUBLIC FUNCTIONS

static int b_initialized = 0;


// public function; see .h
void DiggyMetaInit (unsigned int dgFlags) {
  if (b_initialized) {
    DIGGY_META_BUG("Already initiaized.");
    return ;
  } // if

  CrashRecoveryInit();

  char sC11[11], sO11[11] ;
  char infos1[60] ;
  char infos2[60] ;
  char infos3[60] ;

  datetimeDisplay = NO__DATETIME_DISPLAY; 
  if (dgFlags & TIME_DISPLAY__DG_FLAG) {
    datetimeDisplay = TIME__DATETIME_DISPLAY;
  } // if 
  if (dgFlags & DATE_TIME_DISPLAY__DG_FLAG) { 
    datetimeDisplay = DATETIME__DATETIME_DISPLAY; 
  } // if
  cb_msec = (dgFlags & MSEC_TIME_DISPLAY__DG_FLAG);

  b_shortStackOriginDisplay = (dgFlags & SHORT_STACK_ORIGIN_DISPLAY__DG_FLAG); 
  b_pidDisplay = (dgFlags & PID_DISPLAY__DG_FLAG); 
  b_tidDisplay = (dgFlags & TID_DISPLAY__DG_FLAG); 
  b_hexPidDisplay = (getpid() > 999999);
  b_hexTidDisplay = (gettid() > 999999);

  cvb_multithreadDisplay = !(dgFlags & (MONOTHREAD_DISPLAY__DG_FLAG | MONOTHREAD_NICE_DISPLAY__DG_FLAG));
  b_niceMonothreadDisplay = (dgFlags & MONOTHREAD_NICE_DISPLAY__DG_FLAG) ;

  GetGrowingStackDirection();
  snprintf(infos1,sizeof(infos1),"Probe: %swards growing stack",
           n_growingStack == DOWNWARDS__GROWING_STACK? "DOWN":"UPwards growing stack");
  snprintf(infos2,sizeof(infos2),"Pid: %s (%s)",
           PTidDisplay10String(sC11,getpid(),b_hexPidDisplay),
           PTidDisplay10String(sO11,getpid(),!b_hexPidDisplay));
  snprintf(infos3,sizeof(infos3),"Header layout: %s%s%s[LINE]/(i)",
           b_pidDisplay? "[PID] ":"",
           b_tidDisplay? "[TID] ":"",
           datetimeDisplay != NO__DATETIME_DISPLAY? "[DATE/TIME] ": "");

  /* int threadEntry = */ DiggyMetaGetIn(infos1,infos2,infos3,NULL) ;
  GetOut() ;
} // DiggyMetaInit


// Public function; see .h
void DiggyBollard (void *stackOrigin, const char *p_func, int line, const char *p_file, int b_enabled) {
  int b_newDInstance; 
  int threadEntry = DiggyMetaGetIn(NULL,NULL,NULL,&b_newDInstance) ;
  DiggyGeoPush(threadEntry,stackOrigin, p_func,line,p_file,
               b_newDInstance? b_TRUE: b_enabled);
  GetOut();
} // DiggyBollard


// Public function: see .h
void DiggyBrickStart (void *stackOrigin, const char *p_func, int line, const char *p_file) {
  int threadEntry = DiggyMetaGetIn(NULL,NULL,NULL,NULL) ;

  DiggyRetrieve(threadEntry,stackOrigin, p_func,p_file);

  DiggyBrickPrefix(line);
  DiggyBrickGeoVisibleDepth(threadEntry,INFO__FUNCTION_TRACKING);
}  // DiggyBrickStart


// Public function: see .h
void DiggyBrickEnd (void) {
  putchar('\n') ;
  GetOut();
} // DiggyBrickEnd



// Public function; see .h
void DiggyReturn (void *stackOrigin, const char *p_func, int line, const char *p_file, long returnValue) {
  int threadEntry = DiggyMetaGetIn(NULL,NULL,NULL,NULL) ;

  DiggyRetrieve(threadEntry,stackOrigin, p_func,p_file);
  DiggyGeoPop(threadEntry,line,returnValue) ;

  GetOut();
} // DiggyReturn

