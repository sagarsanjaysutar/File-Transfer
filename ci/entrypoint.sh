#!/bin/bash

#####################################################################################################
#   An entrypoint script to build/run the project.                                                  #
#   Note: This script will run inside the docker container.                                         #
#   Usage:                                                                                          #
#       ./entrypoint           Builds & runs the project's binary inside a docker container.        #
#       ./entrypoint build     Builds & generates the project's AppImage inside a docker container. #
#####################################################################################################

# Configure & build project.
function cmakeBuild(){
    echo "Building the project..."
    cd /File-Transfer    
    cmake -B /File-Transfer/build -S /File-Transfer/ -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=/opt/Qt/6.5.0/gcc_64/
    cmake --build /File-Transfer/build
}

# Install the project in target build dir.
function cmakeInstall(){
    echo "Installing the project..."
    sudo make install --directory=/File-Transfer/build/ DESTDIR=/File-Transfer/build/AppDir
}

# Execute the project binary.
function executeBinary(){
    echo "Executing the project binary... $DISPLAY $QMAKE "
    /File-Transfer/build/file-transfer
}

# Generate AppImage.
function generateAppImage(){
    echo "Generating AppImage..."
    cd /File-Transfer/build
    sudo                                                        \
        QML_SOURCES_PATHS=/File-Transfer/res/qml                \
        QMAKE=/opt/Qt/6.5.0/gcc_64/bin/qmake                    \
        LD_LIBRARY_PATH=/opt/Qt/6.5.0/gcc_64/lib                \
        /home/user/linuxdeploy-x86_64.AppImage                  \
        --appdir /File-Transfer/build/AppDir/                   \
        --output appimage                                       \
        --plugin qt                                             \
        --desktop-file /File-Transfer/ci/file-transfer.desktop  \
        --icon-file /File-Transfer/ci/file-transfer.svg
}

if [ $1 == "build" ]; then
    sudo rm -rf /File-Transfer/build
    cmakeBuild;
    cmakeInstall;
    generateAppImage;
else
    cmakeBuild; 
    executeBinary;
fi