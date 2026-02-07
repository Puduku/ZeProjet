// c-ansi/eol.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "c-ansi/eol.h"
#include "c-ansi/diggy.h"
#include "c-ansi/types.h"


// Public function : see description in .h
int RemoveAsciiEol (char *line) {
  m_DIGGY_BOLLARD()
  int length ;

  length =  strlen(line) ;

  if (length >= 2 && line[length-2] == '\r' && line[length-1] == '\n') {
    line[length-=2] = '\0' ;
  } else if (length >= 1 && line[length-1] == '\n') {
    line[length-=1] = '\0' ;
  } // if 
 
  m_DIGGY_RETURN(length)
} // RemoveAsciiEol


// Public function : see description in .h
int ParseAsciiLines (struct P_STRING input, int n_trueLinesParsingLimit,
  int *nav_parsedLength,  GREEN_COLLECTION_HANDLE lineDelimitersHandle) {
  m_DIGGY_BOLLARD()
  const char *ptr =input.string;
  const char *stopPtr =input.stop ;
  int parsedLinesCount = 0;
  LINE_DELIMITER_STUFF nt_lineDelimiterStuff = (LINE_DELIMITER_STUFF) UNDEFINED;

  m_TRACK_IF(LINE_DELIMITERS_GET_COUNT(lineDelimitersHandle, &nt_lineDelimiterStuff) < 0)

  if (nt_lineDelimiterStuff != NULL) {
    int physicalLengh = UNDEFINED;
    m_GET_LINE_DELIMITER_PHYSICAL_LENGTH(*nt_lineDelimiterStuff, physicalLengh)
    ptr = nt_lineDelimiterStuff->practicalLine.string + physicalLengh ;
    m_ASSERT(ptr >= input.string && ptr <= input.stop)
  } // if

  while (ptr < stopPtr && (n_trueLinesParsingLimit <= 0 || parsedLinesCount <
    n_trueLinesParsingLimit)) {
    const char *linePtr = ptr ; 
    int eolLength = 0 ;

    while (ptr < stopPtr) {
      if (*ptr == '\n') {
        eolLength = 1 ;
        break ;
      } else if (*ptr == '\r') {
        if (ptr + 1 < stopPtr && *(ptr + 1) == '\n') {
          eolLength = 2 ;
          break ;
        } // if
      } // if
      ptr++;
    } // while
    if (ptr < stopPtr || n_trueLinesParsingLimit == -1) {
      LINE_DELIMITERS_FETCH(lineDelimitersHandle,-1,&nt_lineDelimiterStuff) ;
      nt_lineDelimiterStuff->practicalLine.string = linePtr ;
      nt_lineDelimiterStuff->practicalLine.stop = ptr ;
      nt_lineDelimiterStuff->eolLength = eolLength ;
      parsedLinesCount++ ;
    } // if
    ptr += eolLength ; 
  } // while

  if (nav_parsedLength != NULL) {
    *nav_parsedLength = (ptr -input.string) ;
  } // if

  m_DIGGY_RETURN(parsedLinesCount)
} // ParseAsciiLines 
