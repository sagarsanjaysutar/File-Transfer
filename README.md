# File Transfer

Securely transfer files on a local network.

https://github.com/sagarsanjaysutar/File-Transfer/assets/41800823/df94162a-fa58-475e-8184-6c649e7d392b

_The purpose of this project is to learn various advanced concepts in C++._

## Concepts

1. **Object-Oriented**: SOLID principles are followed.
2. **Device Discovery**: Device discovery via `nmap` is performed.
3. **Network Programming**: Peer-to-peer file transfer mechanism via socket programming is implemented.
4. **Multithreading**: Multithreading is utilized to handle multiple file transfers simultaneously.
5. **Checksums**: Checksums calculation for files transmission is implemented to ensure data integrity.
6. **Smart Pointers**: Smart pointers are used to manage dynamic memory allocation and prevent memory leaks.
7. **Timers**: Timers are implemented for synchronous file transfers and for managing timeouts during network operations.
8. **Docker**: A Docker container is used which has pre-installed dependencies.
9. **CMake**: CMake is used as the build system for the project to ensure easy compilation on different platforms.

## Running the project

Run the execute.sh script to launch the application.

## Todo

Explore the libssh library instead of the external linux's openssh-server.

