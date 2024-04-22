cmake_minimum_required(VERSION 3.20)
project(File-Transfer VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Qt6
set(Vulkan_INCLUDE_DIR "/usr/include/vulkan")
set(XKB_INCLUDE_DIR "/usr/include/xcb/")
set(XKB_LIBRARY "/usr/lib/x86_64-linux-gnu/")

find_package(Qt6 REQUIRED COMPONENTS Core Network Gui Qml Quick)
message(STATUS "QT6 found: ${Qt6_FOUND} at ${Qt6_DIR}" )

qt_standard_project_setup()

# Include header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Include source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Include resource files
qt_add_resources(RESOURCES ${PROJECT_SOURCE_DIR}/res/qml.qrc ${PROJECT_SOURCE_DIR}/res/fonts.qrc) 

# Configure the executable
add_executable(file_transfer ${SOURCES} ${RESOURCES})
target_link_libraries(file_transfer PRIVATE Qt6::Core Qt6::Network Qt6::Qml Qt6::Gui Qt6::Quick)
