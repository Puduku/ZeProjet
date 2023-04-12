// diggy/getopts.h, version 1.93 (ANSI)
// (c) Atos-Euronext Belgium - 2001, 2002, 2003
// Aim: parsing program option (aka position parameters) for dg flags

#ifndef __DIGGY_GETOPTS_H_INCLUDED
#define __DIGGY_GETOPTS_H_INCLUDED

#include "diggy/diggy-kitchen.h"
#include "flint/flags.h"


// Parse program arguments and update DG flags accordingly...
// Note: only executive args are parsed - non executive flags like -h help 
// request are not handled by the function
//
// Passed:
// - argc:
// - p_argv:
// - *a_dgFlags: default flags
// - optind: (global variable) args position (1 for 1st position parameter)
//
// Modified:
// - optind: (global variable) args position after parsing (== argc => all args parsed)
// - *a_dgFlags: only useful if DIGGY program execution is confirmed (ret 0)
//               #SEE dg-flags @ diggy/diggy-wings.h
// - info256: 
//   + DIGGY arguments parsed => recap of options
//   + DIGGY bad option(s) argument(s) => explanation 
//
// Returned:
// - 0 : OK, executive DG flags updated 
// - 100 : bad option(s) argument(s) 
int ParseDgFlags (int argc, char **p_argv, unsigned int *a_dgFlags, char *info256) ;

#define DG_FLAGS_EXECUTIVE_ARGS_SUMMARY \
"[ -d (a|a+|p) ] [ -T ] [ -p ] [ -t ] [ -s (s|l) ]"

#define DG_FLAGS_EXECUTIVE_ARGS_DETAILS \
" -d a  => functions calls display adapted to monothread process (condensed form)\n" \
" -d a+ => functions calls display adapted to monothread process (aerial form)\n" \
" -d p  => functions calls display adapted to multithreaded process\n" \
" -T    => display time without date in lines headers\n" \
" -p    => display pid in lines headers\n" \
" -t    => display tid in lines headers\n" \
" -s s  => display truncated stack address\n" \
" -s l  => display full stack address\n"


#endif // __DIGGY_GETOPTS_H_INCLUDED
