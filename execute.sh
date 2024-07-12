#!/bin/sh

#############################################################################
##### A helper script to run the application inside a docker container. #####
#############################################################################

echo "Launching the File Transfer App in a docker container..."

########## Install dependencies. ##########
DEP_COUNT=$(dpkg -l | grep -Ec 'net-tools|ipcalc|docker-ce ')
if [ $DEP_COUNT -ne 3 ]; then
    echo "Need to install dependencies. Please allow root access."
    sudo apt-get install -y net-tools ipcalc docker
else 
    echo "Dependencies installed."
fi

########## Create a docker network  ##########
# 01. Iterate through network interfaces & get IP of the first network interface.
echo "Checking if docker network exists for File Transfer application."
IP=$(ifconfig $(ifconfig | grep "UP,BROADCAST,RUNNING,MULTICAST" | awk 'NR==1 {sub(/:$/, "", $1); print $1}') | grep "inet " | awk '{print $2}')
CIDR=$(ipcalc $IP | grep Network | awk '{print $2}')

# 02. Check if a docker network exists with the given CIDR.
NETWORK_LIST=$(docker network ls | grep bridge | awk '{print $2}')
CIDR_ADDED=false
for i in $NETWORK_LIST; do
    if [ $(docker network inspect $i | grep Subnet | awk '{gsub(/"/, "", $2); print $2}') = $CIDR ]; then
        echo "A docker network already exists."
        CIDR_ADDED=true
        break
    fi
done

if [ $CIDR_ADDED = false ]; then
    echo "Creating a new docker network..."
    docker network create --subnet=$CIDR file_transfer_network
fi

########## Build the docker container ########## 
echo "Building the docker container..."
docker build -t file_transfer:v1.0 $PWD

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
    --env DISPLAY                           \
    --net file_transfer_network             \
    -it file_transfer:v1.0                  \
    sh -c "cd /File-Transfer &&             \
        mkdir -p build &&                   \
        cmake -B build -S . &&              \
        cmake --build build &&              \
        build/file_transfer"