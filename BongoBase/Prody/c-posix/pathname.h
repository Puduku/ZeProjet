// c-posix/pathname.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003, 2004
//
// Utilities to handle (files and directories) pathnames

#ifndef __C_POSIX_PATHNAME_H_INCLUDED
#define __C_POSIX_PATHNAME_H_INCLUDED

#include <stdlib.h>

#include "c-ansi/types.h"
#include "c-ansi/g-string.h"

// Declare and create g-string holding file pathname, constructed with
// directory path and file name.
//  
//
// Passed:
// -mzh_filePathnameStuff: the variable to declare and initialize
// -p_dirPath:
// -p_filename:
//
// Changed:
// -mzh_filePathnameStuff: initialized
#define m_CREATE_FILE_PATHNAME_G_STRING(/*G_STRING_STUFF*/ mzh_filePathnameStuff,\
                                        /*const char* */ p_dirPath,\
                                        /*const char* */ p_filename)\
  G_STRING_STUFF mzh_filePathnameStuff = (G_STRING_STUFF)UNDEFINED;\
  G_STRING_CREATE_INSTANCE(&mzh_filePathnameStuff);\
  GStringPrintf(mzh_filePathnameStuff,0,"%s/%s", p_dirPath, p_filename);

#endif // __C_POSIX_PATHNAME_H_INCLUDED
