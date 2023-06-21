// c-ansi/sucker.c, version 1.93
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

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



// 1. FILE_BUTT_SPOTTER 

struct FILE_BUTT_SPOTTER {
  m_DECLARE_MAGIC_FIELD(FILE_BUTT_SPOTTER_HANDLE)
  int n_readBufferSize; 
  char *ch_readBuffer; // Only significant if n_readBufferSize >= 0  
  FILE *nh_file;
} ; 


// Public function: see .h
int FileButtSpotterCreateInstance(FILE_BUTT_SPOTTER_HANDLE *azh_handle,
  const char *p_filePathname, int n_readBufferSize) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 
  FILE_BUTT_SPOTTER_HANDLE handle = *azh_handle;

  m_ASSIGN_MAGIC_FIELD(FILE_BUTT_SPOTTER_HANDLE,handle)
  const char* openFlags = "w"  ;
  if ((handle->n_readBufferSize = n_readBufferSize) > 0) {
    openFlags = "r"; 
    m_MALLOC(handle->ch_readBuffer,n_readBufferSize)
  } // if 
  handle->nh_file = fopen(p_filePathname,openFlags);

  m_DIGGY_RETURN(RETURNED)
} // FileButtSpotterCreateInstance


// BUTT_SPOTTER_SUCK_FUNCTION
// Public function: see .h
int FileButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  FILE_BUTT_SPOTTER_HANDLE handle = (FILE_BUTT_SPOTTER_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(FILE_BUTT_SPOTTER_HANDLE,handle)

  m_ASSERT(handle->n_readBufferSize > 0)

  if (handle->nh_file == NULL) {
    if (nar_flopCause != NULL) *nar_flopCause = FILE_FLOP_CAUSE__COULD_NOT_OPEN_FILE;
    m_DIGGY_RETURN(ANSWER__NO)
  } // if

  // b_kickButt is ignored

  int ret = fread(handle->ch_readBuffer,handle->n_readBufferSize,1,handle->nh_file);
  m_ASSERT(ret >= 0)
  m_ASSIGN_STRING_PORTION(*ac_chunk,handle->ch_readBuffer,ret) 

  m_DIGGY_RETURN(ANSWER__YES)
} // FileButtSpotterSuck


// BUTT_SPOTTER_FILL_FUNCTION
// Public function: see .h
int FileButtSpotterFill(void *r_handle,  struct STRING_PORTION chunk, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  FILE_BUTT_SPOTTER_HANDLE handle = (FILE_BUTT_SPOTTER_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(FILE_BUTT_SPOTTER_HANDLE,handle)

  m_ASSERT(handle->n_readBufferSize < 0)

  if (handle->nh_file == NULL) {
    if (nar_flopCause != NULL) *nar_flopCause = FILE_FLOP_CAUSE__COULD_NOT_OPEN_FILE;
    m_DIGGY_RETURN(ANSWER__NO)
  } // if

  int ret = fwrite(chunk.string,m_StringPortionLength(&chunk),1,handle->nh_file);
  m_ASSERT(ret >= 0)
  // TODO: gedrer edcritures partielles...
  m_ASSERT(ret == m_StringPortionLength(&chunk)) 

  m_DIGGY_RETURN(ANSWER__YES)
} // FileButtSpotterFill  

// Public function: see .h
int FileButtSpotterDestroyInstance(FILE_BUTT_SPOTTER_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  if (xh_handle->n_readBufferSize >= 0) free(xh_handle->ch_readBuffer);
  if (xh_handle->nh_file != NULL) free(xh_handle->nh_file);
  free(xh_handle);
  
  m_DIGGY_RETURN(RETURNED)
} // FileButtSpotterDestroyInstance


// 2. G_STRING_BUTT_SPOTTER 

struct G_STRING_BUTT_SPOTTER {
  m_DECLARE_MAGIC_FIELD(G_STRING_BUTT_SPOTTER_HANDLE)
  G_STRING_STUFF gStringStuff;  
} ; 
typedef struct G_STRING_BUTT_SPOTTER *G_STRING_BUTT_SPOTTER_HANDLE; // Public handle

// Public function: see .h
int GStringButtSpotterCreateInstance(G_STRING_BUTT_SPOTTER_HANDLE *azh_handle,
  G_STRING_STUFF f_gStringStuff) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 
  (*azh_handle)->gStringStuff = f_gStringStuff;
  m_ASSIGN_MAGIC_FIELD(G_STRING_BUTT_SPOTTER_HANDLE,(*azh_handle))

  m_DIGGY_RETURN(RETURNED)
} // GStringButtSpotterCreateInstance
  

// BUTT_SPOTTER_SUCKER_FUNCTION
// Public function: see .h
int GStringButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  G_STRING_BUTT_SPOTTER_HANDLE handle = (G_STRING_BUTT_SPOTTER_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(G_STRING_BUTT_SPOTTER_HANDLE,handle)

  if (b_kickButt) {
    *ac_chunk = handle->gStringStuff->cv_stringPortion;
  } else {
    m_ASSIGN_EMPTY_STRING_PORTION(*ac_chunk)
  } // if

  m_DIGGY_RETURN(ANSWER__YES)
} // GStringButtSpotterSuck

// BUTT_SPOTTER_FILLER_FUNCTION
// Public function: see .h
int GStringButtSpotterFill(void *r_handle, struct STRING_PORTION chunk, int *nar_flopCause){
  m_DIGGY_BOLLARD()
  G_STRING_BUTT_SPOTTER_HANDLE handle = (G_STRING_BUTT_SPOTTER_HANDLE)r_handle;
  m_CHECK_MAGIC_FIELD(G_STRING_BUTT_SPOTTER_HANDLE,handle)

  m_TRACK_IF(GStringCopy(handle->gStringStuff, -1, &chunk) < 0) 

  m_DIGGY_RETURN(ANSWER__YES)
} // GStringButtSpotterFill


// Public function: see .h
int GStringButtSpotterDestroyInstance(G_STRING_BUTT_SPOTTER_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // GStringButtSpotterDestroyInstance


// 3. SUCKER

struct SUCKER {
  BUTT_SPOTTER_SUCK_FUNCTION sButtSpotterSuckFunction;
  void *r_sButtSpotterSuckHandle;
  BUTT_SPOTTER_FILL_FUNCTION dButtSpotterFillFunction;
  void *r_dButtSpotterFillHandle;
  int suckingLimit ;
} ;


// BUTT_SPOTTER_SUCK_FUNCTION
static int DummyButtSpotterSuck(void *r_handle,  char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *nar_flopCause) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // DummyButtSpotterSuck

// BUTT_SPOTTER_FILL_FUNCTION
int DummyButtSpotterFill(void *r_handle, struct STRING_PORTION chunk, int *nar_flopCause) {
  m_RAISE(ANOMALY__SHOULD_NOT_BE_HERE)
} // DummyButtSpotterFill

// Public function: see .h
int SuckerCreateInstance (SUCKER_HANDLE *azh_handle,  int suckingLimit) {
  m_DIGGY_BOLLARD()

  m_MALLOC_INSTANCE(*azh_handle) 

  (*azh_handle)->sButtSpotterSuckFunction = DummyButtSpotterSuck;
  (*azh_handle)->dButtSpotterFillFunction = DummyButtSpotterFill;

  (*azh_handle)->suckingLimit = suckingLimit ;

  m_DIGGY_RETURN(RETURNED)
} // SuckerCreateInstance


// Public function ; see .h
int SuckerPlugSDButts(SUCKER_HANDLE handle, 
  BUTT_SPOTTER_SUCK_FUNCTION n_sButtSpotterSuckFunction, void *cfr_sButtSpotterSuckHandle,
  BUTT_SPOTTER_FILL_FUNCTION n_dButtSpotterFillFunction,  void *cfr_dButtSpotterFillHandle) {
  m_DIGGY_BOLLARD()

  if (n_sButtSpotterSuckFunction != NULL) {
    handle->sButtSpotterSuckFunction = n_sButtSpotterSuckFunction;
    handle->r_sButtSpotterSuckHandle = cfr_sButtSpotterSuckHandle;
  } // if
  if (n_dButtSpotterFillFunction != NULL) {
    handle->dButtSpotterFillFunction = n_dButtSpotterFillFunction;
    handle->r_dButtSpotterFillHandle = cfr_dButtSpotterFillHandle;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // SuckerPlugSDButts


// Public function ; see .h
int SuckerSuckSButt(SUCKER_HANDLE handle, char b_kickButt, struct STRING_PORTION *ac_chunk,
  int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int answer = handle->sButtSpotterSuckFunction(handle->r_sButtSpotterSuckHandle,b_kickButt,
    ac_chunk, nar_flopCause);
  m_TRACK_IF(answer < 0) 
  m_DIGGY_RETURN(answer)  
} // SuckerSuckSButt


int SuckerFillDButt(SUCKER_HANDLE handle, struct STRING_PORTION chunk, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int answer = handle->dButtSpotterFillFunction(handle->r_dButtSpotterFillHandle,
    chunk, nar_flopCause);
  m_TRACK_IF(answer < 0) 
  m_DIGGY_RETURN(answer)  
} // SuckerFillDButt


// Public function ; see  .h
int SuckerSuckOut (SUCKER_HANDLE handle, int *na_suckedOutLength, int *nar_flopCause) {
  m_DIGGY_BOLLARD()
  int n_suckerStatus = -1 ; // unknown status
  int suckedOutLength = 0;

  struct STRING_PORTION c_chunk; // Not significant in case of flop... 
  while (n_suckerStatus == -1) {
    int suckedLength = 0;

    switch (handle->sButtSpotterSuckFunction(handle->r_sButtSpotterSuckHandle, suckedOutLength==0,
       &c_chunk, nar_flopCause)) {
    case ANSWER__YES:
      suckedLength = m_StringPortionLength(&c_chunk);
      if (b_EMPTY_STRING_PORTION(c_chunk)) {
        n_suckerStatus = SUCKER_STATUS__OK;
      } else if (handle->suckingLimit > 0 && suckedOutLength + suckedLength > handle->suckingLimit) {
        n_suckerStatus = SUCKER_STATUS__LIMIT_BREACHED;
      } else { 
        switch (handle->dButtSpotterFillFunction(handle->r_dButtSpotterFillHandle, c_chunk,
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

  FILE_BUTT_SPOTTER_HANDLE sFileButtSpotterHandle = (FILE_BUTT_SPOTTER_HANDLE)UNDEFINED;
  FILE_BUTT_SPOTTER_HANDLE dFileButtSpotterHandle = (FILE_BUTT_SPOTTER_HANDLE)UNDEFINED;
  
  m_TRACK_IF(FileButtSpotterCreateInstance(&sFileButtSpotterHandle,p_sFilePathname,
    BATEAU__FILE_BUTT_SPOTTER__READ_BUFFER_SIZE) != RETURNED) 
  m_TRACK_IF(FileButtSpotterCreateInstance(&dFileButtSpotterHandle,p_sFilePathname, -1) != RETURNED)

  m_TRACK_IF(SuckerPlugSDButts(handle, FileButtSpotterSuck,sFileButtSpotterHandle,
    FileButtSpotterFill, dFileButtSpotterHandle) != RETURNED)
  int suckerStatus = SuckerSuckOut(handle,na_copiedLength,na_fileFlopCause);
  m_TRACK_IF(suckerStatus < 0)

  m_TRACK_IF(FileButtSpotterDestroyInstance(sFileButtSpotterHandle) != RETURNED)
  m_TRACK_IF(FileButtSpotterDestroyInstance(dFileButtSpotterHandle) != RETURNED)

  m_DIGGY_RETURN(suckerStatus)
} // SuckerFCopy


// Public function ; see description in .h
int SuckerDestroyInstance (SUCKER_HANDLE xh_handle) {
  m_DIGGY_BOLLARD()

  free(xh_handle);

  m_DIGGY_RETURN(RETURNED)
} // SuckerDestroyInstance
