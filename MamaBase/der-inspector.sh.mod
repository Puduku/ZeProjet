#### BEGIN OF REMOVABLE HEADER
#### Intructions:
#### cp der-inspector.sh.mod <garden>/der-inspector.sh
#### Edit <garden>/der-inspector.sh :
#### - Adapt the following tags:
####    <<GARDEN>> : garden's name
####    <<CODE/API DOCUMENTATION SHAPING>> : script purpose description 
####    <<ROOT DIRS LIST>> : dir. names list - eg : toto "tara tata"
#### - Remove that header
#### chmod u+x <garden>/der-inspector.sh
#### END OF REMOVABLE HEADER
#!/bin/bash
# <<GARDEN>>/der-inspector.sh
. mama.esh

#########################################
plot="<<CODE/API DOCUMENTATION SHAPING>>"
#########################################

. der-inspector.esh

##### Execute plot ##### 

CommandPrepareSandbox

rootDirList=(<<root dirs list>)
EvalCommand coco coca -t "$(SandboxLocation)" -m $maxStep -f h $(Adolf "${rootDirList[@]}") 
if [ $pruning -gt 0 ] ;then
  EvalCommand coco cocu -t "$(SandboxLocation)" -m $pruning -f h $(Adolf "${rootDirList[@]}") 
fi

Success
