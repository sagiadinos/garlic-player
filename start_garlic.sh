#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/lib
export QT_PLUGIN_PATH=$PWD/bin/plugins
export QT_QPA_PLATFORM_PLUGIN_PATH=$PWD/bin/platforms

#for debug propeses
#export QT_DEBUG_PLUGINS=1 

#comment out for more debug informations
export QT_LOGGING_RULES="*.debug=false"


bin/garlic-player $1 $2

exit 0;
