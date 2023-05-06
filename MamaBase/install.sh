#!/bin/bash

if [ -f ~/.flintrc ] ;then
  source ~/.flintrc
fi
if [ -z "$FLINT" ] ;then
  echo "FLINT must be first properly installed." 
  exit 100
fi
 
typeset -r PROJVAR=MAMA
. "$FLINT/install.env" 

#PRE: in base position
Concon () {
  echo -n "Install $1 utility..."
  gcc -D_GNU_SOURCE -I$FLINT src/$1.c -o "$PROJVAL/bin/$1"
  echo "OK."
}

cd "$basePosition"
Concon coco
Concon loquet
Concon progress
if realpath 2> /dev/null ;then
  echo "realpath command is directly available on platform."
else
  echo "realpath command is not available on platform (must be recompiled)."
  Concon realpath
fi 

echo -n "Update ~/.flintrc configuration file..."
if [ "${MAMA:?}" != "${PROJVAL:?}" ] ;then
  echo "FAILED: [${MAMA}] != [${PROJVAL}]" > /dev/stderr
  exit -1
fi
echo "export PATH=\"\$PATH:\$MAMA/bin\"" >> ~/.flintrc
echo "OK." 

echo -n "Verification..."
. ~/.flintrc
miss-ross.sh
if [ $? -ne 0 ] ;then
  echo "Mama installation FAILED."  > /dev/stderr
  exit -1
fi
echo "OK." 

Success
