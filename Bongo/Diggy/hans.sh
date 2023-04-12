#!/bin/bash
#Diggy/hans.sh
. mama.env

#############################
plan="Building DIGGY objects"
#############################
cleaning=s

. katzenjammer.env
. lena.env


CreekFunctions

######### Execute plan #######
CommandClean

Stage "Compile diggy sources files"
CC_DEPS=(diggy)
CC_INCS=(./ )
CC_OPTS=()
for src in diggy/*.c ;do 
  obj=./$(ObjectFlatName "$src" "")
  CommandCC "$src" "$obj" libdiggy.a
done
EndStage

Success
