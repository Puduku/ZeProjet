#!/bin/bash
#Prody/hans.sh
. mama.env

##########################
plot="Building PRODUCTION"
##########################
cleaning=s

. katzenjammer.env
. lena.env

#### Creek functions:

CommandCompilePrody () {
  local _localGarden="$(Location "$garden")"
  CC_INCS=(./)     

  local _lib=$1
  if [ $_lib = c-ansi ] ;then
    CC_DEPS=(c-ansi)
  else
    CC_DEPS=(c-ansi c-posix)
  fi
  li="$_localGarden/lib$_lib.a"
  if IsTarget "$li" ;then
    Step "Build production library : $li"
    for src in $_lib/*.c ;do
      obj=$_localGarden/$(ObjectFlatName "$src" "")
      CommandCC "$src" "$obj" "$li"
    done
  fi
}

######### Register or execute creek functions #######
CreekFunctions CommandCompilePrody 

######### Execute full plot #######
CommandClean

MainStage "(Re)compile c-ansi and c-posix production objects."

CommandCreek CommandCompilePrody c-ansi 
CommandCreek CommandCompilePrody c-posix 

EndMainStage


Success
