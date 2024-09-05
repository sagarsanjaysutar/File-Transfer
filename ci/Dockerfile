# Base image with Qt6 pre-installed
# Refer: https://hub.docker.com/layers/stateoftheartio/qt6/6.5-gcc-aqt/images/sha256-c0dfd1cd174d855f0157ce0455270b2ee49f5eea4c7a40ffe0e848d41ae4d074?context=explore
FROM stateoftheartio/qt6:6.5-gcc-aqt

USER user
ENV HOME=/home/user
RUN mkdir -p $HOME

# Update the image
RUN sudo apt-get update --fix-missing && sudo apt-get upgrade -y

# Dependencies required to run GUI app inside a Docker container
RUN sudo apt-get install -y libgl1-mesa-dev libegl1-mesa-dev libxcb-xkb-dev libxcb-cursor0 libvulkan-dev x11-apps

# Common linux utilities.
RUN sudo apt-get install -y net-tools software-properties-common lsb-release file wget ipcalc

# Downloading latest linuxdeploy & it's corresponding plugins to generate AppImage.
# The pre-installed linuxdeploy in this container does not contain appimage plugin.
RUN sudo apt-get install -y fuse libfuse2
ARG LINUX_DEP="https://github.com/linuxdeploy/linuxdeploy/releases/download/1-alpha-20240109-1/linuxdeploy-x86_64.AppImage"
ARG LINUX_DEP_APP_IMG="https://github.com/linuxdeploy/linuxdeploy-plugin-appimage/releases/download/1-alpha-20230713-1/linuxdeploy-plugin-appimage-x86_64.AppImage"
ARG LINUX_DEP_QT="https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/1-alpha-20240109-1/linuxdeploy-plugin-qt-x86_64.AppImage"
RUN wget --no-verbose --show-progress --progress=dot:giga -O linuxdeploy-x86_64.AppImage  ${LINUX_DEP}
RUN wget --no-verbose --show-progress --progress=dot:giga -O linuxdeploy-plugin-appimage-x86_64.AppImage ${LINUX_DEP_APP_IMG}
RUN wget --no-verbose --show-progress --progress=dot:giga -O linuxdeploy-plugin-qt-x86_64.AppImage ${LINUX_DEP_QT}
RUN chmod +x $HOME/linuxdeploy-x86_64.AppImage $HOME/linuxdeploy-plugin-appimage-x86_64.AppImage $HOME/linuxdeploy-plugin-qt-x86_64.AppImage

# Application dependencies.
RUN sudo apt-get install -y openssh-server openssh-client nmap libssh-dev

COPY --chmod=777 entrypoint.sh /entrypoint.sh
# Build & Deploy script on entry


SHELL [ "/bin/bash", "-ec" ]
ENTRYPOINT ["/entrypoint.sh"]
CMD ["/bin/bash"]