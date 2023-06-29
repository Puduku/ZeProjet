#!/bin/bash
# Diggy/der-inspector.sh
. mama.env

########################################################################
plot="Pick up Diggy's coconuts (aka valorize Diggy's API documentation)"
########################################################################

. der-inspector.env

##### Execute plot  ##### 

CommandPrepareSandbox

rootDirList=(diggy)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
