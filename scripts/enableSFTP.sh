#!/bin/bash

#################################################################################################
#   A script to start/stop SSH server & grant SFTP-only access to the user.                     #
#   Usage:                                                                                      #
#       `sudo ./enableSFTP`         Start the SFTP server & grants access to the user.          #
#       `sudo ./enableSFTP stop`    Stop the SFTP access & removes the user.                    #
#   Note: This script needs root access.                                                        #
#################################################################################################          

# Check if the script is run as root
if [ $EUID -ne 0 ]; then
    echo "This script must be run with sudo."
    exit 1
fi

USER=$(whoami)

# Receive the files in Download directory.
HOME_DIR="/home/$USER"
DOWNLOAD_DIR="$HOME_DIR/Downloads"

SSH_CONFIG_FILE=/etc/ssh/file_transfer_config

# Add or removes SSH config file. Default argument is "add".
function manageSSHConfig(){
    
    if [ "$1" = "remove" ]; then
        # Remove the SSH config
        sudo rm -f $SSH_CONFIG_FILE
        if [ $? -eq 0 ]; then
            # If the SSH config is removed successfully, then return success code (0).
            return 0
        else
            return 1
        fi
    else        
        # Add the SSH config
        if [ -f $SSH_CONFIG_FILE ]; then
            # If file already exists then return true.
            return 0
        else
            # Make a copy of main ssh config file.
            sudo cp /etc/ssh/sshd_config $SSH_CONFIG_FILE

            # Add the config in the SSH config copy file.
            #
            # Example:
            #   Match User <username>
            #       ForceCommand internal-sftp
            #       PasswordAuthentication yes
            #       ChrootDirectory /home/<username>
            #       PermitTunnel no
            #       AllowAgentForwarding no
            #       AllowTcpForwarding no
            #       X11Forwarding no
            #
            # Note: This written in a single line as \n & \t is used to maintain formatting in the config file.
            echo -e "Match User $USER           \
                \n\tForceCommand internal-sftp  \
                \n\tPasswordAuthentication yes  \
                \n\tChrootDirectory $USER       \
                \n\tPermitTunnel no             \
                \n\tAllowAgentForwarding no     \
                \n\tAllowTcpForwarding no       \
                \n\tX11Forwarding no"           \
                | tee -a $SSH_CONFIG_FILE > /dev/null
            if [ $? -eq 0 ]; then
                # If the SSH config is added successfully, then return success code (0).
                return 0
            else
                return 1
            fi
        fi
    fi
}

# Sets the permissions for the Download dir. Required by SFTP.
function manageFolderPermissions(){

    # As per SFTP rules, the parent of Download dir i.e. Home dir. needs to be owned by root.
    # Refer: https://man.openbsd.org/sshd_config#ChrootDirectory
    mkdir -p $HOME_DIR
    chown root:root $HOME_DIR
    chmod 755 $HOME_DIR

    # The download dir is owned by the user who has RWX permission.
    mkdir -p $DOWNLOAD_DIR
    chown $USER:$USER $DOWNLOAD_DIR
    chmod 777 $DOWNLOAD_DIR

    return 0
}

function disableSFTP(){

    # Stop the SSH server
    service ssh stop
    if [ $? -eq 0 ]; then       

        # Remove the custom SSH config
        manageSSHConfig "remove"
        MANAGE_SSH_CONFIG_RES=$?

        if [ $MANAGE_SSH_CONFIG_RES -eq 0 ]; then
            echo "SSH service stopped, $USER & it's corresponding SSH config removed."
            return 0
        else
            echo "SSH service stopped but failed to remove $USER & it's corresponding SSH config."
            return 1
        fi
    else
        echo "Failed to stop SSH service."
        return 1
    fi
}

function enableSFTP(){
    # Start the SSH server

    # Add SSH configuration to set up a SFTP-only user.
    manageSSHConfig "add"
    MANAGE_SSH_CONFIG_RES=$?

    # Manage folder permissions
    manageFolderPermissions
    MANAGE_PERMISSION_RES=$?

    # Start ssh
    sudo /usr/sbin/sshd -f $SSH_CONFIG_FILE
    SSH_START_RES=$?

    if [ $MANAGE_SSH_CONFIG_RES -eq 0 ] && [ $MANAGE_PERMISSION_RES -eq 0 ] && [ $SSH_START_RES -eq 0 ]; then
        echo "SSH service started with it's corresponding SSH config."
        return 0
    else
        echo "Failed to start SSH service with it's corresponding SSH config."
        return 1
    fi
}

if [ "$1" = "stop" ]; then
    # Stop SFTP server
    disableSFTP
    exit $?
else
    # Start SFTP server
    enableSFTP
    if [ $? -ne 0 ]; then
        disableSFTP
        exit 1
    else
        exit 0
    fi
fi