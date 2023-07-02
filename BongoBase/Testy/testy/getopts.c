// testy/getopts.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "testy/getopts.h"
#include "diggy/getopts.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "flint/types.h"

const char *p_testyDatadirPath = DEFAULT_TESTY_DATADIR_PATH; 

// Parse TESTY program specific arguments (data dir)
// 
// Passed:
// - argc:
// - p_argv:
// - testyFlags:
// - optind: (global variable) args position (1 for 1st position parameter)
//
// Modified:
// - optind: (global variable) args position after parsing (== argc => all args parsed)
// - info256: 
//   + TESTY program must be executed (ret 0) => recap of options
//   + TESTY program must not execute => explanation 
//
// Returned:
// - 0 : OK, now execute TESTY program
// - 100 : process should NOT execute DIGGY program ; reason:
//   + bad option(s) argument(s)
//   + passed -h (help request switch)
static char b_ParseTestySpecificArguments (int argc, char **p_argv, unsigned int testyFlags,
  char *info256) {
  int b_ok = b_TRUE; 

  info256[0] = '\0';
  if (optind < argc) {
    if (strcmp("SB",p_argv[optind]) == 0) {
      snprintf(info256,256,"SB (sandbox indicator)");
      optind++;
    } else if (testyFlags & SANDBOX__TESTY_FLAG) {
      snprintf(info256,256,"missing SB indicator");
      b_ok = b_FALSE0;
    } // if
  } // if

  if (b_ok) {
    if (optind < argc) {
      p_testyDatadirPath =  p_argv[optind++];
      snprintf(info256,256,"%s; datadir=[%s]",info256,p_testyDatadirPath);
    } else if (testyFlags & DATADIR__TESTY_FLAG) {
      snprintf(info256,256,"missing <datadir> argument");
      b_ok = b_FALSE0;
    } // if  
  } // if
       
  return b_ok;
} // b_ParseTestySpecificArguments


#define TESTY_SPECIFIC_ARGS_SANDBOX_SUMMARY  "SB"

#define TESTY_SPECIFIC_ARGS_OPTIONAL_SANDBOX_SUMMARY  "[ SB ]"

#define TESTY_SPECIFIC_ARGS_DATADIR_SUMMARY  "<datadir>"

#define TESTY_SPECIFIC_ARGS_OPTIONAL_DATADIR_SUMMARY  "[ <datadir> ]"

#define TESTY_SPECIFIC_ARGS_DETAILS \
" SB => indicates that current directory is usable as sandbox\n" \
" <datadir> => dir path for TEST data files\n" \

// See .h
unsigned int ParseTestyCommandArguments (int argc, char **p_argv, unsigned int testyFlags) { 
  unsigned int dgFlags = (testyFlags & MULTITHREADED__TESTY_FLAG)?
    MULTITHREAD_DEFAULT_DG_FLAGS: MONOTHREAD_DEFAULT_DG_FLAGS;
  char info256[256] ;

  info256[0] ='\0';
  char b_ok = b_ParseDgFlags(argc,p_argv,&dgFlags,info256); 
  if (b_ok) { 
    if (info256[0] == '\0') strcpy(info256,"N/A");
    printf("Passed DG DISPLAY options: %s\n",info256);
    info256[0] ='\0';
    b_ok = b_ParseTestySpecificArguments(argc,p_argv, testyFlags, info256) ;
    if (b_ok) { 
      if (info256[0] == '\0') strcpy(info256,"N/A");
      printf("Testy options: %s\n",info256);
    } // if
  } // if
  if (!b_ok) {
    fprintf(stderr,"=> %s\nPossible options:\n",info256) ;
    fprintf(stderr,"%s |\n"
      "( %s %s %s )\n"
      "%s%s%s",
        HELP_ARGS_SUMMARY,
        DG_FLAGS_EXECUTIVE_ARGS_SUMMARY, 
        (testyFlags & SANDBOX__TESTY_FLAG)? 
        TESTY_SPECIFIC_ARGS_SANDBOX_SUMMARY:
        TESTY_SPECIFIC_ARGS_OPTIONAL_SANDBOX_SUMMARY,
        (testyFlags & DATADIR__TESTY_FLAG)? 
        TESTY_SPECIFIC_ARGS_DATADIR_SUMMARY:
        TESTY_SPECIFIC_ARGS_OPTIONAL_DATADIR_SUMMARY,
        HELP_ARGS_DETAILS, DG_FLAGS_EXECUTIVE_ARGS_DETAILS, TESTY_SPECIFIC_ARGS_DETAILS);
    exit(NON_EXECUTIVE__EXIT_STATUS);
  } // switch
// TODO : gueuler s'il reste des arguments non parseds...

  return dgFlags;
} // ParseTestyCommandArguments


