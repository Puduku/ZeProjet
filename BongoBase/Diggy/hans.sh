#!/bin/bash
#Diggy/hans.sh
. mama.env

#############################
plot="Building DIGGY objects"
#############################
cleaning=s

. katzenjammer.env
. lena.env


CreekFunctions

######### Execute plot #######
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
