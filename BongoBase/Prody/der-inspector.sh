#!/bin/bash
# Prody/der-inspector.sh
. mama.esh

########################################
plot="Highlight Prody API documentation"
########################################

. der-inspector.esh

##### Execute plot ##### 

CommandPrepareSandbox

rootDirList=(c-ansi c-posix)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
