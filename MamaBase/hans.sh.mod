#### BEGIN OF REMOVABLE CARTRIDGE
####  hans.sh scripts MODEL ; tags to adapt are: 
####  <<GARDEN>>                        : garden's name
####  <<COMPILATION PLAN DESCRIPTION>>  : plot's (compilation) short description (one single line)
####  <<CODE 4 CREEK FUNCTIONS>>        : bash code for definition of all creek functions
####  <<CREEK FUNCTIONS LIST>>          : list of all creek functions
####  <<CODE 4 PLAN EXECUTION>>         : bash code for executing the plot 
####  <<SPECIFIC EXECUTABLES TO CLEAN>> : list of specific executables to clean...
#### END OF REMOVABLE CARTRIDGE
#!/bin/bash
# <<GARDEN>>/hans.sh
. mama.esh

#######################################
plot="<<COMPILATION PLAN DESCRIPTION>>"
#######################################
cleaning=s

. katzenjammer.esh
. lena.esh

#### Creek functions: 

<<CODE 4 CREEK FUNCTIONS>>

CreekFunctions <<CREEK FUNCTIONS LIST>> 

#### Execute plot... 
CommandClean  <<SPECIFIC EXECUTABLES TO CLEAN>>

<<CODE 4 PLAN EXECUTION>>

Success
