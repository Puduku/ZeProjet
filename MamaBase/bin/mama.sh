#!/bin/bash
# mama.sh : general script for 'Katzenjammer Kids' Island(s) 

. mama.esh


while getopts :hi _o; do
  case $_o in 
  [h])
    if [ $scriptArgsParsingStatus -eq 0 ] ;then
      WrongScriptArg "Help option -$_o conflicts with -V executive option" 
    else
      scriptArgsParsingStatus=100
    fi
    ;;
  [i])
    if [ $scriptArgsParsingStatus -eq 0 ] ;then
      WrongScriptArg "Info option -$_o conflicts with -V executive option" 
    else
      scriptArgsParsingStatus=101
    fi
    ;;
  \?)
    WrongScriptArg "Invalid option -$OPTARG" 
    ;;
  :) 
    WrongScriptArg "Option -$OPTARG requires an argument." 
    ;;
  *)
    ScriptFailure "Unexpected option [$_o] [$OPTARG]"
  esac 
done
if  [ $scriptArgsParsingStatus -eq -1 ] ;then
  WrongScriptArg "Missing program option(s)."
fi

if [ $scriptArgsParsingStatus -eq 0 ] ;then
  WrongScriptArg "NO argument passed."
fi
  

if [ $scriptArgsParsingStatus -ge 100 ] ;then
  if [ $scriptArgsParsingStatus -eq 100 ] ;then
    cat >> /dev/stderr << MAMA_SH_HELP
Usage: $(basename $0) -h | -i 
-h : help => displays that help
-i : info => displays (useful) infos... 

Exit status :
- 100 : (NO ACTION) (displayed some help / info in "stderr") 
- 200 : (catched) "technical" problem (reported in "stderr")

MAMA_SH_HELP
  else
    cat >> /dev/stderr << MAMA_SH_INFO
Index of PUBlic and EXTernal features for "Katzenjammer Kids" scripts :  
All these features are described in bash extension files (.esh) of $MAMA/bin/

Public features (available to katzenjammer scripts):  
$(cd "$MAMA/bin" && grep '# *<PUB>' *.esh)

External features (defined in katzenjammer scripts):  
$(cd "$MAMA/bin" && grep '# *<EXT>' *.esh)

MAMA_SH_INFO
  fi

  exit 100 
fi

ScriptFailure "Should not be there..."
