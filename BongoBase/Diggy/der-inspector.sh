#!/bin/bash
# Diggy/der-inspector.sh
. mama.esh

########################################
plot="Highlight Diggy API documentation"
########################################

. der-inspector.esh

##### Execute plot  ##### 

CommandPrepareSandbox

rootDirList=(diggy)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
