// c-posix/sucker.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"

#include "c-ansi/sucker.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"



// Public function ; see description in .h
int SuckerSuckOut (SUCKER_HANDLE handle, const struct SUCKER_BUTT *ap_sSuckerButt,
  const struct SUCKER_BUTT *ap_dSuckerButt,  int *na_suckedOutLength, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int n_suckerStatus = -1 ; // unknown status
  int suckedOutLength = 0;

  int suckingLimit = SuckerGetSuckingLimit(handle) ;
  m_TRACK_IF(suckingLimit < 0) 

  m_TRACK_IF(SuckerPlugSButt(handle,ap_sSuckerButt) != RETURNED) 
  m_TRACK_IF(SuckerPlugDButt(handle,ap_dSuckerButt) != RETURNED) 
      
  struct STRING_PORTION c_chunk; // Not significant in case of flop... 
  while (n_suckerStatus == -1) {
    int suckedLength = 0;

    switch (SuckerSuckSButt(handle, &c_chunk, nar_flopCause)) {
    case ANSWER__YES:
      suckedLength = m_StringPortionLength(&c_chunk);
      if (b_EMPTY_STRING_PORTION(c_chunk)) {
        n_suckerStatus = SUCKER_STATUS__OK;
      // TODO: suckingLimit suckedOutLength int64_t
      } else if (suckingLimit > 0 && suckedOutLength + suckedLength > suckingLimit) {
        n_suckerStatus = SUCKER_STATUS__LIMIT_BREACHED;
      } else { 
        switch (SuckerFillDButt(handle, c_chunk, nar_flopCause)) {
        case ANSWER__YES:
          suckedOutLength += suckedLength ;
        break; case ANSWER__NO:
          n_suckerStatus = SUCKER_STATUS__D_FLOP;
        break; default:
          m_TRACK()
        } // switch 
      } // if
    break; case ANSWER__NO:
      n_suckerStatus = SUCKER_STATUS__S_FLOP;
    break; default:
      m_TRACK()
    } // switch
  } // while

  if (na_suckedOutLength != NULL) {
    *na_suckedOutLength = suckedOutLength;
  } // if

  m_DIGGY_RETURN(n_suckerStatus)
} // SuckerSuckOut

