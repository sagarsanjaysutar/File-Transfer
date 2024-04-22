# Base image with Qt6 pre-installed
# Refer: https://hub.docker.com/layers/stateoftheartio/qt6/6.5-gcc-aqt/images/sha256-c0dfd1cd174d855f0157ce0455270b2ee49f5eea4c7a40ffe0e848d41ae4d074?context=explore
FROM stateoftheartio/qt6:6.5-gcc-aqt

# Update the image
RUN sudo apt-get update && sudo apt-get -y upgrade

# Developement dependencies.
RUN sudo apt-get -y install libgl1-mesa-dev libegl1-mesa-dev libxcb-xkb-dev libvulkan-dev x11-apps