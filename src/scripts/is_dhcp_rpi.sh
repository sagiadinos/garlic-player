#!/bin/sh
set -e

if [ cat /etc/dhcpcd.conf | grep '#garlic dhcp-static-config' ]; then
	exit 0;
else
	exit 1;
fi
