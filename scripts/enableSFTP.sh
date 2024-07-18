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

# Adds or removes temporary user. Default argument is "add"
function manageUsers(){
    
    if [ $1 = "remove" ]; then
        # Remove the temp user.
        userdel -fr $TEMP_USER >& /dev/null
        USER_DEL_RES=$?
        echo $USER_DEL_RES
        if [ $USER_DEL_RES -eq 0 ] || [ $USER_DEL_RES -eq 6 ]; then
            return 0    # If the user is removed or doesn't exists, return 0.
        else
            return 1
        fi
    else
        # Add the temp user.
        useradd -m $TEMP_USER -s /bin/false >& /dev/null
        USER_ADD_RES=$?
        if [ $USER_ADD_RES -eq 0 ] || [ $USER_ADD_RES -eq 9 ]; then
            echo "$TEMP_USER:toorresu" | chpasswd
            return 0
        else
            return 1
        fi
    fi
}

# Add or removes SSH config of the temporary user.
function manageSSHConfig(){

    cat /etc/ssh/sshd_config | grep -q "Match User $TEMP_USER"
    SSH_CONFIG_EXISTS=$?

    if [ "$1" = "remove" ]; then
        if [ $SSH_CONFIG_EXISTS -ne 0 ]; then
            return 0
        else
            # Find the config & delete it from the SSH config file.
            sed -i "/Match User $TEMP_USER/,+7d" /etc/ssh/sshd_config
            if [ $? -eq 0 ]; then
                return 0
            else
                return 1
            fi
        fi
    else
        if [ $SSH_CONFIG_EXISTS -eq 0 ]; then
            return 0
        else
            # Adds a SSH config for the temporary user.
            #
            # Example:
            #   Match User <username>-temp
            #       ForceCommand internal-sftp
            #       PasswordAuthentication yes
            #       ChrootDirectory /home/<username>/Downloads
            #       PermitTunnel no
            #       AllowAgentForwarding no
            #       AllowTcpForwarding no
            #       X11Forwarding no
            #
            # Note: This written in a single line as \n & \t is used to maintain formatting in the config file.
            PROJECT_DIR=$(dirname $PWD)
            echo -e "Match User $TEMP_USER \n\tForceCommand internal-sftp \n\tPasswordAuthentication yes \n\tChrootDirectory $PROJECT_DIR \n\tPermitTunnel no \n\tAllowAgentForwarding no \n\tAllowTcpForwarding no \n\tX11Forwarding no" | tee -a /etc/ssh/sshd_config > /dev/null
            if [ $? -eq 0 ]; then
                return 0
            else
                return 1
            fi
        fi
    fi
}

if [ "$1" = "stop" ]; then
    # 01. Stop the SSH server
    service ssh stop
    if [ $? -eq 0 ]; then       

        # 02. Remove the temporary user.
        manageUsers "remove"
        MANAGE_USER_RES=$?

        # 03. Remove the SSH config for the temporary user.
        manageSSHConfig "remove"
        MANAGE_SSH_CONFIG_RES=$?

        if [ $MANAGE_USER_RES -eq 0 ] && [ $MANAGE_SSH_CONFIG_RES -eq 0 ]; then
            echo "SSH service stopped, $TEMP_USER user & it's corresponding SSH config removed."
            exit 0
        else
            echo "SSH service stopped but failed to remove $TEMP_USER user & it's corresponding SSH config."
            exit 1
        fi
    else
        echo "Failed to stop SSH service."
        exit 1
    fi

else
    # Start the SSH server

    # 01. Check the SSH status & start the server.
    service ssh status > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        service ssh start
        if [ $? -ne 0 ]; then
            echo "Failed to start SSH service."
            exit 1
        fi
    fi

    # 02. Create a dir for the files to be received in.
    #mkdir -p /home/$USER/Downloads

    # 03. Add temporary user
    manageUsers "add"
    MANAGE_USER_RES=$?

    # 04. Add SSH configuration to set up a temporary SFTP-only user.
    manageSSHConfig "add"
    MANAGE_SSH_CONFIG_RES=$?

    if [ $MANAGE_USER_RES -eq 0 ] && [ $MANAGE_SSH_CONFIG_RES -eq 0 ]; then
        echo "SSH service started & $TEMP_USER user added."
        exit 0
    else
        echo "SSH service started but failed to add $TEMP_USER user & it's corresponding SSH config."
        exit 1
    fi
fi

# service ssh restart
# exit 0

