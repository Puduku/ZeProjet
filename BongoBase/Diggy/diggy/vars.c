// diggy/vars.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003


#include "diggy/vars.h"
#include <stdio.h>
#include "diggy/diggy.h"

#if 0
#define IMAGE_SPECIFIER_MAX_LENGTH 5

// Public function; see .h
void DiggyPrintGenImage (void *r_var, const char *p_specifier) {
  char format[1+IMAGE_SPECIFIER_MAX_LENGTH+1];
  format[0] = '%' ;
  int i = 0;
  for ( ; i < IMAGE_SPECIFIER_MAX_LENGTH; i++) {
    if ((format[i+1] = p_specifier[i]) == '\0') {
      break;
    } // if
  } // for
  if (i >= IMAGE_SPECIFIER_MAX_LENGTH) {
    putchar('?') ;
  } else {
    //m_DIGGY_BRICK_CRASHABLE_PRINT(
      printf(format,r_var);
    //) // m_DIGGY_BRICK_CRASHABLE_PRINT
  } // if
} // DiggyPrintGenImage
#endif



#define MAX_DATA_LENGTH_DISPLAYED 20

// Public function; see .h
void DiggyPrintDataImage(const char* p_data, int length) {
  putchar('[') ;
  putchar(' ') ;
  if (length > MAX_DATA_LENGTH_DISPLAYED) {
    length = MAX_DATA_LENGTH_DISPLAYED;
  } // if
  int i = 0;
  const unsigned char *ptr = (const unsigned char *) p_data;
  //m_DIGGY_BRICK_CRASHABLE_PRINT(
    for ( ; i < length ; i++, ptr++) {
      printf("%02X ",*ptr) ;
    } // for
  //) // m_DIGGY_BRICK_CRASHABLE_PRINT
  //putchar(']') ;
  printf("] (%d bytes)",length);
} // DiggyPrintDataImage


#define MAX_STRING_LENGTH_DISPLAYED 60 


// Public function; see .h
void DiggyPrintStringImage(const char *np_string) {
  if (np_string == NULL) {
    fputs("<nil>",stdout);
  } else {
    //m_DIGGY_BRICK_CRASHABLE_PRINT(
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
    //) // m_DIGGY_BRICK_CRASHABLE_PRINT
  } // if
} // DiggyPrintStringImage


// Public function; see .h
void DiggyPrintValueImage(int value, const char *p_image) {
  fputs(p_image,stdout);
  printf(" (%d)",value) ;
} // DiggyPrintValueImage
