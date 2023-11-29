#!/bin/bash
#Testy/hans.sh
. mama.esh

#################################
plot="Building Testy executables"
#################################
cleaning=s

. katzenjammer.esh
. lena.esh

#### "Global" variables:

typeset -r prodyGarden="$(Garden "Prody")"
typeset -r diggyGarden="${island:?}/Diggy"
typeset -r diggyGardenLocation=$(Location "$diggyGarden")
typeset -r prodyGardenLocation=$(Location "$prodyGarden")
typeset -r testyLib="$garden/libtesty.a"
typeset -r testyLibLocation="$(Location "$testyLib")"
typeset -r cAnsiTestSources=(c-ansi/*-test.c)
eval typeset -r cAnsiTestExes=($(Adolf_F ExecutableFlatName "${cAnsiTestSources[@]}"))
typeset -r cPosixTestSources=(c-posix/*-test.c)
eval typeset -r cPosixTestExes=($(Adolf_F ExecutableFlatName "${cPosixTestSources[@]}"))


#### Creek functions: 

CommandCompileTestyUtils () {
  CC_INCS=(./ "$diggyGardenLocation")
  CC_DEPS=("$diggyGardenLocation/diggy/")
  CC_OPTS=()
  local _src
  for _src in testy/*.c ;do
    local _obj=$(ObjectFlatName "$_src" "")
    CommandCC "$_src" "$_obj" "$testyLibLocation"
  done
}

CommandCompileDiggyPrody () {
  local _localGarden="$(Location "$garden")"
  CommandVisitGarden "$prodyGarden" s
  CC_INCS=(./ "$(Location "$diggyGarden")" "$_localGarden")     
  CC_OPTS=(-DDIGGY)
  local _lib="$1"
  if [ $_lib = c-ansi ] ;then
    CC_DEPS=(c-ansi)
  else
    CC_DEPS=(c-ansi c-posix)
  fi
  local _lili="$_localGarden/lib${_lib}-diggy.a"
  local _src
  for _src in $_lib/*.c ;do
    local _obj=$_localGarden/$(ObjectFlatName "$_src" "-diggy")
    CommandCC "$_src" "$_obj" "$_lili"
  done
  CommandQuitGarden 
}

# Retrieve the source production module corresponding to test source.   
# $1: test source pathname (c-ansi/<NAME>-test.c or c-posix/<NAME>-test.c)  
# Out: corresponding module source  (c-ansi/<NAME>.c or c-posix/<NAME>.c)
OrigSourcePath() {
  if [ "$1" = "c-ansi/g-string-test.c" ] ;then
    echo -n "c-ansi/g-string.c"
  else
    echo -n "${1/-test/}"
  fi
}

CommandCompileTests () {
  CC_INCS=(./ "$prodyGardenLocation" "$diggyGardenLocation")
  eval CC_DEPS=($(Adolf_P "$diggyGardenLocation/diggy/" ''))
  CC_OPTS=(-DDIGGY)
  LD_OPTS=()
  LD_OPT2S=(-lpthread)
  eval LD_LIBS=($(Adolf "$testyLibLocation" "$diggyGardenLocation/libdiggy.a") $(Adolf_P "$prodyGardenLocation/" libc-posix.a libc-ansi.a)) 
  typeset -i _i=0 
  _cat="$1"
  if [ "$_cat" = "c-ansi" ] ;then
    typeset -i _n=${#cAnsiTestSources[@]}
  else
    typeset -i _n=${#cPosixTestSources[@]}
  fi   
  while [ $_i -lt $_n ] ;do 
    if [ "$_cat" = "c-ansi" ] ;then
      _ts="${cAnsiTestSources[$_i]}"
      _te="${cAnsiTestExes[$_i]}"
    else
      _ts="${cPosixTestSources[$_i]}"
      _te="${cPosixTestExes[$_i]}"
    fi   
    if IsTarget "$_te" ;then
      _to=$(ObjectFlatName "$_ts")
      CommandCC "$_ts" "$_to"
      _os=$(OrigSourcePath "$_ts") 
      eval dobjs=($(Adolf_F2 ObjectFlatName -diggy "$_os"))
      #if [ "$_te" = "c-ansi-att-test" ] ;then
      #  dobjs=("${dobjs[@]}" c-ansi-green-diggy.o)
      #fi
      if [ "$_te" = "c-ansi-g-string-test" ] ;then
        dobjs=("${dobjs[@]}" c-ansi-p-string-diggy.o)
      fi
      if [ "$_te" = "c-ansi-blotcode-test" ] ;then
        #dobjs=("${dobjs[@]}" c-posix-sucker-diggy.o)
        #dobjs=("${dobjs[@]}" c-ansi-green-diggy.o c-ansi-g-string-diggy.o)
        echo "BIDON"
      fi
      #eval LD_OBJS=($(Adolf "$_to") $(Adolf_P "$diggyGardenLocation/" "${dobjs[@]}")) 
      eval LD_OBJS=($(Adolf "$_to") $(Adolf "${dobjs[@]}")) 
      CommandLd "$_te"
    fi
    let "_i++"
  done
}

CreekFunctions CommandCompileDiggyPrody CommandCompileTestyUtils CommandCompileTests 

#### Execute plot... 

CommandClean "${cAnsiTestExes[@]}" "${cPosixTestExes[@]}"

MainStage "1. (Re)compile prody, diggy and testy objects and libraries" 

CommandKatzenjammer  "$prodyGarden"
CommandKatzenjammer  "$diggyGarden"
CommandCreek CommandCompileTestyUtils
CommandCreek CommandCompileDiggyPrody c-ansi
CommandCreek CommandCompileDiggyPrody c-posix

EndMainStage

MainStage "2. Build testy executables"

CommandCreek CommandCompileTests c-ansi
CommandCreek CommandCompileTests c-posix

EndMainStage

Success
