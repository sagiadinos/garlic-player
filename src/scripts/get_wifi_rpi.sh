#!/bin/sh

# raspberry needs sudo
set -e
if sudo iwlist $1 scan 2> /dev/null | grep -Po '(?<=ESSID:").*[^"]' ; then
	exit 0
else
	exit 1
fi	
