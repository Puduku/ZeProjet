#### BEGIN OF REMOVABLE CARTRIDGE
####  hans.sh scripts MODEL ; tags to adapt are: 
####  <<GARDEN>>                        : garden's name
####  <<COMPILATION PLAN DESCRIPTION>>  : plan's (compilation) short description (one single line)
####  <<CODE 4 CREEK FUNCTIONS>>        : bash code for definition of all creek functions
####  <<CREEK FUNCTIONS LIST>>          : list of all creek functions
####  <<CODE 4 PLAN EXECUTION>>         : bash code for executing the plan 
####  <<SPECIFIC EXECUTABLES TO CLEAN>> : list of specific executables to clean...
#### END OF REMOVABLE CARTRIDGE
#!/bin/bash
# <<GARDEN>>/hans.sh
. mama.env

#######################################
plan="<<COMPILATION PLAN DESCRIPTION>>"
#######################################
cleaning=s

. katzenjammer.env
. lena.env

#### Creek functions: 

<<CODE 4 CREEK FUNCTIONS>>

CreekFunctions <<CREEK FUNCTIONS LIST>> 

#### Execute plan... 
CommandClean  <<SPECIFIC EXECUTABLES TO CLEAN>>

<<CODE 4 PLAN EXECUTION>>

Success
