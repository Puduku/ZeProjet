#### BEGIN OF REMOVABLE CARTRIDGE
####  fritz.sh scripts MODEL ; tags to adapt are: 
####  <<GARDEN>>                 : garden's name
####  <<TESTS PLAN DESCRIPTION>> : (tests execution) plot's short description (one single line)
####  <<CODE 4 PLAN EXECUTION>>  : bash code for executing the plot
#### END OF REMOVABLE CARTRIDGE
#!/bin/bash
# <<GARDEN>>/fritz.sh
. mama.esh

#################################
plot="<<TESTS PLAN DESCRIPTION>>"
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

<<CODE 4 PLAN EXECUTION>>

Success
