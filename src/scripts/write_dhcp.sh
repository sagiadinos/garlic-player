#!/bin/sh

set -e 

if [ -z "$1" ]; then
	echo "No Interface"
	exit 1;
fi

if [ -z "$2" ]; then
	echo "No IP"
	exit 1;
fi

if [ -z "$3" ]; then
	echo "No Netmask"
	exit 1;
fi

if [ -z "$4" ]; then
	echo "No Gateway"
	exit 1;
fi

INTERFACE=$1
IP=$2
NETMASK=$3
GATEWAY=$4
DNS=$5

# delete any previous garlic inserts
# but do not delete manual inserted
sed '/#garlic dhcp-static-config/q' /etc/dhcp.conf | grep -v '#garlic dhcp-static-config' | sudo tee /etc/dhcpcd.conf

# append 
echo "#garlic dhcp-static-config
interface $INTERFACE
static ip_address=$IP
static routers=$GATEWAY
" | sudo tee -a /etc/dhcpcd.conf

if [ -n "$5" ]; then
	echo "domain_name_servers=$DNS
	" | sudo tee -a /etc/dhcpcd.conf
	
fi


exit 0;
