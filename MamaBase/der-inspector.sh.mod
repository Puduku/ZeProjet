#### BEGIN OF REMOVABLE CARTRIDGE
####  der-inspector.sh MODEL ; tags to adapt are:
####  <<GARDEN>> : garden's name
####  <<ROOT DIRS LIST>> : dir names list - eg : toto "tara tata"
#### END OF REMOVABLE CARTRIDGE
#!/bin/bash
# <<GARDEN>>/der-inspector.sh
. mama.env

#########################################################################
plot="Valorize <<GARDEN>> header files' documentation (pick all COde COmments' NUTS...) 
#########################################################################

. der-inspector.env

##### Execute plot ##### 

CommandPrepareSandbox

rootDirList=(<<root dirs list>)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
