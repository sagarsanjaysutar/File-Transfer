# File Transfer

Securely transfer files on a local network.

https://github.com/user-attachments/assets/1b124237-2bc2-4626-bf9f-8202820805a0

## Concepts

_The purpose of this project is to learn various advanced concepts in C++._

1. **Object-Oriented**: SOLID principles are followed.
2. **Device Discovery**: Device discovery via `nmap` is performed.
3. **File Transfer**: SFTP Protocol is used to transfer file.
4. **Smart Pointers**: Smart pointers are used to manage dynamic memory allocation and prevent memory leaks.
5. **CMake**: CMake is used as the build system for the project to ensure easy compilation on different platforms.
6. **Docker & AppImage**: A docker container with pre-installed dependencies is used to build & generate an AppImage for the application.

## Running the project

1. Builds & runs the project's binary inside a docker container. On the host machine run the following
    ```shell
    modprobe fuse
    xhost +local:docker
    ./execute 
    ```
2. Builds & generates the project's AppImage inside a docker container. The generated AppImage (bin/) can be executed on any Linux system by simply double clicking.
    ```shell
    ./execute build
    ```

### Limitations

1. This project is a wrapper around Linux's SFTP tool (openssh-server & openssh-client). The `scripts/enableSFTP.sh` script meant to manage SFTP uses a lot Ubuntu-specific commands which has made the project dependent on Linux's Ubuntu distribution. 
2. The `scripts/enableSFTP.sh` script managing the SFTP requires the application (generated AppImage) to be run with sudo.

## Todo

1. Explore the libssh library instead of the external linux's openssh-server.
2. Utilize multi-thread to handle multiple file transfers simultaneously.
3. Perform Checksums calculation to ensure data integrity after files transmission.
