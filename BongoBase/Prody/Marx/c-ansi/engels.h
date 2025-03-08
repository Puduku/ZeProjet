// engels.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#ifndef __C_ANSI_ENGELS_H_INCLUDED__
#define __C_ANSI_ENGELS_H_INCLUDED__

/////////////////////////////
#include "c-ansi/engels.topo"
/////////////////////////////

#include "c-ansi/blotcode.h"
#include "c-ansi/g-string.h"


// Public function: see .h
int EngelsCreateInstance() ;



// PamphletExamplars: 

struct PAMPHLET_EXAMPLAR ; // Private!
typedef struct PAMPHLET_EXAMPLAR* PAMPHLET_EXAMPLAR_HANDLE;



// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletExamplarCreateInstance(PAMPHLET_EXAMPLAR_HANDLE *azh_handle, int dummy) ;


// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletExamplarDestroyInstance(void *xhr_handle) ;


#endif // __C_ANSI_ENGELS_H_INCLUDED__
