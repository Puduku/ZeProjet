// diggy/vars-fields.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// (c) Puduku - 2023

#ifndef __DIGGY_VARS_FIELDS_H_INCLUDED
#define __DIGGY_VARS_FIELDS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "flint/types.h"
#include "diggy/diggy-kitchen.h"
#include "diggy/vars.h"


// Aim: Plain macros for displaying struct variables (fields) with "diggy" system...


// #REF DIGGY_PRINT_FIELD_IMAGE_MACRO
// DIGGY_PRINT_IMAGE_ACTION macro for struct/union "vars"... 

// Usefull for designing DIGGY_PRINT_FIELD_IMAGE_MACRO macros..
// Print field name (and useful 'field separator'...)
// Passed:
// m_field: 
#define m_DIGGY_PRINT_FIELD_NAME(m_field)  fputs(" " #m_field ":",stdout);\


// 1. Digging in "general" fields

// #SEE DIGGY_PRINT_FIELD_IMAGE_MACRO
// Print struct/enum "general" field's content... 
//
// Passed:
// - m_var: struct or union
// - m_field: "general" type field
// - m_specifier: "printf() 's family function specifier : d for int; s for string, etc.
#define m_DIGGY_PRINT_FIELD_GEN(m_var, m_field, m_specifier) {\
  m_DIGGY_PRINT_FIELD_NAME(m_field)\
  printf("%" #m_specifier, (m_var).m_field); \
}

// #SEE DIGGY_PRINT_FIELD_IMAGE_MACRO
// Wraps m_DIGGY_PRINT_FIELD_GEN() above.
// Print struct/enum "int type" field's content... 
//
// Passed:
// - m_var: struct or union
// - m_field: "int" type field
#define m_DIGGY_PRINT_FIELD_D(m_var, m_field)  m_DIGGY_PRINT_FIELD_GEN(m_var,m_field,d)


// 2. Digging in "C-string" fields

// #SEE DIGGY_PRINT_FIELD_IMAGE_MACRO
// Print struct/enum field's c-string type... 
//
// Passed:
// - m_var: struct or union
// - m_field: ('\0'-terminated) C-string type field
#define m_DIGGY_PRINT_FIELD_STRING(m_var, m_field) {\
  m_DIGGY_PRINT_FIELD_NAME(m_field)\
  DiggyPrintStringImage((m_var).m_field); \
}


// 3. Digging in generic "DATA" fields

// #SEE DIGGY_PRINT_FIELD_IMAGE_MACRO
// Print struct/enum field's data type... 
//
// Passed:
// - m_var: struct or union
// - m_field: "byte pointer" type field 
// - length: data length (in bytes)
#define m_DIGGY_PRINT_FIELD_DATA(m_var, m_field, length) {\
  m_DIGGY_PRINT_FIELD_NAME(m_field)\
  DiggyPrintDataImage((m_var).m_field,length); \
}

// 4. Digging in "value" (aka. "enum") fields

// #SEE DIGGY_PRINT_FIELD_IMAGE_MACRO
// Print struct/enum field's (enum) value... 
//
// Passed:
// - m_var: struct or union
// - m_field: "value" (typically "enum"...) type field
// - m_valueImageInlineFunction: "inline" function for building value's image
//   #SEE Enum-type-image-function @ flint/types.h
#define m_DIGGY_PRINT_FIELD_VALUE(m_var, m_field, m_valueImageInlineFunction) {\
  const char *emp_image = m_valueImageInlineFunction((m_var).m_field);\
  m_DIGGY_PRINT_FIELD_NAME(m_field)\
  DiggyPrintValueImage((m_var).m_field,emp_image); \
}


#endif // __DIGGY_VARS_FIELDS_H_INCLUDED
