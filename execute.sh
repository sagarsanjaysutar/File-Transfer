#!/bin/bash

#####################################################################################################
#   A helper script to run the project inside a docker container.                                   #
#   Usage:                                                                                          #
#       ./execute           Runs the project binary within the docker container.                    #
#       ./execute build     Generates the project AppImage within the docker container.             #
#                           Generated AppImage is placed in the build dir.                          #
#####################################################################################################

DOCKER_CONTAINER_NAME=file_transfer:v2.0
DOCKER_NETWORK_NAME=file_transfer_network

# Builds the docker container from the DockerFile
function buildDockerContainer(){
    echo "Building the docker container..."
    docker build -t $DOCKER_CONTAINER_NAME $PWD/ci
}

# Create a network for the Docker container.
function createDockerNetwork(){
    
    echo "Creating a docker network for the container..."

    # 01. Fetch Network Info & allocate 8 IP addresses for the docker container
    NETWORK_INFO=$(./scripts/networkInfo.sh)
    NETWORK_NAME=$(echo "$NETWORK_INFO" | grep Name | awk '{print $3}')
    NETWORK_CIDR=$(echo "$NETWORK_INFO" | grep CIDR | awk '{print $3}')
    DOCKER_GATEWAY=$(echo "$NETWORK_INFO" | grep Gateway | awk '{print $3}')
    DOCKER_CIDR=$(echo $NETWORK_CIDR | sed 's/\/[0-9]\+$/\/29/')

    # 02. Check if a docker network exists with the given CIDR.
    NETWORK_LIST=$(docker network ls | grep macvlan | awk '{print $1}')
    NETWORK_ADDED=false
    for i in $NETWORK_LIST; do
        if [ $(docker network inspect $i | grep Subnet | awk -F'"' '{print $4}') = $DOCKER_CIDR ]; then
            echo "A docker network with $CIDR CIDR already exists."
            NETWORK_ADDED=true
            break
        fi
    done

    # 03. If the Docker network doesn't exist, create one.    
    if [ $(docker network ls | grep -c $DOCKER_NETWORK_NAME) -eq 0 ]; then
        echo "Creating a new docker network with $CIDR CIDR..."
        docker network create -d macvlan \
            --subnet=$DOCKER_CIDR \
            --gateway=$DOCKER_GATEWAY \
            -o parent=$NETWORK_NAME \
            $DOCKER_NETWORK_NAME
    fi
}

function runDockerContainer(){
    if [[ $1 == "build" ]]; then
        echo "Running the docker container to build & generate the project's AppImage..."

        # Run the container by configuring with permissions and device access needed for generating an AppImage.
        # Param Info:
        # --volume $PWD:/File-Transfer:             Mount the project directory to the container.
        # --device=/dev/fuse:/dev/fuse              FUSE Device of host is passed in order for linuxdeploy to work. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
        # --cap-add SYS_ADMIN                       FUSE related persmission. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
        # --security-opt apparmor:unconfined        FUSE related persmission. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
        docker run                                  \
            --volume $PWD:/File-Transfer            \
            --device=/dev/fuse:/dev/fuse            \
            --cap-add SYS_ADMIN                     \
            --security-opt apparmor:unconfined      \
            -it $DOCKER_CONTAINER_NAME              \
            build
    else
        echo "Running the docker container to build & run the project's binary..."

        createDockerNetwork;

        # Run the container by configuring with GUI specific flags.
        # Param Info:
        # --volume $PWD:/File-Transfer:             Mount the project directory to the container.
        # --volume /tmp/.X11-unix:/tmp/.X11-unix:   Share X11 socket for enabling GUI on the container. Refer: https://stackoverflow.com/a/77747682 
        # --device=/dev/dri:/dev/dri:               Provide DRI for enabling GUI on the container. Refer: https://stackoverflow.com/a/77747682  
        # --env DISPLAY:                            Sets DISPLAY env. variable for enabling GUI on the container.
        # Refer: https://www.baeldung.com/linux/docker-container-gui-applications https://stackoverflow.com/a/49717572
        docker run                                  \
            --volume $PWD:/File-Transfer            \
            --volume /tmp/.X11-unix:/tmp/.X11-unix  \
            --device=/dev/dri:/dev/dri              \
            --env DISPLAY                           \
            --net $DOCKER_NETWORK_NAME              \
            -it $DOCKER_CONTAINER_NAME
    fi
}

buildDockerContainer;
runDockerContainer $(if [[ $1 == "build" ]]; then echo "build"; fi;)