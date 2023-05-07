#!/bin/bash
 
if [ -f ~/.flintrc ] ;then
  source ~/.flintrc
fi
if [ -z "$FLINT" ] ;then
  echo "FLINT must be first properly installed." 
  exit 100 
fi
if [ -z "$MAMA" ] ;then
  echo "MAMA must be first properly installed." 
  exit 100 
fi
 
typeset -r PROJVAR=BONGO
echo "$FLINT/install.env" 
. "$FLINT/install.env" 
 
Success
