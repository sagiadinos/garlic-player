#!/bin/sh

set -e
if iwlist $1 scan 2> /dev/null | grep -Po '(?<=ESSID:").*[^"]' ; then
	exit 0
else
	exit 1
fi	
