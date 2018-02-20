#!/bin/sh

set -e 

# delete any previous garlic inserts
# but do not delete manual inserted
sed '/#garlic dhcp-static-config/q' /etc/dhcpcd.conf | grep -v '#garlic dhcp-static-config' | sudo tee /etc/dhcpcd.conf


exit 0;
