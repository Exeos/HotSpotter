cmake_minimum_required(VERSION 3.30.5)
project(src)

set(CMAKE_CXX_STANDARD 20)

add_executable(injector main.cpp
        src/AbstractInjector.hpp)

if(WIN32)
    message(STATUS "Building for Windows")
    target_sources(injector PRIVATE src/impl/LinuxInjector.cpp)
elseif(UNIX)
    message(STATUS "Building for Linux/Unix")
    target_sources(injector PRIVATE src/impl/LinuxInjector.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
