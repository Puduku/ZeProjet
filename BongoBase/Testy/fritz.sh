#!/bin/bash
#Testy/fritz.sh
. mama.env

#################################
plan="Running Testy executables "
#################################

specificUsage='[ <special options to Testy executables> ]'

ParseSpecificArgs () {
  testOptions=("$@")
}

SpecificInstructionsOutline () {
  _n=${#testOptions[@]}
  if [ $_n -eq 0 ] ;then 
    echo "<None>"
  else
    echo "Pass [${testOptions[@]}] option(s) to Testy executable"
  fi
}

. katzenjammer.env
. der-captain.env

#### Execute plan...

CommandPrepareSandbox

Step "Run tests" 

CommandCd $(SandboxLocation)
datadirLocation=$(Location "${garden}/data")
for _test in ../c-ansi-*-test ../c-posix-*-test ;do
  if IsTarget "$_test" ;then
    EvalCommand $(Adolf "$_test") $(Adolf "${testOptions[@]}") SB $(Adolf "$datadirLocation")
  fi
done

Success
