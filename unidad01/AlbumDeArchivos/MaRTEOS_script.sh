#!/bin/sh
# Way of use of this script:
# source MaRTE_script.sh 
myuser='pablomdd'
GNAT=/home/${myuser}/myapps/gnat/bin 
# echo "$GNAT:/home/${myuser}/marte_2.0_22Feb2017/utils:$PATH"

export  PATH="$GNAT:/home/${myuser}/marte_2.0_22Feb2017/utils:$PATH"
export  PERL5LIB=/home/${myuser}/marte_2.0_22Feb2017
echo $PATH | cut -d ':' -f -5 | tr ':' '\n'
