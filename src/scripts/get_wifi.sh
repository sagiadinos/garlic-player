#!/bin/sh
set -e

if [ -z "$1" ]; then
	echo "No Interface"
	exit 1;
fi
INTERFACE=$1

if iwlist $1 scan 2> /dev/null | grep -Po '(?<=ESSID:").*[^"]' ; then
	exit 0
else
	exit 1
fi	
