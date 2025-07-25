cmake_minimum_required(VERSION 3.31)
project(client)

set(CMAKE_CXX_STANDARD 20)

include_directories(include include/linux)

add_library(client SHARED src/library.cpp
        src/attacher/Attacher.hpp
        src/logger/logger.hpp
        src/hot_spotter.hpp
        src/hot_spotter.cpp)

if(WIN32)
    message(FATAL_ERROR "Not yet implemented")
elseif(UNIX)
    message(STATUS "Building for Linux/Unix")
    target_sources(client PRIVATE src/logger/impl/logger_linux.cpp)
    target_sources(client PRIVATE src/attacher/impl/LinuxAttacher.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
