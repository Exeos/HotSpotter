cmake_minimum_required(VERSION 3.30.5)
project(client)

if (WIN32)
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
    add_definitions(-D_MBCS)
endif ()

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

set(IMGUI_SOURCES
        external/imgui/imgui.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/backends/imgui_impl_glfw.cpp
        external/imgui/backends/imgui_impl_opengl3.cpp
)

set(TINY_FILE_DIALOGS_SOURCES external/tinyfiledialogs/tinyfiledialogs.h external/tinyfiledialogs/tinyfiledialogs.c)

add_library(client SHARED
        ${IMGUI_SOURCES}
        ${TINY_FILE_DIALOGS_SOURCES}
)

add_subdirectory(src)

# Common include directories
target_include_directories(client PRIVATE
        external/imgui
        external/imgui/backends
        external/glfw/include
        external/java
)

# Platform-specific configurations
if (WIN32)
    message(STATUS "Building for Windows")
    target_include_directories(client PRIVATE external/java/windows)
elseif (UNIX)
    message(STATUS "Building for Linux/Unix")
    target_include_directories(client PRIVATE external/java/linux)
    target_link_libraries(client PRIVATE dl pthread)
else ()
    message(FATAL_ERROR "Unsupported platform")
endif ()

target_link_libraries(client PRIVATE
        glfw
        ${OPENGL_LIBRARIES}
)