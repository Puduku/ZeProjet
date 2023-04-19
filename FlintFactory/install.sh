#!/bin/bash

trap 'echo ABORTED AT LINE=$LINENO ; exit -1' ERR
 
Help () {
  echo "$@"
  echo "Operation is cancelled ; please check your parameters." 
  echo "USAGE: $0 ( <Flint position (directory)> | --back )"
  exit 100 
}

if [ -z "$1" ]; then
  Help "Missing Flint position parameter."
fi

typeset -r FLINT_LIST="crash-freeloader.h flags.h tid-kitchen.h types-kitchen.h crash.h flint.topo tid.h types.h" 
typeset -r ROOT_LIST="FLINT.TXT GENESEE.TXT" 

bDir="$(dirname "$0")"

if [ "$1" = "--back" ]; then
  if [ ! -f ~/.flintrc ] ;then 
    Help "Flint environment is not installed."
  fi
  if [ -z "$FLINT" ] ;then 
    . ~/.flintrc
  fi
  if [ -z "$FLINT" -o ! -d "$FLINT" ] ;then
    Help "Flint is not properly installed."
  fi
  cd "$bDir"
  flintInstallPosition="$PWD"
  cd "$FLINT"
  echo -n "Put back $FLINT modifications in $flintInstallPosition..."
  cp $ROOT_LIST "$flintInstallPosition"
  cd flint
  cp $FLINT_LIST "$flintInstallPosition/flint"
  echo "OK."
  exit 0
fi

posArg="$1"

if [ -f ~/.flintrc ] ;then 
  Help "Flint environment is already installed."
fi

if [ -e "$posArg" ] ;then 
  Help "$posArg file or directory already exist and cannot be used as Flint position."
fi
mkdir -p "$posArg"
if [ $? -ne 0 ] ;then
  Help "$posArg cannot be used as Flint position."
fi

cd "$posArg"
flintPosition="$PWD"
cd - > /dev/null
cd "$bDir"

echo -n "Creating $flintPosition content..."
mkdir -p "$flintPosition/flint"
cd flint
cp $FLINT_LIST "$flintPosition/flint"
cd ../
cp $ROOT_LIST "$flintPosition"
echo OK.

echo -n "Creating ~/.flintrc configuration file..."
# Update configuration file
echo "export FLINT=\"$flintPosition\"" >> ~/.flintrc
echo OK.

echo -n "Verification..."
. ~/.flintrc
if [ -z "$FLINT" ] ;then 
  echo "Flint installation failed!" >> /dev/stderr
  exit -1
fi
echo OK.

echo "Success! To complete the installation:
- either remember to source ~/.flintrc file each time you log in;
- or add '. ~/.flintrc' line in your favorite login configuration file (e.g ~/.profile file) "
exit 0
