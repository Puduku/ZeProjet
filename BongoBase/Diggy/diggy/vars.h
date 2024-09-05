// diggy/vars.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __DIGGY_VARS_H_INCLUDED
#define __DIGGY_VARS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "flint/types.h"
#include "diggy/diggy-kitchen.h"


// Aim: Plain macros for displaying variables (name=content) with "diggy" system...


// I. Base m_DIGGY_VAR() macro
// ---------------------------

// #REF DIGGY_VAR_XXX_MACRO
// Notation:
// m_DIGGY_VAR_<XXX>()      => display of 'XXX' type variable in DIGGY system...

// #REF DIGGY_PRINT_IMAGE_ACTION
// Actions to output (directly in stdout) some variable content IMAGE
// NOTE: IMAGE means DISPLAYABLE (basic visible ASCII characters) content...
// NOTE2: those functions are meant to be called within a "diggy session";  
// and are therefore protected against CRASHING...

// #REF DIGGY_PRINT_IMAGE_FUNCTION
// Simple function allowing to build DIGGY_PRINT_IMAGE_ACTION

// Display variable name and content using "ad hoc" image display action.
// Allows to build  different m_DIGGY_VAR_<XXX> macros...
//
// Passed
// - m_var: <varType> var expression to display
// - m_printImageAction: #see DIGGY_PRINT_IMAGE_ACTION
#define m_DIGGY_VAR(m_var, m_printImageAction) {\
  DiggyBrickStart((void *)&diggyOriginVar,__func__,__LINE__,__FILE__); \
  fputs(#m_var " = ",stdout);\
  m_DIGGY_BRICK_CRASHABLE_PRINT(m_printImageAction) \
  DiggyBrickEnd();\
}


// II. Usable m_DIGGY_VAR_<XXX>() macros 
// ------------------------------------- 

// 1. Digging in "general" variables


// #SEE DIGGY_VAR_XXX_MACRO
// "General" var display
//
// Passed
// - m_var: common var expression to display
// - m_specifier: "alla printf" type specifier (d for int, etc.)
//   NOTE: you are "responsible" to display "printable" content
#define m_DIGGY_VAR_GEN(m_var,m_specifier) \
  m_DIGGY_VAR(m_var,printf("%" #m_specifier, m_var);)

// #SEE DIGGY_VAR_XXX_MACRO
// Simple integer var display
// Wraps m_DIGGY_VAR_GEN above
//
// Passed
// - m_var: "int" type var expression to display
#define m_DIGGY_VAR_D(m_var)  m_DIGGY_VAR_GEN(m_var,d)

// #SEE DIGGY_VAR_XXX_MACRO
// Simple pointer var display
// Wraps m_DIGGY_VAR_GEN above
//
// Passed
// - m_var: "pointer" type var expression to display
#define m_DIGGY_VAR_P(m_var)  m_DIGGY_VAR_GEN(m_var,p)


// 2. Digging in "C-string" vars

// #SEE DIGGY_PRINT_IMAGE_FUNCTION
//
// Passed:
// -np_string
void DiggyPrintStringImage(const char *np_string);


// #SEE DIGGY_VAR_XXX_MACRO
// Simple "C-string" var display
//
// Passed
// - m_var: common var expression to display
#define m_DIGGY_VAR_STRING(m_var) \
  m_DIGGY_VAR(m_var,DiggyPrintStringImage(m_var);)


// 3. Digging in generic "DATA" vars

// #SEE DIGGY_PRINT_IMAGE_FUNCTION
//
// Passed:
// - length: data length
// - p_data: data start pointer
void DiggyPrintDataImage(const char *p_data, int length);


// #SEE DIGGY_VAR_XXX_MACRO
// Simple "data" var display
//
// Passed
// - m_var: common var expression to display
// - length: length (i.e number of bytes) of data to display
#define m_DIGGY_VAR_DATA(m_var,length) \
  m_DIGGY_VAR(m_var, DiggyPrintDataImage(m_var,length);)


// 4. Digging in "value" (e.g "enum") variables


// #SEE DIGGY_PRINT_IMAGE_FUNCTION
//
// Passed:
// - value 
// - p_image 
void DiggyPrintValueImage(int value, const char *p_image);


// #SEE DIGGY_VAR_XXX_MACRO
// Display variable name and content using ad hoc image display function.
//
// Passed
// - m_var:
// - m_valueImageInlineFunction: #SEE Enum-type-image-function @ flint/types.h
#define m_DIGGY_VAR_VALUE(m_var, m_valueImageInlineFunction) {\
  const char *emp_image = m_valueImageInlineFunction(m_var);\
  m_DIGGY_VAR(m_var, DiggyPrintValueImage(m_var,emp_image);) \
}

#endif // __DIGGY_VARS_H_INCLUDED
