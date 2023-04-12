// c-ansi/mime.c, version 1.93
// (c) Atos-Euronext Belgium - 2003

// Description :
//
// MIME type handling 

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "c-ansi/mime.h"

#if 0

// Standard Mime types 

const char *standardMimeTypeStrings[COMMON_MIME_TYPES_NUMBER] = { 
  "application/octet-stream", 
  "application/x-www-form-urlencoded",
  "text/html",
  "text/plain"
} ; 

static int sn_standardMimeTypeStringLengths[COMMON_MIME_TYPES_NUMBER] = { 
  -1,
  -1, 
  -1, 
  -1,
} ;


// Ret:
// - >= 0: common mime type entry
// - -1: not found 
static int SearchCommonMimeType (const char *p_string, int n_length) {
  int length = n_length;
  if (n_length == -1) {
    length = strlen(string) ;
  } // if
  
  int i = 0; 
  for ( ; i < COMMON_MIME_TYPES_NUMBER ; i++) {
    if (sn_standardMimeTypeStringLengths[i] == -1) {
      sn_standardMimeTypeStringLengths[i] =
      length2 = strlen(standardMimeTypeStrings[i]);
    }
    int length2 = sn_standardMimeTypeStringLengths[i];
    if (length == length2 && 
        strncmp(standardMimeTypeStrings[i],string,length2) == 0) {
      return i ; 
    } // if
  } // for 

  return -1;
} // SearchMyStandardMimeType 


// Public function ; see .h
int MimeTypeCreateInstance (MIME_TYPE_HANDLE *azh_handle) {
  m_MALLOC_INSTANCE(*(azh_handle)) 
  m_TRACK_IF( G_STRING_CREATE_INSTANCE((*(azh_handle))->hc_copiedMineType) != RETURNED)  
  return RETURNED;
}


// Public function ; see .h
int MimeTypeCCopy (MIME_TYPE_HANDLE handle,
                    const char* p_string,
                    int n_length) ;
  if ( ((handle).n_commonMimeType = SearchCommonMimeType(p_string,n_length)) == -1 ) {
    m_TRACK_IF(GStringCCopy((handle)->hc_copiedMimeType,0,p_string,n_length) < 0)
    return RESULT__NOT_FOUND;
  } // if 
  return RESULT__FOUND;
} // MimeTypeCCopy


// Public function ; see .h
int MimeTypeDestroyInstance (MIME_TYPE_HANDLE xh_handle) ;
  m_TRACK_IF(G_STRING_DESTROY_INSTANCE((xh_handle)->hc_copiedMineType) != RETURNED)
  free(xh_handle);
  return RETURNED;
} // MimeTypeDestroyInstance

#else
  int varBidon = 23;
#endif
