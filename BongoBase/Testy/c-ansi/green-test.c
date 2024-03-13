#include <stdio.h>

#include "c-ansi/stderr.h"
#include "c-ansi/green.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"
#include "testy/c-ansi-diggy-vars.h"


struct TEST_ITEM {
  int id ;
  const char *n_name; 
} ; 

typedef struct TEST_ITEM *TEST_ITEM_STUFF; 

extern char b_diggyGreenCollectionExam;   

static int testNumber = 0;

#define   ID_INDEX_LABEL 0 
#define NAME_INDEX_LABEL 1 

// #SEE GREEN_HANDLER__COMPARE_FUNCTION @ c-ansi/green.h
static int TestItemHandlerCompare(void *cpr_handle,  char b_frozen,  int indexLabel,
  int keyRank, char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff, void *cpr_bKeys) {
  m_DIGGY_BOLLARD()
  m_ASSERT(keyRank == 0)

  TEST_ITEM_STUFF aTestItemStuff = (TEST_ITEM_STUFF) pr_aGreenItemStuff;
  TEST_ITEM_STUFF n_bTestItemStuff = (TEST_ITEM_STUFF) npr_bGreenItemStuff;
  int comparison = UNDEFINED; 
  switch (indexLabel) {
  case ID_INDEX_LABEL: { 
    int aId = aTestItemStuff->id ;
    int bId = (int)(GENERIC_INTEGER)cpr_bKeys;
    if (n_bTestItemStuff != NULL) bId = n_bTestItemStuff->id;
// m_DIGGY_INFO("aId=%d, bId=%d",aId,bId)
    comparison = GET_COMPARISON(aId,bId);
  } break; case NAME_INDEX_LABEL: {
    const char *n_aName = aTestItemStuff->n_name ;
    const char *n_bName = (const char *)cpr_bKeys;
    if (n_bTestItemStuff != NULL) n_bName = n_bTestItemStuff->n_name;
m_DIGGY_INFO("n_aName=%s, n_bName=%s aTestItemStuff->id=%d",n_aName,n_bName,aTestItemStuff->id)
    comparison = (n_aName==NULL? (n_bName==NULL? EQUAL_TO__COMPARISON: LESS_THAN__COMPARISON) :
      (n_bName==NULL? GREATER_THAN__COMPARISON: DIFFERENCE_2_COMPARISON(strcmp(n_aName,n_bName))));
m_DIGGY_VAR_COMPARISON(comparison)
  } break; default:
    m_RAISE(ANOMALY__VALUE__D,indexLabel)
  } // switch 
  m_DIGGY_RETURN(comparison)
} // TestItemHandlerCompare


// Passed:
// - expectedTestNumber:
// - handle:
// - n_entry: 
// - expectedEntry: 
// - n_expectedId: -1 special value => no fetched item check 
// - c_newId: not significant if no fetched item check 
// - cn_newName: not significant if no fetched item check (NULL => NO update)
static int TestFetch(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,  int n_entry, 
  int expectedEntry,  int n_expectedId, int c_newId, const char *cn_newName) {
  m_DIGGY_BOLLARD()
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)
  if (n_entry >= 0) { 
    m_ASSERT(expectedEntry == n_entry) 
  } else {
    m_ASSERT(n_expectedId != -1) 
  } // if
  TEST_ITEM_STUFF nt_testItemStuff = (TEST_ITEM_STUFF)UNDEFINED;
  int entry = GreenCollectionFetch(handle, n_entry, (char **)&nt_testItemStuff);
  m_TRACK_IF(entry < 0)
  m_ASSERT(entry == expectedEntry) 
  if (n_expectedId >= 0) {
    m_ASSERT(nt_testItemStuff != NULL) 
    m_ASSERT(nt_testItemStuff->id == n_expectedId) 
    nt_testItemStuff->id = c_newId ; 
    if (cn_newName != NULL) nt_testItemStuff->n_name = cn_newName ; 
  } else {
    m_ASSERT(nt_testItemStuff == NULL) 
  } // if

  int completed = UNDEFINED;
  m_TRACK_IF((completed = GreenCollectionVerifyIndexes(handle)) < 0)  
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)
  m_DIGGY_RETURN(RETURNED)
} // TestFetch


// Passed:
// - expectedTestNumber:
// - handle:
static int TestCount(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,  int expectedCount,
  int n_expectedId) {
  m_DIGGY_BOLLARD()
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)
  m_ASSERT(expectedCount != 0 || n_expectedId == -1) 
  TEST_ITEM_STUFF nt_testItemStuff = (TEST_ITEM_STUFF)UNDEFINED;
  int count = GreenCollectionGetCount(handle,(char **)&nt_testItemStuff);
  m_TRACK_IF(count < 0)
  m_ASSERT(count == expectedCount) 
  if (n_expectedId >= 0) {
    m_ASSERT(nt_testItemStuff != NULL) 
    m_ASSERT(nt_testItemStuff->id == n_expectedId) 
  } else {
    m_ASSERT(nt_testItemStuff == NULL) 
  } // if

  m_DIGGY_RETURN(RETURNED)
  //return RETURNED;
} // TestCount


// Passed:
// - expectedTestNumber:
// - handle:
// - indexFetchFlags:
// only significant if INDEX_FETCH_FLAG__RESET is ON => BEGIN
// - c_criteriaNumber: 1 or 2
// - c_indexSeekFlags1: 
// - cc_idKey1: only significant if INDEX_SEEK_FLAG__ANY is OFF
// only significant if criteria number == 2 => BEGIN
// - cc_criteriaOpFlags1 : CRITERIA_OP_FLAG__OR and CRITERIA_OP_FLAG__CLOSE* are not allowed
// - cc_indexSeekFlags2:
// - ccc_nameKey2: only significant if INDEX_SEEK_FLAG__ANY is OFF 
// - cc_criteriaOpFlags2 :
// only significant if criteria number == 2 <= END 
// only significant if INDEX_FETCH_FLAG__RESET is ON <= END 
// - expectedResult:
// - n_expectedId: -1 special value => no fetched item check 
// - c_expectedEntry: only significant if fetched item check  
// - cn_newId: not significant if no fetched item check ; -1 special value => NO update 
// - cn_newName: not significant if no fetched item check (NULL => NO update)
static int TestIndexFetch2(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,
  unsigned int indexFetchFlags, int c_criteriaNumber, unsigned int c_indexSeekFlags1, int cc_idKey1,
  unsigned int cc_criteriaOpFlags1, unsigned int cc_indexSeekFlags2, const char * ccc_nameKey2,
  unsigned int cc_criteriaOpFlags2, int expectedResult, int n_expectedId, int c_expectedEntry,
  int cn_newId, const char *cn_newName) {
  m_DIGGY_BOLLARD()

  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)

  static char b_readOnly = b_TRUE;

  if (n_expectedId == -1) {
    m_ASSERT(indexFetchFlags != INDEX_FETCH_FLAGS__FETCH) 
    m_ASSERT(expectedResult == RESULT__NOT_FOUND) 
  } // if

  TEST_ITEM_STUFF nt_testItemStuff = (TEST_ITEM_STUFF)UNDEFINED;
  int n_entry = UNDEFINED;
m_DIGGY_VAR_INDEX_FETCH_FLAGS(indexFetchFlags)
  if (b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__RESET)) {
m_DIGGY_VAR_INDEX_SEEK_FLAGS(c_indexSeekFlags1)
    b_readOnly = b_FLAG_SET_ON(indexFetchFlags,INDEX_FETCH_FLAG__READ); 
m_DIGGY_VAR_D(cc_idKey1)
    m_TRACK_IF(GreenCollectionIndexRequest(handle, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
      c_criteriaNumber, ID_INDEX_LABEL, c_indexSeekFlags1, (void *)(GENERIC_INTEGER)cc_idKey1,
      cc_criteriaOpFlags1, NAME_INDEX_LABEL,cc_indexSeekFlags2,
      (void *)(GENERIC_INTEGER)ccc_nameKey2, cc_criteriaOpFlags2) != RETURNED)
  } // if
  int result = GreenCollectionIndexFetch(handle, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    indexFetchFlags, (char **)&nt_testItemStuff, &n_entry);
  m_ASSERT(result == expectedResult)
  if (n_expectedId >= 0) {
    m_ASSERT(n_entry != -1) 
m_DIGGY_INFO("n_entry:%d c_expectedEntry:%d",n_entry,c_expectedEntry)
    m_ASSERT(n_entry == c_expectedEntry) 
    m_ASSERT(nt_testItemStuff != NULL) 
    m_ASSERT(nt_testItemStuff->id == n_expectedId) 
    if (cn_newId >= 0) { 
      m_ASSERT(!b_readOnly)
      nt_testItemStuff->id = cn_newId ; 
    } // if
    if (cn_newName != NULL) {
      m_ASSERT(!b_readOnly)
      nt_testItemStuff->n_name = cn_newName ; 
    } // if
  } else {
    m_ASSERT(nt_testItemStuff == NULL) 
    m_ASSERT(n_entry == -1) 
  } // if

  int completed = UNDEFINED;
  m_TRACK_IF((completed = GreenCollectionVerifyIndexes(handle)) < 0)  
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)

  m_DIGGY_RETURN(RETURNED)
} // TestIndexFetch2

// Passed:
// - expectedTestNumber:
// - handle:
// - indexFetchFlags:
// - c_indexSeekFlags: only significant if INDEX_FETCH_FLAG__RESET is ON 
// - cc_idKey: only significant if INDEX_FETCH_FLAG__RESET is ON AND INDEX_SEEK_FLAG__ANY is OFF
// - expectedResult:
// - n_expectedId: -1 special value => no fetched item check 
// - c_expectedEntry: only significant if fetched item check  
// - cn_newId: not significant if no fetched item check ; -1 special value => NO update 
// - cn_newName: not significant if no fetched item check (NULL => NO update)
static int TestIndexFetch(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,
  unsigned int indexFetchFlags, unsigned int c_indexSeekFlags, int cc_idKey,  int expectedResult,
  int n_expectedId, int c_expectedEntry, int cn_newId, const char *cn_newName) {
  m_DIGGY_BOLLARD()
  m_TRACK_IF(TestIndexFetch2(expectedTestNumber,handle,indexFetchFlags,1,c_indexSeekFlags,cc_idKey,
    UNDEFINED,UNDEFINED,(const char *)UNDEFINED,UNDEFINED, expectedResult,n_expectedId,
    c_expectedEntry,cn_newId,cn_newName) != RETURNED)
  m_DIGGY_RETURN(RETURNED)
} // TestIndexFetch

int main (int argc, char **argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)
  int ret = UNDEFINED;

  GREEN_COLLECTION_HANDLE handle = (GREEN_COLLECTION_HANDLE) UNDEFINED;

  b_diggyGreenCollectionExam = b_TRUE;   
  m_TRACK_IF(GreenCollectionCreateInstance(&handle, BATEAU__EXPECTED_ITEMS_NUMBER/*2*/,
    sizeof(struct TEST_ITEM), NULL,TestItemHandlerCompare,NULL,NULL) != RETURNED)


  m_TRACK_IF((ret = GreenCollectionAddIndex(handle,1)) < 0) 
  m_ASSERT(ret == ID_INDEX_LABEL) 
  m_TRACK_IF((ret = GreenCollectionAddIndex(handle,1)) < 0) 
  m_ASSERT(ret == NAME_INDEX_LABEL) 

  m_TRACK_IF(TestFetch(1, handle,-1,0,0,1969,"Julie") < 0)
  m_TRACK_IF(TestCount(2, handle,1,1969) < 0)

  m_TRACK_IF(TestFetch(3, handle,-1,1,0,1968,NULL) < 0)
  m_TRACK_IF(TestCount(4, handle,2,1968) < 0)

  m_TRACK_IF(TestFetch(5, handle,2,2,-1,UNDEFINED,(const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestFetch(6, handle,-1,2,0,1967,NULL) < 0)
  m_TRACK_IF(TestCount(7, handle,3,1967) < 0)

  m_TRACK_IF(TestFetch(8, handle,-1,3,0,1970,NULL) < 0)
  m_TRACK_IF(TestCount(9, handle,4,1970) < 0)

  m_TRACK_IF(TestFetch(10, handle,-1,4,0,1966,NULL) < 0)
  m_TRACK_IF(TestCount(11, handle,5,1966) < 0)

  m_TRACK_IF(TestIndexFetch(12,handle, INDEX_FETCH_FLAGS__FETCH,INDEX_SEEK_FLAGS__EQUAL,1968,
    RESULT__FOUND, 1968, 1, 2068, NULL) < 0)
  m_TRACK_IF(TestCount(13,handle,5,1966) < 0)

  m_TRACK_IF(TestIndexFetch(14,handle, INDEX_FETCH_FLAGS__FETCH,INDEX_SEEK_FLAGS__EQUAL,1968,
    RESULT__NOT_FOUND, 0, 5, 3068, NULL) < 0)
  m_TRACK_IF(TestCount(15, handle,6,3068) < 0)
  
  m_TRACK_IF(TestFetch(16,handle, 1,1,2068,2068,NULL) < 0)
  m_TRACK_IF(TestIndexFetch(17,handle, INDEX_FETCH_FLAGS__REMOVE,INDEX_SEEK_FLAGS__EQUAL,2068,
    RESULT__FOUND, 2068, 1, 2068, NULL) < 0)
  m_TRACK_IF(TestCount(18,handle, 6,3068) < 0)
  m_TRACK_IF(TestFetch(19,handle, 1,1,-1,UNDEFINED,(const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestFetch(20,handle, -1,1,2068,2068,NULL) < 0)
  m_TRACK_IF(TestCount(21,handle, 6,3068) < 0)

  m_TRACK_IF(TestIndexFetch(22,handle, INDEX_FETCH_FLAGS__READ_ONLY,INDEX_SEEK_FLAGS__ANY,UNDEFINED,
    RESULT__FOUND, 1966, 4, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(23,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(24,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(25,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(26,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(27,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 3068, 5, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(28,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(29,handle, INDEX_FETCH_FLAGS__READ_ONLY|INDEX_FETCH_FLAG__DESCENDING,
    INDEX_SEEK_FLAGS__ANY,UNDEFINED, RESULT__FOUND, 3068, 5, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(30,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(31,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(32,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(33,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(34,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(35,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(36,handle, INDEX_FETCH_FLAGS__READ_ONLY,
    INDEX_SEEK_FLAGS__GREATER_EQUAL,1971, RESULT__FOUND, 2068, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(37,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 3068, 5, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(38,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(39,handle, INDEX_FETCH_FLAGS__READ_ONLY|INDEX_FETCH_FLAG__DESCENDING,
    INDEX_SEEK_FLAGS__GREATER_EQUAL,1971, RESULT__FOUND, 3068, 5, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(40,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(41,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(42,handle, INDEX_FETCH_FLAGS__READ_ONLY|INDEX_FETCH_FLAG__DESCENDING,
    INDEX_SEEK_FLAGS__LESS_EQUAL,1969, RESULT__FOUND, 1969, 0, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(43,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(44,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(45,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(46,handle, INDEX_FETCH_FLAGS__READ_ONLY,INDEX_SEEK_FLAGS__LESS_EQUAL,
    1968, RESULT__FOUND, 1966, 4, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(47,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(48,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(GreenCollectionClear(handle) != RETURNED) 

  m_TRACK_IF(TestFetch(49, handle,-1,0,1969,1969,NULL) < 0)
  m_TRACK_IF(TestCount(50, handle,1,1969) < 0)

  m_TRACK_IF(TestFetch(51, handle,-1,1,2068,1968,NULL) < 0)

  m_TRACK_IF(TestFetch(52, handle,-1,2,1967,1969,NULL) < 0)

  m_TRACK_IF(TestFetch(53, handle,-1,3,1970,1970,NULL) < 0)

  m_TRACK_IF(TestFetch(54, handle,-1,4,1966,1966,NULL) < 0)

  m_TRACK_IF(TestFetch(55, handle,-1,5,3068,1971,NULL) < 0)

  m_TRACK_IF(TestIndexFetch(56,handle, INDEX_FETCH_FLAGS__READ_ONLY,
    INDEX_SEEK_FLAGS__GREATER_EQUAL,1968, RESULT__FOUND, 1968, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(57,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(58,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(59,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(60,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1971, 5, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(61,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(62,handle, INDEX_FETCH_FLAGS__READ_ONLY|INDEX_FETCH_FLAG__DESCENDING,
    INDEX_SEEK_FLAGS__LESS_EQUAL, 1970, RESULT__FOUND, 1970, 3, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(63,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 2, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(64,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(65,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1968, 1, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(66,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, -1, NULL) < 0)
  m_TRACK_IF(TestIndexFetch(67,handle, INDEX_FETCH_FLAGS__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED, (const char*)UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch2(68,handle, INDEX_FETCH_FLAGS__READ_ONLY|INDEX_FETCH_FLAG__DESCENDING,
    2, INDEX_SEEK_FLAGS__LESS_EQUAL, 1970, CRITERIA_OP_FLAGS__AND, INDEX_SEEK_FLAGS__EQUAL, "Julie",
    ALL_FLAGS_OFF0, RESULT__FOUND, 1969, 0, -1, NULL) < 0)

  m_TRACK_IF(GreenCollectionDestroyInstance(handle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main


