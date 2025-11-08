// c-ansi/inifile.c

#include "c-ansi/inifile.h"
#include "c-ansi/diggy.h"

struct INICONF {
  int n_iniFileFormat;
  SUCKER_HANDLE c_sourceSuckerHandle;
} ;


// Public function: see .h
int IniconfCreateInstance (INICONF_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD()
 
  m_MALLOC_INSTANCE(*azh_handle)
  INICONF_HANDLE handle = *azh_handle;
  handle->n_iniFileFormat = -1;
  handle->c_sourceSuckerHandle = (SUCKER_HANDLE) UNDEFINED;
  m_DIGGY_RETURN(RETURNED)
} 


// Public function: see .h
int IniconfAttach (INICONF_HANDLE handle, int iniFileFormat, SUCKER_HANDLE f_sourceSuckerHandle){
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(RETURNED)
} 
 

// Public function: see .h
int IniconfReadConfig (INICONF_HANDLE handle, struct P_STRING className,
  G_STRINGS_HANDLE gParamsHandle) {
  m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(RESULT__FOUND)
} 


// Public function: see .h
int IniconfDestroyInstance (INICONF_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()
  free(xh_handle);
  m_DIGGY_RETURN(RETURNED)
} // IniconfDestroyInstance

