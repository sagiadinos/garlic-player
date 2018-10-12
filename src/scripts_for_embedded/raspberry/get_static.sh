#!/bin/sh

set -e 

cdr2mask ()
{
   # Number of args to shift, 255..255, first non-255 byte, zeroes
   set -- $(( 5 - ($1 / 8) )) 255 255 255 255 $(( (255 << (8 - ($1 % 8))) & 255 )) 0 0 0
   [ $1 -gt 1 ] && shift $1 || shift
   NETMASK=${1-0}.${2-0}.${3-0}.${4-0}
}


INTERFACE=$(sed -ne '/#garlic dhcp-static-config/, $ p' /etc/dhcpcd.conf | grep 'interface' | awk '{print $2}')
PREIP4=$(sed -ne '/#garlic dhcp-static-config/, $ p' /etc/dhcpcd.conf | grep 'static ip_address=' | cut -d '=' -f2)

IP4=$(echo $PREIP4 | tr '/' " " | awk '{print $1}')
UI=

cdr2mask $(echo $PREIP4 | tr '/' " " | awk '{print $2}')

GATEWAY=$(sed -ne '/#garlic dhcp-static-config/, $ p' /etc/dhcpcd.conf | grep 'static routers=' | cut -d '=' -f2)
DNS=$(sed -ne '/#garlic dhcp-static-config/, $ p' /etc/dhcpcd.conf | grep 'domain_name_servers=' | cut -d '=' -f2)


echo $INTERFACE,$IP4,$NETMASK,$GATEWAY,$DNS

exit 0
