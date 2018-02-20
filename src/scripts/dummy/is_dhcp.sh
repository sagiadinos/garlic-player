#!/bin/sh
set -e

RET=$(ps -A | grep -i dhclient | wc -l)

if [ $RET -gt 0 ]; then
	exit 0;
else
	exit 1;
fi
