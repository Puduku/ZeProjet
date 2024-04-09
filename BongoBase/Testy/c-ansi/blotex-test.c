// c-ansi/blotex-test.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004
// (c) Puduku - 2023

#include <stdio.h>

#include "c-ansi/blotex.h"
#include "c-posix/sucker.h"

#include "c-ansi/alloc.h"
#include "c-ansi/p-string.h"

#include "diggy/diggy.h"
#include "testy/getopts.h"
#include "diggy/vars.h"

static int testNumber = 0;

#define DUMMY_BLOTLIB1_EXECUTOR_FACTORY_HANDLE (void *)0xCACABEBE

const char* s_localBlotfuncsName1[] = { "func1" , "InitUnDeuxTrois" , "UnDeuxTrois" , "func4" } ;

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
  m_DIGGY_RETURN(RETURNED)
} // Blotlib1ExecutorFactoryCreateProductInstance

// BLOTLIB_EXECUTOR__EXECUTE_C_FUNCTION
static int Blotlib1ExecutorExecuteC(void *r_handle, const struct BLOTFUNC *ap_blotfunc,
  G_STRING_STUFF c_surrogate, gen_BLOTVAL *ac_blotval,  G_STRING_STUFF nc_abandonmentInfo) { 
  BLOTLIB1_EXECUTOR_HANDLE handle = (BLOTLIB1_EXECUTOR_HANDLE)r_handle;
  
  m_DIGGY_BOLLARD()

  if (ap_blotfunc->entry.localBlotfuncNameEntry == 1) { // InitUnDeuxTrois function 
    int answer = ReadGenericIntegerStringPortion(ap_blotfunc->call.arguments, &handle->nbIters,  (int*)NULL);
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
  m_DIGGY_RETURN(RETURNED)
} // Blotlib1ExecutorDestroyInstance 


const char* s_localBlotfuncsName2[] = { "Nom" , "Verbe" , "Nombre", "Adverbe" } ;

// Returned:
// - RETURNED: OK
// - -1: unexpected problem; anomaly is raised  
static int BlotcodeExecutorTest (int expectedTestNumber, BLOTCODE_EXECUTOR_HANDLE handle,
  SUCKER_HANDLE outputSuckerHandle, G_STRING_STUFF outputGStringStuff,  const char *p_template,
  int expectedAnswer, int c_expectedBlotcodeConstructionStatus,  const char *ccp_expectedOutput) {
  m_DIGGY_BOLLARD()
  m_ASSIGN_LOCAL_C_STRING_PORTION(localTemplate,p_template)
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)

m_DIGGY_VAR_GEN(p_template,s)
  int answer = BlotcodeExecutorParseTemplate(handle,  localTemplate, NULL, NULL);
  m_TRACK_IF(answer < 0)
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

#define DUMMY_TEMPLATE4 "Que la ##<< be.Eval(0) >>Beauted du corps est un sublime don"
#define DUMMY_OUTPUT4 "Que la Beauted du corps est un sublime don"

#define DUMMY_TEMPLATE5 "Que la ##<< loop be.Eval(69) >>Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT5 "Que la  du corps est un sublime don"

#define DUMMY_TEMPLATE6 "Que la "\
"##<< lib1.InitUnDeuxTrois(3) >>"\
"##<< loop lib1.UnDeuxTrois() >>"\
  "Beauted"\
"##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT6 "Que la BeautedBeautedBeauted du corps est un sublime don"


#define DUMMY_TEMPLATE10 "Et quand l'" \
"##<<loop 3>>" \
  "heure " \
  "##<<loop 2>>" \
    "viendra " \
  "##<<endLoop>>" \
"##<<endLoop>>" \
"d'entrer dans la Nuit Noire,"
// "Et quand l'##<<loop 3>>heure ##<<loop 2>>viendra ##<<endLoop>>##<<endLoop>>d'entrer dans la Nuit Noire,"
#define DUMMY_OUTPUT10 "Et quand l'heure viendra viendra heure viendra viendra heure viendra viendra d'entrer dans la Nuit Noire,"


int main (int argc, char** argv) {
  unsigned int dgFlags = ParseTestyCommandArguments(argc,argv,ALL_FLAGS_OFF0) ;
  m_DIGGY_INIT_BOLLARD(dgFlags)

  BLOTCODE_HANDLE h_blotcodeHandle = (BLOTCODE_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotcodeCreateInstance(&h_blotcodeHandle) != RETURNED)
  
  BLOTEXLIB_EXECUTOR_FACTORY_HANDLE h_blotexlibExecutorFactoryHandle =
    (BLOTEXLIB_EXECUTOR_FACTORY_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotexlibExecutorFactoryCreateInstance(&h_blotexlibExecutorFactoryHandle) != RETURNED)

  int blotlibLabel = UNDEFINED; 
  blotlibLabel = BlotcodeLinkBlotlib(h_blotcodeHandle,  "lib1", 4, s_localBlotfuncsName1,
    Blotlib1ExecutorFactoryCreateProductInstance,  DUMMY_BLOTLIB1_EXECUTOR_FACTORY_HANDLE,
    Blotlib1ExecutorExecuteC,  Blotlib1ExecutorDestroyInstance);
  m_ASSERT(blotlibLabel == BLOTLIB_ENTRY0)

#define BLOTEXLIB_PREFIX "be"

  int blotexlibLabel = BlotcodeLinkBlotexlib(h_blotcodeHandle, BLOTEXLIB_PREFIX,
    h_blotexlibExecutorFactoryHandle) ;
  m_ASSERT(blotexlibLabel == BLOTLIB_ENTRY0 + 1)


  m_TRACK_IF(BlotcodeFreeze(h_blotcodeHandle) != RETURNED)

  // ==== blotcode executor ====

  BROKEN_PIPE_FIX_HANDLE brokenPipeFixHandle = (BROKEN_PIPE_FIX_HANDLE)UNDEFINED; 
  m_TRACK_IF(BrokenPipeFixCreateInstance(&brokenPipeFixHandle) != RETURNED)

  SUCKER_HANDLE h_outputSuckerHandle = (SUCKER_HANDLE)UNDEFINED;
  m_TRACK_IF(SuckerCreateInstance(&h_outputSuckerHandle, 0) != RETURNED)

  BLOTCODE_EXECUTOR_HANDLE h_blotcodeExecutorHandle = (BLOTCODE_EXECUTOR_HANDLE)UNDEFINED;
  m_TRACK_IF(BlotcodeExecutorCreateInstance(&h_blotcodeExecutorHandle,  h_blotcodeHandle) !=
    RETURNED)

  BLOTEXLIB_EXECUTOR_HANDLE blotexlibExecutorHandle = (BLOTEXLIB_EXECUTOR_HANDLE)UNDEFINED;
  m_ASSERT(BlotcodeExecutorGetBlotexlibExecutorHandle(h_blotcodeExecutorHandle,blotexlibLabel,
    &blotexlibExecutorHandle) == RETURNED)


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
    h_outputGStringStuff, DUMMY_TEMPLATE4, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT4) != RETURNED)

  m_TRACK_IF(BlotcodeExecutorTest(3,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE5, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT5) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(4,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE6, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT6) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(5,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE10, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT10) != RETURNED) 

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_outputGStringStuff) != RETURNED)

  m_TRACK_IF(GStringButtAdeptDestroyInstance(h_gStringButtAdeptHandle) != RETURNED)
  m_TRACK_IF(BlotcodeExecutorDestroyInstance(h_blotcodeExecutorHandle) != RETURNED)
  m_TRACK_IF(SuckerDestroyInstance(h_outputSuckerHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixDestroyInstance(brokenPipeFixHandle) != RETURNED)

  m_TRACK_IF(BlotexlibExecutorFactoryDestroyInstance(h_blotexlibExecutorFactoryHandle) != RETURNED)

  m_TRACK_IF(BlotcodeDestroyInstance(h_blotcodeHandle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main