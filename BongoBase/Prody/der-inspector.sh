#!/bin/bash
# Prody/der-inspector.sh
. mama.env

#########################################################################
plot="Valorize 'enclosed' documentation in Prody header files (pick all COde COmments' NUTS...)"
#########################################################################

. der-inspector.env

##### Execute plot ##### 

CommandPrepareSandbox

rootDirList=(c-ansi c-posix)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
