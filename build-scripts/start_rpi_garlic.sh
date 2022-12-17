#!/bin/bash

#export QT_QPA_EGLFS_WIDTH=1920
#export QT_QPA_EGLFS_HEIGHT=1080
export QT_QPA_EGLFS_PHYSICAL_WIDTH=476
export QT_QPA_EGLFS_PHYSICAL_HEIGHT=268
#export QT_QPA_EGLFS_DEPTH=24
export QT_QPA_EGLFS_ALWAYS_SET_MODE=1

#export QT_DEBUG_PLUGINS=1
#export QT_QPA_EGLFS_DEBUG=1
#export QSG_INFO=1

#comment out for more debug informations
export QT_LOGGING_RULES="*.debug=false"

/home/pi/garlic-player -m fullscreen $1 $2

exit 0; 
