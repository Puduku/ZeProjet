// c-ansi/p-string.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

#include "c-ansi/alloc.h"
#include "c-ansi/p-string.h"
#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// Public function: see .h
int VerifyCPString(struct P_STRING *a_pString) { 
  m_DIGGY_BOLLARD()
  const char *ptr = a_pString->string - 1;
  // invariant: portion [a_pString->string:ptr[ is a pure C-string portion 
  while (++ptr < a_pString->stop && *ptr != '\0') {}
  // ptr >= a_pString->stop || *ptr == '\0'
  int completed = COMPLETED__OK; // a priori
  if (ptr < a_pString->stop) {
    a_pString->stop = ptr;
    completed = COMPLETED__BUT ;
  } // if

  m_DIGGY_RETURN(completed)
} // VerifyCPString 


// Public function: see .h
int CopyPString(char *dstRawString, int dstBufferSize,
  const struct P_STRING *ap_srcPString) {
  m_DIGGY_BOLLARD()

  int copyLength = m_PStringLength(ap_srcPString);
  if (copyLength >= dstBufferSize) copyLength = dstBufferSize-1;
  m_ASSERT(copyLength >= 0)
  memcpy(dstRawString,  ap_srcPString->string,  copyLength);
  dstRawString[copyLength] = '\0';

  m_DIGGY_RETURN(copyLength)
} // CopyPString


// Public function : see description in .h
int ComparePStrings(const struct P_STRING *ap_pString1,
  const struct P_STRING *ap_pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION n_toCharFunction) { 
  m_DIGGY_BOLLARD()
  int difference = 0; // No difference a priori...

  int length1 = m_PStringLength(ap_pString1);
  int length2 = m_PStringLength(ap_pString2);

  if (n_isNeutralCharFunction == NULL && n_toCharFunction == NULL) {
    int minLength = length1 < length2 ? length1 : length2 ;
    if ((difference = memcmp(ap_pString1->string,ap_pString2->string,
      minLength)) == 0) {
      difference = length1 - length2 ;
    } // if

  } else {
    const char *ptr1 = ap_pString1->string - 1;
    const char *ptr2 = ap_pString2->string - 1;
    if (n_isNeutralCharFunction == NULL) {
      // Invariant: portions [ap_pString1->string:ptr1[ and 
        // [ap_pString2->string:ptr2[ are identical
      while (++ptr1 < ap_pString1->stop && ++ptr2 < ap_pString2->stop && 
           (difference = n_toCharFunction(*ptr1) - n_toCharFunction(*ptr2)) == 0) {} 
      if (ptr1 == ap_pString1->stop) {
        if (++ptr2 < ap_pString2->stop) difference = -1; 
        else difference = 0 ; 
      } else if (ptr2 == ap_pString2->stop) difference = 1; 
      // else found difference toCharFunction(*ptr1) - toCharFunction(*ptr2) != 0

    } else {
      if (n_toCharFunction == NULL) {
        // Invariant: portions [ap_pString1->string:ptr1[ and 
          // [ap_pString2->string:ptr2[ are identical
        while (++ptr1 < ap_pString1->stop) {
           if (n_isNeutralCharFunction(*ptr1)) continue;
           while (++ptr2 < ap_pString2->stop && n_isNeutralCharFunction(*ptr2)) {} 
           if (ptr2 == ap_pString2->stop) break; 
           if ((difference = *ptr1 - *ptr2) != 0) break;
        } // while
        if (ptr1 == ap_pString1->stop) {
          while (++ptr2 < ap_pString2->stop && n_isNeutralCharFunction(*ptr2)) ;
          if (ptr2 < ap_pString2->stop) difference = -1; 
          else difference = 0 ; 
        } else if (ptr2 == ap_pString2->stop) difference = 1; 
        // else found difference toCharFunction(*ptr1) - toCharFunction(*ptr2) != 0
      } else {
        // Invariant: portions [ap_pString1->string:ptr1[ and 
          // [ap_pString2->string:ptr2[ are identical
        while (++ptr1 < ap_pString1->stop) {
           if (n_isNeutralCharFunction(*ptr1)) continue;
           while (++ptr2 < ap_pString2->stop && n_isNeutralCharFunction(*ptr2)) ;
           if (ptr2 == ap_pString2->stop) break; 
           if ((difference = n_toCharFunction(*ptr1) - n_toCharFunction(*ptr2)) != 0) break;
        } // while
        if (ptr1 == ap_pString1->stop) {
          while (++ptr2 < ap_pString2->stop && n_isNeutralCharFunction(*ptr2)) ;
          if (ptr2 < ap_pString2->stop) difference = -1; 
          else difference = 0 ; 
        } else if (ptr2 == ap_pString2->stop) difference = 1; 
        // else found difference toCharFunction(*ptr1) - toCharFunction(*ptr2) != 0
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(DIFFERENCE_2_COMPARISON(difference))
} // ComparePStrings


// typedef int (*IS_CHAR_FUNCTION) (int c) ;
static int IsNeverChar (int c) {
  return b_FALSE0;
} // IsNeverChar 

// typedef int (*TO_CHAR_FUNCTION) (int c) ;
static int ToSameChar (int c) {
  return c;
} // ToSameChar  

// Public function : see description in .h
int ParanoidComparePStrings(const struct P_STRING *ap_pString1,
  const struct P_STRING *ap_pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION n_toCharFunction) { 
  m_DIGGY_BOLLARD()
  int comparison1 = ComparePStrings(ap_pString1, ap_pString2,
    n_isNeutralCharFunction,  n_toCharFunction);
  m_TRACK_IF(comparison1 < 0) ; 

  int comparison2 = UNDEFINED; 
  if (n_isNeutralCharFunction == NULL) {
    comparison2 = ComparePStrings(ap_pString1, ap_pString2,
      IsNeverChar,  n_toCharFunction);
    m_TRACK_IF(comparison2 < 0) ; 
    m_ASSERT(comparison1 == comparison2) 
  } // if
  if (n_toCharFunction == NULL) {
    comparison2 = ComparePStrings(ap_pString1, ap_pString2,
      n_isNeutralCharFunction,  ToSameChar);
    m_TRACK_IF(comparison2 < 0) ; 
    m_ASSERT(comparison1 == comparison2) 
  } // if
  if (n_isNeutralCharFunction == NULL && n_toCharFunction == NULL) {
    comparison2 = ComparePStrings(ap_pString1, ap_pString2,
      IsNeverChar,  ToSameChar);
    m_TRACK_IF(comparison2 < 0) ; 
    m_ASSERT(comparison1 == comparison2) 
  } // if
//m_DIGGY_VAR_COMPARISON(comparison1)
  m_DIGGY_RETURN(comparison1)
} // ParanoidComparePStrings


// Passed:
// - b_passCharsTill:
// - isCharFunction:
// - u_char:
#define b_PASS_CHAR(b_passCharsTill, /*IS_CHAR_FUNCTION*/ isCharFunction, /*char*/ u_char) \
( b_passCharsTill ? !isCharFunction(u_char) : isCharFunction(u_char) )

// Passed:
// - b_passCharsTill:
// - isCharFunction:
// - u_char:
#define b_PASS_CHAR2(b_passCharsTill, /*const char* */char, /*char*/ u_char) \
( b_passCharsTill ? ((char) != (u_char)) : ((char) == u_char) )


#undef CURRENT_TRACKING_VALUE
#define CURRENT_TRACKING_VALUE NULL 

// Public function : see description in .h
const char *ScanPString(const struct P_STRING *ap_pString,
  char b_regularScan, char b_passCharsTill, IS_CHAR_FUNCTION n_isCharFunction, int c_char) {
  m_DIGGY_BOLLARD()

  const char *ptr = (const char*)UNDEFINED;
  if (b_regularScan) {
    ptr = ap_pString->string - 1;

    if (n_isCharFunction != NULL) {
      // invariant: No match in portion [ap_pString->string:ptr[ 
      while (++ptr < ap_pString->stop && b_PASS_CHAR(b_passCharsTill,n_isCharFunction,*ptr)) {}
    } else {
      // invariant: No match in portion [ap_pString->string:ptr[ 
      while (++ptr < ap_pString->stop && b_PASS_CHAR2(b_passCharsTill,c_char,*ptr)) {}
    } // if

  } else {
    ptr = ap_pString->stop ;

    if (n_isCharFunction != NULL) {
      // invariant: No match in portion [ptr:ap_pString->stop[ 
      while (--ptr >= ap_pString->string && b_PASS_CHAR(b_passCharsTill,n_isCharFunction,*ptr)) {}
    } else {
      // invariant: No match in portion [ptr:ap_pString->stop[ 
      while (--ptr >= ap_pString->string && b_PASS_CHAR2(b_passCharsTill,c_char,*ptr)) {}
    } // if
    if (ptr < ap_pString->string) ptr = ap_pString->stop;
  } // if

  m_DIGGY_RETURN(ptr)
}  // ScanPString

#undef b_PASS_CHAR


// Public function : see description in .h
const char *ScanPStringTillMatch(const struct P_STRING *ap_pString, 
  const struct P_STRING *ap_subPString,  TO_CHAR_FUNCTION n_toCharFunction) {
  m_DIGGY_BOLLARD()

  int subLength = m_PStringLength(ap_subPString);
  const char *ptr = ap_pString->string - 1;

  if (n_toCharFunction == NULL) { 
    // invariant: No match in portion [ap_pString->string:ptr[ 
    while (++ptr < ap_pString->stop &&
      (ptr+subLength <= ap_pString->stop? memcmp(ptr,ap_subPString->string,
      subLength) != 0: b_TRUE)) {} 

  } else {
    // invariant: No match in portion [ap_pString->string:ptr[ 
    while (++ptr < ap_pString->stop) {
      if (ptr+subLength <= ap_pString->stop) {
        m_ASSIGN_LOCAL_P_STRING(ptrPString,  ptr,subLength)
        int comparison = ComparePStrings(&ptrPString,  ap_subPString,  NULL,
          n_toCharFunction); 
        if (comparison == EQUAL_TO__COMPARISON) break ;
      }  // if
    } // while 
  } // if

  m_DIGGY_RETURN(ptr) 
} // ScanPStringTillMatch

// Public function : see description in .h
const char *ParanoidScanPStringTillMatch(const struct P_STRING *ap_pString, 
  const struct P_STRING *ap_subPString,  TO_CHAR_FUNCTION n_toCharFunction) {
  m_DIGGY_BOLLARD() 

  const char *scanPtr1 = ScanPStringTillMatch(ap_pString,ap_subPString,
    n_toCharFunction);
  m_TRACK_IF(scanPtr1 < 0) 

  if (n_toCharFunction == NULL) {
    const char* scanPtr2 = ScanPStringTillMatch(ap_pString,ap_subPString,
      ToSameChar);
    m_TRACK_IF(scanPtr2 < 0) 
    m_ASSERT(scanPtr1 == scanPtr2)
  } // if
    
  m_DIGGY_RETURN(scanPtr1) 
} // ParanoidScanPStringTillMatch

#undef CURRENT_TRACKING_VALUE
#define CURRENT_TRACKING_VALUE DEFAULT_TRACKING_VALUE

// Public function : see description in .h
int ConvertPString(struct P_STRING *aep_pString, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) {
  m_DIGGY_BOLLARD()

  if (b_cTerminated) {
    m_ASSERT(*(aep_pString->stop) == '\0')
  }
  m_ASSIGN_LOCAL_EP_PTR__SAFE_CAST(char*, ptr,  aep_pString->string - 1)

  if (n_isNeutralCharFunction == (IS_CHAR_FUNCTION)NULL) {
    // invariant: raw string portion [aep_pString->string:ptr[ has been converted 
    while (++ptr < aep_pString->stop) *ptr = toCharFunction(*ptr); 
  } else {
    // invariant: raw string portion [aep_pString->string:ptr[ has been converted 
    while (++ptr < aep_pString->stop) {
      if (n_isNeutralCharFunction(*ptr)) memmove(ptr, ptr+1, (--(aep_pString->stop))-ptr); 
      else *ptr = toCharFunction(*ptr);
    } // while
    if (b_cTerminated) {
      m_ASSIGN_LOCAL_EP_PTR__SAFE_CAST(char*, ep_stop, aep_pString->stop)
      *(ep_stop) = '\0'; 
    } // if
  } // if
 
   m_DIGGY_RETURN(aep_pString->stop - aep_pString->string)
} // ConvertPString

// Public function : see .h
int ParanoidConvertPString(struct P_STRING *aep_pString, char b_cTerminated,
  IS_CHAR_FUNCTION n_isNeutralCharFunction,  TO_CHAR_FUNCTION toCharFunction) {
  m_DIGGY_BOLLARD()
  int length1 = ConvertPString(aep_pString, b_cTerminated, n_isNeutralCharFunction,
    toCharFunction);
  m_TRACK_IF(length1 < 0)
 
  if (n_isNeutralCharFunction == NULL) {
    int length2 = ConvertPString(aep_pString, b_cTerminated, IsNeverChar,
      toCharFunction);
    m_TRACK_IF(length2 < 0)
    m_ASSERT(length2 = length1) 
  } // if

  m_DIGGY_RETURN(length1)
} // ParanoidConvertPString

// Public function : see .h
int SScanfPString(struct P_STRING pString, const char *p_format, ...) {
  m_DIGGY_BOLLARD()

  m_HIJACK_P_STRING(pString)

  va_list arguments;
  va_start(arguments,p_format);
  int ret = vsscanf(pString.string,p_format,arguments);
  va_end(arguments);
  m_ASSERT(ret >= 0)
  
  m_RELEASE_HIJACKED_P_STRING(pString)

  m_DIGGY_RETURN(ret) 
} // SScanfPString

// Public function : see .h
int ReadGenericIntegerCString(const char *p_cString,  GENERIC_INTEGER *ac_value,
  int *nac_parsedLength) {
  m_DIGGY_BOLLARD()

  int answer = ANSWER__YES; // a priori
  char *endptr = (char*)UNDEFINED;
  
  m_CT_ASSERT(sizeof(GENERIC_INTEGER)==sizeof(long int))

  errno = 0;
  *ac_value = strtol(p_cString,&endptr,0);
  if (errno != 0 || endptr == p_cString) answer = ANSWER__NO;
  else if (nac_parsedLength != NULL) *nac_parsedLength = (endptr - p_cString); 

  m_DIGGY_RETURN(answer)
} // ReadGenericIntegerCString


// Public function : see .h
int ReadGenericIntegerPString(struct P_STRING pString,  GENERIC_INTEGER *ac_value,
  int *nac_parsedLength) {
  m_DIGGY_BOLLARD()

  m_HIJACK_P_STRING(pString)

  int answer = ANSWER__YES; // a priori

  answer = ReadGenericIntegerCString(pString.string,ac_value,nac_parsedLength) ;
  m_TRACK_IF(answer < 0)

  m_RELEASE_HIJACKED_P_STRING(pString)

  m_DIGGY_RETURN(answer)
} // ReadGenericIntegerPString
