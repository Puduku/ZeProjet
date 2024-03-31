#include <stdio.h>

#include "c-ansi/g-string.h"
#include "c-ansi/g-param.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"


#define VERS1 "Les griffes de l'Amour, les poisons du tripot,"
#define VERS2 "Tout glisse, tout s'edmousse au Granit de sa peau."
#define VERS3 "Elle croit, elle sait cette vierge Infedconde,"
#define VERS3_FORMAT_S_S "Elle %s, elle %s cette vierge Infedconde,"
#define VERS3_S1 "croit"
#define VERS3_S2 "sait"


static int GStringTest (const char *p_vers3) {
  m_DIGGY_BOLLARD()

  G_STRING_STUFF h_gString = (G_STRING_STUFF) UNDEFINED;
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&h_gString) != RETURNED)

  m_ASSERT(m_GStringCCopy(h_gString,0,VERS1) >= 0)
  m_ASSERT(h_gString->nhi_string != NULL)
  m_ASSERT(strcmp(h_gString->nhi_string,VERS1) == 0)
  m_ASSERT(strlen(VERS1) == h_gString->c_copiedLength)

  int ret = GStringPrintf(h_gString,  0,  VERS3_FORMAT_S_S,VERS3_S1,VERS3_S2);
  m_TRACK_IF(ret  < 0)
  m_ASSERT(ret == strlen(VERS3))
  m_ASSERT(strcmp(h_gString->nhi_string,p_vers3) == 0)

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_gString) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GStringTest


static int GParamTest (const char *p_vers3) {
  m_DIGGY_BOLLARD()

  G_STRING_SET_STUFF h_gParam = (G_STRING_SET_STUFF) UNDEFINED;
  m_TRACK_IF(GStringSetCreateInstance(&h_gParam,G_PARAM_CARDINALITY) != RETURNED)

  int ret = m_GStringCCopy(h_gParam + G_PARAM_NAME_ELEMENT, 0, "VERS1"); 
  m_TRACK_IF(ret < 0)
  ret = m_GStringCCopy(h_gParam + G_PARAM_VALUE_ELEMENT, 0, VERS1); 
  m_TRACK_IF(ret < 0)
  m_ASSERT(ret == strlen(VERS1))

  m_TRACK_IF(GStringPrintf(h_gParam + G_PARAM_NAME_ELEMENT,  0,"%s","VERS3") < 0)
  m_TRACK_IF(GStringPrintf(h_gParam + G_PARAM_VALUE_ELEMENT,  0,VERS3_FORMAT_S_S,
    VERS3_S1,VERS3_S2) < 0)
  m_ASSERT(strcmp(h_gParam[0].nhi_string,"VERS3") == 0)
  m_ASSERT(strcmp(h_gParam[1].nhi_string,p_vers3) == 0)

  m_TRACK_IF(GStringSetDestroyInstance(h_gParam, G_PARAM_CARDINALITY) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GParamTest

static int GStringsTest (const char *p_vers3) {
  m_DIGGY_BOLLARD()

#define LOWER_FOOTER "aaaaaaaa"

#define NON_CASE__INDEX_LABEL 0
#define     CASE__INDEX_LABEL 1

  G_STRINGS_HANDLE h_gStrings = (G_STRINGS_HANDLE) UNDEFINED;
  
  m_TRACK_IF(G_STRINGS_CREATE_INSTANCE(&h_gStrings,100) != RETURNED)

  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(h_gStrings, NULL, NULL) == NON_CASE__INDEX_LABEL)

  m_ASSERT(G_STRINGS_ADD_PLAIN_LEXICAL_INDEX(h_gStrings, NULL, toupper) == CASE__INDEX_LABEL)

  G_STRING_STUFF gStringStuff = (G_STRING_STUFF)UNDEFINED;


  m_TRACK_IF(GStringsFetch(h_gStrings,  -1,  &gStringStuff) < 0) 
  m_TRACK_IF(m_GStringCCopy(gStringStuff,0, VERS1) < 0)

  m_TRACK_IF(GStringsFetch(h_gStrings,  -1,  &gStringStuff) < 0) 
  m_TRACK_IF(m_GStringCCopy(gStringStuff,0, VERS2) < 0)	

  m_TRACK_IF(GStringsFetch(h_gStrings,  -1,  &gStringStuff) < 0)
  m_TRACK_IF(GStringPrintf(gStringStuff,  0,  VERS3_FORMAT_S_S,VERS3_S1,VERS3_S2) < 0)

  m_TRACK_IF(GStringsFetch(h_gStrings,  -1,  &gStringStuff) < 0)
  m_TRACK_IF(m_GStringCCopy(gStringStuff,0, LOWER_FOOTER) < 0)

  int completed = GStringsVerifyIndexes(h_gStrings); 
  m_TRACK_IF(completed < 0)
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)

  m_ASSERT(GStringsGetCount(h_gStrings,&gStringStuff) == 4)
  m_ASSERT(strcmp(gStringStuff->nhi_string,LOWER_FOOTER) == 0)

  completed = GStringsVerifyIndexes(h_gStrings); 
  m_TRACK_IF(completed < 0)
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)

  m_ASSERT(GStringsFetch(h_gStrings,  0,  &gStringStuff) == 0)
  m_ASSERT(strcmp(gStringStuff->nhi_string,VERS1) == 0)

  m_ASSERT(GStringsFetch(h_gStrings,  1,  &gStringStuff) == 1)
  m_ASSERT(strcmp(gStringStuff->nhi_string,VERS2) == 0)

  completed = GStringsVerifyIndexes(h_gStrings); 
  m_TRACK_IF(completed < 0)
  m_RAISE_IF(completed != COMPLETED__OK, ANOMALY__CORRUPTED_INDEXES)

  m_TRACK_IF(m_GStringsIndexSingleFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    NON_CASE__INDEX_LABEL, INDEX_SEEK_FLAGS__ANY, (const struct G_KEY*)UNDEFINED,
    INDEX_FETCH_FLAGS__READ_ONLY, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,p_vers3) == 0)

  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,VERS1) == 0)
  
  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,VERS2) == 0)

  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,LOWER_FOOTER) == 0)

  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__NOT_FOUND)

  m_TRACK_IF(m_GStringsIndexSingleFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    CASE__INDEX_LABEL, INDEX_SEEK_FLAGS__ANY, (const struct G_KEY*)UNDEFINED,
    INDEX_FETCH_FLAGS__READ_ONLY, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,LOWER_FOOTER) == 0)

  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,p_vers3) == 0)

  m_TRACK_IF(GStringsIndexFetch(h_gStrings, (INDEX_REQUEST_AUTOMATIC_BUFFER)NULL,
    INDEX_FETCH_FLAGS__READ_NEXT, &gStringStuff, NULL) != RESULT__FOUND)
  m_ASSERT(strcmp(gStringStuff->nhi_string,VERS1) == 0)

  m_TRACK_IF(GStringsDestroyInstance(h_gStrings) != RETURNED)

  m_DIGGY_RETURN(RETURNED)
} // GStringsTest

//extern char b_diggyGreenCollectionExam;

int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;

//b_diggyGreenCollectionExam = b_TRUE;
  m_DIGGY_INIT_BOLLARD(dgFlags)
  char vers3[256] ;
  snprintf(vers3,sizeof(vers3),VERS3_FORMAT_S_S,VERS3_S1,VERS3_S2) ;

  m_TRACK_IF(GStringTest(vers3) != RETURNED)

  m_TRACK_IF(GParamTest(vers3) != RETURNED)

  m_TRACK_IF(GStringsTest(vers3) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
