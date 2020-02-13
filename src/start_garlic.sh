#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./lib
./bin/garlic-player $1 $2
#comment out for more debug informations
export QT_LOGGING_RULES="*.debug=false"
 
