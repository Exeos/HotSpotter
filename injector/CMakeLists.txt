cmake_minimum_required(VERSION 3.31)
project(injector)

set(CMAKE_CXX_STANDARD 20)

add_executable(injector main.cpp
        injector/AbstractInjector.hpp)

if(WIN32)
    message(STATUS "Building for Windows")
    target_sources(injector PRIVATE injector/impl/LinuxInjector.cpp)
elseif(UNIX)
    message(STATUS "Building for Linux/Unix")
    target_sources(injector PRIVATE injector/impl/LinuxInjector.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()
