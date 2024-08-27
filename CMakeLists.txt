cmake_minimum_required(VERSION 3.20)
project(File-Transfer VERSION 1.0 LANGUAGES CXX)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include libssh
find_package(libssh REQUIRED)
message(STATUS "libSSH found: ${libssh_FOUND} at ${libssh_DIR}")

# Include Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Network Gui Qml Quick)
message(STATUS "QT6 found: ${Qt6_FOUND} at ${Qt6_DIR}" )
qt_standard_project_setup()

add_compile_definitions(PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

# Include header files
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# Include source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Include resource files
qt_add_resources(RESOURCES ${PROJECT_SOURCE_DIR}/res/qml.qrc ${PROJECT_SOURCE_DIR}/res/fonts.qrc)

# Configure the executable
add_executable(file-transfer ${HEADERS} ${SOURCES} ${RESOURCES})
target_include_directories(file-transfer PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(file-transfer PRIVATE Qt6::Core Qt6::Network Qt6::Qml Qt6::Gui Qt6::Quick ssh)

# Install binary & desktop/icon files (required by linuxdeploy) into the destination machine.
install(FILES ${PROJECT_SOURCE_DIR}/ci/file-transfer.desktop DESTINATION share/applications)
install(FILES ${PROJECT_SOURCE_DIR}/ci/file-transfer.svg DESTINATION share/icons/hicolor/scalable/apps)
install(TARGETS file-transfer DESTINATION bin)