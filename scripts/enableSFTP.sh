#!/bin/bash

#################################################################################################
#   A script to start/stop SSH server & grant SFTP-only access to a temporary user.             #
#   The temporary user is `<username>-temp`                                                     #
#   Usage:                                                                                      #
#       `sudo ./enableSFTP`         Start the SFTP server & grants access to a temporary user.  #
#       `sudo ./enableSFTP stop`    Stop the SFTP access & removes the temporary user.          #
#   Note: This script needs root access.                                                        #
#################################################################################################          

# Check if the script is run as root
if [ $EUID -ne 0 ]; then
    echo "This script must be run with sudo."
    exit 1
fi

# Name of the temporary user created for SFTP-only access. Format is `<username>-temp`
USER=$(whoami)
TEMP_USER="$USER-temp"

# Receive the files in Download directory.
TEMP_HOME_DIR="/home/$TEMP_USER"
DOWNLOAD_DIR="$TEMP_HOME_DIR/Downloads"

# Adds or removes temporary user. Default argument is "add"
function manageUsers(){
    
    if [ $1 = "remove" ]; then
        # Forcefully remove the temp user.
        userdel -f $TEMP_USER >& /dev/null
        USER_DEL_RES=$?
        
        if [ $USER_DEL_RES -eq 0 ] || [ $USER_DEL_RES -eq 6 ]; then
            # If the user is removed (0) or doesn't exists (6), return success code (0).
            return 0
        else
            return 1
        fi
    else
        # Add the temp user & disable shell login (-s).
        useradd $TEMP_USER -s /sbin/nologin >& /dev/null
        USER_ADD_RES=$?

        # Assign password to the temp user.
        echo "$TEMP_USER:toorresu" | chpasswd
        USER_PASSWD_RES=$?

        if [ $USER_ADD_RES -eq 0 ] || [ $USER_ADD_RES -eq 9 ] && [ $USER_PASSWD_RES -eq 0 ]; then
            # If the user is added (0) or already exists (9) & the password is assigned, return success code (0).
            return 0
        else
            return 1
        fi
    fi
}

# Add or removes SSH config of the temporary user. Default argument is "add".
function manageSSHConfig(){

    cat /etc/ssh/sshd_config | grep -q "Match User $TEMP_USER"
    SSH_CONFIG_EXISTS=$?

    if [ "$1" = "remove" ]; then
        # Remove the SSH config
        if [ $SSH_CONFIG_EXISTS -ne 0 ]; then
            # If SSH config does not exists, return success code (0).
            return 0
        else
            # Find the config & delete it from the SSH config file.
            sed -i "/Match User $TEMP_USER/,+7d" /etc/ssh/sshd_config
            if [ $? -eq 0 ]; then
                # If SSH config is removed, return success code (0).
                return 0
            else
                return 1
            fi
        fi
    else
        # Add the SSH config
        if [ $SSH_CONFIG_EXISTS -eq 0 ]; then
            return 0
        else
            # Adds a SSH config for the temporary user.
            #
            # Example:
            #   Match User <username>-temp
            #       ForceCommand internal-sftp
            #       PasswordAuthentication yes
            #       ChrootDirectory /home/<username>-temp/
            #       PermitTunnel no
            #       AllowAgentForwarding no
            #       AllowTcpForwarding no
            #       X11Forwarding no
            #
            # Note: This written in a single line as \n & \t is used to maintain formatting in the config file.
            echo -e "Match User $TEMP_USER \n\tForceCommand internal-sftp \n\tPasswordAuthentication yes \n\tChrootDirectory $TEMP_HOME_DIR \n\tPermitTunnel no \n\tAllowAgentForwarding no \n\tAllowTcpForwarding no \n\tX11Forwarding no" | tee -a /etc/ssh/sshd_config > /dev/null
            if [ $? -eq 0 ]; then
                # If the SSH config is added successfully, then return success code (0).
                return 0
            else
                return 1
            fi
        fi
    fi
}

# The permissions of the Download dir & it's parent needs to be set for SFTP to work.
function managePermissions(){

    # The parent dir of Download i.e. Home dir. needs to be owned by root. Only root will have RWX permissions.
    chown root:root $TEMP_HOME_DIR
    chmod 755 $TEMP_HOME_DIR

    # The download dir is owned by the temp user & it will any user can RWX.
    chown $TEMP_USER:$TEMP_USER $DOWNLOAD_DIR
    chmod 777 $DOWNLOAD_DIR

    return 0
}

function disableSFTP(){

    # Stop the SSH server
    service ssh stop
    if [ $? -eq 0 ]; then       

        # Remove the temporary user.
        manageUsers "remove"
        MANAGE_USER_RES=$?

        # Remove the SSH config for the temporary user.
        manageSSHConfig "remove"
        MANAGE_SSH_CONFIG_RES=$?

        if [ $MANAGE_USER_RES -eq 0 ] && [ $MANAGE_SSH_CONFIG_RES -eq 0 ]; then
            echo "SSH service stopped, $TEMP_USER user & it's corresponding SSH config removed."
            return 0
        else
            echo "SSH service stopped but failed to remove $TEMP_USER user & it's corresponding SSH config."
            return 1
        fi
    else
        echo "Failed to stop SSH service."
        return 1
    fi
}

function enableSFTP(){
    # Start the SSH server

    # Check the SSH status & start the server.
    service ssh status > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        service ssh start
        if [ $? -ne 0 ]; then
            echo "Failed to start SSH service."
            exit 1
        fi
    fi

    # Create a dir for the files to be received in.
    mkdir -p $TEMP_HOME_DIR
    mkdir -p $DOWNLOAD_DIR

    # Add temporary user
    manageUsers "add"
    MANAGE_USER_RES=$?

    # Add SSH configuration to set up a temporary SFTP-only user.
    manageSSHConfig "add"
    MANAGE_SSH_CONFIG_RES=$?

    # Manage folder permissions
    managePermissions
    MANAGE_PERMISSION_RES=$?

    if [ $MANAGE_USER_RES -eq 0 ] && [ $MANAGE_SSH_CONFIG_RES -eq 0 ] && [ $MANAGE_PERMISSION_RES -eq 0 ]; then
        echo "SSH service started & $TEMP_USER user added."
        service ssh restart
        return 0
    else
        echo "SSH service started but failed to add $TEMP_USER user & it's corresponding SSH config."
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