// c-ansi/mime.h, version 1.93
// (c) Atos-Euronext Belgium - 2003

// Description :
// Necessary stuff to handle standard MIME types 

#ifndef __C_ANSI_MIME_H_INCLUDED__
#define __C_ANSI_MIME_H_INCLUDED__

#if 0

#include "mystring.h"


// Our common MIME types enumeration (array entry)
enum {
  APPLICATION_OCTET_STREAM__COMMON_MIME_TYPE = 0,
  APPLICATION_X_WWW_FORM_URLENCODED__COMMON_MIME_TYPE,
  TEXT_HTML__COMMON_MIME_TYPE,
  TEXT_PLAIN__COMMON_MIME_TYPE,
  COMMON_MIME_TYPES_NUMBER 
} ; 

extern const char *commonMimeTypesStrings[COMMON_MIME_TYPES_NUMBER] ;

struct MIME_TYPE {
  int n_commonMimeType ; // -1 special value => not a "common" mime type...
  // CONVENTION: when non common MIME type, always warranted in copied state :
  G_STRING_STUFF hc_copiedMimeType ;
} ;

typedef struct MIME_TYPE *MIME_TYPE_HANDLE ;

// Ret:
// RETURNED: OK
// - -1: anomaly is raised
int MimeTypeCreateInstance (MIME_TYPE_HANDLE *azh_handle) ;
  
// Ret:
// - RESULT__FOUND: common mime type 
// - RESULT__NOT_FOUND: not a "common" mime type"... 
// - -1: anomaly is raised
int MimeTypeCCopy (MIME_TYPE_HANDLE handle,
                    const char* p_string,
                    int n_length) ;


// Ret:
// RETURNED: OK
// - -1: anomaly is raised
int MimeTypeDestroyInstance (MIME_TYPE_HANDLE xh_handle) ;

#endif

#endif // __C_ANSI_MIME_H_INCLUDED__
