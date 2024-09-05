#!/bin/bash

#############################################################################
#   Print out details of the first network interface on the host machine.   #
#   Usage:                                                                  #
#       `./networkInfo`                                                     #
#############################################################################
# 
INTERFACE=$(ifconfig $(ifconfig | grep "UP,BROADCAST,RUNNING,MULTICAST" | awk 'NR==1 {sub(/:$/, "", $1); print $1}'))
INTERFACE_NAME=$(echo $INTERFACE | awk '{sub(/:$/, "", $1);print $1}')
INTERFACE_IP=$(echo $INTERFACE | awk '{print $6}')
INTERFACE_NETMASK=$(ipcalc $INTERFACE_IP | grep Netmask | awk '{print $2}')
INTERFACE_CIDR=$(ipcalc $INTERFACE_IP | grep Network | awk '{print $2}')
INTERFACE_GATEWAY=$(ipcalc $INTERFACE_IP | grep HostMin | awk '{print $2}')

echo Interface Name: $INTERFACE_NAME
echo Interface IP: $INTERFACE_IP
echo Interface Netmask: $INTERFACE_NETMASK
echo Interface CIDR: $INTERFACE_CIDR
echo Interface Gateway: $INTERFACE_GATEWAY