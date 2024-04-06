# Advanced File Transfer Utility

A file transfer application that allows users to securely transfer files between two computers.

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

The project compilation will be done in a docker container which have pre-installed development dependencies.

1. Download, verify and run the docker container by running the following commands in terminal:

   ```shell
   > docker pull stateoftheartio/qt6:6.5-gcc-aqt
   > docker images
   > docker run -it -v '<Local-Path-of-this-repo>:/File-Transfer' <image-id>
   ```

2. The repo will be mounted to the docker container. Next, using `cmake` the project will be compiled. Inside the docker container run the following commands:

    ```shell
    > cd /File-Transfer/
    > mkdir build
    > cd build
    > cmake ../
    > cmake --build .
    ```

3. This will generate the executable binary in the docker container.