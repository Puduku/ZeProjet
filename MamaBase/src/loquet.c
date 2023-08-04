#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/select.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#include <flint/types.h>

#include "vibes.h"


#define m_WRONG_VAL(val) {\
  fprintf(stderr,"[%d]: UNEXPECTED " #val "==%d\n",__LINE__,val);\
  exit(-1);\
}

#define m_PERROR(str) {\
  perror(str);\
  exit(-1);\
}


static  char b_verbose = b_FALSE0;

#define m_JASE(p_blablaFmt, ...) {\
  if (b_verbose) printf(p_blablaFmt "\n", ##__VA_ARGS__);\
}


#define COLOURISED_LOCKED_PATTERN  ANSI_YELLOW_SEQ LOCKED_PATTERN ANSI_NC_SEQ 
#define TERMINAL_LOCKED_PLAINT  "\007" COLOURISED_LOCKED_PATTERN

#define m_PRINT(str) write(1,str,strlen(str)); 

static int b_isatty = b_FALSE0;  


//    m_PRINT(COLOURISED_LOCKED_PATTERN)
//    m_PRINT(LOCKED_PATTERN)
#define m_PRINT_LOCKED() {\
  if (b_isatty) {\
    m_PRINT(TERMINAL_LOCKED_PLAINT)\
  } else {\
    m_PRINT(COLOURISED_LOCKED_PATTERN)\
  }\
}

static void Bloque (int fd, int lock) { 
  while (1) { 
    alarm(1);
    int ret = flock(fd,lock); 
    if (ret != 0) {
      switch (errno) {
      case EINTR: // Interrupted by signal
        m_PRINT_LOCKED()
      break; default:
        perror("flock");
        exit(-1);
      } // switch
    } // if
    alarm(0);
    break;
  } // while
} // Bloque
    

#define MAX_SECTIONS_NUMBER 100
#define DEFAULT_SECTIONS_NUMBER 2
#define SECTION_SIZE 16

static void Ecris (int fd, const char *p_section, int entry) {
  if (entry < 0 || entry >= MAX_SECTIONS_NUMBER) m_WRONG_VAL(entry) 
  char raw_section[SECTION_SIZE] ;
  memset(raw_section,0,SECTION_SIZE);
  int ret = lseek(fd,entry*SECTION_SIZE,SEEK_SET);
  if (ret < 0) m_PERROR("lseek()") 
  ret = write(fd,raw_section,SECTION_SIZE) ;
  if (ret < 0) m_PERROR("write(raw_section)") 
  ret = lseek(fd,entry*SECTION_SIZE,SEEK_SET);
  if (ret < 0) m_PERROR("lseek()") 
  int len = strlen(p_section);
  if (len > SECTION_SIZE) m_WRONG_VAL(len) 
  ret = write(fd, p_section,strlen(p_section)) ;
  if (ret < 0) m_PERROR("write(p_section)") 
} // Ecris

enum {
  C__LOQUET_OP, // Create (clear) 
  R__LOQUET_OP, // Set Read (shared) 
  W__LOQUET_OP, // Set Write (exclusive)
  U__LOQUET_OP, // Unset
  X__LOQUET_OP, // Break 
} ;

static char *LoquetOpImage(int loquetOp) {
  static char image[32] = "???";
  switch(loquetOp) {
  case C__LOQUET_OP: snprintf(image,32,"Create (clear)") ;
  break; case R__LOQUET_OP: snprintf(image,32,"Set Read (shared)");
  break; case W__LOQUET_OP: snprintf(image,32,"Set Write (exclusive)");
  break; case U__LOQUET_OP: snprintf(image,32,"Unset");
  break; case X__LOQUET_OP: snprintf(image,32,"Break");
  break; default: snprintf(image,32,"???");
  } // switch
  return image;
} // LoquetOpImage


static void UsageAndExit (const char *p_argv0) {
  fprintf(stderr,
    "Usage: %s [-N <sections number>] [-v] [-c|-C] ((-r|-w|-u|-x) <section id>)* [-t (r|d)] [-p <pid>] [<loquet file>]\n"
    " -N <sections number> => total number of sections between [1-%d] (default is %d)\n"
    " -v => verbose\n"
    " -c => create loquet (hard)\n"
    " -C => create loquet (soft)\n"
    " -r => set Read loquet (shared)\n"
    " -w => set Write loquet (exclusive)\n"
    " -u => unset loquet\n"
    " -x => break loquet (test purpose)\n"
    " <section id> => between [1-%d] (0 for all sections)\n"
    " -t r => non-blocking test mode; real op.\n"
    " -t d => non-blocking test mode; dry run\n"
    " -p <pid> => specify process with exclusive loquet"
    "Exit status: \n"
    " 0: OK\n"
    " 1: permanent refusal (loquet file is out of order and must be recreated) \n"
    " 2: temporary refusal (locked; try later)\n"
    " 3: incompatible loquet file (loquet file was created with a different number of sections)\n"
    " 4: loquet file not found (loquet file must be created)\n" 
    " 100: invalid parameter(s); display this help\n"
    " (-1: Technical problem)\n",
    p_argv0, MAX_SECTIONS_NUMBER,DEFAULT_SECTIONS_NUMBER,MAX_SECTIONS_NUMBER);
  exit(NON_EXECUTIVE__EXIT_STATUS);
} // UsageAndExit 

int main (int argc, char * const *argv) {
  int sectionsNumber = DEFAULT_SECTIONS_NUMBER;
  int requiredSectionsNumber = 1;
  int n_loquetOp = -1, n_cLoquetOp = -1;
  int sn_loquetOps[MAX_SECTIONS_NUMBER];
  char *loquetFile = ".loquet";
  pid_t refPid = 0;
  int b_nonBlocking = b_FALSE0, b_cVariant = b_FALSE0;
  int b_dryRun = b_FALSE0;
  int entry = UNDEFINED;

  for (entry = 0; entry < MAX_SECTIONS_NUMBER; entry++) {
    sn_loquetOps[entry] = -1;
  } // for

  if (argc > 1) {
    int sectionId = UNDEFINED;
    int opt = UNDEFINED;
    while ((opt = getopt(argc, argv, ":N:vcCr:w:u:x:p:t:")) != -1) {
      switch (opt) {
      case 'N': 
        sectionsNumber = atoi(optarg) ;
        if (sectionsNumber <= 0 || sectionsNumber > MAX_SECTIONS_NUMBER) {
          fprintf(stderr,"invalid -%c opt=%s\n",opt,optarg);
          UsageAndExit(argv[0]);
        } // if
      break; case 'v':
        b_verbose = b_TRUE;
      break; case 'C':
        b_cVariant = b_TRUE;
      case 'c':
        n_cLoquetOp = C__LOQUET_OP;
      break; case 'r':
      case 'w':
      case 'u':
      case 'x':
        switch (opt) {
        case 'r':
          n_loquetOp = R__LOQUET_OP;
        break; case 'w': 
          n_loquetOp = W__LOQUET_OP;
        break; case 'u': 
          n_loquetOp = U__LOQUET_OP;
          b_nonBlocking = b_TRUE;
        break; case 'x':
          n_loquetOp = X__LOQUET_OP;
          b_nonBlocking = b_TRUE;
        break; default:
          m_WRONG_VAL(opt)
        } // switch
        sectionId = atoi(optarg) ;
        if (sectionId < 0 || sectionId > MAX_SECTIONS_NUMBER) {
          fprintf(stderr,"invalid -%c opt=%s\n",opt,optarg);
          UsageAndExit(argv[0]);
        } // if
        if (sectionId == 0) { // All sections
          for (entry = 0; entry < MAX_SECTIONS_NUMBER; entry++) {
            sn_loquetOps[entry] = n_loquetOp;
          } // for
        } else {
          sn_loquetOps[sectionId-1] = n_loquetOp;
          if (sectionId > requiredSectionsNumber) requiredSectionsNumber = sectionId;
        } // if 
      break; case 'p':
        refPid = atoi(optarg) ;
        if (refPid < 0) {
          fprintf(stderr,"invalid -%c opt=%s\n",opt,optarg);
          UsageAndExit(argv[0]);
        } // if
      break; case 't':
        switch (optarg[0]) {
        case 'd':  
          b_dryRun = b_TRUE;
        break; case 'r':
        break; default:
          fprintf(stderr,"invalid -%c opt=%s\n",opt,optarg);
          UsageAndExit(argv[0]);
        } // switch 
        b_nonBlocking = b_TRUE;
      break; default: /* ':' '?' */
        fprintf(stderr,"bad opt -%c (%c)\n", optopt,opt);
        UsageAndExit(argv[0]);
      } // switch
    } // while 
    if (n_loquetOp < 0 && n_cLoquetOp < 0) {
      fprintf(stderr,"missing option\n");
      UsageAndExit(argv[0]);
    } // if 
    if (optind < argc) {
      if (optind+1 < argc) {
        fprintf(stderr,"Too many program arguments\n");
        UsageAndExit(argv[0]);
      } // if
      loquetFile = argv[optind];
    } // if 
  } else {
    fprintf(stderr,"Missing program argument(s)\n");
    UsageAndExit(argv[0]);
  } // if

  b_isatty = isatty(1) ;

  if (requiredSectionsNumber > sectionsNumber) {
    m_JASE("Required sections number is %d but only %d are available.",requiredSectionsNumber,
      sectionsNumber)
    return 3; // Incompatible loquet file
  } // if
     
  int n_exitStatus = 0; // a priori
  if (n_cLoquetOp == C__LOQUET_OP) {
    int fd = open(loquetFile, O_WRONLY | O_CREAT | (b_cVariant? O_EXCL:  O_TRUNC), 0644);
    if (fd < 0) {
      switch (errno) {
      case EEXIST: 
        n_exitStatus = 1; // permanent refusal
        if (b_cVariant) { 
          m_JASE("Loquet file '%s' already exists.",loquetFile);
          break ;
        } // if
      default: 
        m_PERROR("open()")
      } // switch
    } else { 
      for (entry = 0; entry < sectionsNumber; entry++) {
        Ecris(fd,"U",entry) ;
      } // for
      close(fd);
      m_JASE("Loquet file '%s' created.",loquetFile);
    } // if 
  } // if

  if (n_loquetOp < 0) {
    return n_exitStatus;
  } // if

  int fd = open(loquetFile,O_RDWR,0644);
  if (fd < 0) { 
    switch(errno) {
    case ENOENT: 
      m_JASE("loquet file '%s' does not exist.",loquetFile);
      return 4;
    break; default:
      m_PERROR("open")
    } // switch
  } // if

  { struct stat st;
    if (fstat(fd,&st) < 0) m_PERROR("fstat()")
    if (st.st_size != sectionsNumber * SECTION_SIZE) {
      close(fd);
      m_JASE("Loquet file '%s' is set for %d sections, not %d.",loquetFile,
        (int)(st.st_size / SECTION_SIZE), sectionsNumber);
      return 3; // Incompatible loquet file
    } // if
  } // st

  n_exitStatus = -1; // unknown for the moment 
  int cpt = 0;
  while (n_exitStatus == -1) { 
    char sectionsBufs[sectionsNumber][SECTION_SIZE] ;
    Bloque (fd, LOCK_EX); 
    // INVARIANT: sectionsBufs[e < entry] updated with content to write 
    for (entry = 0 ; entry < sectionsNumber ; entry++) {
      char *sectionBuf = sectionsBufs[entry];
      int ret = lseek(fd,entry*SECTION_SIZE,SEEK_SET);
      if (ret < 0) m_PERROR("lseek()")
      ret = read(fd,sectionBuf,SECTION_SIZE) ;
      if (ret < 0) m_PERROR("read")
      if (ret != SECTION_SIZE) m_WRONG_VAL(ret) 
      if (sectionBuf[ret-1] != '\0')  m_WRONG_VAL(sectionBuf[ret-1])  
      
      int currentLoquetOp = UNDEFINED;
      pid_t c_pid = UNDEFINED; // only significant with 'W' op 
      int rLevel = 0;
      switch (sectionBuf[0]) {
      case 'R':
        currentLoquetOp = R__LOQUET_OP;
        rLevel = atoi(sectionBuf+1);
      break; case 'W':
        currentLoquetOp = W__LOQUET_OP;
        c_pid = atoi(sectionBuf+1);
        if (c_pid > 0 && kill(c_pid,0) < 0) {
          switch (errno) {
          case ESRCH: // No process or process group can be found...
            m_JASE("Loquet file is locked by dead process (pid %d).",c_pid); 
            n_exitStatus = 1; // "permanent" refusal
          break; case EPERM: // The calling process does not have permission to send the signal...
          break; default:
            m_PERROR("kill()")
          } // switch
        } // if
      break; case 'U':
        currentLoquetOp = U__LOQUET_OP;
      break; case 'X':
        currentLoquetOp = X__LOQUET_OP; // loquet is broken
        m_JASE("Loquet file '%s' is broken.",loquetFile);
        n_exitStatus = 1; // "permanent" refusal
      break; default:
        m_WRONG_VAL(sectionBuf[0]);
      } // switch 

      // ASSERT: n_exitStatus == -1 (unknow) or == 1 (permanent refusal)
    
      if (n_exitStatus != -1) { // exit status directly estblished... 
        if (n_exitStatus != 1) m_WRONG_VAL(n_exitStatus) 
        m_JASE("Loquet file '%s' must be recreated.",loquetFile) 
        break;
      } // if
      // Not directly refused... 
      if (sn_loquetOps[entry] == -1) continue;
      switch (sn_loquetOps[entry]) {
      case R__LOQUET_OP:
        if (currentLoquetOp == R__LOQUET_OP || currentLoquetOp == U__LOQUET_OP) {
          sprintf(sectionBuf,"R%d",++rLevel);
        } else { 
          n_exitStatus = 2; // "temporary" refusal
        } // if
      break; case W__LOQUET_OP:
        if ( currentLoquetOp == U__LOQUET_OP || 
             (currentLoquetOp == W__LOQUET_OP && c_pid != 0 && refPid == c_pid) ) {
          sprintf(sectionBuf,"W%d",refPid);
        } else { 
          n_exitStatus = 2; // "temporary" refusal 
        } // if
      break; case U__LOQUET_OP:
        if (currentLoquetOp == R__LOQUET_OP) {     
          if (--rLevel == 0) {
            sprintf(sectionBuf,"U");
          } else {
            sprintf(sectionBuf,"R%d",rLevel);
          } // if
        } else if (currentLoquetOp == W__LOQUET_OP && c_pid == refPid) { 
          sprintf(sectionBuf,"U");
        } else if (currentLoquetOp == U__LOQUET_OP) { // Already unlocked 
          sprintf(sectionBuf,"U");
        } else { // 'u' op not possible 
          n_exitStatus = 2; // "temporary" refusal 
        } // if
      break; case X__LOQUET_OP:
        sprintf(sectionBuf,"X");
      break; default: 
        m_WRONG_VAL(sn_loquetOps[entry])
      } // switch 

      if (n_exitStatus != -1) { // "temporary" refusal ? 
        if (n_exitStatus != 2) m_WRONG_VAL(n_exitStatus) 
        break; 
      } // if
    } // for
    if (n_exitStatus == -1) { 
      if (entry != sectionsNumber) m_WRONG_VAL(entry) 
      // ASSERT : sectionsBufs[e < sectionsNumber] updated with content to write 
      n_exitStatus = 0;
    } // if
    if (n_exitStatus == 2) { // "temporary" refusal 
      if (b_nonBlocking) { // We can't wait... 
        m_JASE("Operation is currently not permitted on these sections.") 
        break; 
      } else { // We can wait...
        n_exitStatus = -1; // Do not exit now 
      } // if
    } // if

    if (n_exitStatus != -1) { // Exit now
      // No actual change in "dry run" 
      if ((n_exitStatus == 0) && !b_dryRun) { 
        for (entry = 0 ; entry < sectionsNumber ; entry++) {
          if (sn_loquetOps[entry] == -1) continue;
          Ecris(fd,sectionsBufs[entry],entry);  
        } // for
      } // if
    } else { // Do not exit now
      if (++cpt >= 100) {
        m_PRINT_LOCKED()
        cpt = 0;
      } // if
    } // if
    Bloque(fd, LOCK_UN); 
    if (n_exitStatus == -1) usleep(50000);
  } // while

  if (close(fd) < 0) {
    perror("close");
    exit(-1);
  } // if
   
  if (n_exitStatus == 0) { 
    if (b_dryRun) m_JASE("Operation is currently permitted on these sections.") 
    else m_JASE("Operation is successful.")
  } // if 
  return n_exitStatus;
} // main
