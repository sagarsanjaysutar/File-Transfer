FROM stateoftheartio/qt6:6.5-gcc-aqt

# Update the image
RUN sudo apt-get update && sudo apt-get -y upgrade

# Developement dependencies.
RUN sudo apt-get -y install libgl1-mesa-dev libegl1-mesa-dev libxcb-xkb-dev libvulkan-dev x11-apps