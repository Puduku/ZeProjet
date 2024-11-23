// c-ansi/blotcode-test.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004
// (c) Puduku - 2023

#include <stdio.h>

#include "c-ansi/blotcode.h"
#include "c-posix/sucker.h"

#include "c-ansi/alloc.h"
#include "c-ansi/p-string.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"

#include "c-ansi/testy-diggy.h"

static int testNumber = 0;

#define DUMMY_BLOTLIB_EXECUTOR_FACTORY_HANDLE (void *)0xCACABEBE
#define DUMMY_BLOTLIB_EXECUTOR_HANDLE (void *)0xFADABEDE

const char* s_localBlotfuncsName1[] = { "func1" , "InitUnDeuxTrois" , "UnDeuxTrois" , "func4" } ;

#define DUMMY_BLOTLIB1_EXECUTOR_FACTORY_HANDLE DUMMY_BLOTLIB_EXECUTOR_FACTORY_HANDLE

struct BLOTLIB1_EXECUTOR {
  gen_BLOTVAL nbIters ;
} ;
typedef struct BLOTLIB1_EXECUTOR *BLOTLIB1_EXECUTOR_HANDLE;

// BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION
static int Blotlib1ExecutorFactoryCreateProductInstance(void *r_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(r_handle == DUMMY_BLOTLIB1_EXECUTOR_FACTORY_HANDLE)

  BLOTLIB1_EXECUTOR_HANDLE h_blotlib1ExecutorHandle = (BLOTLIB1_EXECUTOR_HANDLE)UNDEFINED;
  m_MALLOC_INSTANCE(h_blotlib1ExecutorHandle);
  h_blotlib1ExecutorHandle->nbIters = 3;
  *azhr_blotlibExecutorHandle = h_blotlib1ExecutorHandle;
  //*azhr_blotlibExecutorHandle = DUMMY_BLOTLIB_EXECUTOR_HANDLE;
  m_DIGGY_RETURN(RETURNED)
} // Blotlib1ExecutorFactoryCreateProductInstance

// BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION
static int Blotlib1ExecutorExecuteC(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval,  G_STRING_STUFF nc_abandonmentInfo) { 
  BLOTLIB1_EXECUTOR_HANDLE handle = (BLOTLIB1_EXECUTOR_HANDLE)r_handle;
  
  m_DIGGY_BOLLARD()
  //m_ASSERT(r_handle == DUMMY_BLOTLIB_EXECUTOR_HANDLE)
  if (ap_blotfunc->entry.localBlotfuncNameEntry == 1) { // InitUnDeuxTrois function 
    int answer = ReadGenericIntegerPString(ap_blotfunc->call.arguments, &handle->nbIters,  (int*)NULL);
    m_TRACK_IF(answer < 0)
//m_DIGGY_VAR_D(handle->nbIters)
    if (answer == ANSWER__NO) m_DIGGY_RETURN(ANSWER__NO)
  } else if (ap_blotfunc->entry.localBlotfuncNameEntry == 2) { // UnDeuxTrois function 
    if (handle->nbIters-- > 0) *ac_blotval = ITERATE_LOOP__BLOTVAL0;
    else *ac_blotval = BREAK_LOOP__BLOTVAL; 
//m_DIGGY_INFO("blot value=%d (TRUE=0 FALSE=1)",*ac_blotval)
  } else {
    *ac_blotval = 0;
  } // if
  m_DIGGY_RETURN(ANSWER__YES)
} // Blotlib1ExecutorExecuteC 

// BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION
static int Blotlib1ExecutorDestroyInstance(void *xhr_handle) {
  m_DIGGY_BOLLARD()
  free(xhr_handle);
  //m_ASSERT(xhr_handle == DUMMY_BLOTLIB_EXECUTOR_HANDLE)
  m_DIGGY_RETURN(RETURNED)
} // Blotlib1ExecutorDestroyInstance 


const char* s_localBlotfuncsName2[] = { "Nom" , "Verbe" , "Nombre", "Adverbe" } ;

#define DUMMY_BLOTLIB2_EXECUTOR_FACTORY_HANDLE (DUMMY_BLOTLIB_EXECUTOR_FACTORY_HANDLE + 1)
#define DUMMY_BLOTLIB2_EXECUTOR_HANDLE  (DUMMY_BLOTLIB_EXECUTOR_HANDLE + 1) 

// BLOTLIB_EXECUTOR_FACTORY__CREATE_PRODUCT_INSTANCE_FUNCTION
static int Blotlib2ExecutorFactoryCreateProductInstance(void *r_handle,
  void **azhr_blotlibExecutorHandle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(r_handle == DUMMY_BLOTLIB2_EXECUTOR_FACTORY_HANDLE)

  *azhr_blotlibExecutorHandle = DUMMY_BLOTLIB2_EXECUTOR_HANDLE;
  m_DIGGY_RETURN(RETURNED)
} // Blotlib2ExecutorFactoryCreateProductInstance

// BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION
static int Blotlib2ExecutorExecuteC(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval, G_STRING_STUFF nc_abandonmentInfo) { 
  m_DIGGY_BOLLARD()
  m_ASSERT(r_handle == DUMMY_BLOTLIB2_EXECUTOR_HANDLE)
  *ac_blotval = 0;
  int answer = ANSWER__YES; // a priori
  switch (ap_blotfunc->entry.localBlotfuncNameEntry) {
  case 1:  // Verbe
    m_ASSERT(b_EqualToCString(ap_blotfunc->call.arguments,"savoir, 1ps"))
    m_ASSERT(GStringCopy(c_surrogate,0,m_PString("sait")) >= 0)
  break; case 2: // Nombre
    answer = ReadGenericIntegerPString(ap_blotfunc->call.arguments, ac_blotval,(int*)NULL);
    m_TRACK_IF(answer < 0)
//m_DIGGY_VAR_D(*ac_blotval)
  break; default: ;
  } // switch
  m_DIGGY_RETURN(answer)
} // Blotlib2ExecutorExecuteC 

// BLOTLIB_EXECUTOR__DESTROY_INSTANCE_FUNCTION
static int Blotlib2ExecutorDestroyInstance (void *xhr_handle) {
  m_DIGGY_BOLLARD()
  m_ASSERT(xhr_handle == DUMMY_BLOTLIB2_EXECUTOR_HANDLE)
  m_DIGGY_RETURN(RETURNED)
} // Blotlib2ExecutorDestroyInstance 


// Returned:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised  
static int BlotcodeExecutorTest (int expectedTestNumber, BLOTCODE_EXECUTOR_HANDLE handle,
  SUCKER_HANDLE outputSuckerHandle, G_STRING_STUFF outputGStringStuff,  const char *p_template,
  int expectedAnswer, int c_expectedBlotcodeConstructionStatus,  const char *ccp_expectedOutput) {
  m_DIGGY_BOLLARD()
  struct P_STRING localTemplate = m_PString(p_template);
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)
  G_STRING_STUFF nh_parsingErrorInfo = NULL;

  if (nh_parsingErrorInfo == NULL) m_TRACK_IF(G_STRING_CREATE_INSTANCE(&nh_parsingErrorInfo) < 0)

m_DIGGY_VAR_GEN(p_template,s)
  int answer = BlotcodeExecutorParseTemplate(handle,  localTemplate, NULL, nh_parsingErrorInfo);
  m_TRACK_IF(answer < 0)
  if (answer == ANSWER__NO) m_DIGGY_VAR_P_STRING(nh_parsingErrorInfo->cv_pString)
  m_ASSERT(answer == expectedAnswer)
  if (answer == ANSWER__YES) {
    m_G_STRING_CLEAR(outputGStringStuff)
    int blotcodeConstructionStatus = BlotcodeExecutorConstructPage(handle, outputSuckerHandle,
      NULL,  NULL);
    m_TRACK_IF(blotcodeConstructionStatus < 0)
    m_ASSERT(blotcodeConstructionStatus == c_expectedBlotcodeConstructionStatus)
    if (blotcodeConstructionStatus == BLOTCODE_CONSTRUCTION_STATUS__OK) {
m_DIGGY_VAR_STRING(outputGStringStuff->nhi_string)
m_DIGGY_VAR_STRING(ccp_expectedOutput)
      m_ASSERT(outputGStringStuff->c_copiedLength == strlen(ccp_expectedOutput));
      m_ASSERT(strcmp(outputGStringStuff->nhi_string,ccp_expectedOutput) == 0);
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // BlotcodeExecutorTest 


#define DUMMY_TEMPLATE "La Nature est un temple oug de vivants piliers"
#define DUMMY_OUTPUT DUMMY_TEMPLATE

#define DUMMY_TEMPLATE2 "Elle croit, elle ##<< lib2.Verbe(savoir, 1ps) >> cette vierge infedconde"
#define DUMMY_OUTPUT2 "Elle croit, elle sait cette vierge infedconde"

#define DUMMY_TEMPLATE3 "Et pourtant ##<< lib2.Adjectif(nedcessaire, ms) >> ag la Marche du Monde"

#define DUMMY_TEMPLATE4 "Que la ##<< loop 1 >>Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT4 "Que la Beauted du corps est un sublime don"

#define DUMMY_TEMPLATE5 "Que la ##<< loop 2 >>Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT5 "Que la BeautedBeauted du corps est un sublime don"

#define DUMMY_TEMPLATE6 "Que la "\
"##<< lib1.InitUnDeuxTrois(3) >>"\
"##<< loop lib1.UnDeuxTrois() >>"\
  "Beauted"\
"##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT6 "Que la BeautedBeautedBeauted du corps est un sublime don"

#define DUMMY_TEMPLATE7 "Que la ##<< lib1.InitUnDeuxTrois(4) ;;\n loop lib1.UnDeuxTrois()>>"\
"Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT7 "Que la BeautedBeautedBeautedBeauted du corps est un sublime don"

#define DUMMY_TEMPLATE8 "Qui de toute ##<< switch lib2.Nombre(69) >>##<<case 68>>connerie##<<case 69>>infasmie##<<defaultCase>>prout##<<endSwitch>> arrache le pardon"
#define DUMMY_OUTPUT8 "Qui de toute infasmie arrache le pardon"

#define DUMMY_TEMPLATE9 "Elle ##<<switch lib2.Nombre(3)>>##<<case 4>>Dedconne##<<case 3>>##<<loop 4>>ignore##<<endLoop>>##<<endSwitch>> l'Enfer comme le Purgatoire."
#define DUMMY_OUTPUT9 "Elle ignoreignoreignoreignore l'Enfer comme le Purgatoire."

#define DUMMY_TEMPLATE10 "Elle "\
"##<<switch lib2.Nombre(3)>>"\
"##<<case 4>>Dedconne"\
"##<<case 3>>"\
  "##<< lib1.InitUnDeuxTrois(5) >>"\
  "##<<loop lib1.UnDeuxTrois()>>"\
    "ignore"\
  "##<<endLoop>>"\
"##<<endSwitch>>"\
" l'Enfer comme le Purgatoire."
#define DUMMY_OUTPUT10 "Elle ignoreignoreignoreignoreignore l'Enfer comme le Purgatoire."

#define DUMMY_TEMPLATE11 "Elle "\
"##<<switch lib2.Nombre(3);;\n"\
"    case 4;;\n"\
"    case 3;;\n"\
"      lib1.InitUnDeuxTrois(5);;\n"\
"      loop lib1.UnDeuxTrois()>>ignore##<<\n"\
"      endLoop;;\n"\
"    endSwitch\n>>"\
" l'Enfer comme le Purgatoire."
#define DUMMY_OUTPUT11 "Elle ignoreignoreignoreignoreignore l'Enfer comme le Purgatoire."


#define DUMMY_TEMPLATE12 "Et quand l'" \
"##<<loop 3>>" \
  "heure " \
  "##<<loop 2>>" \
    "viendra " \
  "##<<endLoop>>" \
"##<<endLoop>>" \
"d'entrer dans la Nuit Noire,"
#define DUMMY_OUTPUT12 "Et quand l'heure viendra viendra heure viendra viendra heure viendra viendra d'entrer dans la Nuit Noire,"


int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  BLOTCODE_HANDLE h_blotcodeHandle = (BLOTCODE_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotcodeCreateInstance(&h_blotcodeHandle) != RETURNED)
  
  int blotlibLabel = UNDEFINED; 
  blotlibLabel = BlotcodeLinkBlotlib(h_blotcodeHandle,  "lib1", 4, s_localBlotfuncsName1,
    Blotlib1ExecutorFactoryCreateProductInstance,  DUMMY_BLOTLIB1_EXECUTOR_FACTORY_HANDLE,
    Blotlib1ExecutorExecuteC,  Blotlib1ExecutorDestroyInstance);
  m_ASSERT(blotlibLabel == BLOTLIB_ENTRY0)

  blotlibLabel = BlotcodeLinkBlotlib(h_blotcodeHandle,  "lib2",  4, s_localBlotfuncsName2,
    Blotlib2ExecutorFactoryCreateProductInstance,  DUMMY_BLOTLIB2_EXECUTOR_FACTORY_HANDLE,
    Blotlib2ExecutorExecuteC,  Blotlib2ExecutorDestroyInstance);
  m_ASSERT(blotlibLabel == BLOTLIB_ENTRY0 + 1)

  m_TRACK_IF(BlotcodeFreeze(h_blotcodeHandle) != RETURNED)

  // ==== blotcode executor ====

  BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle = (BROKEN_PIPE_FIX_HANDLE)UNDEFINED; 
  m_TRACK_IF(BrokenPipeFixCreateInstance(&brokenPipeFixHandle) != RETURNED)

  SUCKER_HANDLE h_outputSuckerHandle = (SUCKER_HANDLE)UNDEFINED;
  m_TRACK_IF(SuckerCreateInstance(&h_outputSuckerHandle, 0) != RETURNED)

  BLOTCODE_EXECUTOR_HANDLE h_blotcodeExecutorHandle = (BLOTCODE_EXECUTOR_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotcodeExecutorCreateInstance(&h_blotcodeExecutorHandle,  h_blotcodeHandle) !=
    RETURNED)

  G_STRING_STUFF h_outputGStringStuff = (G_STRING_STUFF)UNDEFINED;
  m_TRACK_IF(G_STRING_CREATE_INSTANCE(&h_outputGStringStuff) != RETURNED)

  //m_TRACK_IF(m_SuckerPlugDButt_GString(h_outputSuckerHandle, h_outputGStringStuff) != RETURNED) 
  G_STRING_BUTT_ADEPT_HANDLE h_gStringButtAdeptHandle = (G_STRING_BUTT_ADEPT_HANDLE)UNDEFINED; 
  m_TRACK_IF(GStringButtAdeptCreateInstance(&h_gStringButtAdeptHandle,h_outputGStringStuff) !=
     RETURNED)
  m_TRACK_IF(SuckerPlugSDButts(h_outputSuckerHandle, NULL, (void*)UNDEFINED,
    GStringButtAdeptFill,h_gStringButtAdeptHandle) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(1,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(2,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE2, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT2) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(3,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE3, ANSWER__NO,  UNDEFINED,  (const char *)UNDEFINED) !=
    RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(4,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE4, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT4) != RETURNED)

  m_TRACK_IF(BlotcodeExecutorTest(5,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE5, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT5) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(6,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE6, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT6) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(7,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE7, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT7) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(8,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE8, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT8) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(9,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE9, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT9) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(10,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE10, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT10) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(11,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE11, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT11) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(12,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE12, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT12) != RETURNED) 

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_outputGStringStuff) != RETURNED)

  m_TRACK_IF(GStringButtAdeptDestroyInstance(h_gStringButtAdeptHandle) != RETURNED)
  m_TRACK_IF(BlotcodeExecutorDestroyInstance(h_blotcodeExecutorHandle) != RETURNED)
  m_TRACK_IF(SuckerDestroyInstance(h_outputSuckerHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixDestroyInstance(brokenPipeFixHandle) != RETURNED)

  m_TRACK_IF(BlotcodeDestroyInstance(h_blotcodeHandle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
