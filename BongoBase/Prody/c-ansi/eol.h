// c-ansi/eol.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// Purpose: handling EOLs in ASCII strings

#ifndef __C_ANSI_EOL_H_INCLUDED
#define __C_ANSI_EOL_H_INCLUDED

#include <string.h>

#include "c-ansi/stderr.h"
#include "c-ansi/types.h"
#include "c-ansi/green.h"
#include "c-ansi/p-string.h"


// Reminder:
// EOL stands for "End of line" ; delimits logical lines in ASCII strings of characters.   
// - Physically, an EOL is:
//   + a single LF ("Line Feed") ASCII char (UNIX...), or
//   + a CR-LF ("Carriage Return" - "Line Feed") ASCII sequence of chars  (MS-DOS...)
// - Those two shapes are totally equivalent.
// - A standalone CR ASCII char (not followed with LF) is NOT an EOL.


// 1 - Isolate EOL in single ASCII line

// This function is intended to correct line buffers filled in by fgets() - or similar - 
// function. The final end of line (EOL), if existing, is eliminated in the string.
// Note: only ONE eol will be eliminated by the function.  
//
// Passed parameters
// - line : C string line susceptible to contain a trailing LF or CR/LF (the eol)
//
// Modified parameters
// - line : C string corrected
//
// Returned:
// new string length (pure function) 
int RemoveAsciiEol (char *line) ;


// 2 - Parsing EOLs in ASCII lines

// Main pieces :
// - "LINE DELIMITER":  Delimit ONE ascii line within a string buffer
// - "LINE DELIMITERS" (aka green collection of line delimiters)
//   delimit ALL ascii lines of a string buffer
// - "LINES COMPARATOR": that piece is optional ; allows sorting lines of a PARTITION
// - ParseAsciiLines() : the function for parsing multiple lines in a buffer


// "LINE DELIMITER" :
//  ---- ---------

// GREEN ITEM:
struct LINE_DELIMITER {
  struct P_STRING practicalLine; 
  int eolLength ;
} ;
typedef struct LINE_DELIMITER* LINE_DELIMITER_STUFF;



//
// Passed:
// - m_lineDelimiter:
// 
// Changed:
// - m_physicalLength:
#define m_GET_LINE_DELIMITER_PHYSICAL_LENGTH(/*struct LINE_DELIMITER*/m_lineDelimiter,\
  /*int*/m_physicalLength) {\
  m_physicalLength = m_PStringLength((m_lineDelimiter).practicalLine);\
  (m_physicalLength) += (m_lineDelimiter).eolLength;\
}

// "LINE DELIMITERS" :
//  ----- ---------

// Create a LINE DELIMITERS instance.
//
// Passed:
// - azh_handle:
// - expectedItemsNumber: #SEE GreenCollectionCreateInstance-expectedItemsNumber@c-ansi/green.h <line delimiter>
// - cfr_linesComparatorPrivateHandle:
#define /*int*/ LINE_DELIMITERS_CREATE_INSTANCE(/*GREEN_COLLECTION_HANDLE* */azh_handle,\
  /*int*/ expectedItemsNumber,\
  /*GREEN_HANDLER__KEYS_COMPARE_FUNCTION*/n_linesComparatorKeysCompareFunction,\
  /*void* */ cfr_linesComparatorHandle) \
  GreenCollectionCreateInstance(azh_handle,  expectedItemsNumber,  sizeof(struct LINE_DELIMITER),\
    NULL,n_linesComparatorKeysCompareFunction,NULL, cfr_linesComparatorHandle)


// #SEE GreenCollectionGetCount@c-ansi/green.h <line delimiter>
#define /*int*/ LINE_DELIMITERS_GET_COUNT(/*GREEN_COLLECTION_HANDLE*/cp_handle,\
  /*LINE_DELIMITER_STUFF* */navnt_lineDelimiterStuff) \
  GreenCollectionGetCount(cp_handle,(char**)(navnt_lineDelimiterStuff))


// #SEE GreenCollectionFetch@c-ansi/green.h <line delimiter>
#define /*int*/ LINE_DELIMITERS_FETCH(/*GREEN_COLLECTION_HANDLE*/cp_handle, /*int*/n_entry,\
  /*LINE_DELIMITER_STUFF* */acnt_lineDelimiterStuff) \
  GreenCollectionFetch(cp_handle,n_entry,(char**)(acnt_lineDelimiterStuff))


// EOLs parsing function
// The function parses the EOLs that delimit the lines of the input buffer.
//
// Passed:
// - input: input buffer containing the lines to parse 
// - n_trueLinesParsingLimit:
//   + -1 special value: do NOT care about "true" lines (i.e physically terminated with EOL) ;
//     parse all (remaining) input in one "pass" (including the last line even if it is not physically
//     terminated with EOL)
//   + >= 0: only parse "true" lines (i.e physically terminated with EOL)  
//     . 0 : means "indefinite" : that is, parse as many as possible true lines (i.e physically 
//       terminated with EOL) in that pass.
//       If NOT physically terminated with an EOL, the remaining portion is an incomplete line and
//       is discarded (i.e does NOT appear as last line of the line delimiters)
//     . > 0 : max number of physical lines to parse in that "pass".
// - nav_parsedLength: NULL special value if not used
// - lineDelimitersHandle: current partition.
//   + for the first "pass" : pass a "clean" partition (i.e formal count == 0)
//   + for each extra "pass" : pass the partition again (do NOT "clean" between two passes)
//
// Modified:
// - *nav_parsedLength: (if used) TOTAL parsing length (related to physical eol of last parsed line)
// - lineDelimitersHandle: updated with new parsed lines
//
// Ret:
// - >= 0: number of NEW lines parsed in that pass
// - -1 : unexpected problem ; anomaly is raised
int ParseAsciiLines(struct P_STRING input, int n_trueLinesParsingLimit,
  int *nav_parsedLength,  GREEN_COLLECTION_HANDLE lineDelimitersHandle) ;



#endif // __C_ANSI_EOL_H_INCLUDED
