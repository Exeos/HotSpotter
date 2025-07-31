cmake_minimum_required(VERSION 3.31)
project(client)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
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


add_library(client SHARED
        src/library.cpp
        src/hot_spotter.cpp
        ${IMGUI_SOURCES}
)

target_include_directories(client PRIVATE
        external/imgui
        external/imgui/backends
        external/glfw/include
        external/java
        external/java/linux
)

target_link_libraries(client PRIVATE
        glfw
        ${OPENGL_LIBRARIES}
        dl pthread
)

if(WIN32)
    message(FATAL_ERROR "Not yet implemented")
elseif(UNIX)
    message(STATUS "Building for Linux/Unix")
    target_sources(client PRIVATE src/logger/impl/logger_linux.cpp)
    target_sources(client PRIVATE src/attacher/impl/LinuxAttacher.cpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()