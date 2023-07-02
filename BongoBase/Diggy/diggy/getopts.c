// diggy/getopts.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "diggy/getopts.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "flint/types.h"

// Description: see .h 
char b_ParseDgFlags (int argc, char **p_argv, unsigned int *a_dgFlags, char *info256) {
  char b_ok = b_TRUE;

  info256[0] = '\0';
  
  int opt = UNDEFINED;
  while ((opt = getopt(argc, p_argv, ":d:s:ptTh")) != -1) {
    switch (opt) {
    case 'd':
      if (strcmp(optarg,"a+") == 0) {
        *a_dgFlags |= MONOTHREAD_NICE_DISPLAY__DG_FLAG;
        snprintf(info256,256,"%s; NICE monothread display",info256);
      } else if (strcmp(optarg,"a") == 0) {
        *a_dgFlags |= MONOTHREAD_DISPLAY__DG_FLAG;
        *a_dgFlags &= ~MONOTHREAD_NICE_DISPLAY__DG_FLAG;
        snprintf(info256,256,"%s; monothread display",info256);
      } else if (strcmp(optarg,"p") == 0) {
        *a_dgFlags &= ~(MONOTHREAD_DISPLAY__DG_FLAG | MONOTHREAD_NICE_DISPLAY__DG_FLAG);
        snprintf(info256,256,"%s; multi-thread display",info256);
      } else {
        snprintf(info256,256,"bad optarg=[%s] with opt -%c",optarg,opt);
        b_ok = b_FALSE0;
      } // if
    break; case 's':
      if (strcmp(optarg,"s") == 0) {
        *a_dgFlags |= SHORT_STACK_ORIGIN_DISPLAY__DG_FLAG;
        snprintf(info256,256,"%s; SHORT stack display",info256);
      } else if (strcmp(optarg,"l") == 0) {
        *a_dgFlags &= ~SHORT_STACK_ORIGIN_DISPLAY__DG_FLAG;
        snprintf(info256,256,"%s; LONG stack display",info256);
      } else {
        snprintf(info256,256,"bad optarg=[%s] with opt -%c",optarg,opt);
        b_ok = b_FALSE0;
      } // if

    break; case 'p':
      *a_dgFlags |= PID_DISPLAY__DG_FLAG;
      snprintf(info256,256,"%s; PID display",info256);
    break; case 't':
      *a_dgFlags |= TID_DISPLAY__DG_FLAG;
      snprintf(info256,256,"%s; TID display",info256);
    break; case 'T':
      *a_dgFlags |= TIME_DISPLAY__DG_FLAG;
      *a_dgFlags &= ~DATE_TIME_DISPLAY__DG_FLAG;
     snprintf(info256,256,"%s; TIME only display",info256);
    break; case 'h':
      b_ok = b_FALSE0;
      snprintf(info256,256,"help display request");
    break; default: /* '?' */
      snprintf(info256,256,"bad opt -%c (%c)",optopt,opt);
      b_ok = b_FALSE0;
    } // switch
  } // while

  return b_ok;
} // b_ParseDgArgs

