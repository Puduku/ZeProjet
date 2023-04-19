#!/bin/bash
 
trap 'echo ABORTED AT LINE=$LINENO ; exit -1' ERR

Help () {
  echo "$@"
  echo "Operation is cancelled ; please check your parameters." 
  echo "USAGE: $0 ( <Mama position (directory)> | --back )"
  exit 100 
}

if [ -z "$1" ]; then
  Help "Missing Mama position parameter."
fi

bDir="$(dirname "$0")"

typeset -r ROOT_LIST="MAMA.TXT fritz.sh.mod hans.sh.mod der-inspector.sh.mod"
typeset -r BIN_LIST="adolf.env der-captain.sh dummy.sh mama.env bye.env der-inspector.env katzenjammer.env \
mama.sh der-captain.env dirks.env lena.env miss-ross.sh"

if [ ! -f ~/.flintrc ] ;then
  Help "Flint is not installed."
fi
if [ -z "$FLINT" -o -z "$MAMA" ] ;then
  . ~/.flintrc
fi
if [ -z "$FLINT" -o ! -d "$FLINT" ] ;then
  Help "Flint is not properly installed."
fi

if [ "$1" = "--back" ]; then 
  if [ -z "$MAMA" ] ;then 
    Help "Mama is not installed."
  fi
  if [ ! -d "$MAMA" ] ;then
    Help "Mama is not properly installed."
  fi  
  cd "$bDir"
  mamaInstallPosition="$PWD"
  cd "$MAMA"
  echo -n "Put back $MAMA modifications in $mamaInstallPosition..."
  cp $ROOT_LIST "$mamaInstallPosition" 
  cd bin/
  cp $BIN_LIST "$mamaInstallPosition/bin"
  echo "OK."
  exit 0
fi

if [ -n "$MAMA" ] ;then 
  Help "Mama is already installed."
fi

posArg="$1"
if [ -e "$posArg" ] ;then 
  Help "$posArg (file or) directory already exists and cannot be used as Mama position."
fi

mkdir -p "$posArg"
cd "$posArg"
mamaPosition=$PWD
cd - > /dev/null
cd "$bDir"

logFile="$mamaPosition/install.log"

Underline () {
  local _len=${#1}
  local _i=0
  while [ "$_i" -lt "$_len" ] ;do
    _line=${_line}=
    let "_i++"
  done
  echo -n "$_line"
}
 
MLog () {
  echo "$@"
  echo "$@ - $(date)" >> "$logFile"
  echo "$(Underline "$@")" >> "$logFile"
}
 
SLog () {
  echo "$@"
  echo "$@" >> "$logFile"
}

Confirm () {
SLog "$@"
 echo "Type "OK" to confirm."
while [ "$REPLY" -ne "OK" ] ;do
 read
done
}

if [ -z "$1" ]; then
  Help "Missing Flint position parameter."
fi

MLog "Installing Mama environment"

mkdir -p "$mamaPosition/bin"
 
Concon () {
  SLog "Install $1 utility..."
  gcc -D_GNU_SOURCE -I$FLINT src/$1.c -o "$mamaPosition/bin/$1"
  if [ $? -ne 0 ] ;then
    echo "Failed to compile $1 ; please check!" > /dev/null
    SLog "Failed to compile $1 ; installation is ABORTED!"
    exit -1
  fi
  SLog "OK."
}

Concon coco
Concon loquet
Concon progress
if realpath 2> /dev/null ;then
  SLog "realpath command is directly available on platform."
else
  SLog "realpath command is not available on platform."
  Concon realpath
fi 

cp $ROOT_LIST "$mamaPosition" 
cd bin/
cp $BIN_LIST "$mamaPosition/bin"
cd ..
 
SLog "Update ~/.flintrc Flint configuration file..."
echo "export MAMA=\"$mamaPosition\"" >> ~/.flintrc
echo "export PATH=\"\$PATH:\$MAMA/bin\"" >> ~/.flintrc
SLog "OK." 

SLog "Verification..."
. ~/.flintrc
miss-ross.sh
if [ $? -ne 0 ] ;then
  MLog "Mama installation FAILED." 
  exit -1
fi
SLog "OK." 

MLog "Mama installation is successful." 
exit 0
