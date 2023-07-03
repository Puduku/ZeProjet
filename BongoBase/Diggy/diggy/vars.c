// diggy/vars.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023


#include "diggy/vars.h"
#include <stdio.h>
#include "diggy/diggy.h"



#define MAX_DATA_LENGTH_DISPLAYED 20

// Public function; see .h
// #SEE DIGGY_PRINT_IMAGE_FUNCTION @ diggy/vars.h
void DiggyPrintDataImage(const char* p_data, int length) {
  putchar('[') ;
  putchar(' ') ;
  if (length > MAX_DATA_LENGTH_DISPLAYED) {
    length = MAX_DATA_LENGTH_DISPLAYED;
  } // if
  int i = 0;
  const unsigned char *ptr = (const unsigned char *) p_data;
  for ( ; i < length ; i++, ptr++) {
    printf("%02X ",*ptr) ;
  } // for
  //putchar(']') ;
  printf("] (%d bytes)",length);
} // DiggyPrintDataImage


#define MAX_STRING_LENGTH_DISPLAYED 60 

// Public function; see .h
// #SEE DIGGY_PRINT_IMAGE_FUNCTION @ diggy/vars.h
void DiggyPrintStringImage(const char *np_string) {
  if (np_string == NULL) {
    fputs("<nil>",stdout);
  } else {
    int length = strlen(np_string);
    int displayedLength = length;
    putchar('"');
    if (displayedLength > MAX_STRING_LENGTH_DISPLAYED) {
      displayedLength = MAX_STRING_LENGTH_DISPLAYED;
    } // if
    const char *ptr = np_string;
    int i = 0;
    for ( ; i < displayedLength ; i++, ptr++) {
      if (*ptr < 32 || *ptr > 126) {
        putchar('.') ;
      } else {
        putchar(*ptr) ;
      } // if
    } // for
    if (displayedLength < length) {
      fputs("(...)",stdout);
    } // if
    printf("\" (length:%d)",length);
  } // if
} // DiggyPrintStringImage


// Public function; see .h
// #SEE DIGGY_PRINT_IMAGE_FUNCTION @ diggy/vars.h
void DiggyPrintValueImage(int value, const char *p_image) {
  fputs(p_image,stdout);
  printf(" (%d)",value) ;
} // DiggyPrintValueImage
