cmake_minimum_required(VERSION 4.1)
project(agent)

set(CMAKE_CXX_STANDARD 23)

add_library(agent SHARED)
add_subdirectory(src)

target_include_directories(agent PRIVATE include/java)
if (WIN32)
    target_include_directories(agent PRIVATE include/java/windows)
elseif (UNIX)
    target_include_directories(agent PRIVATE include/java/linux)
    target_link_libraries(agent PRIVATE dl pthread)
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()
