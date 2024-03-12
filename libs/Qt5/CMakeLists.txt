cmake_minimum_required(VERSION 3.20)
include(FetchContent)

find_package(Qt6 COMPONENTS Core)

message(STATUS "${Qt6_FOUND}")

if(NOT Qt6_FOUND)
    message(STATUS "Qt6 Not found, installing... ${CMAKE_SYSTEM_PROCESSOR}")
    set(FETCHCONTENT_QUIET FALSE)

    FetchContent_Declare(Qt6 
        GIT_REPOSITORY "https://github.com/qt/qt5.git"                      
        GIT_TAG 6.5.0
        GIT_PROGRESS TRUE
    )
    message(STATUS "Installing...")
    FetchContent_MakeAvailable(Qt6)

    message(STATUS "Installed at ${Qt6_SOURCE_DIR}")
else()
    message(STATUS "Qt6 found at ${Qt6_DIR}")
endif()

if(TARGET Qt6::Core)
    message(STATUS "Qt6 core found")
else()
    message(STATUS "Qt6 core not found")
endif()



