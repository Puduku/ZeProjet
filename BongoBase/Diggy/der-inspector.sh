#!/bin/bash
# Diggy/der-inspector.sh
. mama.env

##############################################################################################
plot="Collect all the coconuts in Diggy's garden (i.e. highlight doc in Diggy's header files)"
##############################################################################################

. der-inspector.env

##### Execute plot  ##### 

CommandPrepareSandbox

rootDirList=(diggy)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
