#!/bin/bash

#########################################################################
#   A helper script to run the application inside a docker container.   #
#   Usage:                                                              #
#       ./execute                                                       #
#########################################################################

echo "Launching the File Transfer App in a docker container..."

########## Install host dependencies. ##########
DEP_COUNT=$(dpkg -l | grep -Ec 'net-tools|ipcalc|docker-ce ')
if [ $DEP_COUNT -ne 3 ]; then
    if [ $EUID -ne 0 ]; then
        echo "Need to install dependencies on the host machine. Please re-run the script with sudo."
        exit 1
    else 
        echo "Installing host dependencies..."
        sudo apt-get install -y net-tools ipcalc docker
    fi
    
else 
    echo "Host Dependencies installed."
fi

# ########## Create a docker network  ##########

# # 01. Iterate through network interfaces & get IP of the first network interface.
# echo "Checking if docker network exists for File Transfer application."
# IP=$(ifconfig $(ifconfig | grep "UP,BROADCAST,RUNNING,MULTICAST" | awk 'NR==1 {sub(/:$/, "", $1); print $1}') | grep "inet " | awk '{print $2}')
# CIDR=$(ipcalc $IP | grep Network | awk '{print $2}')

# # 02. Check if a docker network exists with the given CIDR.
# NETWORK_LIST=$(docker network ls | grep bridge | awk '{print $2}')
# NETWORK_ADDED=false
# for i in $NETWORK_LIST; do
#     if [ $(docker network inspect $i | grep Subnet | awk '{gsub(/"/, "", $2); print $2}') = $CIDR ]; then
#         echo "A docker network with $CIDR CIDR already exists."
#         NETWORK_ADDED=true
#         break
#     fi
# done

# if [ $NETWORK_ADDED = false ]; then
#     echo "Creating a new docker network with $CIDR CIDR..."
#     # Todo: Figure out a way to generate a random unused IP for docker network's gateway.
#     # The default gateway assigned by docker messes up the host's internet.
#     docker network create --subnet=$CIDR --gateway=192.168.0.150 file_transfer_network
# fi

########## Build the docker container ########## 
echo "Building the docker container..."
docker build -t file_transfer:v2.0 $PWD

########## Run the docker container ########## 

# 01. Mount the project directory to the container.
# 02. Share X11 socket for enabling GUI on the container.
# 03. Provide DRI for enabling GUI on the container.
# 04. Sets DISPLAY env. variable for enabling GUI on the container.
# 05. Provide a custom network to the container.
# 06. Run cmake commands to build & run the application on launch of the container.
# Refer: https://www.baeldung.com/linux/docker-container-gui-applications, https://stackoverflow.com/a/77747682 
echo "Running the docker container..."
docker run                                  \
    --volume $PWD:/File-Transfer            \
    --volume /tmp/.X11-unix:/tmp/.X11-unix  \
    --device=/dev/dri:/dev/dri              \
    --device=/dev/fuse:/dev/fuse            \
    --cap-add SYS_ADMIN                     \
    --security-opt apparmor:unconfined      \
    --env DISPLAY                           \
    -it file_transfer:v2.0                  \
    sh -c "cd /File-Transfer &&             \
        mkdir -p /File-Transfer/build &&                   \
        cmake -B /File-Transfer/build -S /File-Transfer/ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/opt/Qt/6.5.0/gcc_64/ &&              \
        cmake --build /File-Transfer/build &&              \
        /File-Transfer/build/file_transfer"
    #     cmake --install /File-Transfer/build --prefix /File-Transfer/build/AppDir &&\
    #     /File-Transfer/utils/AppImage/linuxdeploy-x86_64.AppImage --appdir /File-Transfer/build/AppDir/ --output appimage --plugin qt"