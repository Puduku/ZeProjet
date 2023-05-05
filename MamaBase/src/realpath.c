#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "basedirname.c"


// Construct RCA (Real - Canonicalized Absolute) pathname
static char *RcaPathname (const char*pathname, char *rcaPathname) {
  // Elaborated regarding realpath() "BSD" behavior documented and observed on Apple macOS High 
  // Sierra 10.13.6
  if (realpath(Dirname(pathname),rcaPathname) != NULL &&
      rcaPathname[0] != '\0') {
    char *basename = Basename(pathname) ;
    if (strcmp(basename,"..") == 0) {
      if (strcmp(rcaPathname,"/") !=  0) {
        char *ptr = rcaPathname + strlen(rcaPathname)-1 ;
        while (ptr > rcaPathname && *ptr != '/') {
          *(ptr--) = '\0';
        } // while
        if (*ptr == '/' && strcmp(rcaPathname,"/") != 0) { 
          *(ptr--) = '\0';
        } // if
      } // if
    } else if (strcmp(basename,".") != 0) {
      if (strcmp(rcaPathname,"/") !=  0) {
        strcat(rcaPathname,"/") ;
      } // if
      strcat(rcaPathname,basename) ;
    } // if
    return rcaPathname;
  } // if  
  return NULL;
} // RcaPathname


//
static char* RelativePathname (const char *rcaPathname, const char *rcaPathref) {
  static char buf[1024] ;
  int offset = 0 ;
  int lastPortionOffset = 0;
  while (rcaPathname[offset] == rcaPathref[offset]) {
    if (rcaPathname[offset] == '/') {
      lastPortionOffset = offset; 
    } // if
    offset++;
  } // while 
  if ( (rcaPathname[offset] == '\0' || rcaPathname[offset] == '/') &&
       (rcaPathref[offset] == '\0'  || rcaPathref[offset] == '/' ) ) { 
    // last parsed components are identical
   
  } else { 
    // last parsed components have common "prefix", but are actually different
    offset = lastPortionOffset;
  } // if
  
  buf[0] = '\0' ;

  // Construct upping path to ref 
  { const char *ptr = rcaPathref + offset ;
    while (*ptr != '\0') {
      if (*(ptr++) == '/') {
        if (buf[0] != '\0') {
          strcat(buf,"/..") ;
        } else {
          strcat(buf,"..") ;
        } // if
      } // if
    } // while  
  } // ptr

  if (rcaPathname[offset] == '/') {
    if (buf[0] != '\0') {
      strcat(buf,"/") ;
    } // if
    strcat(buf,rcaPathname + offset+1); 
  } // if

  if (buf[0] == '\0') {
    strcpy(buf,".");
  } // if

  if (strchr(buf,'/') == NULL) {
    int len =strlen(buf) ;
    memmove(buf+2,buf,len+1) ;    
    memcpy(buf,"./",2) ;
  } // if
  return buf; 
} // RelativePathname



#define REL_BASE_SWITCH "--relative-base="
#define REL_TO_SWITCH "--relative-to="

//
int main (int argc, const char **argv) {
  int lenRelBaseSw = strlen(REL_BASE_SWITCH);
  int lenRelToSw = strlen(REL_TO_SWITCH);

  if (argc == 2) {
    char rcaPathname[1024];
    if (RcaPathname(argv[1], rcaPathname) != NULL) {
      puts(rcaPathname) ;
    } // if  

  } else if ((argc == 4) && (memcmp(argv[1],REL_BASE_SWITCH,lenRelBaseSw) == 0) 
             && (memcmp(argv[2],REL_TO_SWITCH,lenRelToSw) == 0)) {  
    char rcaPathname[1024], relBase[1024];
    char b_ok = (RcaPathname(argv[3],rcaPathname) != NULL); 
    if (b_ok) {
      b_ok = (RcaPathname(argv[1]+lenRelBaseSw,relBase) != NULL); 
    } // if  

    if (b_ok) {
      int relBaseLen = strlen(relBase);
      int rcaPathnameLen = strlen(rcaPathname);
      if (rcaPathnameLen >= relBaseLen &&
          memcmp(rcaPathname,relBase,relBaseLen) == 0) {
        // We are well under the base (i.e the "Island"...) 
        char relTo[1024];
        int relToLen = strlen(relTo);
        b_ok = (RcaPathname(argv[2]+lenRelToSw,relTo) != NULL); 
        if (b_ok) {
          // display the relative path 
          puts(RelativePathname(rcaPathname,relTo));
        } // if

      } else { 
        // We are "out of" the base (island) => display rca pathname
        puts(rcaPathname);
      } // if   
    } // if   

  } else {
    fputs("Invalid argument(s)\n",stderr);
    fputs(argv[0],stderr);
    fputs(" is supposed to behave exaclty like standard realpath command present in LINUX/Ubuntu\n",stderr);
    fputs("Actually, only TWO specific modes are supported:\n",stderr);
    fputs("1. Basic mode (NO options) ; unique argument : <pathname>\n",stderr);
    fputs("   => construct RCA (real canonicalized absolute) pathname.\n",stderr);
    fputs("2. Enhanced mode (NO options) ; three arguments - in that order : \n",stderr);
    fputs("   " REL_BASE_SWITCH "<base pathname> " REL_TO_SWITCH "<relative to pathname> <pathname>\n",stderr);
    fputs("   => construct \"smart location\" pathname.\n",stderr);
    return 100;
  } // if 

  return 0;
} 
