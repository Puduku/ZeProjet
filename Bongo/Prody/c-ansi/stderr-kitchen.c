// c-ansi/stderr-kitchen.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "c-ansi/stderr-kitchen.h"
#include "c-ansi/types.h"

#include <unistd.h>

#define PTHREAD

#ifdef PTHREAD 
#include <pthread.h>
#include "flint/tid.h"
#endif

static char b_anomaly = b_FALSE0 ;


#ifdef PTHREAD
static pthread_mutex_t h_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif


// Ret:
// - true: anomaly is raised
// - false: no anomaly 
char b_Anomaly (void) {
  return b_anomaly ;
} // b_Anomaly


// Passed:
// - p_legend:
// - p_field:
// - refLen: 
// - overflow : 
//   + >0 : overflow (of previous field display)
//   + 0: NO overflow
//
// Ret: >= 0: overflow of field display (0 => no overflow) 
static int StderrField(const char *p_legend, const char *p_field, int refLen, int overflow) {
  int feedLen = refLen - strlen(p_field) ;
  char feed[32] ;
  feed[0] = '\0';
  if (feedLen > 0) {
    feedLen -= overflow;
    if (feedLen > 0) {
      if (feedLen > sizeof(feed) - 1) {
        feedLen = sizeof(feed) - 1;
      } // if
      memset(feed,' ',feedLen) ;
      feed[feedLen] = '\0';
    } // if
  } // if
  fprintf(stderr," %s[%s]%s",p_legend,p_field,feed) ;
  return (feedLen < 0? -feedLen: 0) ;
} // StderrField

// Passed:
// - buf20: buffer (min size : 20)
// - field: 
// Ret: buffer address (i.e field string to display)
static char *NumField(char *buf20, unsigned long field) {
  if (field > 0xFFFFFF) {
    snprintf(buf20,20,"0x%lx",field);
  } else {
    snprintf(buf20,20,"%lu",field);
  } // if
  return buf20;
} // NumField

// Display anomaly header in stderr
//
// Passed:
// - p_file:
// - p_function:
// - line:
static void StderrHead(const char *p_file, const char *p_function, int line) {
  b_anomaly = b_TRUE ;
  char buf20[20];

  fputs("ANOMALY",stderr);
  int overflow = StderrField("PID",NumField(buf20,getpid()),5,0);
#ifdef PTHREAD
  overflow = StderrField("TID",NumField(buf20,gettid()),5,overflow);
#endif
  overflow = StderrField("FILE",p_file,25,overflow);
  overflow = StderrField("FUNC",p_function,15,overflow);
  overflow = StderrField("LINE",NumField(buf20,line),5,overflow);
} // StderrHead


// public function: see description in .h
void StderrRaise(const char *p_file, const char *np_function, int line, const char *p_anomaly,...) {
#ifdef PTHREAD
  pthread_mutex_lock(&h_mutex);
#endif

  StderrHead(p_file,np_function,line) ;

  fputs(" RAISE[",stderr);

  // TODO: filtrer caractegres de controsle
  va_list arguments ;
  va_start(arguments,p_anomaly) ;
  vfprintf(stderr,p_anomaly,arguments) ;
  va_end(arguments) ;

  fputs("]\n",stderr) ;

#ifdef PTHREAD
  pthread_mutex_unlock(&h_mutex);
#endif
} // StderrRaise


// public function: see description in .h
void StderrTrack(const char *p_file, const char *np_function, int line) {
#ifdef PTHREAD
  pthread_mutex_lock(&h_mutex);
#endif

  StderrHead(p_file,np_function,line) ;
  fputs(" TRACK\n",stderr) ;

#ifdef PTHREAD
  pthread_mutex_unlock(&h_mutex);
#endif
} // StderrTrack

