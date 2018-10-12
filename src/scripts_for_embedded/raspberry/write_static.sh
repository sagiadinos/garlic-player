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

# From https://forum.openwrt.org/viewtopic.php?pid=220781#p220781
mask2cdr ()
{
   # Assumes there's no "255." after a non-255 byte in the mask
   local x=${1##*255.}
   set -- 0^^^128^192^224^240^248^252^254^ $(( (${#1} - ${#x})*2 )) ${x%%.*}
   x=${1%%$3*}
   NETMASK=$(( $2 + (${#x}/4) ))
}


INTERFACE=$1
IP4=$2
mask2cdr $3
GATEWAY=$4
DNS=$5

# delete any previous garlic inserts
# but do not delete manual inserted
sed '/#garlic dhcp-static-config/q' /etc/dhcpcd.conf | grep -v '#garlic dhcp-static-config' | sudo tee /etc/dhcpcd.conf

# append 
echo "#garlic dhcp-static-config
interface $INTERFACE
static ip_address=$IP4/$NETMASK
static routers=$GATEWAY
" | sudo tee -a /etc/dhcpcd.conf

if [ -n "$5" ]; then
	echo "domain_name_servers=$DNS
	" | sudo tee -a /etc/dhcpcd.conf
	
fi


exit 0;
