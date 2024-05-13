// c-ansi/sucker.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/alloc.h"
#include "c-ansi/magic.h"

#include "c-ansi/sucker.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"



// 1. FILE_BUTT_ADEPT 

struct FILE_BUTT_ADEPT {
  m_DECLARE_MAGIC_FIELD(FILE_BUTT_ADEPT_HANDLE)
  int n_readBufferSize; 
  char *ch_readBuffer; // Only significant if n_readBufferSize >= 0  
  FILE *nh_file;
} ; 


// Public function: see .h
int FileButtAdeptCreateInstance(FILE_BUTT_ADEPT_HANDLE *azh_handle,
  const char *p_filePathname, int n_readBufferSize) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 
  FILE_BUTT_ADEPT_HANDLE handle = *azh_handle;

  m_ASSIGN_MAGIC_FIELD(FILE_BUTT_ADEPT_HANDLE,handle)
  const char* openFlags = "w"  ;
  if ((handle->n_readBufferSize = n_readBufferSize) > 0) {
    openFlags = "r"; 
    m_MALLOC(handle->ch_readBuffer,n_readBufferSize)
  } // if 
  handle->nh_file = fopen(p_filePathname,openFlags);

  m_DIGGY_RETURN(RETURNED)
} // FileButtAdeptCreateInstance


// BUTT_ADEPT_SUCK_FUNCTION
// Public function: see .h
int FileButtAdeptSuck(void *r_handle,  char b_kickButt, struct P_STRING *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  FILE_BUTT_ADEPT_HANDLE handle = (FILE_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(FILE_BUTT_ADEPT_HANDLE,handle)

  m_ASSERT(handle->n_readBufferSize > 0)

  if (handle->nh_file == NULL) {
    if (nar_flopCause != NULL) *nar_flopCause = FILE_FLOP_CAUSE__COULD_NOT_OPEN_FILE;
    m_DIGGY_RETURN(ANSWER__NO)
  } // if

  // b_kickButt is ignored

  int ret = fread(handle->ch_readBuffer,handle->n_readBufferSize,1,handle->nh_file);
  m_ASSERT(ret >= 0)
  m_ASSIGN_P_STRING(*ac_chunk,handle->ch_readBuffer,ret) 

  m_DIGGY_RETURN(ANSWER__YES)
} // FileButtAdeptSuck


// BUTT_ADEPT_FILL_FUNCTION
// Public function: see .h
int FileButtAdeptFill(void *r_handle,  struct P_STRING chunk, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  FILE_BUTT_ADEPT_HANDLE handle = (FILE_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(FILE_BUTT_ADEPT_HANDLE,handle)

  m_ASSERT(handle->n_readBufferSize < 0)

  if (handle->nh_file == NULL) {
    if (nar_flopCause != NULL) *nar_flopCause = FILE_FLOP_CAUSE__COULD_NOT_OPEN_FILE;
    m_DIGGY_RETURN(ANSWER__NO)
  } // if

  int ret = fwrite(chunk.string,m_PStringLength(&chunk),1,handle->nh_file);
  m_ASSERT(ret >= 0)
  // TODO: gedrer edcritures partielles...
  m_ASSERT(ret == m_PStringLength(&chunk)) 

  m_DIGGY_RETURN(ANSWER__YES)
} // FileButtAdeptFill  

// Public function: see .h
int FileButtAdeptDestroyInstance(FILE_BUTT_ADEPT_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  if (xh_handle->n_readBufferSize >= 0) free(xh_handle->ch_readBuffer);
  if (xh_handle->nh_file != NULL) free(xh_handle->nh_file);
  free(xh_handle);
  
  m_DIGGY_RETURN(RETURNED)
} // FileButtAdeptDestroyInstance


// 2. G_STRING_BUTT_ADEPT 

struct G_STRING_BUTT_ADEPT {
  m_DECLARE_MAGIC_FIELD(G_STRING_BUTT_ADEPT_HANDLE)
  G_STRING_STUFF gStringStuff;  
} ; 
typedef struct G_STRING_BUTT_ADEPT *G_STRING_BUTT_ADEPT_HANDLE; // Public handle

// Public function: see .h
int GStringButtAdeptCreateInstance(G_STRING_BUTT_ADEPT_HANDLE *azh_handle,
  G_STRING_STUFF f_gStringStuff) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 
  (*azh_handle)->gStringStuff = f_gStringStuff;
  m_ASSIGN_MAGIC_FIELD(G_STRING_BUTT_ADEPT_HANDLE,(*azh_handle))

  m_DIGGY_RETURN(RETURNED)
} // GStringButtAdeptCreateInstance
  

// BUTT_ADEPT_SUCKER_FUNCTION
// Public function: see .h
int GStringButtAdeptSuck(void *r_handle,  char b_kickButt, struct P_STRING *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  G_STRING_BUTT_ADEPT_HANDLE handle = (G_STRING_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(G_STRING_BUTT_ADEPT_HANDLE,handle)

  if (b_kickButt) {
    *ac_chunk = handle->gStringStuff->cv_pString;
  } else {
    m_ASSIGN_EMPTY_P_STRING(*ac_chunk)
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // GStringButtAdeptSuck

// BUTT_ADEPT_FILLER_FUNCTION
// Public function: see .h
int GStringButtAdeptFill(void *r_handle, struct P_STRING chunk, int *nar_flopCause){
  m_DIGGY_BOLLARD()
  G_STRING_BUTT_ADEPT_HANDLE handle = (G_STRING_BUTT_ADEPT_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(G_STRING_BUTT_ADEPT_HANDLE,handle)

  m_TRACK_IF(GStringCopy(handle->gStringStuff, -1, &chunk) < 0) 

  m_DIGGY_RETURN(ANSWER__YES)
} // GStringButtAdeptFill


// Public function: see .h
int GStringButtAdeptDestroyInstance(G_STRING_BUTT_ADEPT_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // GStringButtAdeptDestroyInstance


// 3. SUCKER

struct SUCKER {
  BUTT_ADEPT_SUCK_FUNCTION sButtAdeptSuckFunction;
  void *r_sButtAdeptSuckHandle;
  BUTT_ADEPT_FILL_FUNCTION dButtAdeptFillFunction;
  void *r_dButtAdeptFillHandle;
  int suckingLimit ;
} ;


// BUTT_ADEPT_SUCK_FUNCTION
static int DummyButtAdeptSuck(void *r_handle,  char b_kickButt, struct P_STRING *ac_chunk,
  int *nar_flopCause) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // DummyButtAdeptSuck

// BUTT_ADEPT_FILL_FUNCTION
int DummyButtAdeptFill(void *r_handle, struct P_STRING chunk, int *nar_flopCause) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // DummyButtAdeptFill

// Public function: see .h
int SuckerCreateInstance (SUCKER_HANDLE *azh_handle,  int suckingLimit) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 

  (*azh_handle)->sButtAdeptSuckFunction = DummyButtAdeptSuck;
  (*azh_handle)->dButtAdeptFillFunction = DummyButtAdeptFill;

  (*azh_handle)->suckingLimit = suckingLimit ;

  m_DIGGY_RETURN(RETURNED)
} // SuckerCreateInstance


// Public function ; see .h
int SuckerPlugSDButts(SUCKER_HANDLE handle, 
  BUTT_ADEPT_SUCK_FUNCTION n_sButtAdeptSuckFunction, void *cfr_sButtAdeptSuckHandle,
  BUTT_ADEPT_FILL_FUNCTION n_dButtAdeptFillFunction,  void *cfr_dButtAdeptFillHandle) {
  m_DIGGY_BOLLARD()

  if (n_sButtAdeptSuckFunction != NULL) {
    handle->sButtAdeptSuckFunction = n_sButtAdeptSuckFunction;
    handle->r_sButtAdeptSuckHandle = cfr_sButtAdeptSuckHandle;
  } // if
  if (n_dButtAdeptFillFunction != NULL) {
    handle->dButtAdeptFillFunction = n_dButtAdeptFillFunction;
    handle->r_dButtAdeptFillHandle = cfr_dButtAdeptFillHandle;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // SuckerPlugSDButts


// Public function ; see .h
int SuckerSuckSButt(SUCKER_HANDLE handle, char b_kickButt, struct P_STRING *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int answer = handle->sButtAdeptSuckFunction(handle->r_sButtAdeptSuckHandle,b_kickButt,
    ac_chunk, nar_flopCause);
  m_TRACK_IF(answer < 0) 
  m_DIGGY_RETURN(answer)  
} // SuckerSuckSButt


int SuckerFillDButt(SUCKER_HANDLE handle, struct P_STRING chunk, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int answer = handle->dButtAdeptFillFunction(handle->r_dButtAdeptFillHandle,
    chunk, nar_flopCause);
  m_TRACK_IF(answer < 0) 
  m_DIGGY_RETURN(answer)  
} // SuckerFillDButt


// Public function ; see  .h
int SuckerSuckOut (SUCKER_HANDLE handle, int *na_suckedOutLength, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int n_suckerStatus = -1 ; // unknown status
  int suckedOutLength = 0;

  struct P_STRING c_chunk; // Not significant in case of flop... 
  while (n_suckerStatus == -1) {
    int suckedLength = 0;

    switch (handle->sButtAdeptSuckFunction(handle->r_sButtAdeptSuckHandle, suckedOutLength==0,
       &c_chunk, nar_flopCause)) {
    case ANSWER__YES:
      suckedLength = m_PStringLength(&c_chunk);
      if (b_EMPTY_P_STRING(c_chunk)) {
        n_suckerStatus = SUCKER_STATUS__OK;
      } else if (handle->suckingLimit > 0 && suckedOutLength + suckedLength > handle->suckingLimit) {
        n_suckerStatus = SUCKER_STATUS__LIMIT_BREACHED;
      } else { 
        switch (handle->dButtAdeptFillFunction(handle->r_dButtAdeptFillHandle, c_chunk,
          nar_flopCause)) {
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


// Public function; see .h
int SuckerFCopy (SUCKER_HANDLE handle, const char *p_sFilePathname, const char *p_dFilePathname,
  int *na_copiedLength, int *na_fileFlopCause) {
  m_DIGGY_BOLLARD() 

  FILE_BUTT_ADEPT_HANDLE sFileButtAdeptHandle = (FILE_BUTT_ADEPT_HANDLE)UNDEFINED;
  FILE_BUTT_ADEPT_HANDLE dFileButtAdeptHandle = (FILE_BUTT_ADEPT_HANDLE)UNDEFINED;
  
  m_TRACK_IF(FileButtAdeptCreateInstance(&sFileButtAdeptHandle,p_sFilePathname,
    BATEAU__FILE_BUTT_ADEPT__READ_BUFFER_SIZE) != RETURNED) 
  m_TRACK_IF(FileButtAdeptCreateInstance(&dFileButtAdeptHandle,p_sFilePathname, -1) != RETURNED)

  m_TRACK_IF(SuckerPlugSDButts(handle, FileButtAdeptSuck,sFileButtAdeptHandle,
    FileButtAdeptFill, dFileButtAdeptHandle) != RETURNED)
  int suckerStatus = SuckerSuckOut(handle,na_copiedLength,na_fileFlopCause);
  m_TRACK_IF(suckerStatus < 0)

  m_TRACK_IF(FileButtAdeptDestroyInstance(sFileButtAdeptHandle) != RETURNED)
  m_TRACK_IF(FileButtAdeptDestroyInstance(dFileButtAdeptHandle) != RETURNED)

  m_DIGGY_RETURN(suckerStatus)
} // SuckerFCopy


// Public function ; see description in .h
int SuckerDestroyInstance (SUCKER_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // SuckerDestroyInstance
