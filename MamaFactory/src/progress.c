#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <flint/types.h>

#include "vibes.h"

#define DOT_PROGRESS_CHAR_SYMBOL "."

// NOTE: There is only progress display if output is a a tty..

enum { 
  DOT__MODE,
  SPIN__MODE
} ;

void AnsiForward (char *sequence, int n) {
  if (n <= 0) return;
  char seq[8];
  snprintf(seq,sizeof(seq),"\033[%dC",n) ;
  strcat(sequence,seq);
} // AnsiForward

void AnsiBackward (char *sequence, int n) {
  if (n <= 0) return;
  char seq[8];
  snprintf(seq,sizeof(seq),"\033[%dD",n) ;
  strcat(sequence,seq);
} // AnsiBackward

void AnsiBlue (char *sequence) {
  strcat(sequence,ANSI_BLUE_SEQ);
} // AnsiBlue

void AnsiNC (char *sequence) {
  strcat(sequence,ANSI_NC_SEQ);
} // AnsiNC


int b_isatty = b_FALSE0; 

static void OutProgress(const char *output, int len) {
  if (!b_isatty) return ;
  write(1,output,len);
} // OutProgress 



#define READ_BUF_SIZE 1000

int main (int argc, char * const *argv) {
  int threshold = 10;
  int mode = SPIN__MODE;
  int position = 0;
  b_isatty = isatty(1) ;

  if (argc > 1) {
    int opt = UNDEFINED;
    while ((opt = getopt(argc, argv, ":ds")) != -1) {
      switch (opt) {
      case 'd':
        mode = DOT__MODE;  
      break; case 's': 
        mode = SPIN__MODE;  
      break; default: /* '?' */
        fprintf(stderr,"bad opt -%c (%c)\nUsage: %s [ -s | -d  ] [ <position> [ <threshold> ] ]\n",
                optopt,opt,argv[0]);
        exit(NON_EXECUTIVE__EXIT_STATUS);
      } // switch
    } // switch
    if (optind < argc) {
      position = atoi(argv[optind]);
      if (position < 0) position = 0;  
      if (position > 15) position = (position % 16) ;
      if (optind+1 < argc) {
        threshold = atoi(argv[optind+1]);
        if (threshold <= 0) threshold = 10;  
      } // if
    } // if 
  } // if
     
  int inCpt = 0, c_outCpt=0;
  int c_spinState = 0;
  char *spins[4] = { "/)", "-)", "\\)", "|)" } ;
  char *lockedSpin = LOCKED_CHAR_SYMBOL ")"; 
  char readBuf[READ_BUF_SIZE] ;
  char sequence[256] ;
  sequence[0] = '\0';
  if (mode == DOT__MODE) {
    AnsiBlue(sequence);
    strcat(sequence,DOT_PROGRESS_CHAR_SYMBOL); c_outCpt++;
    AnsiNC(sequence);
  } else {
    AnsiForward(sequence,position*3) ;
    AnsiBlue(sequence);
    strcat(sequence,"(.)"); 
    AnsiNC(sequence);
    AnsiBackward(sequence,3*(position+1)) ;
  } // if
  OutProgress(sequence,strlen(sequence));
  int ret = UNDEFINED;
  while ((ret = read(0,readBuf,READ_BUF_SIZE-1)) > 0) {
    // Too bad if we miss blocking pattern split between two reads... 
    readBuf[ret] = '\0';
//printf("readBuf=[%lu]",strlen(readBuf));
//printf("readBuf=[%s]",readBuf);
    int i = 0;
    for (i = 0; i < ret; i++) {
      if (strcmp(readBuf+i,LOCKED_PATTERN) == 0) {
        sequence[0] = '\0';
        if (mode == DOT__MODE) {
          AnsiBlue(sequence);
          strcat(sequence,LOCKED_CHAR_SYMBOL); c_outCpt++;
          AnsiNC(sequence);
        } else {
          AnsiForward(sequence,position*3 + 1) ;
          AnsiBlue(sequence);
          strcat(sequence,lockedSpin);
          AnsiNC(sequence);
          AnsiBackward(sequence,3*(position+1)) ;
        } // if
        OutProgress(sequence,strlen(sequence));
      } else if (++inCpt >= threshold) {
        sequence[0] = '\0';
        if (mode == DOT__MODE) {
          AnsiBlue(sequence);
          strcat(sequence,DOT_PROGRESS_CHAR_SYMBOL); c_outCpt++;
          AnsiNC(sequence);
        } else {
          AnsiForward(sequence,position*3 + 1) ;
          AnsiBlue(sequence);
          strcat(sequence,spins[c_spinState]);
          AnsiNC(sequence);
          if (++c_spinState > 3) c_spinState = 0;
          AnsiBackward(sequence,3*(position+1)) ;
        } // if
        OutProgress(sequence,strlen(sequence));
        inCpt = 0; 
      } // if
    } // for
  } // while
  if (mode == SPIN__MODE) {
    sequence[0] = '\0';
    AnsiForward(sequence,position*3) ;
    AnsiBlue(sequence);
    strcat(sequence,"(.)"); 
    AnsiBackward(sequence,3) ;
    strcat(sequence,"( )"); 
    AnsiNC(sequence);
    AnsiBackward(sequence,3*(position+1)) ;
    OutProgress(sequence,strlen(sequence));
  } // if 

  return 0;
} // main
