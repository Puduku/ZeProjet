#include <stdio.h>

#include "c-ansi/stderr.h"
#include "c-ansi/green.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


struct TEST_ITEM {
  int id ;
} ; 

typedef struct TEST_ITEM *TEST_ITEM_STUFF; 

extern char b_diggyGreenCollectionExam;   

static int testNumber = 0;

// #SEE GREEN_ITEM_HANDLER__KEYS_COMPARE_FUNCTION @ c-ansi/green.h
static int TestItemHandlerKeysCompare(void *cpr_handle,  char b_frozen,  int indexLabel,
  int keyRank, char *pr_aGreenItemStuff,  char *npr_bGreenItemStuff, void *cpr_bKeys) {
  m_DIGGY_BOLLARD()
  m_ASSERT(keyRank == 0)

  TEST_ITEM_STUFF aTestItemStuff = (TEST_ITEM_STUFF) pr_aGreenItemStuff;
  TEST_ITEM_STUFF n_bTestItemStuff = (TEST_ITEM_STUFF) npr_bGreenItemStuff;
  int aId = aTestItemStuff->id ;
  int bId = (int)(GENERIC_INTEGER)cpr_bKeys;
  if (n_bTestItemStuff != NULL) bId = n_bTestItemStuff->id;

  // m_DIGGY_INFO("aId=%d, bId=%d",aId,bId)
  int comparison = GET_COMPARISON(aId,bId); 

  m_DIGGY_RETURN(comparison)
} // TestItemHandlerKeysCompare


// Passed:
// - handle:
static int TestFetch(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,  int n_entry, 
  int expectedEntry,  int n_expectedId, int c_newId) {
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
  } else {
    m_ASSERT(nt_testItemStuff == NULL) 
  } // if

  int completed = UNDEFINED;
  m_TRACK_IF((completed = GreenCollectionVerifyIndexes(handle)) < 0)  
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)
  m_DIGGY_RETURN(RETURNED)
} // TestFetch


// Passed:
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
// - handle:
static int TestIndexFetch(int expectedTestNumber,  GREEN_COLLECTION_HANDLE handle,
  int indexFetch, int c_indexSeek, int cc_idKey,  int expectedResult,
  int n_expectedId, int c_expectedEntry, int cc_newId) {
  m_DIGGY_BOLLARD()
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)

  if (n_expectedId == -1) {
    m_ASSERT(indexFetch != INDEX_FETCH__FETCH) 
    m_ASSERT(expectedResult != RESULT__FOUND) 
  } // if

  TEST_ITEM_STUFF nt_testItemStuff = (TEST_ITEM_STUFF)UNDEFINED;
  int n_entry = UNDEFINED;
  int result = GreenCollectionIndexFetch(handle,  (INDEX_ITERATOR_AUTOMATIC_BUFFER)NULL,
    INDEX_LABEL0, indexFetch,c_indexSeek,  (char **)&nt_testItemStuff,  &n_entry,
    (void *)(GENERIC_INTEGER)cc_idKey);
  m_ASSERT(result == expectedResult)
  if (n_expectedId >= 0) {
    m_ASSERT(n_entry != -1) 
    m_ASSERT(n_entry == c_expectedEntry) 
    m_ASSERT(nt_testItemStuff != NULL) 
    m_ASSERT(nt_testItemStuff->id == n_expectedId) 
    if (indexFetch != INDEX_FETCH__READ_ONLY && indexFetch != INDEX_FETCH__READ_NEXT) { 
      nt_testItemStuff->id = cc_newId ; 
    } // if
  } else {
    m_ASSERT(nt_testItemStuff == NULL) 
    m_ASSERT(n_entry == -1) 
  } // if

  int completed = UNDEFINED;
  m_TRACK_IF((completed = GreenCollectionVerifyIndexes(handle)) < 0)  
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)

  m_DIGGY_RETURN(RETURNED)
} // TestIndexFetch


int main (int argc, char **argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)
  int ret = UNDEFINED;

  GREEN_COLLECTION_HANDLE handle = (GREEN_COLLECTION_HANDLE) UNDEFINED;

  b_diggyGreenCollectionExam = b_TRUE;   
  m_TRACK_IF(GreenCollectionCreateInstance(&handle,  /*BATEAU__EXPECTED_ITEMS_NUMBER*/2,
    sizeof(struct TEST_ITEM),  NULL,  TestItemHandlerKeysCompare, NULL) != RETURNED)


  m_TRACK_IF((ret = GreenCollectionAddIndex(handle,1)) < 0) 
  m_ASSERT(ret == INDEX_LABEL0) 

  m_TRACK_IF(TestFetch(1, handle,-1,0,0,1969) < 0)
  m_TRACK_IF(TestCount(2, handle,1,1969) < 0)

  m_TRACK_IF(TestFetch(3, handle,-1,1,0,1968) < 0)
  m_TRACK_IF(TestCount(4, handle,2,1968) < 0)

  m_TRACK_IF(TestFetch(5, handle,2,2,-1,UNDEFINED) < 0)

  m_TRACK_IF(TestFetch(6, handle,-1,2,0,1967) < 0)
  m_TRACK_IF(TestCount(7, handle,3,1967) < 0)

  m_TRACK_IF(TestFetch(8, handle,-1,3,0,1970) < 0)
  m_TRACK_IF(TestCount(9, handle,4,1970) < 0)

  m_TRACK_IF(TestFetch(10, handle,-1,4,0,1966) < 0)
  m_TRACK_IF(TestCount(11, handle,5,1966) < 0)

  m_TRACK_IF(TestIndexFetch(12, handle, INDEX_FETCH__FETCH,INDEX_SEEK__KEY,1968, RESULT__FOUND,
    1968, 1, 2068) < 0)
  m_TRACK_IF(TestCount(13, handle,5,1966) < 0)

  m_TRACK_IF(TestIndexFetch(14, handle, INDEX_FETCH__FETCH,INDEX_SEEK__KEY,1968, RESULT__NOT_FOUND,
    0, 5, 3068) < 0)
  m_TRACK_IF(TestCount(15, handle,6,3068) < 0)
  
  m_TRACK_IF(TestFetch(16, handle,1,1,2068,2068) < 0)
  m_TRACK_IF(TestIndexFetch(17, handle, INDEX_FETCH__REMOVE,INDEX_SEEK__KEY,2068, RESULT__FOUND,
    2068, 1, 2068) < 0)
  m_TRACK_IF(TestCount(18, handle,6,3068) < 0)
  m_TRACK_IF(TestFetch(19, handle,1,1,-1,UNDEFINED) < 0)

  m_TRACK_IF(TestFetch(20, handle,-1,1,2068,2068) < 0)
  m_TRACK_IF(TestCount(21, handle,6,3068) < 0)

  m_TRACK_IF(TestIndexFetch(22, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__UP,UNDEFINED,
    RESULT__FOUND, 1966, 4, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(23, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(24, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(25, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(26, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(27, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 3068, 5, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(28, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(29, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__DOWN,UNDEFINED,
    RESULT__FOUND, 3068, 5, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(30, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(31, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(32, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(33, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(34, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(35, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(36, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__BOTTOM_UP,1971,
    RESULT__FOUND, 2068, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(37, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 3068, 5, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(38, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(39, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__BOTTOM_DOWN,1971,
    RESULT__FOUND, 3068, 5, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(40, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 2068, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(41, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(42, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__TOP_DOWN,1969,
    RESULT__FOUND, 1969, 0, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(43, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(44, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(45, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(46, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__TOP_UP,1968,
    RESULT__FOUND, 1966, 4, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(47, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1967, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(48, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(GreenCollectionClear(handle) != RETURNED) 

  m_TRACK_IF(TestFetch(49, handle,-1,0,1969,1969) < 0)
  m_TRACK_IF(TestCount(50, handle,1,1969) < 0)

  m_TRACK_IF(TestFetch(51, handle,-1,1,2068,1968) < 0)

  m_TRACK_IF(TestFetch(52, handle,-1,2,1967,1969) < 0)

  m_TRACK_IF(TestFetch(53, handle,-1,3,1970,1970) < 0)

  m_TRACK_IF(TestFetch(54, handle,-1,4,1966,1966) < 0)

  m_TRACK_IF(TestFetch(55, handle,-1,5,3068,1971) < 0)

  m_TRACK_IF(TestIndexFetch(56, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__BOTTOM_UP,1968,
    RESULT__FOUND, 1968, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(57, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(58, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(59, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1970, 3, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(60, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1971, 5, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(61, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(TestIndexFetch(62, handle, INDEX_FETCH__READ_ONLY,INDEX_SEEK__TOP_DOWN,1970,
    RESULT__FOUND, 1970, 3, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(63, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 2, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(64, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1969, 0, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(65, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1968, 1, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(66, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__FOUND, 1966, 4, UNDEFINED) < 0)
  m_TRACK_IF(TestIndexFetch(67, handle, INDEX_FETCH__READ_NEXT,UNDEFINED,UNDEFINED,
    RESULT__NOT_FOUND, -1, UNDEFINED, UNDEFINED) < 0)

  m_TRACK_IF(GreenCollectionDestroyInstance(handle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main


