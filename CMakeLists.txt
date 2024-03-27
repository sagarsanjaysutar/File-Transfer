cmake_minimum_required(VERSION 3.20)
project(File-Transfer VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Network)
message(STATUS "QT6 found: ${Qt6_FOUND} at ${Qt6_DIR}")

# Include header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Include source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Configure the executable
add_executable(file_transfer main.cpp ${SOURCES})
target_link_libraries(file_transfer PUBLIC Qt6::Core Qt6::Network)