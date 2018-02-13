#!/bin/sh

set -e 

if [ -z "$1" ]; then
	echo "Error: No ESSID"
	exit 1;
fi

if [ -z "$2" ]; then
	echo "Error: No Password"
	exit 1;
fi

SSID=$1
PSK=$2

# delete any previous garlic inserts
# but do not delete manual inserted
sed '/#garlic wifi-config/q' /etc/wpa_supplicant/wpa_supplicant.conf | grep -v '#garlic wifi-config' | sudo tee /etc/wpa_supplicant/wpa_supplicant.conf

# append 
echo "#garlic wifi-config
" | sudo tee -a /etc/wpa_supplicant/wpa_supplicant.conf

wpa_passphrase $SSID $PSK | grep -v '#psk=' | sudo tee -a /etc/wpa_supplicant/wpa_supplicant.conf

exit 0;
