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
  m_ASSIGN_LOCAL_C_P_STRING(localTemplate,p_template)
  m_ASSERT(expectedTestNumber == ++testNumber)
  m_DIGGY_VAR_D(testNumber)
  static G_STRING_STUFF n_info = NULL;
  if (n_info == NULL) m_TRACK_IF(G_STRING_CREATE_INSTANCE(&n_info) !=
    RETURNED)
  m_ASSERT(n_info != NULL)

m_DIGGY_VAR_GEN(p_template,s)
  int answer = BlotcodeExecutorParseTemplate(handle,  localTemplate, NULL, n_info);
  m_TRACK_IF(answer < 0)
  if (answer == ANSWER__NO) m_DIGGY_VAR_STRING(n_info->nhi_string)
  m_ASSERT(answer == expectedAnswer)
  if (answer == ANSWER__YES) {
    m_G_STRING_CLEAR(outputGStringStuff)
    int blotcodeConstructionStatus = BlotcodeExecutorConstructPage(handle, outputSuckerHandle,
      NULL, n_info);
    m_TRACK_IF(blotcodeConstructionStatus < 0)
    if (blotcodeConstructionStatus == BLOTCODE_CONSTRUCTION_STATUS__ABANDONNED) m_DIGGY_VAR_STRING(
      n_info->nhi_string)
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


#define DUMMY_TEMPLATE1 "La Nature est un temple oug de vivants piliers"
#define DUMMY_OUTPUT1 DUMMY_TEMPLATE1

#define DUMMY_TEMPLATE2 "Que la ##<< be.Eval(0) >>Beauted du corps est un sublime don"
#define DUMMY_OUTPUT2 "Que la Beauted du corps est un sublime don"

#define DUMMY_TEMPLATE3 "Que la ##<< loop be.Eval(69) >>Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT3 "Que la  du corps est un sublime don"

#define DUMMY_TEMPLATE4 \
  "##<< be.Eval(.toto := 69)>>Que la "\
  "##<<loop be.Eval(.toto)>>"\
  "Beauted##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT4 "Que la  du corps est un sublime don"

#define DUMMY_TEMPLATE5 \
  "Que la ##<< be.Eval(.toto := 3)>>"\
  "##<< loop be.Eval(.toto > 0) >>"\
  "Beauted##<< be.Eval(.toto := .toto - 1)>>"\
  "##<<endLoop>> du corps est un sublime don"
#define DUMMY_OUTPUT5 "Que la BeautedBeautedBeauted du corps est un sublime don"

#define DUMMY_TEMPLATE6 \
  "##<<be.Eval(.julie$ := \"Beauted\")>>Que la "\
  "##<< be.OutputF(%s, .julie$) >> du corps est un sublime don"
#define DUMMY_OUTPUT6 "Que la Beauted du corps est un sublime don"

#define DUMMY_TEMPLATE7 \
  "##<<be.Eval(.Julie$ := \"Beauté\")>>##<< be.OutputF(%s, .Julie!$) >> "\
  "est une ##<< be.OutputF(%s, .julie$) >>."
#define DUMMY_OUTPUT7 "Julie est une Beauted."

#define DUMMY_TEMPLATE8 \
  "##<<be.Eval(.Julie$ := \"Beauted\")>>Que la "\
  "##<<be.OutputF(%s, ?: !$ == \"Julie\" :?+=$)>> du corps est un sublime don"
#define DUMMY_OUTPUT8 "Que la Beauted du corps est un sublime don"

#define DUMMY_TEMPLATE9 \
  "##<<be.Eval(recettes.bolo$ := \"Pastes, frometon et sauce tomate\")>>"\
  "##<<be.Eval(recettes.galettes$ := \"Farine de Sarrasin, sel et eau\")>>"\
  "##<<be.Eval(recettes?:!$* :?)>>"\
  "##<<loop be.Eval(recettes?+ )>>"\
  "##<<be.OutputF(%s, recettes?=$)>>-"\
  "##<<endLoop>>"\
  "##<<be.Eval(recettes?:$* :?)>>"\
  "##<<loop be.Eval(recettes?+ )>>"\
  "##<<be.OutputF(%s, recettes?=$)>>-"\
  "##<<endLoop>>"
#define DUMMY_OUTPUT9 "Pastes, frometon et sauce tomate-Farine de Sarrasin, sel et eau-"\
  "Farine de Sarrasin, sel et eau-Pastes, frometon et sauce tomate-"


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
    h_outputGStringStuff, DUMMY_TEMPLATE1, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT1) != RETURNED) 

  m_TRACK_IF(BlotcodeExecutorTest(2,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE2, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT2) != RETURNED)

  m_TRACK_IF(BlotcodeExecutorTest(3,h_blotcodeExecutorHandle, h_outputSuckerHandle,
    h_outputGStringStuff, DUMMY_TEMPLATE3, ANSWER__YES,  BLOTCODE_CONSTRUCTION_STATUS__OK,
    DUMMY_OUTPUT3) != RETURNED) 

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

  m_TRACK_IF(G_STRING_DESTROY_INSTANCE(h_outputGStringStuff) != RETURNED)

  m_TRACK_IF(GStringButtAdeptDestroyInstance(h_gStringButtAdeptHandle) != RETURNED)
  m_TRACK_IF(BlotcodeExecutorDestroyInstance(h_blotcodeExecutorHandle) != RETURNED)
  m_TRACK_IF(SuckerDestroyInstance(h_outputSuckerHandle) != RETURNED)
  m_TRACK_IF(BrokenPipeFixDestroyInstance(brokenPipeFixHandle) != RETURNED)

  m_TRACK_IF(BlotexlibExecutorFactoryDestroyInstance(h_blotexlibExecutorFactoryHandle) != RETURNED)

  m_TRACK_IF(BlotcodeDestroyInstance(h_blotcodeHandle) != RETURNED)

  m_DIGGY_RETURN(0)
} // main
