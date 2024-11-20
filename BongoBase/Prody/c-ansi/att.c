// c-ansi/att.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#include <stdlib.h>

#include "c-ansi/stderr.h"
#include "c-ansi/att.h"
#include "c-ansi/types.h"
#include "c-ansi/green.h"
#include "c-ansi/alloc.h"

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"

struct TIMELINE {
  g_ATT_ID_unsigned_int attId ;
  gen_ATT_TIME_long attTime ;
};

typedef struct TIMELINE *TIMELINE_STUFF ;


struct ALARM_TIMETABLE {
  GREEN_COLLECTION_HANDLE h_timelinesHandle ;
  m_DECLARE_MAGIC_FIELD(ALARM_TIMETABLE_HANDLE)
};


#define   ID_INDEX_LABEL 0
#define TIME_INDEX_LABEL 1


// #see GREEN_HANDLER__COMPARE_FUNCTION @ c-ansi/green.h
static int AlarmTimetableCompare (void *nr_privateHandle, char b_frozen, int indexLabel,
  int keyRank, char *pr_aGreenItemStuff, char *npr_bGreenItemStuff, const void *cpr_bKeys) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_INFO("indexLabel=%d",indexLabel)
  ALARM_TIMETABLE_HANDLE handle = (ALARM_TIMETABLE_HANDLE) nr_privateHandle;
  m_CHECK_MAGIC_FIELD(ALARM_TIMETABLE_HANDLE,handle)
  m_ASSERT(keyRank == 0)

  TIMELINE_STUFF p_aTimelineStuff = (TIMELINE_STUFF) pr_aGreenItemStuff;
  TIMELINE_STUFF np_bTimelineStuff = (TIMELINE_STUFF) npr_bGreenItemStuff;

  int comparison = UNDEFINED;
  switch (indexLabel) {
  case ID_INDEX_LABEL:
    { g_ATT_ID_unsigned_int bAttId = (g_ATT_ID_unsigned_int)(GENERIC_INTEGER)cpr_bKeys; // a priori
      if (np_bTimelineStuff != NULL) {
        bAttId = np_bTimelineStuff->attId;
      } // if
      comparison = GET_COMPARISON(p_aTimelineStuff->attId,bAttId);
    } // bAttId
  break; case TIME_INDEX_LABEL:
    { gen_ATT_TIME_long bAttTime = (gen_ATT_TIME_long)(GENERIC_INTEGER)cpr_bKeys; // a priori
      if (np_bTimelineStuff != NULL) {
        bAttTime = np_bTimelineStuff->attTime;
      } // if
      comparison = GET_COMPARISON(p_aTimelineStuff->attTime,bAttTime);
    } // bAttTime
  break; default:
    m_RAISE(ANOMALY__VALUE__D,indexLabel)
  } // switch

  m_DIGGY_RETURN(comparison)
} // AlarmTimetableCompare


// Public function; see description in .h
int AlarmTimetableCreateInstance(ALARM_TIMETABLE_HANDLE *azh_handle) {
  m_DIGGY_BOLLARD() 

  m_MALLOC_INSTANCE(*azh_handle)

  ALARM_TIMETABLE_HANDLE handle = *azh_handle;
  m_ASSIGN_MAGIC_FIELD(ALARM_TIMETABLE_HANDLE,handle)

  m_TRACK_IF(GreenCollectionCreateInstance(&handle->h_timelinesHandle, 50, sizeof(struct TIMELINE),
    NULL, AlarmTimetableCompare, NULL, handle) != RETURNED)

  m_ASSERT(GreenCollectionAddIndex(handle->h_timelinesHandle,1) == ID_INDEX_LABEL)
  m_ASSERT(GreenCollectionAddIndex(handle->h_timelinesHandle,1) == TIME_INDEX_LABEL)
  
  m_DIGGY_RETURN(RETURNED)
} // AlarmTimetableCreateInstance


// Public function: see description in .h
int AlarmTimetableProgram (ALARM_TIMETABLE_HANDLE handle, g_ATT_ID_unsigned_int attId,
  char b_newAlarm, gen_ATT_TIME_long c_newAttTime, gen_ATT_TIME_long *nac_ancientAttTime) {
  m_DIGGY_BOLLARD() 
  // Note: Fetched timeline is not significant in the following "corner case":
  // No "new alarm" AND no "ancient alarm"
  TIMELINE_STUFF tc_timelineStuff = (TIMELINE_STUFF) UNDEFINED;
  int result = UNDEFINED; 

  m_TRACK_IF(GreenCollectionIndexRequest(handle->h_timelinesHandle, NULL, 1, ID_INDEX_LABEL,
    INDEX_SEEK_FLAGS__EQUAL,(void *)(GENERIC_INTEGER)attId) != RETURNED) 

  switch (result = GreenCollectionIndexFetch(handle->h_timelinesHandle, NULL, 
    b_newAlarm ? INDEX_FETCH_FLAGS__FETCH: INDEX_FETCH_FLAGS__REMOVE, 
    (char **)&tc_timelineStuff, NULL)) {
  case RESULT__FOUND:
    if (nac_ancientAttTime != NULL) {
      *nac_ancientAttTime = tc_timelineStuff->attTime;
    } // if
  break; case RESULT__NOT_FOUND: // Not found
  break; default:
    m_TRACK()
  } // switch

  if (b_newAlarm) {
    tc_timelineStuff->attId = attId;
    tc_timelineStuff->attTime = c_newAttTime;
  } // if

  m_DIGGY_RETURN(result)
} // AlarmTimetableProgram


// Public function: see description in .h
int AlarmTimetableGetRefreshTime (ALARM_TIMETABLE_HANDLE handle,
  gen_ATT_TIME_long *ac_refreshAttTime) {
  m_DIGGY_BOLLARD() 
  int result = UNDEFINED;
  TIMELINE_STUFF tc_timelineStuff = (TIMELINE_STUFF) UNDEFINED;

  m_TRACK_IF(GreenCollectionIndexRequest(handle->h_timelinesHandle, NULL, 1, TIME_INDEX_LABEL,
    INDEX_SEEK_FLAGS__ANY, (void *)UNDEFINED) != 0) 

  switch (result = GreenCollectionIndexFetch(handle->h_timelinesHandle, NULL, 
    INDEX_FETCH_FLAGS__READ_ONLY, (char**)&tc_timelineStuff,  NULL)) {
  case RESULT__FOUND:
    *ac_refreshAttTime = tc_timelineStuff->attTime ;
  break; case RESULT__NOT_FOUND:
  break; default:
    m_TRACK()
  } // switch

  m_DIGGY_RETURN(result)
} // AlarmTimetableGetRefreshTime


// Public function; see description in .h
int AlarmTimetableRefresh (ALARM_TIMETABLE_HANDLE handle, gen_ATT_TIME_long refreshAttTime,
  RING_ALARM_FUNCTION ringAlarmFunction, void *r_ringAlarmFunctionHandle) {
  m_DIGGY_BOLLARD() 

  TIMELINE_STUFF tc_timelineStuff = (TIMELINE_STUFF) UNDEFINED;
  int result = UNDEFINED;
  int cpt = 0;
  unsigned long delay = UNDEFINED;

  do {
    m_TRACK_IF(GreenCollectionIndexRequest(handle->h_timelinesHandle, NULL, 1, TIME_INDEX_LABEL,
      INDEX_SEEK_FLAGS__LESS_EQUAL, (void*)refreshAttTime) != RETURNED)
    switch (result = GreenCollectionIndexFetch(handle->h_timelinesHandle, NULL, 
      INDEX_FETCH_FLAGS__REMOVE, (char**)&tc_timelineStuff, NULL)) {
    case RESULT__FOUND:
      m_ASSERT(refreshAttTime >= tc_timelineStuff->attTime)
      delay = (unsigned long) (refreshAttTime - tc_timelineStuff->attTime);
      m_TRACK_IF(ringAlarmFunction(r_ringAlarmFunctionHandle,tc_timelineStuff->attId,delay) !=
        RETURNED)
      cpt++;
    break; case RESULT__NOT_FOUND:
    break; default:
      m_TRACK()
    } // switch
  } while (result != RESULT__NOT_FOUND);

  m_DIGGY_RETURN(cpt)
} // AlarmTimetableRefresh


// Public function; see description in .h 
int AlarmTimetableDestroyInstance (ALARM_TIMETABLE_HANDLE xh_handle) {
  m_DIGGY_BOLLARD() 
  ALARM_TIMETABLE_HANDLE handle = xh_handle;

  m_TRACK_IF(GreenCollectionDestroyInstance(handle->h_timelinesHandle) != RETURNED)

  free(xh_handle) ;

  m_DIGGY_RETURN(RETURNED)
} // AlarmTimetableDestroyInstance
