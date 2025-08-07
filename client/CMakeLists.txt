cmake_minimum_required(VERSION 3.31)
project(client)

if (WIN32)
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    #    add_definitions(-DUNICODE -D_UNICODE)
    add_definitions(-D_MBCS)
endif()

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
        src/hooks/hooks.hpp
        src/hooks/hooks.cpp
        src/gui/Window.hpp
        src/gui/MainWindow.cpp
        src/gui/MainWindow.hpp
        src/capabilities/capabilities.hpp
        src/capabilities/capabilities.cpp
        src/class_dumper/class_dumper.hpp
        src/class_dumper/class_dumper.cpp
        src/globals.hpp
        src/gui/components/FileTree.hpp
)

# Platform-specific entry points
if(WIN32)
    set(PLATFORM_ENTRY_POINT
            src/platform/dllmain.cpp
            src/platform/windows_utils.h
    )
elseif(UNIX)
    set(PLATFORM_ENTRY_POINT src/platform/library.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

add_library(client SHARED
        ${PLATFORM_ENTRY_POINT}
        src/hot_spotter.cpp
        ${IMGUI_SOURCES}
)

# Common include directories
target_include_directories(client PRIVATE
        external/imgui
        external/imgui/backends
        external/glfw/include
        external/java
)

# Platform-specific configurations
if(WIN32)
    message(STATUS "Building for Windows")
    target_sources(client PRIVATE src/logger/impl/logger_windows.cpp)
    target_sources(client PRIVATE src/attacher/impl/WindowsAttacher.cpp)
    target_include_directories(client PRIVATE external/java/windows)
elseif(UNIX)
    message(STATUS "Building for Linux/Unix")
    target_sources(client PRIVATE src/logger/impl/logger_linux.cpp)
    target_sources(client PRIVATE src/attacher/impl/LinuxAttacher.cpp)
    target_include_directories(client PRIVATE external/java/linux)
    target_link_libraries(client PRIVATE dl pthread)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

target_link_libraries(client PRIVATE
        glfw
        ${OPENGL_LIBRARIES}
)