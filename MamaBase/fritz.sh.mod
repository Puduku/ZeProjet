#### BEGIN OF REMOVABLE HEADER
#### Instructions:
#### cp fritz.sh.mod <garden>/fritz.sh
#### Edit <garden>/fritz.sh :
#### - Adapt the following tags:
####  <<GARDEN>>                 : garden's name
####  <<TESTS>> : (tests execution) script purpose description (one single line)
####  <<CODE 4 TESTS EXECUTION>>  : bash code for executing the tests
#### - Remove that header
#### chmod u+x <garden>/der-inspector.sh
#### END OF REMOVABLE HEADER
#!/bin/bash
# <<GARDEN>>/fritz.sh
. mama.esh

#################################
plot="<<TESTS>>"
#################################

specificUsage='[ <special options to <<GARDEN>> executables> ]'

ParseSpecificArgs () {
  testOptions=("$@")
}

SpecificInstructionsOutline () {
  _n=${#testOptions[@]}
  if [ $_n -eq 0 ] ;then 
    echo "<None>"
  else
    echo "Pass [${testOptions[@]}] option(s) to <<GARDEN>> executable"
  fi
}

. katzenjammer.esh
. der-captain.esh

#### Execute plot...

CommandPrepareSandbox

<<CODE 4 TESTS EXECUTION>>

Success
