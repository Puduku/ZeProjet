// c-ansi/g-sequence.c, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

#include "c-ansi/g-sequence.h"
#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-ansi/c-stack.h"
#include "flint/flags.h"

#include <string.h>
#include <stdarg.h>

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// INDEX SEQUENCE:
// --------------


struct INDEX_SEQUENCE {
  // index entries "blocks":
  int indexEntryBlockCount2; // between 0 and 2 : 0 => 'disabled' ; >0 -> 'enabled' 
  struct INDEX_ENTRY_BLOCK indexEntryBlocks2[2];
  // Fields below are only significant if 'enabled' (indexEntryBlockCount2 > 0) :
  int cv_firstIndexEntry; // first index entry for ALL "blocks"  
  int cv_lastIndexEntry; // last index entry for ALL "blocks"  
  int ci_indexEntryCursor; // "current" index entry:
    // - < cv_firstIndexEntry => Ascending:"soft reset", Descending:'no more' 
    // - between [indexEntries[0].first - cv_lastIndexEntry] => 'in sequence' 
    // - cv_lastIndexEntry => Ascending:"no more", Descending:"soft reset" 
  const struct INDEX_ENTRY_BLOCK *cv_indexEntryBlockPtr; // "block" corresponding to "current"
    // index entry 
} ;

// Public function; see .h
int o_IndexSequenceNew(char *meBuffer) { m_DIGGY_BOLLARD()
  struct INDEX_SEQUENCE* mePtr = (struct INDEX_SEQUENCE*) meBuffer;
  mePtr->indexEntryBlockCount2 = 0;
  m_DIGGY_RETURN(RETURNED)
} // o_IndexSequenceNew

// Public function; see .h
int o_IndexSequenceSize(void) { m_DIGGY_BOLLARD()
  m_DIGGY_RETURN(sizeof(struct INDEX_SEQUENCE))
} // o_IndexSequenceSize


// G-REQUEST CRITERIA: 
// ------------------

// Ensure criteria op flags all set properly for actual criteria evaluation
//
// Passed:
// - s_me:
// - meCount: >= 1 
//
// Ret:
// - COMPLETED__OK: Ok (no rectification)
// - COMPLETED__BUT: criteria op. flags needed rectification
// - -1: unexpected problem; anomaly is raised
static inline int g_GRequestCriteriaRectifyOpFlags(struct G_REQUEST_CRITERION *s_me, int meCount) {
  m_DIGGY_BOLLARD()

  int completed = COMPLETED__OK; // Not rectified a priori
  int depth = 0;
  if (meCount == 1) {
    if (s_me[0].criteriaOpFlags != ALL_FLAGS_OFF0) completed = COMPLETED__BUT;
    s_me[0].criteriaOpFlags = ALL_FLAGS_OFF0;
    m_DIGGY_RETURN(completed)
  } // if 

  if (b_FLAGS_OFF(s_me[0].criteriaOpFlags,CRITERIA_OP_FLAG__AND)) {
    completed = COMPLETED__BUT;
    m_FLAGS_SET_ON(s_me[0].criteriaOpFlags,CRITERIA_OP_FLAG__AND)
  } // if
  int i = 0; for (; i < meCount; i++) {
    int initialCriteriaOpFlags = s_me[i].criteriaOpFlags;      
    if (b_FLAGS_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__OR)) {
      // Ensure AND and OR ops are not enabled simultaneously
      m_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__AND)
      // Put needed bracket to clarify precedence between AND / OR ops. :
      if (i > 0 && b_FLAGS_ON(s_me[i-1].criteriaOpFlags,CRITERIA_OP_FLAG__AND) &&
        om_CriteriaOpFlagsOpenBracketCount(s_me[i].criteriaOpFlags) == 0) m_FLAGS_SET_ON(
          s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)
    } else if (b_FLAGS_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__AND) && i > 0 &&
      b_FLAGS_ON(s_me[i-1].criteriaOpFlags,CRITERIA_OP_FLAG__OR) &&
      om_CriteriaOpFlagsOpenBracketCount(s_me[i].criteriaOpFlags) == 0) m_FLAGS_SET_ON(
        s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__OPEN1)
    depth += om_CriteriaOpFlagsOpenBracketCount(s_me[i].criteriaOpFlags);
    if (depth < om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags)) {
      // Remove excessive closing brackets
      m_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,o_FLAGS3(CRITERIA_OP_FLAG__CLOSE1,
        CRITERIA_OP_FLAG__CLOSE2, CRITERIA_OP_FLAG__CLOSE3)) 
      switch (depth) {
      case 0: 
      break; case 1: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)
      break; case 2: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)
      break; default: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3) } // switch
    } // if
    m_ASSERT((depth -= om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags)) >= 0) 
    if (i == meCount - 1) {
      if (depth - om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags) > 0) {
        // Complete missing last closing brackets
        m_FLAGS_SET_OFF(s_me[i].criteriaOpFlags,o_FLAGS3(CRITERIA_OP_FLAG__CLOSE1,
          CRITERIA_OP_FLAG__CLOSE2,CRITERIA_OP_FLAG__CLOSE3))
        switch (depth) {
        case 0:
        break; case 1: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE1)
        break; case 2: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE2)
        break; default: m_FLAGS_SET_ON(s_me[i].criteriaOpFlags,CRITERIA_OP_FLAG__CLOSE3) } // switch
      } // if  
      m_ASSERT((depth -= om_CriteriaOpFlagsCloseBracketCount(s_me[i].criteriaOpFlags)) == 0) 
    } // if
    if (s_me[i].criteriaOpFlags != initialCriteriaOpFlags) completed = COMPLETED__BUT;
  } // for

  m_DIGGY_RETURN(completed)
} // GRequestCriteriaRectifyOpFlags

// Public function; see .h
int GRequestCriteriaAdd(struct G_REQUEST_CRITERION *s_me, int meCountMax, int *a_meCount,
  struct G_REQUEST_CRITERION criterion, char b_lastCriterion) { m_DIGGY_BOLLARD()

  m_ARRAY_ADD_ITEM(s_me, meCountMax, *a_meCount, criterion)

  int completed = COMPLETED__OK; // a priori
  if (b_lastCriterion) m_TRACK_IF((completed = g_GRequestCriteriaRectifyOpFlags(s_me, *a_meCount))
    < 0)
  m_DIGGY_RETURN(completed)
} // GRequestCriteriaAdd


// GC-EVALUATOR:
// ------------

// Simple status stack used for g-criteria evaluation
// depth >= 0 
// depth == count - 1 => count always >= 1
m_STRUCT_C_STACK(GC_EVALUATOR,char)

// Possible "status":
// 'U' -> Unknown (initial)
// 'O' -> ORed evaluation
// 'A' -> ANDed evaluation
// 'V' -> Valid (OK) 
// 'X' -> Rejected (KO) 

// Re-init g-criteria "evaluator".
//
// Passed:
// - a_me: 
//
// Changed:
// - *a_me: reset for new criteria eval: depth == 0 ; 'U' 
//
// Ret:
// - RETURNED
static inline int m_GcEvaluatorReset(struct GC_EVALUATOR* a_me) { m_DIGGY_BOLLARD_S()
  m_C_STACK_CLEAR(*a_me)
  m_C_STACK_PUSH(*a_me,'U'); 
  m_DIGGY_RETURN(RETURNED)
} // m_GcEvaluatorReset

// Passed:
// - a_me:
// - minDepth: >= 0
//
// Ret:
// - >=0:  evualator's depth
// - 1: unexpected problem; anomaly is raised
static inline int m_GcEvaluatorDepth(const struct GC_EVALUATOR* ap_me, int minDepth) {
  m_DIGGY_BOLLARD_S()
  m_ASSERT(ap_me->count > minDepth)
  return ap_me->count - 1;
} // m_GcEvaluatorDepth 

// Iterate next criterion evaluation
//
// Passed:
// - *a_me: current status of g-criteria evaluator 
// - nb_passed: 
//   + -1 special value: criterion not evualuated (boost mode)
//   + >= 0: current criterion adequation ? (true/false)
// - criteriaOpFlags: current criterion's op. flags (must have been prepared properly: see
//   g_GRequestCriteriaRectifyOpFlags() above)
//
// Changed:
// - *a_me: updated status of g-criteria evaluator 
// - *ab_boost: need to evaluate next criterion ? (TRUE/FALSE)
//
// Ret:
// - RETURNED: Ok
// - -1: unexpected problem; anomaly is raised
static inline int m_GcEvaluatorIterate(struct GC_EVALUATOR* a_me, signed char nb_passed,
  int criteriaOpFlags, char *ab_boost) { m_DIGGY_BOLLARD_S()
  int openBracketCount = om_CriteriaOpFlagsOpenBracketCount(criteriaOpFlags);
  int i = 0; for (; i < openBracketCount; i++) {
    m_C_STACK_PUSH(*a_me,'U')
  } // for
  char status; m_C_STACK_PEEK(*a_me,status);
  int closeBracketCount = om_CriteriaOpFlagsCloseBracketCount(criteriaOpFlags);
m_DIGGY_INFO("nb_passed=%d openBracketCount=%d closeBracketCount=%d status=%c",nb_passed,openBracketCount,closeBracketCount,status)
  *ab_boost = b_FALSE0; // No boost a priori

  if (b_FLAGS_OFF(criteriaOpFlags,o_FLAGS2(CRITERIA_OP_FLAG__AND,CRITERIA_OP_FLAG__OR)) ||
    closeBracketCount > 0) {
    // NO logical AND/OR op. or closing brackets => last operand 
    m_TRACK_IF(m_GcEvaluatorDepth(a_me,closeBracketCount) < 0)
    // Finalize status (on that depth):
    switch (status) {
    case 'U':
    case 'O': // OR expression
    case 'A': // AND expression
      m_ASSERT(nb_passed >= 0)
      status = (nb_passed? 'V': 'X');
      m_C_STACK_POKE(*a_me,status);
    break; case 'V':
    case 'X':
    break; default: m_RAISE(ANOMALY__VALUE__D,status)
    } // switch 
m_DIGGY_VAR_GEN(status,c)
    m_ASSERT(status == 'V' || status == 'X')
  } // if
  for (i = 0; i < closeBracketCount; i++) {
    char termStatus; m_C_STACK_PEEK(*a_me,termStatus);
    m_ASSERT(termStatus == 'V' || termStatus == 'X')
    m_C_STACK_POP(*a_me,status);
    m_TRACK_IF(m_GcEvaluatorDepth(a_me,0) < 0)
    // Finalize status (on that depth):
    switch (status) {
    case 'U':
    case 'O': // OR expression
    case 'A': // AND expression
      status = termStatus; 
    break; case 'V':
    case 'X':
    break; default: m_RAISE(ANOMALY__VALUE__D,status)
    } // switch 
    m_ASSERT(status == 'V' || status == 'X')
  m_DIGGY_VAR_GEN(termStatus,c)
    m_C_STACK_POKE(*a_me,status)
  } // for

  if (!b_FLAGS_OFF(criteriaOpFlags,o_FLAGS2(CRITERIA_OP_FLAG__AND,CRITERIA_OP_FLAG__OR))) { 
    // OR / AND logical op. 
    m_ASSERT(nb_passed >= 0)
    switch (status) {
    case 'U':
      if (b_FLAGS_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OR)) 
        status = (nb_passed? 'V': 'O');
      else if (b_FLAGS_ON(criteriaOpFlags,CRITERIA_OP_FLAG__AND)) 
        status = (nb_passed? 'A': 'X');
      else status = (nb_passed? 'V': 'X');
    break; case 'O':
      m_ASSERT(b_FLAGS_ON(criteriaOpFlags,CRITERIA_OP_FLAG__OR))
      if ((*ab_boost = nb_passed)) status = 'V';
    break; case 'A':
      m_ASSERT(b_FLAGS_ON(criteriaOpFlags,CRITERIA_OP_FLAG__AND))
      if ((*ab_boost = !nb_passed)) status = 'X';
    break; case 'V':
    case 'X':
    break; default: m_RAISE(ANOMALY__VALUE__D,status)
    } // switch 
m_DIGGY_VAR_GEN(status,c)
    m_C_STACK_POKE(*a_me,status)
  } // if

  m_DIGGY_RETURN(RETURNED)
} // m_GcEvaluatorIterate


// Public function; see .h
int l_GIndexesSequenceReset(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *ap_1stGRequestCriterion, char b_descending,
  char *indexSequenceBuffer) { m_DIGGY_BOLLARD()
  m_ASSERT(ap_1stGRequestCriterion->indexLabel < o_GIndexesGetIndexCount(handle)) 
m_DIGGY_VAR_INDEX_SEEK_FLAGS(ap_1stGRequestCriterion->indexSeekFlags)
m_DIGGY_VAR_P(ap_1stGRequestCriterion->cr_gKeys)

  struct INDEX_SEQUENCE *a_indexSequence = (struct INDEX_SEQUENCE*)indexSequenceBuffer;
  a_indexSequence->indexEntryBlockCount2 = 0; // Empty ("disabled") selection a priori
  int itemCount = o_GIndexesGetItemCount(handle);
  if (itemCount > 0) { 
m_DIGGY_VAR_INDEX_SEEK_FLAGS(ap_1stGRequestCriterion->indexSeekFlags)

    int n_firstIndexEntry = -1; // Not set a priori
    int n_lastIndexEntry = -1; // Not set a priori
    if (b_FLAGS_ON(ap_1stGRequestCriterion->indexSeekFlags,INDEX_SEEK_FLAG__ANY)) { // NOT key-based selection
      n_firstIndexEntry = 0; 
    } else { // key-based selection
      int top = UNDEFINED;
      int n_indexEntry = UNDEFINED;
      struct INDEX_ENTRY_BLOCK c_indexEntries; // UNDEFINED
m_DIGGY_VAR_P(ap_1stGRequestCriterion->cr_gKeys)
      m_TRACK_IF(GIndexesBSearch(handle,ap_1stGRequestCriterion->indexLabel, -1,ap_1stGRequestCriterion->cr_gKeys, &n_indexEntry,&top,
        &c_indexEntries) != RETURNED);
      switch (ap_1stGRequestCriterion->indexSeekFlags) {
      case INDEX_SEEK_FLAGS__EQUAL:
        if (n_indexEntry != -1) {
          n_firstIndexEntry = c_indexEntries.first;
          n_lastIndexEntry = c_indexEntries.last;
        } // if
      break; case INDEX_SEEK_FLAGS__GREATER_EQUAL:
        if (n_indexEntry != -1) n_firstIndexEntry = c_indexEntries.first; 
        else if (top < itemCount) n_firstIndexEntry = top;
      break; case INDEX_SEEK_FLAGS__GREATER:
        if (n_indexEntry != -1 && c_indexEntries.last+1 < itemCount) n_firstIndexEntry =
          c_indexEntries.last+1;
        else if (n_indexEntry == -1 && top < itemCount) n_firstIndexEntry = top;
      break; case INDEX_SEEK_FLAGS__LESS_EQUAL: 
        if (n_indexEntry != -1) n_lastIndexEntry = n_indexEntry;
        else if (n_indexEntry == -1 && top-1 >= 0) n_lastIndexEntry = top-1;
      break; case INDEX_SEEK_FLAGS__LESS: 
        if (n_indexEntry != -1 && c_indexEntries.first-1 >= 0) n_lastIndexEntry = 
          c_indexEntries.first-1;
        else if (n_indexEntry == -1 && top-1 >= 0) n_lastIndexEntry = top-1;
      break; case INDEX_SEEK_FLAGS__NOT_EQUAL:
        if (n_indexEntry != -1) {
          if (c_indexEntries.first-1 >= 0) n_lastIndexEntry = c_indexEntries.first;
          if (c_indexEntries.last+1 < itemCount) n_firstIndexEntry = c_indexEntries.last+1;
        } else n_firstIndexEntry = 0; 
      break; default: // INDEX_SEEK_FLAGS__LIKE case 
        m_RAISE(ANOMALY__VALUE__D,ap_1stGRequestCriterion->indexSeekFlags)
      } // switch
    } // if 

    if (n_firstIndexEntry >= 0 && n_lastIndexEntry >= 0 && n_firstIndexEntry > n_lastIndexEntry) {
      a_indexSequence->indexEntryBlockCount2 = 2; 
      a_indexSequence->indexEntryBlocks2[0].first = 0;
      a_indexSequence->indexEntryBlocks2[0].last = n_lastIndexEntry;
      a_indexSequence->indexEntryBlocks2[1].first = n_firstIndexEntry;
      a_indexSequence->cv_lastIndexEntry = a_indexSequence->indexEntryBlocks2[1].last =
        itemCount-1;
    } else if (n_firstIndexEntry >= 0 || n_lastIndexEntry >= 0) {
      a_indexSequence->indexEntryBlockCount2 = 1; 
      if (n_firstIndexEntry < 0) n_firstIndexEntry = 0;
      if (n_lastIndexEntry < 0) n_lastIndexEntry = itemCount-1;
      a_indexSequence->indexEntryBlocks2[0].first = n_firstIndexEntry;
      a_indexSequence->cv_lastIndexEntry =
        a_indexSequence->indexEntryBlocks2[0].last = n_lastIndexEntry;
    } // if
    // Prime the pump: ("soft reset")
    a_indexSequence->ci_indexEntryCursor = (b_descending? a_indexSequence->cv_lastIndexEntry
      + 1: a_indexSequence->indexEntryBlocks2[0].first - 1);
    a_indexSequence->cv_indexEntryBlockPtr = a_indexSequence->indexEntryBlocks2;
  } // if

  m_DIGGY_RETURN(RETURNED)
} // l_GIndexSequenceReset


// Update index sequence (NEXT) and get entry 
//
// Passed:
// - handle: 
// - b_descending: 
// - a_indexSequence-> : current state of sequence
//
// Changed:
// - a_indexSequence-> : new state of sequence
// - *an_entry: item entry to retrieve : 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
static inline int ml_GIndexesSequenceNextInternal(G_INDEXES_HANDLE handle, int indexLabel,
  char b_descending, struct INDEX_SEQUENCE *a_indexSequence, int *an_entry) { m_DIGGY_BOLLARD_S()
  *an_entry = -1; // "disabled" / "No more" a priori
  if (a_indexSequence->indexEntryBlockCount2 > 0) { // "enabled"
    if (b_descending) {
      if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntryBlocks2[0].first) { 
        // NOT YET "no more"...
        if (a_indexSequence->ci_indexEntryCursor > a_indexSequence->cv_lastIndexEntry) {
          // "soft reset"
          a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntryBlockPtr =
            a_indexSequence->indexEntryBlocks2 + a_indexSequence->indexEntryBlockCount2 - 1)->last;
        } else if (--(a_indexSequence->ci_indexEntryCursor) <
          a_indexSequence->cv_indexEntryBlockPtr->first && a_indexSequence->cv_indexEntryBlockPtr >
          a_indexSequence->indexEntryBlocks2) {
          // Pass to next block:
          a_indexSequence->ci_indexEntryCursor = (--(a_indexSequence->cv_indexEntryBlockPtr))->last;
        } // if
        if (a_indexSequence->ci_indexEntryCursor >= a_indexSequence->indexEntryBlocks2[0].first) 
          // STILL NOT YET "no more"
          *an_entry = o_GIndexesGetItemEntry(handle,indexLabel,a_indexSequence->ci_indexEntryCursor);
      } // if
    } else { // Ascending...
      if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) { 
        // NOT YET "no more"...
        if (a_indexSequence->ci_indexEntryCursor < a_indexSequence->indexEntryBlocks2[0].first) {
          // "soft reset"
          a_indexSequence->ci_indexEntryCursor = (a_indexSequence->cv_indexEntryBlockPtr =
            a_indexSequence->indexEntryBlocks2)->first;
        } else if (++(a_indexSequence->ci_indexEntryCursor) >
          a_indexSequence->cv_indexEntryBlockPtr->last && a_indexSequence->cv_indexEntryBlockPtr <
          a_indexSequence->indexEntryBlocks2 + a_indexSequence->indexEntryBlockCount2 - 1) {
          // Pass to next block:
          a_indexSequence->ci_indexEntryCursor = (++(a_indexSequence->cv_indexEntryBlockPtr))->first;
        } // if
        if (a_indexSequence->ci_indexEntryCursor <= a_indexSequence->cv_lastIndexEntry) 
          // STILL NOT YET "no more"
          *an_entry = o_GIndexesGetItemEntry(handle,indexLabel,a_indexSequence->ci_indexEntryCursor);
      } // if
    } // if
  } // if

  m_DIGGY_RETURN(RETURNED)
} // ml_GIndexesSequenceNextInternal

// Perform "equation" of entry with a key. 
//
// Passed:
// - handle:
// - indexLabel: 
// - aEntry: "A" entry 
// - indexSeekFlags: 
// - r_bGKeys: "B" keys(s) 
//
// Returned:
// - >=0: "equation" between :"A" and "B" with that key component... 
//   + ANSWER__YES : "A" item and "B" key(s) are similar 
//   + ANSWER__NO : "A" item and "B" key(s) are NOT similar 
// - -1: unexpected problem; anomaly is raised
static int ml_GIndexesSeekEntryEquate(G_INDEXES_HANDLE handle, int indexLabel, int aEntry,
  unsigned int indexSeekFlags, void *r_bGKeys) { m_DIGGY_BOLLARD()
  if (indexSeekFlags == INDEX_SEEK_FLAGS__ANY) return ANSWER__YES;

  int answer = ANSWER__NO; // a priori
  if (indexSeekFlags != INDEX_SEEK_FLAGS__LIKE) {
    int comparison = GIndexesEntryCompare(handle, indexLabel, aEntry, -1, r_bGKeys);
    m_TRACK_IF(comparison < 0)
    switch (indexSeekFlags) {
    case INDEX_SEEK_FLAGS__EQUAL:
      if (comparison == EQUAL_TO__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__LESS:
      if (comparison == LESS_THAN__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__LESS_EQUAL:
      if (comparison == LESS_THAN__COMPARISON || comparison == EQUAL_TO__COMPARISON) answer =
        ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__GREATER:
      if (comparison == GREATER_THAN__COMPARISON) answer = ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__GREATER_EQUAL:
      if (comparison == GREATER_THAN__COMPARISON || comparison == EQUAL_TO__COMPARISON) answer =
        ANSWER__YES;
    break; case INDEX_SEEK_FLAGS__NOT_EQUAL:
      if (comparison != EQUAL_TO__COMPARISON) answer = ANSWER__YES;
    break; default:
      m_RAISE(ANOMALY__VALUE__D,indexSeekFlags)
    } // switch
  } else { // INDEX_SEEK_FLAGS__LIKE
    m_TRACK_IF((answer = GIndexesEntryEquate(handle,indexLabel, aEntry, r_bGKeys)) < 0)
  } // if 
  m_DIGGY_RETURN(answer)
} // ml_GIndexesSeekEntryEquate

// Public function; see .h
int l_GIndexesSequenceNext(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, int gRequestCriterionCount,
  char b_descending, char *indexSequenceBuffer, int *an_entry) { m_DIGGY_BOLLARD_S()
  m_ASSERT(sp_gRequestCriteria[0].indexLabel < o_GIndexesGetIndexCount(handle)) 
  struct INDEX_SEQUENCE *a_indexSequence = (struct INDEX_SEQUENCE*)indexSequenceBuffer;
  struct GC_EVALUATOR gcEvaluator = { UNDEFINED } ;
  m_C_STACK_INIT(gcEvaluator,5)

  char status = (char)UNDEFINED;
  do {
    m_TRACK_IF(ml_GIndexesSequenceNextInternal(handle,sp_gRequestCriteria[0].indexLabel,
      b_descending, a_indexSequence, an_entry) != RETURNED) 
    if (*an_entry >= 0) {
      const struct G_REQUEST_CRITERION* p_gRequestCriterionPtr = sp_gRequestCriteria;
// m_ASSERT(1st criterion op flags == AND , at least one '('
      m_TRACK_IF(m_GcEvaluatorReset(&gcEvaluator) != RETURNED)
      signed char nb_passed = b_TRUE; // 1st iteration  
      char cb_boost = b_FALSE0; // Not significant for 1st iteration 
      int i = 0; for (; i < gRequestCriterionCount; i++, p_gRequestCriterionPtr++) {
        if (i > 0) {
          if (cb_boost) nb_passed = -1; else {
            int answer = UNDEFINED;
            m_TRACK_IF((answer = ml_GIndexesSeekEntryEquate(handle,
              p_gRequestCriterionPtr->indexLabel,*an_entry,
              p_gRequestCriterionPtr->indexSeekFlags, p_gRequestCriterionPtr->cr_gKeys)) < 0)
            nb_passed = (answer == ANSWER__YES);
          } // if
        } // if
        m_TRACK_IF(m_GcEvaluatorIterate(&gcEvaluator, nb_passed,
          p_gRequestCriterionPtr->criteriaOpFlags, &cb_boost) != RETURNED)
      } // for
      m_ASSERT(m_GcEvaluatorDepth(&gcEvaluator,0) == 0) 
      m_C_STACK_PEEK(gcEvaluator,status)
m_DIGGY_VAR_GEN(status,c)
      m_ASSERT(status == 'V' || status == 'X') 
      if (status == 'V') break; // entry matches criteria
    } else break; // No more entry => finished 
  } while (b_TRUE) ;
  m_C_STACK_FREE(gcEvaluator)

  m_DIGGY_VAR_D(*an_entry)
  m_DIGGY_RETURN(RETURNED)
} // l_GIndexesSequenceNext


// Public function; see .h
int l_GIndexesSequenceCurrent(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, const char *p_indexSequenceBuffer,
  int *an_entry) { m_DIGGY_BOLLARD()
  const struct INDEX_SEQUENCE* ap_indexSequence = (struct INDEX_SEQUENCE*)p_indexSequenceBuffer;
  m_ASSERT(sp_gRequestCriteria[0].indexLabel < o_GIndexesGetIndexCount(handle)) 
  
  if (ap_indexSequence->indexEntryBlockCount2 > 0 && ap_indexSequence->ci_indexEntryCursor >= 
    ap_indexSequence->indexEntryBlocks2[0].first && ap_indexSequence->ci_indexEntryCursor <=
    ap_indexSequence->cv_lastIndexEntry)
    *an_entry = o_GIndexesGetItemEntry(handle,sp_gRequestCriteria[0].indexLabel,
      ap_indexSequence->ci_indexEntryCursor);
  else *an_entry = -1; // No current item 

  m_DIGGY_RETURN(RETURNED)
} // l_GIndexesSequenceCurrent
