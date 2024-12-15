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
int CopyPString(char *dstRawString, int dstBufferSize, struct P_STRING srcPString) {
  m_DIGGY_BOLLARD()

  int copyLength = m_PStringLength(srcPString);
  if (copyLength >= dstBufferSize) copyLength = dstBufferSize-1;
  m_ASSERT(copyLength >= 0)
  memcpy(dstRawString,  srcPString.string,  copyLength);
  dstRawString[copyLength] = '\0';

  m_DIGGY_RETURN(copyLength)
} // CopyPString


// Public function : see description in .h
int ComparePStrings(struct P_STRING pString1, struct P_STRING pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2) {
  m_DIGGY_BOLLARD()
m_DIGGY_VAR_P_STRING(pString1)
m_DIGGY_VAR_P_STRING(pString2)
  int difference = 0; // No difference a priori...

  if (n_isNeutralCharFunction == NULL && n_toCharFunction == NULL) {
    int length1 = m_PStringLength(pString1);
    int length2 = m_PStringLength(pString2);

    int minLength = length1 < length2 ? length1 : length2 ;
    if ((difference = memcmp(pString1.string,pString2.string, minLength)) == 0) {
      int lengthsDifference = length1 - length2; 
      if (lengthsDifference < 0) difference = -1;
      else if ((!b_subString2) && lengthsDifference > 0) difference = 1;
    } // if

  } else {
    const char *ptr1 = pString1.string;
    const char *ptr2 = pString2.string;
    // Invariant: portions [pString1:ptr1[ and [pString2:ptr2[ are identical
    while (b_TRUE) {
      if (n_isNeutralCharFunction != NULL) {
        if (ptr1 < pString1.stop && n_isNeutralCharFunction(*ptr1)) { ptr1++; continue; }
        if (ptr2 < pString2.stop && n_isNeutralCharFunction(*ptr2)) { ptr2++; continue; } 
      } // if

      if (ptr1 >= pString1.stop || ptr2 >= pString2.stop) break; 
      if (n_toCharFunction != NULL) {
        if ((difference = n_toCharFunction(*ptr1) - n_toCharFunction(*ptr2)) != 0) break;
      } else {
        if ((difference = *ptr1 - *ptr2) != 0) break;
      } // if
      ptr1++, ptr2++;
    } // while

    m_ASSERT(ptr1 <= pString1.stop)
    m_ASSERT(ptr2 <= pString2.stop)
    if (ptr1 == pString1.stop) {
      m_ASSERT(difference == 0)
      if (ptr2 < pString2.stop) { 
        m_ASSERT(n_isNeutralCharFunction != NULL? !n_isNeutralCharFunction(*ptr2): b_TRUE)
        difference = -1; 
      } // if
    } else {
      if (ptr2 == pString2.stop) { 
        m_ASSERT(difference == 0)
        if (!b_subString2) difference = 1; 
      } else m_ASSERT(difference != 0)
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
int ParanoidComparePStrings(struct P_STRING pString1, struct P_STRING pString2,
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2) {
  m_DIGGY_BOLLARD()
  int comparison1 = ComparePStrings(pString1, pString2, n_isNeutralCharFunction,
    n_toCharFunction, b_subString2);
  m_ASSERT(comparison1 >= 0) ; 

  int comparison2 = UNDEFINED; 
  if (n_isNeutralCharFunction == NULL) {
    comparison2 = ComparePStrings(pString1, pString2, IsNeverChar, n_toCharFunction,
      b_subString2);
    m_ASSERT(comparison1 == comparison2) 
  } // if
  if (n_toCharFunction == NULL) {
    comparison2 = ComparePStrings(pString1, pString2, n_isNeutralCharFunction, ToSameChar,
      b_subString2);
    m_ASSERT(comparison1 == comparison2) 
  } // if
  if (n_isNeutralCharFunction == NULL && n_toCharFunction == NULL) {
    comparison2 = ComparePStrings(pString1, pString2, IsNeverChar, ToSameChar, b_subString2);
    m_ASSERT(comparison1 == comparison2) 
  } // if
//m_DIGGY_VAR_COMPARISON(comparison1)
  m_DIGGY_RETURN(comparison1)
} // ParanoidComparePStrings

// Public function: see .h
int b_EqualToCString(struct P_STRING pString1, const char *p_cString2) {
  int comparison = ComparePStrings(pString1,m_PString(p_cString2),NULL,NULL,!b_SUB_STRING_2);
  switch (comparison) {
  case LESS_THAN__COMPARISON:
  case GREATER_THAN__COMPARISON:
  case EQUAL_TO__COMPARISON:
  break; default: 
    m_RAISE_FATAL(ANOMALY__VALUE__D,comparison) 
  } // switch
  return comparison == EQUAL_TO__COMPARISON; 
} // b_EqualToCString 

// Public function: see .h
int ComparePStringsAmongR(struct P_STRING pString1, 
  IS_CHAR_FUNCTION n_isNeutralCharFunction, TO_CHAR_FUNCTION n_toCharFunction, char b_subString2,
  int *navn_matchedEntry, int *cnavn_matchedId, int string2sCount, const struct P_STRING sp_pString2s[],
  int nsn_ids[]) {
  m_DIGGY_BOLLARD_S()
m_DIGGY_VAR_P_STRING(pString1)
m_DIGGY_VAR_D(string2sCount)
m_DIGGY_VAR_P(nsn_ids)
  int comparison = UNDEFINED; 
  m_ASSERT(string2sCount > 0)
  int i = 0;
  if (navn_matchedEntry != NULL) *navn_matchedEntry = -1; // a priori
  if (nsn_ids != NULL) {
    if (cnavn_matchedId != NULL) *cnavn_matchedId = -1; // a priori
    for (; i < string2sCount ; i++) { 
m_DIGGY_VAR_D(i)
      if (nsn_ids[i] < 0) continue;
      if ((comparison = ComparePStrings(pString1, sp_pString2s[i], n_isNeutralCharFunction,
        n_toCharFunction,b_subString2)) == EQUAL_TO__COMPARISON) break;
    } // for 
  } else {
    for (; i < string2sCount ; i++) { 
      if ((comparison = ComparePStrings(pString1, sp_pString2s[i], n_isNeutralCharFunction,
        n_toCharFunction,b_subString2)) == EQUAL_TO__COMPARISON) break;
    } // for 
  } // if
  if (i < string2sCount) {
    m_ASSERT(comparison == EQUAL_TO__COMPARISON)
    if (navn_matchedEntry != NULL) *navn_matchedEntry = i;
    if (nsn_ids != NULL && cnavn_matchedId != NULL) *cnavn_matchedId = nsn_ids[i];
  } // if 

  m_DIGGY_RETURN(comparison)
} // ComparePStringsAmongR


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
const char *ScanPString(struct P_STRING pString, char b_regularScan, char b_passCharsTill,
  IS_CHAR_FUNCTION n_isCharFunction, int c_char) {
  m_DIGGY_BOLLARD()

  const char *ptr = (const char*)UNDEFINED;
  if (b_regularScan) {
    ptr = pString.string - 1;

    if (n_isCharFunction != NULL) {
      // invariant: No match in portion [pString.string:ptr[ 
      while (++ptr < pString.stop && b_PASS_CHAR(b_passCharsTill,n_isCharFunction,*ptr)) {}
    } else {
      // invariant: No match in portion [pString.string:ptr[ 
      while (++ptr < pString.stop && b_PASS_CHAR2(b_passCharsTill,c_char,*ptr)) {}
    } // if

  } else {
    ptr = pString.stop ;

    if (n_isCharFunction != NULL) {
      // invariant: No match in portion [ptr:pString.stop[ 
      while (--ptr >= pString.string && b_PASS_CHAR(b_passCharsTill,n_isCharFunction,*ptr)) {}
    } else {
      // invariant: No match in portion [ptr:pString.stop[ 
      while (--ptr >= pString.string && b_PASS_CHAR2(b_passCharsTill,c_char,*ptr)) {}
    } // if
    if (ptr < pString.string) ptr = pString.stop;
  } // if

  m_DIGGY_RETURN(ptr)
}  // ScanPString

#undef b_PASS_CHAR


// Public function : see description in .h
const char *ScanPStringTillMatch(struct P_STRING pString, struct P_STRING subPString,
  TO_CHAR_FUNCTION n_toCharFunction) {
  m_DIGGY_BOLLARD()

  int subLength = m_PStringLength(subPString);
  const char *ptr = pString.string - 1;

  // invariant: No match in portion [pString.string:ptr[ 
  while (++ptr < pString.stop) {
    if (ptr+subLength <= pString.stop) {
      //TODO: utiliser les sub-strings ?
      struct P_STRING ptrPString = m_PString2(ptr,subLength);
      int comparison = ComparePStrings(ptrPString,  subPString,  NULL,
        n_toCharFunction, !b_SUB_STRING_2); 
      if (comparison == EQUAL_TO__COMPARISON) break ;
    }  // if
  } // while 

  m_DIGGY_RETURN(ptr) 
} // ScanPStringTillMatch


// Public function : see description in .h
const char *ScanPStringTillFirstMatchR(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *cnavn_matchedId, int subStringsCount,
  const struct P_STRING sp_subPStrings[], int nsn_ids[]) {
  m_DIGGY_BOLLARD()

  if (navn_matchedEntry != NULL) *navn_matchedEntry = -1; // a priori
  if (nsn_ids != NULL && cnavn_matchedId != NULL) *cnavn_matchedId = -1; // a priori

  while (!b_EMPTY_P_STRING(pString)) {
    int comparison = ComparePStringsAmongR(pString,(IS_CHAR_FUNCTION)NULL,n_toCharFunction,
      b_SUB_STRING_2,navn_matchedEntry,cnavn_matchedId, subStringsCount,sp_subPStrings, nsn_ids);
    if (comparison == EQUAL_TO__COMPARISON) break ;
    pString.string++; 
  } // while 

  m_DIGGY_RETURN(pString.string) 
} // ScanPStringTillFirstMatchR

// Public function : see description in .h
const char *ScanPStringTillFirstMatchV(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *navn_matchedId, int subStringsCount, va_list subPStringsIds) {
  m_DIGGY_BOLLARD()
  m_ASSERT(subStringsCount > 0)
  struct P_STRING s_subPStrings[subStringsCount];
  struct P_STRING *subPStringPtr = s_subPStrings;
  int sn_ids[subStringsCount];
  int *n_idPtr = sn_ids; 
  int i = 0; while (i++ < subStringsCount) *(subPStringPtr++) = va_arg(subPStringsIds,
    struct P_STRING), *(n_idPtr++) = va_arg(subPStringsIds,int) ;
  m_DIGGY_RETURN(ScanPStringTillFirstMatchR(pString,n_toCharFunction,navn_matchedEntry,
    navn_matchedId,subStringsCount,s_subPStrings,sn_ids))
} // ScanPStringTillFirstMatchV

// Public function : see description in .h
const char *ScanPStringTillFirstMatch(struct P_STRING pString, TO_CHAR_FUNCTION n_toCharFunction,
  int *navn_matchedEntry, int *navn_matchedId, int subStringsCount, /*struct P_STRING subPString0,
  int n_id0, */ ...) {
  m_DIGGY_BOLLARD()
  va_list subPStringsIds;
  va_start(subPStringsIds,subStringsCount);

  const char *scanPtr = ScanPStringTillFirstMatchV(pString,n_toCharFunction,navn_matchedEntry,
    navn_matchedId, subStringsCount,subPStringsIds);
   
  va_end(subPStringsIds);

  m_DIGGY_RETURN(scanPtr)
} // ScanPStringTillFirstMatch

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
