#### BEGIN OF REMOVABLE HEADER
#### Instructions:
#### cp hans.sh.mod <garden>/hans.sh
#### Edit <garden>/hans.sh :
#### - Adapt the following tags:
####    <<GARDEN>>                        : garden's name
####    <<COMPILING>>                     : script purpose (one single line)
####    <<CREEK FUNCTIONS CODE>>          : bash code for definition of all creek functions
####    <<CREEK FUNCTIONS LIST>>          : list of all creek functions
####    <<MAIN CODE>>                     : script main code 
####    <<SPECIFIC EXECUTABLES TO CLEAN>> : list of specific executables to clean...
#### - Remove that header
#### chmod u+x <garden>/hans.sh
#### END OF REMOVABLE HEADER
#!/bin/bash
# <<GARDEN>>/hans.sh
. mama.esh

#######################################
plot="<<COMPILING>>"
#######################################
cleaning=s

. katzenjammer.esh
. lena.esh

#### Creek functions: 

<<CREEK FUNCTIONS CODE>>

CreekFunctions <<CREEK FUNCTIONS LIST>> 

#### Execute plot... 
CommandClean  <<SPECIFIC EXECUTABLES TO CLEAN>>

<<MAIN CODE>>

Success
