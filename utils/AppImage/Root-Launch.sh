#!/bin/bash

# Debugging statements
ls -lh $(dirname $0)/
ls -lh $(dirname $0)/usr/
ls -lh $(dirname $0)/usr/bin/
echo "XAUTHORITY: $1" 
echo "Current directory: $(pwd)"

# Wrapper script to run the AppImage with pkexec
pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY $(dirname $0)/usr/bin/file_transfer