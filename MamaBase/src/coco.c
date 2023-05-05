//# coco : expand references in COde COmments
//#TODO : constant (25) for callsControl
//# TODO: replacement does not work when repl is <'\0'-terminated C string> or <line delimitor (handle)>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>

#include <sys/stat.h>
#include <sys/time.h>

#include "flint/types.h"
#include "getintarg.c"


// 0. ==== Utils (bah!)

// Passed:
// - msg:
#define m_ANOMALY(/* const char* */ msg) {\
  fprintf(stderr,"ANOMALY LINE[%d] ",__LINE__);\
  fprintf(stderr,"%s\n",msg);\
  exit(-1);\
}

// Passed:
// - ub_unexpectedCondition:
#define m_ANOMALY_IF(ub_unexpectedCondition) \
if (ub_unexpectedCondition) m_ANOMALY(# ub_unexpectedCondition)


enum {
  SILENT__VERBOSITY,
  LOW__VERBOSITY,
  HIGH__VERBOSITY,
} ;  

int verbosity = UNDEFINED;

#define  m_MAJOR_INFO(infoFmt, ...) \
if (verbosity >= LOW__VERBOSITY) {\
  printf(infoFmt, ##__VA_ARGS__);\
  putchar('\n');\
}

#define  m_MINOR_INFO(infoFmt, ...) \
if (verbosity >= HIGH__VERBOSITY) {\
  putchar(' ');\
  printf(infoFmt, ##__VA_ARGS__);\
  putchar('\n');\
}

// Ret : like mem/strcmp() (=> 0 for matching)
static int XCmp (const char *s1, int n_len1, const char *s2, int n_len2) {
  if (n_len1 == -1 && n_len2 == -1) {
    return strcmp(s1,s2) ;
  } // if

  int len = n_len1 ;
  if (n_len1 == -1 || (n_len2 != -1 && n_len1 > n_len2)) {
    len = n_len2 ;
  } // if
  int ret = memcmp(s1,s2,len) ;
  if (ret == 0) {
    if (n_len1 != -1 && n_len2 != -1) {
      if (n_len1 != n_len2) {
        ret = (n_len1 > n_len2 ? 1 : -1);
      } // if
    } else if (n_len1 == -1) {
      if (s1[len] != '\0') {
        ret = 1;
      } // if
    } else { 
      if (s2[len] != '\0') {
        ret = -1;
      } // if
    } // if
  } // if
  return ret;
} // XCmp

// Passed:
// - m_dst:
// - src:
// - len:
#define m_CPY(/* char * */ m_dst, /* const char * */ src, /* int */ len) {\
  int _len = (len >= sizeof(m_dst)? sizeof(m_dst)-1: len);\
  memcpy(m_dst,src,_len);\
  m_dst[_len] = '\0';\
}

static int b_rawModifTime = b_FALSE0;

static void UpdateFile (const char *p_newFile, const char *p_filepath) {
  struct timeval c_timevals[2] ;

  if (!b_rawModifTime) { 
    struct stat buf;
    if (stat(p_filepath, &buf) < 0) {
      perror("stat()") ; 
      m_ANOMALY("stat()")
    } // if
#ifndef __APPLE__
#define st_atimespec st_atim
#define st_mtimespec st_mtim
#endif
    c_timevals[0].tv_sec = buf.st_atimespec.tv_sec;
    c_timevals[0].tv_usec = buf.st_atimespec.tv_nsec / 1000 ;
    c_timevals[1].tv_sec = buf.st_mtimespec.tv_sec;
    c_timevals[1].tv_usec = buf.st_mtimespec.tv_nsec / 1000 ;
#ifndef __APPLE__
#undef st_atimespec
#undef st_mtimespec
#endif
  } // if
  if (rename(p_newFile,p_filepath) < 0) {
    perror("rename()"); // EXDEV: the implementation does not support "cross-device" rename 
    m_ANOMALY("rename()")
  } // if
  if (!b_rawModifTime) { 
    if (utimes(p_filepath, c_timevals) < 0) {
      perror("utimes()") ; 
      m_ANOMALY("utimes()")
    } // if
  } // if
}
// 1. ==== Meta info handling 

#define META_INFO_COMMENT "//"
#define META_INFO_PREFIX	"=COCO="

#define META_INFO_LINE_PREFIX	META_INFO_COMMENT META_INFO_PREFIX
#define NEEDED_STEP__FMT__D	" [%d]"
#define PROCESSED__FMT__D	" PROCESSED %d"


// Passed:
// - *an_processed: (-1 special value for unknown)
// - *an_neededStep: (-1 special value for unknown)
//
// Modified:
// - *an_processed: updated if new information found
// - *an_neededStep: updated if new information found
//
// Ret: is line a meta info line ?
// True: => line is a meta info line => see found meta info param(s)
// False: => line is not meta info line => no meta info available
static int FindMetaInfoInLine (const char *line, int *an_processed, int *an_neededStep) {
  const char *ptr = line ;
  int len = strlen(META_INFO_LINE_PREFIX) ;
  if (*ptr != '\0' && memcmp(META_INFO_LINE_PREFIX,ptr,len) == 0) {
    ptr += len;
    if (sscanf(ptr,PROCESSED__FMT__D,an_processed) != 1) {
      sscanf(ptr,NEEDED_STEP__FMT__D,an_neededStep);
    } // if
    return 1 /* True */;
  } // if
  return 0 /* False */;
} // FindMetaInfoInLine
// Ret:


// 2. ==== Coco "nuts"... 

#define COMMENT_IN_LINE "//"

// Parsing in comment line (not META info line)
#define b_NUT_PARSING_OK	b_TRUE 			// coco element (#REF tag, etc.) recognised and parsed
#define b_NUT_PARSING_KO	(! b_NUT_PARSING_OK)	// coco element NOT recognized in comment

// - p_tag : "SEE" or "REF"
// - *a_ptr:
// 
// Changed:
// - *a_ptr:
// 
// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseNutTag (const char *p_tag, const char **a_ptr) {
  const char *ptr = *a_ptr;
  while (*ptr == ' ') ptr++;
  while (*ptr != '\0' && *(ptr++) != '#') ;
  if (*ptr == '\0') return b_NUT_PARSING_KO;
  while (*ptr == ' ') ptr++;
  const char *ptr2 = p_tag;
  int val = 0;
  while (*ptr2 != '\0' && val == 0) {
    if (*ptr != '\0') {
      val = toupper( *(ptr++) ) - toupper( *(ptr2++) );
    } else {
      val = -1;
    } // if
  } // while
  if (val != 0) return b_NUT_PARSING_KO;
  *a_ptr = ptr;
  return b_NUT_PARSING_OK;
} // b_ParseNutTag


#define b_ASCII_CHAR_BETWEEN(c,ib,sb) \
  ( (unsigned char)(c) >= (unsigned char)(ib) && (unsigned char)(c) <= (unsigned char)(sb) ) 

#define b_LABEL_CHARSET(c) (\
  b_ASCII_CHAR_BETWEEN(c,'A','Z')  || \
  b_ASCII_CHAR_BETWEEN(c,'a','z')  || \
  b_ASCII_CHAR_BETWEEN(c,'0','9')  || \
  (unsigned char)(c) == '_' || (unsigned char)(c) == '-' \
)

#define b_SPOT_CHARSET(c) (\
  b_ASCII_CHAR_BETWEEN(c,'A','Z')  || \
  b_ASCII_CHAR_BETWEEN(c,'a','z')  || \
  b_ASCII_CHAR_BETWEEN(c,'0','9')  || \
  (unsigned char)(c) == '_' || (unsigned char)(c) == '-' || \
  (unsigned char)(c) == '.' || (unsigned char)(c) == '/' \
)


#define SEE_TAG "SEE"

// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseSeeNut (const char **a_ptr, const char  **ac_labelPtr, int *ac_labelLen,
                          const char **acn_spotFilePtr, int *acc_spotFileLen) {
  const char *ptr = *a_ptr ;
  if (*ptr != '\0' && b_ParseNutTag(SEE_TAG,&ptr)) {
    while (*ptr == ' ') ptr++;
    *ac_labelPtr = ptr;
    while (b_LABEL_CHARSET(*ptr)) ptr++;
    *ac_labelLen = (ptr - *ac_labelPtr);
    *acn_spotFilePtr = NULL; // a priori
    { const char *ptr2 = ptr; 
      while (*ptr2 == ' ') ptr2++;
      if (*ptr2 == '@') {
        while (*(++ptr2) == ' ') ;
        *acn_spotFilePtr = ptr2;
        while (b_SPOT_CHARSET(*ptr2)) ptr2++;
        *acc_spotFileLen = (ptr2 - *acn_spotFilePtr);
        ptr = ptr2;
      } // if 
    } // ptr2 
    *a_ptr = ptr;
    return b_NUT_PARSING_OK;
  } // if
  return b_NUT_PARSING_KO;
} // b_ParseSeeNut


#define REF_TAG "REF"

// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseRefNut (const char **a_ptr, const char  **ac_labelPtr, int *ac_labelLen) {
  const char *ptr = *a_ptr ;
  if (*ptr != '\0' && b_ParseNutTag(REF_TAG,&ptr)) {
    while (*ptr == ' ') ptr++;
    *ac_labelPtr = ptr; char c;
    while (b_LABEL_CHARSET(*ptr)) ptr++;
    *ac_labelLen = (ptr - *ac_labelPtr);
    *a_ptr = ptr;
    return b_NUT_PARSING_OK;
  } // if
  return b_NUT_PARSING_KO;
} // b_ParseRefNut

// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseVarNut (const char **a_ptr, const char  **ac_varPtr, int *ac_varLen) {
  const char *ptr = *a_ptr ;
  while (*ptr == ' ') ptr++;
  while (*ptr != '\0' && *ptr != '<') ptr++;
  if (*ptr != '\0') {
    *ac_varPtr = ptr;
    int depth = 0;
    while (*ptr == '<') {
      depth++;
      ptr++;
    } // while
    while (*ptr != '\0' &&  *ptr != '>') ptr++;
    while (*ptr != '\0' &&  --depth > 0 && *ptr == '>') ptr++;
    if (*ptr == '\0') return b_NUT_PARSING_KO;
    *a_ptr= ++ptr;
    *ac_varLen = (ptr - *ac_varPtr);
    return b_NUT_PARSING_OK;
  } // if
  return b_NUT_PARSING_KO;
} // b_ParseVarNut


#define ENDREF_TAG "ENDREF"

// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseEndrefNut (const char **a_ptr) {
  const char *ptr = *a_ptr ;
  if (*ptr != '\0' && b_ParseNutTag(ENDREF_TAG,&ptr)) {
    *a_ptr = ptr;
    return b_NUT_PARSING_OK;
  } // if
  return b_NUT_PARSING_KO;
} // b_ParseEndrefNut

#define SKIP_TAG "SKIP"

// Ret:
//  True:  => *a_ptr updated
//  False: => *a_ptr not updated
static int b_ParseSkipNut (const char **a_ptr) {
  const char *ptr = *a_ptr ;
  if (*ptr != '\0' && b_ParseNutTag(SKIP_TAG,&ptr)) {
    *a_ptr = ptr;
    return b_NUT_PARSING_OK;
  } // if
  return b_NUT_PARSING_KO;
} // b_ParseSkipNut



//
// - commentLine: point to (1st) '//' occurence in line
// - p_origVar1: original 'var 1' to be replaced
// - p_replVar1: replacement 'var 1'
// - p_origVar2: original 'var 2' to be replaced
// - p_replVar2: replacement 'var 2'
static char *ReplaceNutVar (char *commentLinePtr,
                            const char *p_origVar, const char *p_replVar) {
  char *c_varPtr = (char *) UNDEFINED;
  int c_varLen = UNDEFINED;
  const char *ptr = commentLinePtr;
  int n_replLen = -1;
  while (1) {
    if (b_ParseVarNut(&ptr, (const char **)&c_varPtr, &c_varLen)) {
      // Found
      if (XCmp(p_origVar,-1,c_varPtr,c_varLen) == 0) { // corresponds
        if (n_replLen == -1) { // Unknown length
          n_replLen = strlen(p_replVar);
          if (n_replLen > 1 && *p_replVar == '<' && *(p_replVar + n_replLen - 1) == '>') {
            p_replVar++ ; n_replLen -= 2;
          } // if          
        } // if          
        memmove(c_varPtr + n_replLen, c_varPtr + c_varLen,
                strlen(c_varPtr + c_varLen) + 1);
        memcpy(c_varPtr,p_replVar,n_replLen);
        ptr += (n_replLen - c_varLen);
      } // if
      continue;
    } // if
    break;
  } // while 

  return commentLinePtr;
} // ReplaceNutVar


#define COMMENT_PREFIX2 "//"
#define EXPANSION_PREFIX3 COMMENT_PREFIX2 "+"

// Add "//+" (i.e "expansion prefix" to line)
//
// Passed:
// - line1024: read line 
// - commentOffset: locate the 1st comment prefix in the line
//
// Changed: 
// - line1024: modified line 
static int AddExpansionPrefix (char *line1024, int commentOffset) {
  if (line1024[commentOffset] != '/') m_ANOMALY("no comment...")
  // Aim: "1234//56789" => "//+123456789"
  int lineLen = strlen(line1024);
  if (lineLen + 1 >= 1024) m_ANOMALY("line too long")
  // 1. "1234//56789" => "1234//.56789" (with final '\0')
  memmove(line1024+commentOffset+3, line1024+commentOffset+2, lineLen-commentOffset-1); 
  // 2. "1234//.56789" => "...123456789"
  memmove(line1024+3, line1024, commentOffset); 
  // 3. "...123456789" => "//+123456789"
  line1024[0] =  EXPANSION_PREFIX3[0]; line1024[1] = EXPANSION_PREFIX3[1]; line1024[2] = EXPANSION_PREFIX3[2];
  return lineLen + 1;
} // AddExpansionPrefix


#define EXPANSION_PROBLEM_LINE__FMT__S	EXPANSION_PREFIX3 " !!! INVALID REF: %s !!!\n"

// 3. ==== Coco parsers

enum {
  FIND_SPOT_FILE_OP__CONTENT,
  FIND_SPOT_FILE_OP__DEPTH,
} ;

enum {
  IN_REFERENCE__NOT_IN_REFERENCE,
  IN_REFERENCE__IN_REFERENCE,
  IN_REFERENCE__IN_REFERENCE_SKIP_DIRECTIVE,
  IN_REFERENCE__IN_REFERENCE_EXPANDED_LINE,
  IN_REFERENCE__IN_REFERENCE_END_DIRECTIVE,
} ;

#define b_SPOT_FILE_FOUND		1
#define b_SPOT_FILE_NOT_FOUND	(! b_SPOT_FILE_FOUND)

// Passed:
// - p_filepath:
// - p_label:
// - findSpotFileOp: FIND_SPOT_FILE_OP__CONTENT / FIND_SPOT_FILE_OP__DEPTH  
// - cnp_replacVar1: (FIND_SPOT_FILE_OP__CONTENT)
// - cnp_replacVar2: (FIND_SPOT_FILE_OP__CONTENT)
// - c_output: (FIND_SPOT_FILE_OP__CONTENT)
// - c_callsControl: (FIND_SPOT_FILE_OP__DEPTH)
//
// Changed:
// - cac_depth : (FIND_SPOT_FILE_OP__DEPTH) only significant if "found" ; >=0:  depth 0, 1, 2, ...
//
// Ret:
// - True  : spot file found and parsed (but label may or may not be found)
// - False : spot file not found (i.e could not have been opened)
static int b_FindSpotInFile (const char *p_filepath,
                             const char *p_label, int findSpotFileOp,
                             const char *cnp_replacVar1, const char *cnp_replacVar2,
                             FILE *c_output, int *cac_contentLength,
                             int c_callsControl, int *cac_depth) {
  FILE *h_f = fopen(p_filepath,"r") ;
  if (h_f == NULL) return b_SPOT_FILE_NOT_FOUND;
  if (findSpotFileOp == FIND_SPOT_FILE_OP__DEPTH) {
    if (c_callsControl > 25) m_ANOMALY("OVERFLOW")
  } else *cac_contentLength = 0;
  int inReference = IN_REFERENCE__NOT_IN_REFERENCE; // not in reference
  int maxDepth = 0 ;
  char b_origVar1 = b_FALSE0;
  char c_origVar1[256];
  char b_origVar2 = b_FALSE0;
  char c_origVar2[256];
  char line[1024] ;

  while (fgets(line,sizeof(line),h_f) != NULL) {
    int len = strlen(line);

    if (len == 0 || line[len-1] != '\n') {
      m_ANOMALY("line in file is too long")
    } // if
    if (memcmp(line,META_INFO_LINE_PREFIX,strlen(META_INFO_LINE_PREFIX)) == 0) {
      continue;
    } // if
    //# 1. ESTABLISH inReference state:
    char *n_commentPtr = strstr(line,COMMENT_IN_LINE) ;

    if (n_commentPtr != NULL) {
      if (memcmp(line,EXPANSION_PREFIX3,3) != 0) {
        const char *ptr = n_commentPtr;
        const char *c_labelPtr = NULL;
        int c_labelLen = UNDEFINED ;

        if (b_ParseRefNut(&ptr,&c_labelPtr,&c_labelLen)) {
          // Found a REF tag
          if (XCmp(p_label,-1,c_labelPtr,c_labelLen) == 0) { // in reference
            inReference = IN_REFERENCE__IN_REFERENCE;
            if (findSpotFileOp == FIND_SPOT_FILE_OP__CONTENT) {
              char *c_origVarPtr = (char *)UNDEFINED;
              int c_origVarLen = UNDEFINED;

              if (b_ParseVarNut(&ptr,(const char **)&c_origVarPtr,&c_origVarLen)) {
                // Found a ref orig value
                b_origVar1 = b_TRUE;
                m_CPY(c_origVar1,c_origVarPtr,c_origVarLen)
                if (b_ParseVarNut(&ptr,(const char **)&c_origVarPtr,&c_origVarLen)) {
                  // Found a 2nd ref orig value
                  b_origVar2 = b_TRUE;
                  m_CPY(c_origVar2,c_origVarPtr,c_origVarLen)
                } else { // Not found 2nd ref orig value
                  b_origVar2 = b_FALSE0;
                } // if
              } else { // Not found ref orig value
                b_origVar1 = b_origVar2 = b_FALSE0;
              } // if
            } // if
          } else {
            b_origVar1 = b_origVar2 = b_FALSE0;
            inReference = IN_REFERENCE__NOT_IN_REFERENCE ;
          } // if
        } else { // Not found ref TAG
          if (inReference > IN_REFERENCE__NOT_IN_REFERENCE) {
            if (b_ParseEndrefNut(&ptr)) {
              // Found a END REF tag
              inReference = IN_REFERENCE__IN_REFERENCE_END_DIRECTIVE ;
            } else { // Not found END REF tag
              if (b_ParseSkipNut(&ptr)) {
                // Found a SKIP tag
                inReference = IN_REFERENCE__IN_REFERENCE_SKIP_DIRECTIVE ;
              } // if
            } // if
          } // if
        } // if
      } else if (inReference > IN_REFERENCE__NOT_IN_REFERENCE) {
        inReference = IN_REFERENCE__IN_REFERENCE_EXPANDED_LINE;
      } // if 
    } else {
      inReference = IN_REFERENCE__NOT_IN_REFERENCE ; 
    } // if
    //# 2. TREAT inReference state 
    if (findSpotFileOp == FIND_SPOT_FILE_OP__CONTENT) {
      if (inReference > IN_REFERENCE__NOT_IN_REFERENCE &&
          inReference != IN_REFERENCE__IN_REFERENCE_SKIP_DIRECTIVE) {
        if (b_origVar1 && cnp_replacVar1 != NULL) {
          ReplaceNutVar(n_commentPtr,
                        c_origVar1, cnp_replacVar1);
        } // if
        if (b_origVar2 && cnp_replacVar2 != NULL) {
          ReplaceNutVar(n_commentPtr,
                        c_origVar2, cnp_replacVar2);
        } // if
        *cac_contentLength += AddExpansionPrefix(line,n_commentPtr-line);
        fputs(line,c_output);
      } // if 
    } else { //# findSpotFileOp == FIND_SPOT_FILE_OP__DEPTH
      if (inReference > IN_REFERENCE__NOT_IN_REFERENCE &&
          inReference != IN_REFERENCE__IN_REFERENCE_EXPANDED_LINE) {
        if (n_commentPtr == NULL) m_ANOMALY("No comment...")
        const char *ptr = n_commentPtr;
        const char *n_labelPtr = NULL, *cn_spotFilePtr = NULL;
        int c_labelLen, cc_spotFileLen;
        if (b_ParseSeeNut(&ptr, &n_labelPtr, &c_labelLen,
                                   &cn_spotFilePtr, &cc_spotFileLen)) {
          char label256[256], filepath256[256];
          m_CPY(label256,n_labelPtr,c_labelLen)
          if (cn_spotFilePtr != NULL) {
            m_CPY(filepath256,cn_spotFilePtr,cc_spotFileLen)
          } else {
            m_CPY(filepath256,p_filepath,strlen(p_filepath))
          } // if
          int thisDepth = UNDEFINED;
          if (!b_FindSpotInFile(filepath256, label256, FIND_SPOT_FILE_OP__DEPTH,
                                (const char *)UNDEFINED, (const char *)UNDEFINED,
                                (FILE *)UNDEFINED, (int *)UNDEFINED,
                                c_callsControl,&thisDepth)) thisDepth = 0;
          thisDepth++;
          if (thisDepth > maxDepth) maxDepth = thisDepth ;
        } // if
      } // if
    } // if
    //# 3. UPDATE inReference state
    if (inReference == IN_REFERENCE__IN_REFERENCE_END_DIRECTIVE) {
      inReference = IN_REFERENCE__NOT_IN_REFERENCE ; 
    } else if (inReference == IN_REFERENCE__IN_REFERENCE_SKIP_DIRECTIVE) {
      inReference = IN_REFERENCE__IN_REFERENCE ; 
    } // if
  } // while 
 
  fclose(h_f) ;
  if (findSpotFileOp == FIND_SPOT_FILE_OP__DEPTH) {
    *cac_depth = maxDepth;
  } // if
  return b_SPOT_FILE_FOUND;
} // b_FindSpotInFile;

#define b_NO_META_DATA	1

// Passed:
// - p_filepath:
// - n_step:
//   + -1 special value => preliminary stage
//   + >=0 : actual step
// - p_newFile:
// Ret:
// - True (b_NO_META_DATA) file is "clean" ; that is, does not contain meta data anymore
// - False: "temporary state" ; still contains meta data ; need to complete extra step...
static int  b_UpdateRefs (const char *p_filepath, int n_step, const char *p_newFile) {
  m_MINOR_INFO("file : %s",p_filepath);
  int n_processed = -1, n_neededStep = -1; // unknown for the moment
  int  b_noMetaData = b_FALSE0 ;
  if (n_step == -1) {
    b_noMetaData = b_TRUE ;
  } else if (n_step >= 0) {
    b_noMetaData = (n_step > 0) ;
  } // if
  FILE *f = fopen(p_filepath,"r") ;
  m_ANOMALY_IF(f == NULL) 

  FILE* output = fopen(p_newFile,"w");
  if (output == NULL) { 
    perror("fopen()"); // Permission problem...
    m_ANOMALY("Cannot open new temp file");
  } // if

  char line[1024] ;
  while (fgets(line,sizeof(line),f) != NULL) {
    int len = strlen(line);
    if (len == 0 || line[len-1] != '\n') {
      m_ANOMALY("line in file is too long")
    } // if

    if (FindMetaInfoInLine(line, &n_processed, &n_neededStep)) {
      continue;
    } // if
    if (memcmp(line,EXPANSION_PREFIX3,3) == 0) {
      if (n_step > 0) fputs(line,output) ;
      continue;
    } // if

    char *n_commentPtr = strstr(line,COMMENT_IN_LINE) ;
    if (n_commentPtr == NULL) {
      fputs(line,output);
      continue;
    } // if
    const char *ptr = n_commentPtr;
    const char *n_labelPtr = NULL, *cn_spotFilePtr ;
    int b_replVar1 = b_FALSE0, b_replVar2 = b_FALSE0;
    char c_replVar1_256[256],  c_replVar2_256[256];
    int c_labelLen = UNDEFINED, cc_spotFileLen;
    if (b_ParseSeeNut(&ptr, &n_labelPtr, &c_labelLen,
                      &cn_spotFilePtr, &cc_spotFileLen)) {
      char label256[256], filepath256[256];
      m_CPY(label256,n_labelPtr,c_labelLen)
      if (cn_spotFilePtr != NULL) {
        m_CPY(filepath256,cn_spotFilePtr,cc_spotFileLen)
      } else {
        m_CPY(filepath256,p_filepath,strlen(p_filepath))
      } // if

      const char *n_replVar1Ptr = NULL, *n_replVar2Ptr = NULL;
      int c_replVar1Len, c_replVar2Len;
      if (b_ParseVarNut(&ptr,&n_replVar1Ptr,&c_replVar1Len)) {
        // Found a ref orig value
        b_replVar1 = b_TRUE;
        m_CPY(c_replVar1_256,n_replVar1Ptr,c_replVar1Len)
        if (b_ParseVarNut(&ptr,&n_replVar2Ptr,&c_replVar2Len)) {
          // Found a 2nd ref orig value
          b_replVar2 = b_TRUE;
          m_CPY(c_replVar2_256,n_replVar2Ptr,c_replVar2Len)
        } else { // Not found 2nd ref orig value
          b_replVar2 = b_FALSE0;
        } // if
      } else { // Not found ref orig value
        b_replVar1 = b_replVar2 = b_FALSE0;
      } // if

      if (n_step == 0) {
        if (!b_FindSpotInFile(filepath256, label256, FIND_SPOT_FILE_OP__DEPTH,
                              (const char *)UNDEFINED, (const char *)UNDEFINED,
                              (FILE *)UNDEFINED, (int *)UNDEFINED,
                              0,&n_neededStep)) n_neededStep = 0;
      } // if 
      if (n_step != -1 && n_neededStep != -1 && n_neededStep >= n_step) {
        b_noMetaData = b_FALSE0 ;
        fprintf(output,META_INFO_LINE_PREFIX NEEDED_STEP__FMT__D "\n",n_neededStep);
      } // if
      fputs(line,output);
      if (n_step != -1 && n_neededStep != -1 && n_neededStep < n_step) {
        int c_contentLen = UNDEFINED;
        int b_found = b_FindSpotInFile(filepath256, label256, FIND_SPOT_FILE_OP__CONTENT,
                                       b_replVar1? c_replVar1_256: NULL, b_replVar2? c_replVar2_256: NULL,
                                       output, &c_contentLen,
                                       UNDEFINED,(int *)UNDEFINED);
        if (b_found) {
          if (c_contentLen == 0) {
            fprintf(output,EXPANSION_PROBLEM_LINE__FMT__S,"LABEL not found");
          } // if
        } else {
          fprintf(output,EXPANSION_PROBLEM_LINE__FMT__S,"FILE not found");
        } // if
      } // if
    } else {
      fputs(line,output);
    } // if
    n_neededStep = -1;
  } // while

  if (!b_noMetaData) {
    fprintf(output,META_INFO_LINE_PREFIX PROCESSED__FMT__D "\n",n_step);
  } // if

  fclose(f);
  fclose(output);

  return b_noMetaData;
} // b_UpdateRefs

#define b_ALL_STEPS_COMPLETED	1

//  # This helper wraps "Update Refs" awk script (<file> <step>)
//
// Passed:
// - p_filepath:
// - n_step:
//   + -1 special value => preliminary stage
//   + >=0 : actual step
//
// Ret:
// - True (b_ALL_STEPS_COMPLETED)
// - False
static int b_RefreshRefs (const char *p_filepath, int n_step, const char *p_newFile) {
  int b_allStepsCompleted = b_FALSE0; // Still "In progress" a priori...
  if (b_UpdateRefs(p_filepath, n_step, p_newFile)) {
    b_allStepsCompleted = (n_step != -1);
  } // if
  UpdateFile(p_newFile,p_filepath);
  return b_allStepsCompleted ;
} // b_RefreshRefs


//
static int PruneExpandedRefs (const char *p_filepath, int pruning, const char *p_newFile) {
  m_MINOR_INFO("file: %s",p_filepath)
  FILE *f = fopen(p_filepath,"r") ;
  int count = 0;
  m_ANOMALY_IF(f == NULL)

  FILE* output = fopen(p_newFile,"w");
  if (output == NULL) { 
    perror("fopen()"); // Permission problem...
    m_ANOMALY("Cannot open new temp file");
  } // if

  char line[1024] ;
  m_ANOMALY_IF(pruning < 0)
  if (pruning > 200) pruning = 200;
  int fLen = 3 + pruning;
  char *h_filter = malloc(fLen + 1); 
  h_filter[0] = '/';  
  h_filter[1] = '/';  
  int i = 2;
  for (; i < fLen ; i++) h_filter[i] = '+';
  h_filter[fLen] = '\0';
  while (fgets(line,sizeof(line),f) != NULL) {
    int len = strlen(line);
    if (len == 0 || line[len-1] != '\n') {
      m_ANOMALY("line in file is too long")
    } // if
    if (len < fLen || memcmp(line,h_filter,fLen) != 0) {
      fputs(line,output);
    } else count++;
  } // while

  fclose(f) ;
  fclose(output);
  free(h_filter);
  UpdateFile(p_newFile,p_filepath);
  return count;
} // PruneExpandedRefs


// 4. ==== File pathnames handling

//
static int GetFilesList (const char *p_dirList, int n_dirDepth, const char *np_filter,
                         char ***aznhsnh_filepathsList) {
  char *h_findListCommand = (char *)UNDEFINED;
  char *h_maxDepthDirective = (char *)UNDEFINED;
  char *h_nameDirective  = (char *)UNDEFINED;

  if (np_filter != NULL) {
    asprintf(&h_nameDirective, "-name \"*.%s\"", np_filter);
  } else {
    h_nameDirective = strdup("");
  } // if
  m_ANOMALY_IF(h_nameDirective == NULL)
  

  if (n_dirDepth != -1) {
    asprintf(&h_maxDepthDirective, "-maxdepth %d", n_dirDepth);
  } else {
    h_maxDepthDirective = strdup("");
  } // if

  asprintf(&h_findListCommand,
           "find %s %s -type f %s",
             p_dirList,h_maxDepthDirective,h_nameDirective);
  m_ANOMALY_IF(h_findListCommand == NULL)

  FILE *stream = popen(h_findListCommand, "r");
  if (stream == NULL) {
    return 0;
  } // if
  char pathname[1024] ;
  int cpt = 0;

  *aznhsnh_filepathsList = NULL;
  while (fgets(pathname,sizeof(pathname),stream) != NULL) {
    *aznhsnh_filepathsList = realloc(*aznhsnh_filepathsList,sizeof(char *) * ++cpt) ;
    int len = strlen(pathname);
    if (len > 0 && pathname[len-1] == '\n') {
      pathname[len-1] = '\0';
    } // if
    (*aznhsnh_filepathsList)[cpt-1] = strdup(pathname);
  } // while

  pclose(stream);
  free(h_findListCommand);
  return cpt ;
} // GetFilesList


//
static char *Flatname (const char *p_filepath) {
  static char buffer[1024];
  int len = strlen(p_filepath);
  m_ANOMALY_IF(len >= 1024)
  const char *src = p_filepath;
  char *dst = buffer;
  for (; (*dst = *src) != '\0'; src++, dst++) {
    if (*dst == '/') *dst = '-';
  } // for
  return buffer;
}

//
static char *NewVersion (const char *p_filepath, const char *p_tmpdir) {
  static char buffer[1024] ;

  snprintf(buffer,sizeof(buffer),"%s/%s.new",p_tmpdir,Flatname(p_filepath));

  return buffer;
} // NewVersion


// 5. ==== Program arguments parsing

// 
static void CocoCommandWrongArgument (const char *p_cause) {
  fprintf(stderr,
          "%s. Expected arguments:\n"
          " (coca | cocu)\n"
          " [ -t <tmpdir> ] [ -m <max step/pruning> ] [ -f <filter> ]\n"
          " [ -d <dir depth> ] [ -v (s | v | V) ]\n"
          " [ -r ]\n"
          " <root dir1> [ <root dir2> ... ]\n\n"
          " -v s => SILENT\n"
          "    v => LOW verbosity\n"
          "    V => HIGH verbosity\n"
          " -r => raw modif date - DO NOT preserve source files' last modif date\n",
          p_cause);
  exit(100);
} // CocoCommandWrongArgument


enum {
  COCA_ACTION,
  COCU_ACTION
} ;


#define DEFAULT_MAX_STEP 10
#define DEFAULT_PRUNING  1
#define DEFAULT_TMPDIR "/tmp"
#define n_DEFAULT_FILTER NULL
#define n_DEFAULT_MAX_DIR_DEPTH -1

#define DEFAULT_VERBOSITY   LOW__VERBOSITY

//
void ParseCocoCommandArguments (int argc, char **p_argv, char *info256,
                                int *a_action, char **azh_tmpdir, int *acn_maxStepOrPruning,
                                char **aznh_filter, int *an_dirDepth, 
                                char **azh_dirList,
                                int *ab_rawModifTime) {
 
  info256[0] = '\0';

  if (argc < 3) {
    CocoCommandWrongArgument("Missing arguments (min 2)") ;
  } // if

  if (strcmp(p_argv[1],"coca") == 0) {
    *a_action = COCA_ACTION;
    *acn_maxStepOrPruning = DEFAULT_MAX_STEP; 
  } else if (strcmp(p_argv[1],"cocu") == 0) {
    *a_action = COCU_ACTION;
    *acn_maxStepOrPruning = DEFAULT_PRUNING; 
  } else  {
    snprintf(info256,256,"Invalid argument (action) [%s]",p_argv[1]);
    CocoCommandWrongArgument(info256) ;
  } // if
  optind += 1;

  int opt = UNDEFINED;
  char info32[32];
  *ab_rawModifTime = b_FALSE0;
  *azh_tmpdir = strdup(DEFAULT_TMPDIR) ; 
  *aznh_filter = n_DEFAULT_FILTER; 
  *an_dirDepth = n_DEFAULT_MAX_DIR_DEPTH; 
  verbosity = DEFAULT_VERBOSITY;
  while ((opt = getopt(argc, p_argv, ":t:m:f:d:v:r")) != -1) {
    switch (opt) {
    case 't':
      free(*azh_tmpdir);
      *azh_tmpdir = strdup(optarg);
    break; case 'm':
      if (!GetIntArg(optarg,*a_action == COCA_ACTION? -1: 0,25,acn_maxStepOrPruning,info32)) {
        snprintf(info256,256,"Bad optarg=[%s] (%s) with opt -%c",optarg,info32,opt);
        CocoCommandWrongArgument(info256) ;
      } // if
    break; case 'f':
      if (*aznh_filter != n_DEFAULT_FILTER) {
        snprintf(info256,256,"Bad opt -%c %s: option already passed",opt,optarg);
        CocoCommandWrongArgument(info256) ;
      } // if
      free(*aznh_filter);
      *aznh_filter = strdup(optarg); 
    break; case 'd':
      if (!GetIntArg(optarg,1,25,an_dirDepth,info32)) {
        snprintf(info256,256,"Bad optarg=[%s] (%s) with opt -%c",optarg,info32,opt);
        CocoCommandWrongArgument(info256) ;
      } // if
    break; case 'v':
      switch (optarg[0]) {
      case 's': 
        verbosity = SILENT__VERBOSITY; 
      break; case 'v':
        verbosity = LOW__VERBOSITY; 
      break; case 'V':
        verbosity = HIGH__VERBOSITY; 
      break; default:
        snprintf(info256,256,"Bad optarg=[%s] with opt -%c",optarg,opt);
        CocoCommandWrongArgument(info256) ;
      } // switch
    break; case 'r':
      *ab_rawModifTime = b_TRUE;
    break; default: /* '?' */
      snprintf(info256,256,"bad opt -%c (%c)",optopt,opt);
      CocoCommandWrongArgument(info256) ;
    } // switch
  } // while

  if (optind >= argc) {
    CocoCommandWrongArgument("Missing root dir") ;
  } // if 
  char s256[256];
  snprintf(s256,sizeof(s256),"\"%s\"",p_argv[optind++]);
  *azh_dirList = strdup(s256);  
  int len = strlen(s256);
  while (optind < argc) {
    snprintf(s256,sizeof(s256)," \"%s\"",p_argv[optind++]);
    len += strlen(s256);
    *azh_dirList = realloc(*azh_dirList,len+1) ;
    strcat(*azh_dirList,s256);
  } // while

  snprintf(info256,256,
           "action=%s ; tmpdir=%s ; max step/pruning=%d ; "
           "filter=%s ; "
           "dir depth=%d ; verbosity=%d ; root dir list=%s",
           p_argv[1], *azh_tmpdir, *acn_maxStepOrPruning,
           *aznh_filter == NULL? "n/a": *aznh_filter,
           *an_dirDepth, verbosity, *azh_dirList);
} // ParseCocoCommandArguments


// 6. ==== main

//
int main (int argc, char **p_argv) {
  char *h_dirList ;
  int n_dirDepth = UNDEFINED ;
  int cn_maxStepOrPruning = UNDEFINED ;
  int i = UNDEFINED;
  int action = UNDEFINED;
  char *h_tmpdir;
  char *nh_filter;
  char info256[256];

  ParseCocoCommandArguments(argc,p_argv,info256,
                            &action, &h_tmpdir, &cn_maxStepOrPruning,
                            &nh_filter, &n_dirDepth,
                            &h_dirList,
                            &b_rawModifTime) ;
  m_MINOR_INFO("Arguments: %s",info256)

  char **nhsnh_filepathsList ; 
  int cpt = GetFilesList(h_dirList, n_dirDepth, nh_filter, &nhsnh_filepathsList);
  m_MAJOR_INFO("%d file(s) will be processed...",cpt)
  int n_step = -1;
  int b_allProcessed = b_FALSE0;

  switch(action) {
  case COCA_ACTION:
    { char *hsb_allStepsCompleted = calloc(cpt,sizeof(char));
      while (! b_allProcessed) {
        m_MAJOR_INFO("=> Step:%d",n_step)
        b_allProcessed = b_TRUE; // A priori
        for (i = 0 ; i < cpt ; i++) {
          if (hsb_allStepsCompleted[i]) continue;
          const char *p_f = nhsnh_filepathsList[i];
          if (! (hsb_allStepsCompleted[i] = b_RefreshRefs(p_f, n_step, 
                                                          NewVersion(p_f,h_tmpdir))) ) {
            b_allProcessed = b_FALSE0;
          } else {
            m_MINOR_INFO("file : %s => COMPLETED",p_f)
          } // if
        } // for 
        if (b_allProcessed) {
          m_MAJOR_INFO("All files processed.")
        } else {
          n_step++;
          if (n_step > cn_maxStepOrPruning) {
            m_MAJOR_INFO("Completed step %d => stop processing",cn_maxStepOrPruning)
            b_allProcessed = b_TRUE;
          } // if 
        } // if
      } // while 
      free(hsb_allStepsCompleted);
    } // hsb_allStepsCompleted
  break; case COCU_ACTION:
    for (i = 0; i < cpt; i++) {
      const char *p_f = nhsnh_filepathsList[i];
      PruneExpandedRefs(p_f, cn_maxStepOrPruning, NewVersion(p_f,h_tmpdir));
    } // for
    m_MAJOR_INFO("All files processed.")
  break; default:
    m_ANOMALY("unknown action")
  } // switch  

  // Too lazy to free the memory (nhsnh_filepathsList / hsb_allStepsCompleted, etc.)
  return 0;
} // main
