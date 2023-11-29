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
// - m_stringPortion
// - p_string:
// - negativeOffset: < 0 
// 
// Assigned:
// - m_stringPortion: 
#define m_ASSIGN_CRAZY_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion,\
  /*const char* */p_string, /*int*/negativeOffset) {\
  m_ASSERT((negativeOffset) < 0);\
  (m_stringPortion).stop = ((m_stringPortion).string = (p_string)) + (negativeOffset);\
}

// Assign a "trivial" empty string portion.
// 
// Passed:
// - m_stringPortion
// 
// Assigned:
// - m_stringPortion: 
#define m_ASSIGN_TRIVIAL_STRING_PORTION(/*struct STRING_PORTION*/m_stringPortion) {\
  (m_stringPortion).stop = ((m_stringPortion).string = NULL);\
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
static int TestsVerifyCStringPortion(void) {
  m_DIGGY_BOLLARD()
  // TODO: ag complester...
  struct STRING_PORTION stringPortion; 
  int completed = UNDEFINED; 
  m_ASSIGN_STRING_PORTION(stringPortion,"aaaaa",-1)
  completed = VerifyCStringPortion(&stringPortion);
  m_ASSERT(completed == COMPLETED__OK);

  m_DIGGY_RETURN(RETURNED)
}

// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsCopyStringPortion(void) {
#define m_TEST_COPY_STRING_PORTION(/*const char* */p_string, /*int*/n_length,\
  /*int*/expectedRet) {\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion,p_string,n_length)\
  ret = CopyStringPortion(s32,sizeof(s32),&stringPortion) ;\
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

#define m_TEST_COPY_CRAZY_STRING_PORTION(/*const char* */p_string, /*int*/negativeOffset) {\
  struct STRING_PORTION crazyStringPortion;\
  m_ASSIGN_CRAZY_STRING_PORTION(crazyStringPortion,p_string, negativeOffset)\
  ret = CopyStringPortion(s32,sizeof(s32),&crazyStringPortion) ;\
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == 0) \
  m_ASSERT(s32[0] == '\0') \
}

#define m_TEST_COPY_TRIVIAL_STRING_PORTION() {\
  struct STRING_PORTION trivialStringPortion;\
  m_ASSIGN_TRIVIAL_STRING_PORTION(trivialStringPortion)\
  ret = CopyStringPortion(s32,sizeof(s32),&trivialStringPortion) ;\
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == 0) \
  m_ASSERT(s32[0] == '\0') \
}

  m_DIGGY_BOLLARD()

  m_TEST_COPY_STRING_PORTION(VERS1_9 ,  -1,  9)
  m_TEST_COPY_STRING_PORTION(VERS1_9 ,   8,  8)
  m_TEST_COPY_STRING_PORTION(VERS1_9 ,   9,  9)
  m_TEST_COPY_STRING_PORTION(VERS1_9 ,  10, 10)
  m_TEST_COPY_STRING_PORTION(VERS1_9 ,1000, 31)
  m_TEST_COPY_STRING_PORTION(VERS1_31,  -1, 31)
  m_TEST_COPY_STRING_PORTION(VERS1_32,  -1, 31)
  m_TEST_COPY_STRING_PORTION(VERS1   ,  -1, 31)
  m_TEST_COPY_STRING_PORTION(VERS1   ,  31, 31)
  m_TEST_COPY_STRING_PORTION(VERS1   ,  32, 31)
  m_TEST_COPY_STRING_PORTION(VERS1   ,  33, 31)
  m_TEST_COPY_STRING_PORTION(VERS1   ,1000, 31)
  m_TEST_COPY_STRING_PORTION(""      ,  -1,  0)
  m_TEST_COPY_STRING_PORTION(""      ,   0,  0)
  m_TEST_COPY_STRING_PORTION(""      ,   1,  1)
  m_TEST_COPY_STRING_PORTION(""      ,1000, 31)

  m_TEST_COPY_CRAZY_STRING_PORTION("", -1)
  m_TEST_COPY_CRAZY_STRING_PORTION("", -2)
  m_TEST_COPY_CRAZY_STRING_PORTION("", -155588888)
  m_TEST_COPY_CRAZY_STRING_PORTION(VERS1, -1)
  m_TEST_COPY_CRAZY_STRING_PORTION(VERS1, -2)
  m_TEST_COPY_CRAZY_STRING_PORTION(VERS1, -155588888)

  m_TEST_COPY_TRIVIAL_STRING_PORTION()

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_COPY_STRING_PORTION
#undef m_TEST_COPY_CRAZY_STRING_PORTION
#undef m_TEST_TRIVIAL_STRING_PORTION
} // TestsCopyStringPortion


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsCompareStringPortions(void) {
#define m_TEST_COMPARE_STRING_PORTIONS(/*const char* */p_string1, /*int*/n_length1,\
  /*const char* */p_string2, /*int*/n_length2,  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction,  m_expectedComparison) {\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion1,p_string1,n_length1)\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion2,p_string2,n_length2)\
  ret = ParanoidCompareStringPortions(&stringPortion1,&stringPortion2, n_isNeutralCharFunction,\
    n_toCharFunction);\
  m_TRACK_IF(ret < 0)\
  m_COMPARISON_2_DIFFERENCE(ret,difference)\
  m_ASSERT(difference m_expectedComparison 0) \
} 

#define m_TEST_COMPARE_C_STRING_PORTIONS(/*const char* */p_cString1,\
  /*const char* */ p_cString2,  m_expectedComparison) {\
  m_TEST_COMPARE_STRING_PORTIONS(p_cString1,-1,  p_cString2,-1, NULL, NULL,\
    m_expectedComparison)\
} 

#define m_CHARS_TEST_COMPARE_STRING_PORTIONS(/*const char* */p_cString1,\
  /*const char* */p_cString2,  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction,\
  /*TO_CHAR_FUNCTION*/n_toCharFunction,  m_expectedComparison)\
  m_TEST_COMPARE_STRING_PORTIONS(p_cString1,-1,  p_cString2,-1,  n_isNeutralCharFunction,\
  n_toCharFunction,  m_expectedComparison)

#define m_LENGTHS_TEST_COMPARE_STRING_PORTIONS(/*const char* */p_string1, /*int*/n_length1,\
  /*const char* */p_string2, /*int*/n_length2,  m_expectedComparison)\
  m_TEST_COMPARE_STRING_PORTIONS(p_string1,n_length1, p_string2,n_length2,  NULL,NULL,\
  m_expectedComparison)

  m_DIGGY_BOLLARD()

  m_DIGGY_INFO("m_TEST_COMPARE_C_STRING_PORTIONS()...")
  m_TEST_COMPARE_C_STRING_PORTIONS("A"          ,"B"  ,<)
  m_TEST_COMPARE_C_STRING_PORTIONS("a"          ,"B"  ,>)
  m_TEST_COMPARE_C_STRING_PORTIONS(UPPER_VERS1  ,VERS1,<)
  m_TEST_COMPARE_C_STRING_PORTIONS(VERS1        ,VERS1,==)
  m_TEST_COMPARE_C_STRING_PORTIONS(""           ,""   ,==)
  m_TEST_COMPARE_C_STRING_PORTIONS("A"          ,""   ,>)
  m_TEST_COMPARE_C_STRING_PORTIONS(""           ,"A"  ,<)
  m_TEST_COMPARE_C_STRING_PORTIONS("LLa Nature" ,"La Nature"  , <)

  m_DIGGY_INFO("m_CHARS_TEST_COMPARE_STRING_PORTIONS()...")
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("A"          ,"B"                  ,NULL   ,toupper, <)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("a"          ,"B"                  ,NULL   ,toupper, <)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS(UPPER_VERS1  ,VERS1                ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS(VERS1        ,VERS1                ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("La Nature " ,"La Nature"          ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("La Nature " ,"LaNature"           ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("La Nature " ,"LANATURE"           ,isspace,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("LLa Nature ","\tL a  N a t u r e ",isspace,NULL   , <)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS("XLa Nature ","\tL A  N A T U R E ",isspace,toupper, >)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS(""           ,""                   ,NULL   ,toupper, ==)
  m_CHARS_TEST_COMPARE_STRING_PORTIONS(""           ,""                   ,isspace,toupper, ==)

  m_DIGGY_INFO("m_LENGTHS_TEST_COMPARE_STRING_PORTIONS()...")
  m_LENGTHS_TEST_COMPARE_STRING_PORTIONS(VERS1       ,vers1Len,VERS1,vers1Len-1, >)
  m_LENGTHS_TEST_COMPARE_STRING_PORTIONS(VERS1       ,vers1Len,VERS1,vers1Len+1, <)
  m_LENGTHS_TEST_COMPARE_STRING_PORTIONS("La\0Nature",       9,"La" ,        -1, >)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_COMPARE_STRING_PORTIONS
#undef m_TEST_COMPARE_C_STRING_PORTIONS
#undef m_CHARS_TEST_COMPARE_STRING_PORTIONS
#undef m_LENGTHS_TEST_COMPARE_STRING_PORTIONS
} // TestsCompareStringPortions


#define b_EXPECT_LOCATED b_TRUE
#define b_EXPECT_NOT_LOCATED b_FALSE0


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsScanStringPortion(void) {
#define m_TEST_SCAN_STRING_PORTION(/*const char* */p_string, /*int*/n_length, b_regularScan,\
  b_passCharsTill,  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) {\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion, p_string,n_length)\
  retScan = ScanStringPortion(&stringPortion,b_regularScan,b_passCharsTill,isCharFunction,UNDEFINED);\
  m_ASSERT((expectedOffset) == (retScan - p_string)) \
  m_ASSERT(retScan <= stringPortion.stop) \
  int emb_located = b_SCAN_STRING_PORTION_LOCATED(stringPortion, retScan);\
  if (b_expectLocated) { m_ASSERT(emb_located) }\
  else { m_ASSERT(!emb_located) }\
} 

#define m_TEST_REGULAR_SCAN_STRING_PORTION(/*const char* */p_string, /*int*/n_length,\
  b_passCharsTill,  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_STRING_PORTION(p_string,n_length,b_REGULAR_SCAN, b_passCharsTill, isCharFunction,\
  expectedOffset, b_expectLocated)   

#define m_TEST_SCAN_C_STRING_PORTION(/*const char* */p_cString, /*int*/b_passCharsTill,\
  /*IS_CHAR_FUNCTION*/isCharFunction, /*int*/expectedOffset, b_expectLocated) {\
  m_TEST_REGULAR_SCAN_STRING_PORTION(p_cString, -1, b_passCharsTill, isCharFunction,\
    expectedOffset,b_expectLocated)\
}

// VERS1 = "Tout cela ne vaut pas
  m_DIGGY_BOLLARD()

  m_TEST_SCAN_C_STRING_PORTION(VERS1,b_PASS_CHARS_WHILE,isdigit ,0       ,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION(VERS1,b_PASS_CHARS_TILL ,isdigit ,vers1Len,b_EXPECT_NOT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION(VERS1,b_PASS_CHARS_TILL ,isLowerC,5       ,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION(VERS1,b_PASS_CHARS_WHILE,isupper ,1       ,b_EXPECT_LOCATED)

  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout cela..." , 4, b_PASS_CHARS_TILL , isLowerC,
    4, b_EXPECT_NOT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout cela..." , 5, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_NOT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout cela..." , 6, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout cela..." , 7, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout cela..." , 6, b_PASS_CHARS_WHILE, isLowerC,
    0, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout\0cela...", 7, b_PASS_CHARS_TILL , isLowerC,
    5, b_EXPECT_LOCATED)
  m_TEST_REGULAR_SCAN_STRING_PORTION("Tout\0cela...",-1, b_PASS_CHARS_TILL , isLowerC,
    4, b_EXPECT_NOT_LOCATED)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_SCAN_STRING_PORTION
#undef m_TEST_REGULAR_SCAN_STRING_PORTION
#undef m_TEST_SCAN_C_STRING_PORTION
} // TestsScanStringPortion


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsScanStringPortionTillMatch(void) {
#define m_TEST_SCAN_STRING_PORTION_TILL_MATCH(/*const char* */p_string, /*int*/n_length,\
  /*const char* */p_subString, /*int*/n_subLength,\
  /*TO_CHAR_FUNCTION*/ n_toCharFunction,  /*int*/expectedOffset, b_expectLocated) {\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion,p_string,n_length)\
  m_ASSIGN_LOCAL_STRING_PORTION(subStringPortion,p_subString,n_subLength)\
  retScan = ParanoidScanStringPortionTillMatch(&stringPortion, &subStringPortion, n_toCharFunction);\
  m_ASSERT((expectedOffset) == (retScan - p_string)) \
  m_ASSERT(retScan <= stringPortion.stop) \
  int emb_located = b_SCAN_STRING_PORTION_LOCATED(stringPortion, retScan);\
  if (b_expectLocated) { m_ASSERT(emb_located) }\
  else { m_ASSERT(!emb_located) }\
} 

#define m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH(/*const char* */p_cString,\
  /*const char* */ p_subCString,  /*int*/expectedOffset, b_expectLocated) {\
  m_TEST_SCAN_STRING_PORTION_TILL_MATCH(p_cString,-1,  p_subCString,-1,  NULL,\
    expectedOffset,b_expectLocated)\
} 

#define m_CHARS_TEST_SCAN_STRING_PORTION_TILL_MATCH(/*const char* */p_cString,\
  /*const char* */p_subCString, /*TO_CHAR_FUNCTION*/n_toCharFunction,\
  /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_STRING_PORTION_TILL_MATCH(p_cString,-1,  p_subCString,-1,  n_toCharFunction,\
    expectedOffset,b_expectLocated)

#define m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH(/*const char* */p_string, /*int*/n_length,\
  /*const char* */p_subString, /*int*/n_subLength,  /*int*/expectedOffset, b_expectLocated) \
  m_TEST_SCAN_STRING_PORTION_TILL_MATCH(p_string,n_length,  p_subString,n_subLength,  NULL,\
    expectedOffset, b_expectLocated)

  m_DIGGY_BOLLARD()

  m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"cela",  5,b_EXPECT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"Cela", 15,b_EXPECT_NOT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"ne " , 15,b_EXPECT_NOT_LOCATED)
  m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,""    ,  0,b_EXPECT_LOCATED)

  m_CHARS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"Cela", toupper,\
    5 ,b_EXPECT_LOCATED)
  m_CHARS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"XCela", toupper,\
    15,b_EXPECT_NOT_LOCATED)
  m_CHARS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,"Cila", toupper,\
    15,b_EXPECT_NOT_LOCATED)

  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,12 ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,10 ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,9  ,"cela",4,\
    5,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,8  ,"cela",4,\
    8,b_EXPECT_NOT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." ,12 ,"cela",0,\
    0,b_EXPECT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout cela ne..." , 0 ,"cela",0,\
    0,b_EXPECT_NOT_LOCATED) 
  m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH("Tout\0cela ne...",12 ,"cela",4,\
    5,b_EXPECT_LOCATED) 

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_SCAN_STRING_PORTION_TILL_MATCH
#undef m_CHARS_TEST_SCAN_STRING_PORTION_TILL_MATCH
#undef m_LENGTHS_TEST_SCAN_STRING_PORTION_TILL_MATCH
#undef m_TEST_SCAN_C_STRING_PORTION_TILL_MATCH
} // TestsScanStringPortionTillMatch


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsConvertStringPortion(void) {
#define m_TEST_CONVERT_STRING_PORTION(/*const char* */p_string, /*int*/n_length,\
  /*IS_CHAR_FUNCTION*/n_isNeutralCharFunction, /*TO_CHAR_FUNCTION*/toCharFunction,\
  /*const char* */p_expectedString, /*int*/n_expectedLength) {\
  int em_length = n_length;\
  if (em_length < 0) em_length = strlen(p_string);\
  m_ASSERT(em_length < sizeof(s256));\
  memcpy(s256,p_string,em_length);\
  s256[em_length] = n_length < 0? '\0': DUMMY_CHAR;\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion,s256,n_length)\
  ret = ParanoidConvertStringPortion(&stringPortion,(n_length < 0),n_isNeutralCharFunction,\
    toCharFunction) ; \
  m_TRACK_IF(ret < 0) \
  m_ASSERT(ret == (stringPortion.stop - stringPortion.string)) \
  if (n_expectedLength < 0) {\
    m_ASSERT(strcmp(s256,p_expectedString) == 0)\
  } else {\
    m_ASSERT(ret == n_expectedLength) \
    m_ASSERT(memcmp(s256,p_expectedString,ret) == 0)\
  }\
}

  m_DIGGY_BOLLARD()

  m_TEST_CONVERT_STRING_PORTION(VERS1       ,      -1,NULL   ,toupper, UPPER_VERS1,      -1)
  m_TEST_CONVERT_STRING_PORTION(VERS1       ,vers1Len,NULL   ,toupper, UPPER_VERS1,vers1Len)
  m_TEST_CONVERT_STRING_PORTION(VERS1       ,      10,NULL   ,toupper, UPPER_VERS1,      10)
  m_TEST_CONVERT_STRING_PORTION("La\0Nature",       9,NULL   ,toupper,"LA\0NATURE",       9)
  m_TEST_CONVERT_STRING_PORTION( "La Nature",       9,isspace,toupper,  "LANATURE",       8)

  m_DIGGY_RETURN(RETURNED)

#undef m_TEST_CONVERT_STRING_PORTION
} // TestsConvertStringPortion


// Ret: 
// - RETURNED: Ok
// - -1: anomaly is raised
static int TestsReadGenericIntegerStringPortion(void) {
#define m_TEST_READ_C_LONG_STRING_PORTION(/*const char* */p_string, /*int*/n_length,\
  /*int*/expectedAnswer, /*long*/c_expectedValue, /*int*/c_expectedParsedLength) {\
  m_ASSIGN_LOCAL_STRING_PORTION(stringPortion,p_string,n_length)\
  answer = ReadGenericIntegerStringPortion(stringPortion,&c_value,&c_parsedLength);\
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

  m_TEST_READ_C_LONG_STRING_PORTION("6910"    ,2 , ANSWER__YES,69        ,2)
  m_TEST_READ_C_LONG_STRING_PORTION("70ABC"   ,4 , ANSWER__YES,70        ,2)
  m_TEST_READ_C_LONG_STRING_PORTION("1969X"   ,4 , ANSWER__YES,1969      ,4)
  m_TEST_READ_C_LONG_STRING_PORTION("1969X"   ,5 , ANSWER__YES,1969      ,4)
  m_TEST_READ_C_LONG_STRING_PORTION("0x1969X" ,4 , ANSWER__YES,0x19      ,4)
  m_TEST_READ_C_LONG_STRING_PORTION("  1971"  ,-1, ANSWER__YES,1971      ,6)
  m_TEST_READ_C_LONG_STRING_PORTION("ZZZZZ"   ,3 , ANSWER__NO ,UNDEFINED,UNDEFINED)
  m_TEST_READ_C_LONG_STRING_PORTION(VERS1     ,-1 ,ANSWER__NO ,UNDEFINED,UNDEFINED)
  m_TEST_READ_C_LONG_STRING_PORTION("CA69"    ,-1 ,ANSWER__NO ,UNDEFINED,UNDEFINED)

  m_DIGGY_RETURN(RETURNED)
#undef m_TEST_READ_C_LONG_STRING_PORTION
#undef m_TEST_READ_C_LONG_STRING_PORTION
} // TestsReadGenericIntegerStringPortion


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
  m_TRACK_IF(TestsVerifyCStringPortion() != RETURNED)
  m_TRACK_IF(TestsCopyStringPortion() != RETURNED)
  m_TRACK_IF(TestsCompareStringPortions() != RETURNED)
  m_TRACK_IF(TestsScanStringPortion() != RETURNED)
  m_TRACK_IF(TestsScanStringPortionTillMatch() != RETURNED)
  m_TRACK_IF(TestsConvertStringPortion() != RETURNED)
  m_TRACK_IF(TestsReadGenericIntegerStringPortion() != RETURNED)

  m_DIGGY_RETURN(0)
} // main
