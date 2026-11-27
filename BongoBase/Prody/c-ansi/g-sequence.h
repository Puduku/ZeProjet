// c-ansi/g-sequence.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003

// #SEE double-inclusion-4-generic-imaged-enums @ flint/images.h <__C_ANSI_G_SEQUENCE_H_INCLUDED__>
#ifndef __C_ANSI_G_SEQUENCE_H_INCLUDED__
#define __C_ANSI_G_SEQUENCE_H_INCLUDED__ 0
#endif

#if __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 0 || __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 2 
#if __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 0

#include "flint/flags.h"
#include "c-ansi/stderr.h"
#include "c-ansi/alloc.h"
#include "c-ansi/c-stack.h"
#include "c-ansi/g-index.h"

#include <string.h>
#include <stdarg.h>

#include "c-ansi/diggy.h"
#include "c-ansi/testy-diggy.h"


// Index sequence:
// ---------------

// Set new index sequence ("disabled" state - disambiguation purpose...)
// 
// Passed:
// - meBuffer:
//
// Changed:
// - *meBuffer:
// 
// Ret:
// - RETURNED: Ok
int o_IndexSequenceNew(char *meBuffer) ;

// 
// Ret: index sequence buffer size
int o_IndexSequenceSize(void);   


// G-Request criteria:
// ------------------

#define CRITERIA_OP_FLAG__CLOSE1      0x001 // Close one bracket before op.
#define CRITERIA_OP_FLAG__CLOSE2      0x002 // Close two brackets before op.
#define CRITERIA_OP_FLAG__CLOSE3      0x004 // Close three brackets before op.
#define CRITERIA_OP_FLAG__AND         0x010 // And op.
#define CRITERIA_OP_FLAG__OR          0x020 // Or op.
#define CRITERIA_OP_FLAG__OPEN1       0x100 // Open one bracket after op. 
#define CRITERIA_OP_FLAG__OPEN2       0x200 // Open two brackets after op. 
#define CRITERIA_OP_FLAG__OPEN3       0x400 // Open three brackets after op. 

// Passed:
// - criteriaOpFlags
static inline int om_CriteriaOpFlagsOpenBracketCount(int me) {
  if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__OPEN3)) return 3;
  else if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__OPEN2)) return 2;
  else if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__OPEN1)) return 1;
  return 0;
} // om_CriteriaOpFlagsOpenBracketCount 

// Passed:
// - criteriaOpFlags
static inline int om_CriteriaOpFlagsCloseBracketCount(int me) {
  if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__CLOSE3)) return 3;
  else if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__CLOSE2)) return 2;
  else if (b_FLAGS_ON(me,CRITERIA_OP_FLAG__CLOSE1)) return 1;
  return 0;
} // om_CriteriaOpFlagsCloseBracketCount 

// May or may be NOT index-based selection:
struct G_REQUEST_CRITERION {
  int indexLabel;
  unsigned int indexSeekFlags;
  void *cr_gKeys; // Only significant with Key-based seek flag(s)
  unsigned int criteriaOpFlags;
} ;

// #REF om_GRequestCriterion <keys>
// Establish request criterion
//
// Passed:
// - indexLabel:
// - indexSeekFlags:
// - cr_gKeys: only significant with Key-based seek flag(s) 
// - criteriaOpFlags:
//
// Returned:
// - request criterion
static inline struct G_REQUEST_CRITERION om_GRequestCriterion(int indexLabel,
  unsigned int indexSeekFlags, void* cr_gKeys, unsigned int criteriaOpFlags) {
  struct G_REQUEST_CRITERION me = { .indexLabel = indexLabel,
    .indexSeekFlags = indexSeekFlags, .cr_gKeys = cr_gKeys, .criteriaOpFlags = criteriaOpFlags };
  return me;
} // om_GRequestCriterion


// Add criterion. When last criterion, ensure the flags are set properly:
// - first criterion:
//   + no flag set if unique criterion
//   + AND (<other criteria) otherwise
// - other criteria:
//   + AND op. always wrapped by brackets to ensure precedence over OR op.
//   + last criterion terninated properly with closing brackets
//
// Passed:
// - s_me: before (potential) rectification...
// - meCountMax: MAX number of criteria (>= 1)
// - *a_meCount: number of criteria (>= 1)
// - criterion:
// - b_lastCriterion:
//
// Changed:
// - *s_me: possibly rectifed
// - *a_meCount: number of criteria (>= 1)
//
// Ret:
// - COMPLETED__OK:
// - COMPLETED__BUT: (only possible when last criterion) rectification was necessary
int GRequestCriteriaAdd(struct G_REQUEST_CRITERION *s_me, int meCountMax, int *a_meCount,
  struct G_REQUEST_CRITERION criterion, char b_lastCriterion) ;



#define b_ASCENDING b_FALSE0
#define b_DESCENDING b_TRUE

// (re-)set index iterator sequence according to the 1st criterion 
//
// Passed:
// - handle: 
// - ap_1stGRequestCriterion: "rectified" criteria (see GRequestCriteriaAdd() above)
// - b_descending: indicating "direction" of "soft reset" 
//
// Changed:
// - *indexSequenceBuffer: sequence is (re-)set ; call l_GIndexesSequenceNext() to
//   (re-)play the sequence. 
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int l_GIndexesSequenceReset(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *ap_1stGRequestCriterion, char b_descending,
  char *indexSequenceBuffer) ; 

// Update index iterator sequence: NEXT.
//
// Passed:
// - handle: 
// - sp_gRequestCriteria: "rectified" criteria op. flags (see GRequestCriteriaAdd() above)
// - gRequestCriterionCount
// - b_descending:
// - *indexSequenceBuffer : iterator sequence "current"
//
// Changed:
// - *indexSequenceBuffer : iterator sequence "next"
// - *an_entry: item entry to retrieve : 
//   + -1 special value : not found, that is "disabled" or "no more"
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int l_GIndexesSequenceNext(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, int gRequestCriterionCount,
  char b_descending, char *indexSequenceBuffer, int *an_entry);


// Get current entry in index sequence.
//
// Passed:
// - handle:
// - sp_gRequestCriteria: "rectified" criteria op. flags (see GRequestCriteriaAdd() above)
// - *p_indexSequenceBuffer:
//
// Changed:
// - *an_entry: 
//   + -1 special value : not found
//   + >= 0: corresponding entry
//
// Ret:
// - RETURNED: Ok
// - -1: anomaly is raised
int l_GIndexesSequenceCurrent(G_INDEXES_HANDLE handle,
  const struct G_REQUEST_CRITERION *sp_gRequestCriteria, const char *p_indexSequenceBuffer,
  int *an_entry);



#endif // __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 0

// Manage double inclusion: update inclusion state:
#if __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 0
#undef __C_ANSI_G_SEQUENCE_H_INCLUDED__
// Manage double inclusion: provoke 2nd inclusion:
#define __C_ANSI_G_SEQUENCE_H_INCLUDED__ 2
#include "c-ansi/g-sequence.h"
#else
#undef __C_ANSI_G_SEQUENCE_H_INCLUDED__
#define __C_ANSI_G_SEQUENCE_H_INCLUDED__ 3
#endif

#endif // __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 0 || __C_ANSI_G_SEQUENCE_H_INCLUDED__ == 2

