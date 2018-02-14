#!/bin/sh
set -e

if [ /run/dhclient ]; then
	exit 0;
else
	exit 1;
fi
