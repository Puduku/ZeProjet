#include "c-ansi/p-string.h"
#include "c-ansi/alloc.h"
#include "diggy/diggy.h"
#include "diggy/vars.h"
#include "testy/getopts.h"
#include <stdio.h>
#include <ctype.h>


// IS_CHAR_FUNCTION
static int isLowerC(int c) {
  return (c == 'c') ;
} // isLowerC

#define VERS1 "Tout cela ne vaut pas le poison qui decoule De tes yeux, de tes yeux verts."
#define VERS1_31 "Tout cela ne vaut pas le poison"
#define VERS1_32 "Tout cela ne vaut pas le poison "
#define VERS1_9 "Tout cela"
#define UPPER_VERS1 "TOUT CELA NE VAUT PAS LE POISON QUI DECOULE DE TES YEUX, DE TES YEUX VERTS."
#define UPPER10_VERS1 "TOUT CELA ne vaut pas le poison qui decoule De tes yeux, de tes yeux verts."

#define DUMMY_CHAR 0x69
#define ANOTHER_DUMMY_CHAR 0x68

// Assign a "crazy" string portion.
// 
// Passed:
// - m_pString
// - p_string:
// - negativeOffset: < 0 
// 
// Assigned:
// - m_pString: 
#define m_ASSIGN_CRAZY_P_STRING(/*struct P_STRING*/m_pString,\
  /*const char* */p_string, /*int*/negativeOffset) {\
  m_ASSERT((negativeOffset) < 0);\
  (m_pString).stop = ((m_pString).string = (p_string)) + (negativeOffset);\
}

// Assign a "trivial" empty string portion.
// 
// Passed:
// - m_pString
// 
// Assigned:
// - m_pString: 
#define m_ASSIGN_TRIVIAL_P_STRING(/*struct P_STRING*/m_pString) {\
  (m_pString).stop = ((m_pString).string = NULL);\
}

int vers1Len = UNDEFINED;
char s256[256] ;
char s32[32] ;
int difference = UNDEFINED;
int ret = UNDEFINED;
const char *retScan = (const char *)UNDEFINED;
int answer = UNDEFINED;
long c_value = (long)UNDEFINED;
int c_parsedLength = UNDEFINED;


// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsVerifyCPString(void) {
  m_DIGGY_BOLLARD()
  // TODO: ag complester...
  struct P_STRING pString; 
  int completed = UNDEFINED; 
  m_ASSIGN_P_STRING(pString,"aaaaa",-1)
  completed = VerifyCPString(&pString);
  m_ASSERT(completed == COMPLETED__OK);

  m_DIGGY_RETURN(RETURNED)
}

// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsCopyPString(void) {
#define m_TEST_COPY_P_STRING(/*const char* */p_string, /*int*/n_length,\
  /*int*/expectedRet) {\
  m_ASSIGN_LOCAL_P_STRING(pString,p_string,n_length)\
  ret = CopyPString(s32,sizeof(s32),&pString) ;\
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == expectedRet) \
  m_ASSERT(ret < sizeof(s32)) \
  int em_expectedRet2 = n_length;\
  if (em_expectedRet2 < 0) \
    em_expectedRet2 = strlen(p_string);\
  if (em_expectedRet2 >= sizeof(s32)) em_expectedRet2 = sizeof(s32) -1;\
  m_ASSERT(ret == em_expectedRet2) \
  m_ASSERT(memcmp(s32,p_string,ret) == 0) \
  m_ASSERT(s32[ret] == '\0') \
}

#define m_TEST_COPY_CRAZY_P_STRING(/*const char* */p_string, /*int*/negativeOffset) {\
  struct P_STRING crazyPString;\
  m_ASSIGN_CRAZY_P_STRING(crazyPString,p_string, negativeOffset)\
  ret = CopyPString(s32,sizeof(s32),&crazyPString) ;\
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == 0) \
  m_ASSERT(s32[0] == '\0') \
}

#define m_TEST_COPY_TRIVIAL_P_STRING() {\
  struct P_STRING trivialPString;\
  m_ASSIGN_TRIVIAL_P_STRING(trivialPString)\
  ret = CopyPString(s32,sizeof(s32),&trivialPString) ;\
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == 0) \
  m_ASSERT(s32[0] == '\0') \
}

  m_DIGGY_BOLLARD()

  m_TEST_COPY_P_STRING(VERS1_9 ,  -1,  9)
  m_TEST_COPY_P_STRING(VERS1_9 ,   8,  8)
  m_TEST_COPY_P_STRING(VERS1_9 ,   9,  9)
  m_TEST_COPY_P_STRING(VERS1_9 ,  10, 10)
  m_TEST_COPY_P_STRING(VERS1_9 ,1000, 31)
  m_TEST_COPY_P_STRING(VERS1_31,  -1, 31)
  m_TEST_COPY_P_STRING(VERS1_32,  -1, 31)
  m_TEST_COPY_P_STRING(VERS1   ,  -1, 31)
  m_TEST_COPY_P_STRING(VERS1   ,  31, 31)
  m_TEST_COPY_P_STRING(VERS1   ,  32, 31)
  m_TEST_COPY_P_STRING(VERS1   ,  33, 31)
  m_TEST_COPY_P_STRING(VERS1   ,1000, 31)
  m_TEST_COPY_P_STRING(""      ,  -1,  0)
  m_TEST_COPY_P_STRING(""      ,   0,  0)
  m_TEST_COPY_P_STRING(""      ,   1,  1)
  m_TEST_COPY_P_STRING(""      ,1000, 31)

  m_TEST_COPY_CRAZY_P_STRING("", -1)
  m_TEST_COPY_CRAZY_P_STRING("", -2)
  m_TEST_COPY_CRAZY_P_STRING("", -155588888)
  m_TEST_COPY_CRAZY_P_STRING(VERS1, -1)
  m_TEST_COPY_CRAZY_P_STRING(VERS1, -2)
  m_TEST_COPY_CRAZY_P_STRING(VERS1, -155588888)

  m_TEST_COPY_TRIVIAL_P_STRING()

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_COPY_P_STRING
#undef m_TEST_COPY_CRAZY_P_STRING
#undef m_TEST_TRIVIAL_P_STRING
} // TestsCopyPString


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsComparePStrings(void) {
#define m_TEST_COMPARE_P_STRINGS(/*const char* */p_string1, /*int*/n_length1,\
  /*const char* */p_string2, /*int*/n_length2,  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction,  m_expectedComparison) {\
  m_ASSIGN_LOCAL_P_STRING(pString1,p_string1,n_length1)\
  m_ASSIGN_LOCAL_P_STRING(pString2,p_string2,n_length2)\
  ret = ParanoidComparePStrings(&pString1,&pString2, n_isNeutralCharFunction,\
    n_toCharFunction,!b_SUB_STRING_2);\
  m_TRACK_IF(ret < 0)\
  m_COMPARISON_2_DIFFERENCE(ret,difference)\
  m_ASSERT(difference m_expectedComparison 0) \
} 

#define m_TEST_COMPARE_C_P_STRINGS(/*const char* */p_cString1,\
  /*const char* */ p_cString2,  m_expectedComparison) {\
  m_TEST_COMPARE_P_STRINGS(p_cString1,-1,  p_cString2,-1, NULL, NULL,\
    m_expectedComparison)\
} 

#define m_CHARS_TEST_COMPARE_P_STRINGS(/*const char* */p_cString1,\
  /*const char* */p_cString2,  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction,  m_expectedComparison)\
  m_TEST_COMPARE_P_STRINGS(p_cString1,-1,  p_cString2,-1,  n_isNeutralCharFunction,\
  n_toCharFunction,  m_expectedComparison)

#define m_LENGTHS_TEST_COMPARE_P_STRINGS(/*const char* */p_string1, /*int*/n_length1,\
  /*const char* */p_string2, /*int*/n_length2,  m_expectedComparison)\
  m_TEST_COMPARE_P_STRINGS(p_string1,n_length1, p_string2,n_length2,  NULL,NULL,\
  m_expectedComparison)

  m_DIGGY_BOLLARD()

  m_DIGGY_INFO("m_TEST_COMPARE_C_P_STRINGS()...")
  m_TEST_COMPARE_C_P_STRINGS("A"          ,"B"  ,<)
  m_TEST_COMPARE_C_P_STRINGS("a"          ,"B"  ,>)
  m_TEST_COMPARE_C_P_STRINGS(UPPER_VERS1  ,VERS1,<)
  m_TEST_COMPARE_C_P_STRINGS(VERS1        ,VERS1,==)
  m_TEST_COMPARE_C_P_STRINGS(""           ,""   ,==)
  m_TEST_COMPARE_C_P_STRINGS("A"          ,""   ,>)
  m_TEST_COMPARE_C_P_STRINGS(""           ,"A"  ,<)
  m_TEST_COMPARE_C_P_STRINGS("LLa Nature" ,"La Nature"  , <)

  m_DIGGY_INFO("m_CHARS_TEST_COMPARE_P_STRINGS()...")
  m_CHARS_TEST_COMPARE_P_STRINGS("A"          ,"B"                  ,NULL   ,toupper, <)
  m_CHARS_TEST_COMPARE_P_STRINGS("a"          ,"B"                  ,NULL   ,toupper, <)
  m_CHARS_TEST_COMPARE_P_STRINGS(UPPER_VERS1  ,VERS1                ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS(VERS1        ,VERS1                ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS("La Nature " ,"La Nature"          ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS("La Nature " ,"LaNature"           ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS("La Nature " ,"LANATURE"           ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS("LLa Nature ","\tL a  N a t u r e ",isspace,NULL   , <)
  m_CHARS_TEST_COMPARE_P_STRINGS("XLa Nature ","\tL A  N A T U R E ",isspace,toupper, >)
  m_CHARS_TEST_COMPARE_P_STRINGS(""           ,""                   ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_P_STRINGS(""           ,""                   ,isspace,toupper, ==)

  m_DIGGY_INFO("m_LENGTHS_TEST_COMPARE_P_STRINGS()...")
  m_LENGTHS_TEST_COMPARE_P_STRINGS(VERS1       ,vers1Len,VERS1,vers1Len-1, >)
  m_LENGTHS_TEST_COMPARE_P_STRINGS(VERS1       ,vers1Len,VERS1,vers1Len+1, <)
  m_LENGTHS_TEST_COMPARE_P_STRINGS("La\0Nature",       9,"La" ,        -1, >)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_COMPARE_P_STRINGS
#undef m_TEST_COMPARE_C_P_STRINGS
#undef m_CHARS_TEST_COMPARE_P_STRINGS
#undef m_LENGTHS_TEST_COMPARE_P_STRINGS
} // TestsComparePStrings


#define b_EXPECT_LOCATED b_TRUE


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsScanPString(void) {
#define m_TEST_SCAN_P_STRING(/*const char* */p_string, /*int*/n_length, b_regularScan,\
  b_passCharsTill,  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) {\
  m_ASSIGN_LOCAL_P_STRING(pString, p_string,n_length)\
  retScan = ScanPString(&pString,b_regularScan,b_passCharsTill,isCharFunction,UNDEFINED);\
  m_ASSERT((expectedOffset) == (retScan - p_string)) \
  m_ASSERT(retScan <= pString.stop) \
  int emb_located = b_SCAN_P_STRING_LOCATED(pString, retScan);\
  if (b_expectLocated) { m_ASSERT(emb_located) }\
  else { m_ASSERT(!emb_located) }\
} 

#define m_TEST_REGULAR_SCAN_P_STRING(/*const char* */p_string, /*int*/n_length,\
  b_passCharsTill,  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_P_STRING(p_string,n_length,b_REGULAR_SCAN, b_passCharsTill, isCharFunction,\
  expectedOffset, b_expectLocated)   

#define m_TEST_SCAN_C_P_STRING(/*const char* */p_cString, /*int*/b_passCharsTill,\
  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) {\
  m_TEST_REGULAR_SCAN_P_STRING(p_cString, -1, b_passCharsTill, isCharFunction,\
    expectedOffset,b_expectLocated)\
}

// VERS1 = "Tout cela ne vaut pas
  m_DIGGY_BOLLARD()

  m_TEST_SCAN_C_P_STRING(VERS1,b_PASS_CHARS_WHILE,isdigit ,0       ,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING(VERS1,b_PASS_CHARS_TILL ,isdigit ,vers1Len,!b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING(VERS1,b_PASS_CHARS_TILL ,isLowerC,5       ,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING(VERS1,b_PASS_CHARS_WHILE,isupper ,1       ,b_EXPECT_LOCATED)

  m_TEST_REGULAR_SCAN_P_STRING("Tout cela..." , 4, b_PASS_CHARS_TILL , isLowerC,
    4, !b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout cela..." , 5, b_PASS_CHARS_TILL , isLowerC,
    5, !b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout cela..." , 6, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout cela..." , 7, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout cela..." , 6, b_PASS_CHARS_WHILE, isLowerC,
    0, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout\0cela...", 7, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_P_STRING("Tout\0cela...",-1, b_PASS_CHARS_TILL , isLowerC,
    4, !b_EXPECT_LOCATED)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_SCAN_P_STRING
#undef m_TEST_REGULAR_SCAN_P_STRING
#undef m_TEST_SCAN_C_P_STRING
} // TestsScanPString


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsScanPStringTillMatch(void) {
#define m_TEST_SCAN_P_STRING_TILL_MATCH(/*const char* */p_string, /*int*/n_length,\
  /*const char* */p_subString, /*int*/n_subLength,\
  /*TO_CHAR_FUNCTION*/ n_toCharFunction,  /*int*/expectedOffset, b_expectLocated) {\
  m_ASSIGN_LOCAL_P_STRING(pString,p_string,n_length)\
  m_ASSIGN_LOCAL_P_STRING(subPString,p_subString,n_subLength)\
  retScan = ScanPStringTillMatch(&pString, &subPString, n_toCharFunction);\
  m_ASSERT((expectedOffset) == (retScan - p_string)) \
  m_ASSERT(retScan <= pString.stop) \
  int emb_located = b_SCAN_P_STRING_LOCATED(pString, retScan);\
  if (b_expectLocated) { m_ASSERT(emb_located) }\
  else { m_ASSERT(!emb_located) }\
} 

#define m_TEST_SCAN_C_P_STRING_TILL_MATCH(/*const char* */p_cString,\
  /*const char* */ p_subCString,  /*int*/expectedOffset, b_expectLocated) {\
  m_TEST_SCAN_P_STRING_TILL_MATCH(p_cString,-1,  p_subCString,-1,  NULL,\
    expectedOffset,b_expectLocated)\
} 

#define m_CHARS_TEST_SCAN_P_STRING_TILL_MATCH(/*const char* */p_cString,\
  /*const char* */p_subCString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_P_STRING_TILL_MATCH(p_cString,-1,  p_subCString,-1,  n_toCharFunction,\
    expectedOffset,b_expectLocated)

#define m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH(/*const char* */p_string, /*int*/n_length,\
  /*const char* */p_subString, /*int*/n_subLength,  /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_P_STRING_TILL_MATCH(p_string,n_length,  p_subString,n_subLength,  NULL,\
    expectedOffset, b_expectLocated)

  m_DIGGY_BOLLARD()

  m_TEST_SCAN_C_P_STRING_TILL_MATCH("Tout cela ne..." ,"cela",  5,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING_TILL_MATCH("Tout cela ne..." ,"Cela", 15,!b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING_TILL_MATCH("Tout cela ne..." ,"ne " , 15,!b_EXPECT_LOCATED)
  m_TEST_SCAN_C_P_STRING_TILL_MATCH("Tout cela ne..." ,""    ,  0,b_EXPECT_LOCATED)

  m_CHARS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,"Cela", toupper,\
    5 ,b_EXPECT_LOCATED)
  m_CHARS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,"XCela", toupper,\
    15,!b_EXPECT_LOCATED)
  m_CHARS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,"Cila", toupper,\
    15,!b_EXPECT_LOCATED)

  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,12 ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,10 ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,9  ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,8  ,"cela",4,\
    8,!b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." ,12 ,"cela",0,\
    0,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout cela ne..." , 0 ,"cela",0,\
    0,!b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH("Tout\0cela ne...",12 ,"cela",4,\
    5,b_EXPECT_LOCATED) 

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_SCAN_P_STRING_TILL_MATCH
#undef m_CHARS_TEST_SCAN_P_STRING_TILL_MATCH
#undef m_LENGTHS_TEST_SCAN_P_STRING_TILL_MATCH
#undef m_TEST_SCAN_C_P_STRING_TILL_MATCH
} // TestsScanPStringTillMatch


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsScanPStringTillFirstMatch(void) {
#define m_TEST_SCAN_P_STRING_TILL_FIRST_MATCH(/*const char* */p_cString,\
  /*int*/expectedOffset, b_expectLocated, /*int*/expectedMatchedEntry,\
  /*const char* */p_subCString0, /*const char* */p_subCString1, /*const char* */p_subCString2) {\
  m_ASSIGN_LOCAL_P_STRING(pString,p_cString,-1)\
  m_ASSIGN_LOCAL_P_STRING(subPString0,p_subCString0,-1)\
  m_ASSIGN_LOCAL_P_STRING(subPString1,p_subCString1,-1)\
  m_ASSIGN_LOCAL_P_STRING(subPString2,p_subCString2,-1)\
  int em_matchedEntry = UNDEFINED;\
  retScan = ScanPStringTillFirstMatch(pString,NULL,&em_matchedEntry,3,subPString0,subPString1,\
    subPString2);\
  m_ASSERT((expectedOffset) == (retScan - p_cString)) \
  m_ASSERT(retScan <= pString.stop) \
  int emb_located = b_SCAN_P_STRING_LOCATED(pString, retScan);\
  if (b_expectLocated) {\
    m_ASSERT(emb_located)\
    m_ASSERT((expectedMatchedEntry) == em_matchedEntry)\
  }\
  else m_ASSERT(!emb_located) \
} 

  m_DIGGY_BOLLARD()

  m_TEST_SCAN_P_STRING_TILL_FIRST_MATCH("Tout cela ne...",5,b_EXPECT_LOCATED,0, "cela","ceci",
   "CELA")
  m_TEST_SCAN_P_STRING_TILL_FIRST_MATCH("Tout cela ne...",5,b_EXPECT_LOCATED,2, "CELA","ceci",
   "cela")
  m_TEST_SCAN_P_STRING_TILL_FIRST_MATCH("Tout cela ne...",15,!b_EXPECT_LOCATED,UNDEFINED, "CELA",
   "ceci", "Cela")
  m_TEST_SCAN_P_STRING_TILL_FIRST_MATCH("Tout cela ne...",0,b_EXPECT_LOCATED,1, "CELA",
   "", "Cela")

  m_DIGGY_RETURN(RETURNED)
} // TestsScanPStringTillFirstMatch

// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsConvertPString(void) {
#define m_TEST_CONVERT_P_STRING(/*const char* */p_string, /*int*/n_length,\
  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction, /*TO_CHAR_FUNCTION*/toCharFunction,\
  /*const char* */p_expectedString, /*int*/n_expectedLength) {\
  int em_length = n_length;\
  if (em_length < 0) em_length = strlen(p_string);\
  m_ASSERT(em_length < sizeof(s256));\
  memcpy(s256,p_string,em_length);\
  s256[em_length] = n_length < 0? '\0': DUMMY_CHAR;\
  m_ASSIGN_LOCAL_P_STRING(pString,s256,n_length)\
  ret = ParanoidConvertPString(&pString,(n_length < 0),n_isNeutralCharFunction,\
    toCharFunction) ; \
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == (pString.stop - pString.string)) \
  if (n_expectedLength < 0) {\
    m_ASSERT(strcmp(s256,p_expectedString) == 0)\
  } else {\
    m_ASSERT(ret == n_expectedLength) \
    m_ASSERT(memcmp(s256,p_expectedString,ret) == 0)\
  }\
}

  m_DIGGY_BOLLARD()

  m_TEST_CONVERT_P_STRING(VERS1       ,      -1,NULL   ,toupper, UPPER_VERS1,      -1)
  m_TEST_CONVERT_P_STRING(VERS1       ,vers1Len,NULL   ,toupper, UPPER_VERS1,vers1Len)
  m_TEST_CONVERT_P_STRING(VERS1       ,      10,NULL   ,toupper, UPPER_VERS1,      10)
  m_TEST_CONVERT_P_STRING("La\0Nature",       9,NULL   ,toupper,"LA\0NATURE",       9)
  m_TEST_CONVERT_P_STRING( "La Nature",       9,isspace,toupper,  "LANATURE",       8)

  m_DIGGY_RETURN(RETURNED)

#undef m_TEST_CONVERT_P_STRING
} // TestsConvertPString


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsReadGenericIntegerPString(void) {
#define m_TEST_READ_C_LONG_P_STRING(/*const char* */p_string, /*int*/n_length,\
  /*int*/expectedAnswer, /*long*/c_expectedValue, /*int*/c_expectedParsedLength) {\
  m_ASSIGN_LOCAL_P_STRING(pString,p_string,n_length)\
  answer = ReadGenericIntegerPString(pString,&c_value,&c_parsedLength);\
  m_ASSERT(answer == expectedAnswer) \
  if (answer == ANSWER__YES) { \
    m_ASSERT(c_value == c_expectedValue) \
    m_ASSERT(c_parsedLength == c_expectedParsedLength) \
  } \
  if (n_length < 0) {\
    answer = ReadGenericIntegerCString(p_string,&c_value,&c_parsedLength);\
    m_ASSERT(answer == expectedAnswer) \
    if (answer == ANSWER__YES) { \
      m_ASSERT(c_value == c_expectedValue) \
      m_ASSERT(c_parsedLength == c_expectedParsedLength) \
    } \
  } \
} 

  m_DIGGY_BOLLARD()

  m_TEST_READ_C_LONG_P_STRING("6910"    ,2 , ANSWER__YES,69        ,2)
  m_TEST_READ_C_LONG_P_STRING("70ABC"   ,4 , ANSWER__YES,70        ,2)
  m_TEST_READ_C_LONG_P_STRING("1969X"   ,4 , ANSWER__YES,1969      ,4)
  m_TEST_READ_C_LONG_P_STRING("1969X"   ,5 , ANSWER__YES,1969      ,4)
  m_TEST_READ_C_LONG_P_STRING("0x1969X" ,4 , ANSWER__YES,0x19      ,4)
  m_TEST_READ_C_LONG_P_STRING("  1971"  ,-1, ANSWER__YES,1971      ,6)
  m_TEST_READ_C_LONG_P_STRING("ZZZZZ"   ,3 , ANSWER__NO ,UNDEFINED,UNDEFINED)
  m_TEST_READ_C_LONG_P_STRING(VERS1     ,-1 ,ANSWER__NO ,UNDEFINED,UNDEFINED)
  m_TEST_READ_C_LONG_P_STRING("CA69"    ,-1 ,ANSWER__NO ,UNDEFINED,UNDEFINED)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_READ_C_LONG_P_STRING
#undef m_TEST_READ_C_LONG_P_STRING
} // TestsReadGenericIntegerPString


int main(int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  // Basic set-up 
  vers1Len = strlen(VERS1);
  memset(s32,DUMMY_CHAR,32);
  memset(s256,ANOTHER_DUMMY_CHAR,256);
  m_ASSERT(strlen(VERS1_9) == 9)
  m_ASSERT(strlen(VERS1_31) == 31)
  m_ASSERT(strlen(VERS1_32) == 32)

  // All tests:
  m_TRACK_IF(TestsVerifyCPString() != RETURNED)
  m_TRACK_IF(TestsCopyPString() != RETURNED)
  m_TRACK_IF(TestsComparePStrings() != RETURNED)
  m_TRACK_IF(TestsScanPString() != RETURNED)
  m_TRACK_IF(TestsScanPStringTillMatch() != RETURNED)
  m_TRACK_IF(TestsScanPStringTillFirstMatch() != RETURNED)
  m_TRACK_IF(TestsConvertPString() != RETURNED)
  m_TRACK_IF(TestsReadGenericIntegerPString() != RETURNED)

  m_DIGGY_RETURN(0)
} // main
