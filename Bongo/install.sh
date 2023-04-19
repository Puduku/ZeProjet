#!/bin/bash
 
trap 'echo ABORTED AT LINE=$LINENO ; exit -1' ERR 

Help () {
  echo "$@"
  echo "Operation is cancelled ; please check your parameters." 
  echo "USAGE: $0 ( <'working' Bongo position (directory)> | --back )"
  exit 100
}

if [ -z "$1" ]; then
  Help "Missing 'working' Bongo position parameter."
fi

bDir="$(dirname "$0")"

if [ ! -f ~/.flintrc ] ;then
  Help "Flint is not installed."
fi
if [ -z "$FLINT" -o -z "$MAMA" -o -z "$BONGO" ] ;then
  . ~/.flintrc
fi
if [ -z "$FLINT" -o ! -d "$FLINT" ] ;then
  Help "Flint is not properly installed."
fi
if [ -z "$MAMA" -o ! -d "$MAMA" ] ;then
  Help "Mama is not properly installed."
fi


if [ "$1" = "--back" ]; then 
  if [ -z "$BONGO" ] ;then 
    Help "'working' Bongo is not installed."
  fi  
  if [ ! -d "$BONGO" ] ;then
    Help "'working' Bongo is not properly installed."
  fi  
  cd "$bDir"
  bongoInstallPosition="$PWD"
  cd "$BONGO"
  echo -n "Put back $BONGO modifications in $bongoInstallPosition..."
  find . -name "*.sh" -exec cp {} "$bongoInstallPosition/{}" \; 
  find . -name "*.[ch]" -exec cp {} "$bongoInstallPosition/{}" \; 
  find . -name "*.topo" -exec cp {} "$bongoInstallPosition/{}" \; 
  find . -name "*.TXT" -exec cp {} "$bongoInstallPosition/{}" \; 
  echo "OK."
  exit 0
fi

posArg="$1"
 
if [ -n "$BONGO" ] ;then 
  Help "'working' Bongo  is already installed."
fi

if [ -e "$posArg" ] ;then 
  Help "$posArg (file or) directory already exists and cannot be used as 'working' Bongo  position."
fi

mkdir -p "$posArg"
cd "$posArg"
bongoPosition=$PWD
cd - > /dev/null
cd "$bDir"

echo "Installing 'working' Bongo"

echo -n "Copying files..."
cp -r * "$bongoPosition" 
rm "$bongoPosition/install.sh"
rm "$bongoPosition/README-INSTALL.TXT"
echo "OK." 

echo -n "Update ~/.flintrc Flint configuration file..."
echo "export BONGO=\"$bongoPosition\"" >> ~/.flintrc
echo "OK." 

echo -n "Verification..."
. ~/.flintrc
if [ -z "$BONGO" ] ;then
  echo "'working' Bongo installation FAILED." 
  exit -1
fi
echo "OK." 

echo "'working' Bongo installation is successful." 
exit 0
