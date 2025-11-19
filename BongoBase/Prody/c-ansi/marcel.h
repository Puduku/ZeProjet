// c-ansi/marcel.h, version 1.93
// (c) Atos-Euronext Belgium - 2003
// (c) Puduku - 2023

// Description :
// Necessary stuff to handle standard MIME types 

#ifndef __C_ANSI_MARCEL_H_INCLUDED__
#define __C_ANSI_MARCEL_H_INCLUDED__

#include "c-ansi/g-token.h"


// Marcel's common MIME types enumeration
enum {
  APPLICATION_OCTET_STREAM__COMMON_MIME_TYPE = 0,
  APPLICATION_X_WWW_FORM_URLENCODED__COMMON_MIME_TYPE,
  TEXT_HTML__COMMON_MIME_TYPE,
  TEXT_PLAIN__COMMON_MIME_TYPE,
  COMMON_MIME_TYPES_NUMBER 
} ; 

// TODO: add indexes

extern const struct TOKEN_DEFINITION p_commonMimeTypes[] ;



static inline int m_MarcelCreateInstance(g_G_TOKENS_HANDLE *azh_handle) {
  m_TRACK_IF(ml_GTokensCreateInstance(azh_handle,COMMON_MIME_TYPES_NUMBER) != RETURNED)
  m_G_TOKENS_IMPORT((*azh_handle),p_commonMimeTypes,  COMMON_MIME_TYPES_NUMBER)
  m_TRACK_IF(g_GTokensFreeze(*azh_handle,NULL) != RETURNED)
  return RETURNED;
} // m_MarcelCreateInstance


#endif // __C_ANSI_MARCEL_H_INCLUDED__
