# Advanced File Transfer Utility

A file transfer application that allows users to securely transfer files between two computers.

<video width="600" controls>
   <source src="demo/demo.mp4" type="video/mp4">
   Your browser does not support the video tag.
</video>

_The purpose of this project is to learn various advanced concepts in C++._

## Concepts

1. **Object-Oriented**: SOLID principles are followed.
2. **Host Discovery**: Host discovery (other computers) via `nmap` is performed.
3. **Network Programming**: Peer-to-peer file transfer mechanism via socket programming is implemented.
4. **Multithreading**: Multithreading is utilized to handle multiple file transfers simultaneously.
5. **Checksums**: Checksums calculation for files transmission is implemented to ensure data integrity.
6. **Smart Pointers**: Smart pointers are used to manage dynamic memory allocation and prevent memory leaks.
7. **Timers**: Timers are implemented for synchronous file transfers and for managing timeouts during network operations.
8. **Docker**: A Docker container is used which has pre-installed dependencies.
9. **CMake**: CMake is used as the build system for the project to ensure easy compilation on different platforms.

## Running the project

The project compilation will be done in a [docker container](https://hub.docker.com/layers/stateoftheartio/qt6/6.5-gcc-aqt/images/sha256-c0dfd1cd174d855f0157ce0455270b2ee49f5eea4c7a40ffe0e848d41ae4d074?context=explore) which has pre-installed development dependencies.

1. Download and run the docker container. This repo will be mounted to the container. Standard configuration for X11-forwarding is also added to run GUI apps inside the container.

   ```shell
   > cd File-Transfer
   > docker build -t file_transfer:v1.0 ${PWD}
   > docker images
   > docker run --volume ${PWD}:/File-Transfer --volume /tmp/.X11-unix:/tmp/.X11-unix --device=/dev/dri:/dev/dri --env DISPLAY --net host -it file_transfer:v1.0
   ```

1. From inside the docker container, build and run the app.
   
   ```shell
   > /bin/sh /File-Transfer/build-and-run.sh
   ```

## Todo

1. Figure out a way to generate the final binary bundled with all required static libraries.
