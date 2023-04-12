#!/bin/bash
 
Help () {
  echo "$@"
  echo "USAGE: $0 <'working' Bongo position (directory)>
Ret status:
 - 0: OK - post installation is successful
 - 100: please check your parameters - post installation not done
 - other value: unexpected problem - 'working' Bongo is not properly installed"
  exit 100
}

if [ -z "$1" ]; then
  Help "Missing 'working' Bongo position parameter."
fi
bDir="$(dirname "$0")"
posArg="$1"
 
if [ ! -f ~/.flintrc ] ;then
  Help "Flint is not installed."
fi
if [ -z "$FLINT" -o ! -d "$FLINT" ] ;then
  Help "Flint is not properly installed."
fi

if [ -z "$MAMA" -o ! -d "$MAMA" ] ;then 
  Help "Mame is not properly installed."
fi

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

cp -r * "$bongoPosition" 
rm "$bongoPosition/install.sh"
rm "$bongoPosition/README-INSTALL.TXT"

echo "Update ~/.flintrc Flint configuration file..."
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
