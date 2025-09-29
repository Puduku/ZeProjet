// engels.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#ifndef __C_ANSI_ENGELS_H_INCLUDED__
#define __C_ANSI_ENGELS_H_INCLUDED__

/////////////////////////////
#include "c-ansi/engels.topo"
/////////////////////////////

#include "c-ansi/g-string.h"


// PamphletExamplars: 

struct PAMPHLET_EXAMPLAR ; // Private!
typedef struct PAMPHLET_EXAMPLAR* PAMPHLET_EXAMPLAR_HANDLE;

// Create new pamphlet examplar
//
// Passed:
// - *azh_handle: "un-initialized" handle
//
// Changed:
// - *azh_handle: initilized instance handle
//
// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletExamplarCreateInstance(PAMPHLET_EXAMPLAR_HANDLE *azh_handle, int dummy) ;


// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletExamplarDestroyInstance(PAMPHLET_EXAMPLAR_HANDLE xh_handle) ;


#endif // __C_ANSI_ENGELS_H_INCLUDED__
