#!/bin/sh
set -e

# Raspberry use dhcpd even if static IP is set.
# so we need to determine in a different way
#
if [ cat /etc/dhcpcd.conf | grep '#garlic dhcp-static-config' ]; then
	exit 1;
else
	exit 0;
fi
