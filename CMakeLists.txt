cmake_minimum_required(VERSION 3.20)
project(File-Transfer VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Network Gui Qml Quick)
message(STATUS "QT6 found: ${Qt6_FOUND} at ${Qt6_DIR}" )

qt_standard_project_setup()

# Include header files
file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/inc/*.h)

# Include source files
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Include resource files
qt_add_resources(RESOURCES ${PROJECT_SOURCE_DIR}/res/qml.qrc ${PROJECT_SOURCE_DIR}/res/fonts.qrc)

add_compile_definitions(PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR})

# Configure the executable
add_executable(file_transfer ${HEADERS} ${SOURCES} ${RESOURCES})
target_include_directories(file_transfer PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(file_transfer PRIVATE Qt6::Core Qt6::Network Qt6::Qml Qt6::Gui Qt6::Quick)

# target_compile_definitions(file_transfer PRIVATE PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# add execute permissions

install(FILES ${PROJECT_SOURCE_DIR}/utils/Root-Launch.sh DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(FILES ${PROJECT_SOURCE_DIR}/utils/File-Transfer.desktop DESTINATION share/applications)
install(FILES ${PROJECT_SOURCE_DIR}/utils/File-Transfer.png DESTINATION share/icons/hicolor/512x512/apps)
install(TARGETS file_transfer DESTINATION bin)