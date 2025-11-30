// c-ansi/marcel.c, version 1.93
// (c) Atos-Euronext Belgium - 2003
// (c) Puduku - 2023


#include "c-ansi/marcel.h"

const struct TOKEN_DEFINITION p_commonMimeTypes[] = {
  { "application/octet-stream",  APPLICATION_OCTET_STREAM__COMMON_MIME_TYPE },
  { "application/x-www-form-urlencoded", APPLICATION_X_WWW_FORM_URLENCODED__COMMON_MIME_TYPE },
  { "text/html", TEXT_HTML__COMMON_MIME_TYPE },
  { "text/plain", TEXT_PLAIN__COMMON_MIME_TYPE },
} ;

// Public function: see .h 
int l_MarcelCreateInstance(g_G_TOKENS_HANDLE *azh_handle) {
  m_TRACK_IF(l_GTokensCreateInstance(azh_handle,COMMON_MIME_TYPES_NUMBER) != RETURNED)
  m_TRACK_IF(l_GTokensImport((*azh_handle),p_commonMimeTypes,  COMMON_MIME_TYPES_NUMBER) != RETURNED)
  m_TRACK_IF(g_GTokensFreeze(*azh_handle,NULL) != RETURNED)
  return RETURNED;
} // l_MarcelCreateInstance
