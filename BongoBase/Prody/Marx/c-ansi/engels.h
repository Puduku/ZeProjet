// engels.h, version 1.92 (ANSI)
// (c) Atos-Euronext Belgium - 2008
// (c) Puduku - 2025

#ifndef __C_ANSI_ENGELS_H_INCLUDED__
#define __C_ANSI_ENGELS_H_INCLUDED__

/////////////////////////////
#include "c-ansi/engels.topo"
/////////////////////////////

#include "c-ansi/g-string.h"



// 1. Engels (manage pamphlet models)
// ----------------------------------

struct ENGELS;
typedef struct ENGELS *ENGELS_HANDLE;

// Create Engels instance
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
int EngelsCreateInstance(ENGELS_HANDLE *azh_handle);

// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int EngelsDestroyInstance(ENGELS_HANDLE xh_handle) ;

// Pamphlets (aka "super blot tables"...)
// --------------------------------------

struct PAMPHLET ; // Private!
typedef struct PAMPHLET* PAMPHLET_HANDLE;

// Create new pamphlet examplar
//
// Passed:
// - *azh_handle: "un-initialized" handle
// - f_engelsHandle: pamphlet models handler  
//
// Changed:
// - *azh_handle: initilized instance handle
//
// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
int PamphletCreateInstance(PAMPHLET_HANDLE *azh_handle, ENGELS_HANDLE f_engelsHandle) ;


// Ret:
// - RETURNED: Ok
// - -1 special value: anomaly raised
// NAMED_OBJECT_DESTROY_INSTANCE_FUNCTION
int PamphletDestroyInstance(void *xhr_handle) ;
// int PamphletDestroyInstance(PAMPHLET_HANDLE xh_handle) ;


#endif // __C_ANSI_ENGELS_H_INCLUDED__
