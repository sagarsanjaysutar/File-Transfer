#!/bin/bash

#####################################################################################################
#   A helper script to build/run the project inside a docker container.                             #
#   Usage:                                                                                          #
#       ./execute           Builds & runs the project's binary inside a docker container.           #
#       ./execute build     Builds & generates the project's AppImage inside a docker container.    #
#####################################################################################################

########## Build the docker container ########## 
echo "Building the docker container..."
docker build -t file_transfer:v2.0 $PWD/ci

########## Run the docker container ########## 
if [ "$1" = "build" ]; then
    echo "Running the docker container to build & generate the project's AppImage."
    # --volume $PWD:/File-Transfer:             Mount the project directory to the container.
    # --device=/dev/fuse:/dev/fuse              FUSE Device of host is passed in order for linuxdeploy to work. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
    # --cap-add SYS_ADMIN                       FUSE related persmission. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
    # --security-opt apparmor:unconfined        FUSE related persmission. Refer: https://stackoverflow.com/questions/48402218/fuse-inside-docker
    # sh ...                                    Builds the project & generates AppImage in project's bin dir.
    docker run                                  \
        --volume $PWD:/File-Transfer            \
        --device=/dev/fuse:/dev/fuse            \
        --cap-add SYS_ADMIN                     \
        --security-opt apparmor:unconfined      \
        -it file_transfer:v2.0                  \
        sh -c "cd /File-Transfer &&             \
            sudo rm -rf /File-Transfer/build /File-Transfer/bin && \
            mkdir -p /File-Transfer/build /File-Transfer/bin &&                   \
            cmake -B /File-Transfer/build -S /File-Transfer/ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/opt/Qt/6.5.0/gcc_64/ &&              \
            cmake --build /File-Transfer/build &&              \
            sudo make install --directory=/File-Transfer/build/ DESTDIR=/File-Transfer/build/AppDir &&    \
            cd /File-Transfer/bin/ && \
            sudo QML_SOURCES_PATHS=/File-Transfer/res/qml QMAKE=/opt/Qt/6.5.0/gcc_64/bin/qmake LD_LIBRARY_PATH=/opt/Qt/6.5.0/gcc_64/lib \
            /home/user/linuxdeploy-x86_64.AppImage --appdir /File-Transfer/build/AppDir/ --output appimage --plugin qt --desktop-file /File-Transfer/ci/file-transfer.desktop --icon-file /File-Transfer/ci/file-transfer.svg"
else
    echo "Running the docker container to build & run the project's binary."
    # Refer: https://www.baeldung.com/linux/docker-container-gui-applications https://stackoverflow.com/a/49717572
    # --volume $PWD:/File-Transfer:             Mount the project directory to the container.
    # --volume /tmp/.X11-unix:/tmp/.X11-unix:   Share X11 socket for enabling GUI on the container. Refer: https://stackoverflow.com/a/77747682 
    # --device=/dev/dri:/dev/dri:               Provide DRI for enabling GUI on the container. Refer: https://stackoverflow.com/a/77747682  
    # --env DISPLAY:                            Sets DISPLAY env. variable for enabling GUI on the container.
    # sh ...                                    Builds the project & runs the binary in Docker container.
    docker run                                  \
        --volume $PWD:/File-Transfer            \
        --volume /tmp/.X11-unix:/tmp/.X11-unix  \
        --device=/dev/dri:/dev/dri              \
        --env DISPLAY                           \
        --net pub_net                           \
        -it file_transfer:v2.0                  \
        sh -c "cd /File-Transfer &&             \
            mkdir -p /File-Transfer/build /File-Transfer/bin &&                   \
            cmake -B /File-Transfer/build -S /File-Transfer/ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/opt/Qt/6.5.0/gcc_64/ &&              \
            cmake --build /File-Transfer/build &&              \
            sudo /File-Transfer/build/file-transfer"
fi