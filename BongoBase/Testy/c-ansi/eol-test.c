#include <stdio.h>

#include "c-ansi/stderr.h"
#include "c-ansi/eol.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"

int main (int argc, char **argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  int ret = UNDEFINED;
  int ret2 = UNDEFINED ;
  GREEN_COLLECTION_HANDLE linesPartition ;
  int parsedLength = UNDEFINED;
  const char *p_s = "PIPI\nCACA\nPOUET\n" ;
  const char *p_s2 = "PIM  \n PAM \r\n POUM" ;
  
  
  m_TRACK_IF(LINES_PARTITION_CREATE_INSTANCE(&linesPartition,10, NULL, (void *) UNDEFINED) !=
    RETURNED)

  LINE_DELIMITOR_STUFF lineDelimitor;
  m_TRACK_IF((ret = LINES_PARTITION_GET_COUNT(linesPartition,&lineDelimitor)) < 0)
  m_ASSERT(ret == 0)
  
  struct P_STRING pString ;
  pString = m_PString(p_s,-1); 
  m_TRACK_IF((ret2 = ParseAsciiLines(pString,-1, &parsedLength, linesPartition)) < 0)
  m_ASSERT(ret2 == 3)

  m_TRACK_IF((ret = LINES_PARTITION_GET_COUNT(linesPartition,&lineDelimitor)) < 0)
  m_ASSERT(ret == 3)
  
  int i = 0;
  for (; i < 4; i++) {
    m_ASSERT(LINES_PARTITION_FETCH(linesPartition,i,&lineDelimitor) == i)
  } // for


  m_TRACK_IF(GreenCollectionClear(linesPartition) != RETURNED)
  
  pString = m_PString(p_s2,-1); 
  ret2 = ParseAsciiLines(pString, 0,&parsedLength, linesPartition) ;
  m_TRACK_IF((ret = LINES_PARTITION_GET_COUNT(linesPartition,&lineDelimitor)) < 0)
  m_ASSERT(ret == 2)

  for (i = 0; i < 3; i++) {
    m_TRACK_IF(LINES_PARTITION_FETCH(linesPartition,i,&lineDelimitor) != i)
  } // for

  m_TRACK_IF(GreenCollectionDestroyInstance(linesPartition) != RETURNED)
  
  m_DIGGY_RETURN(0)
} // main


