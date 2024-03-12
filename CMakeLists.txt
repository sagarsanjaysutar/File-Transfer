cmake_minimum_required(VERSION 3.20)
project(File-Transfer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_subdirectory(libs/Qt5)
find_package(Qt6 COMPONENTS Core)

add_executable(MyExecutable Discovery.cpp)

# if(TARGET Qt6::Core)
#     message(STATUS "Qt6 core found")
# else()
#     message(STATUS "Qt6 core not found")
# endif()
target_link_libraries(MyExecutable PUBLIC Qt6::Core)