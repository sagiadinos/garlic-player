#!/bin/sh
set -e

if [ -z "$1" ]; then
	echo "No Interface"
	exit 1;
fi
INTERFACE=$1

# raspberry needs sudo
if sudo iwlist $INTERFACE scan 2> /dev/null | grep -Po '(?<=ESSID:").*[^"]' ; then
	exit 0
else
	exit 1
fi	
