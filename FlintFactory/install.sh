#!/bin/bash
 
Help () {
  echo "$@"
  echo "USAGE: $0 <Flint position (directory)>
Ret status:
 - 0: OK - installation is successful
 - 100: please check your parameters - installation not done
 - other value: unexpected problem - Flint is not properly installed"
  exit 100 
}

if [ -z "$1" ]; then
  Help "Missing Flint position parameter."
fi
bDir="$(dirname "$0")"
posArg="$1"


if [ -f ~/.flintrc ] ;then 
  Help "Flint environment is already installed."
fi

if [ -e "$posArg" ] ;then 
  Help "$posArg file or directory already exist and cannot be used as Flint position."
fi

mkdir -p "$posArg"
cd "$posArg"
flintPosition=$PWD
cd - > /dev/null
cd "$bDir"

echo -n "Creating $flintPosition content..."
mkdir -p "$flintPosition/flint"
cd flint
cp crash-freeloader.h flags.h tid-kitchen.h types-kitchen.h crash.h flint.topo tid.h types.h "$flintPosition/flint"
cd ../
cp FLINT.TXT GENESEE.TXT "$flintPosition"
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
