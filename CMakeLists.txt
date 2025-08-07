cmake_minimum_required(VERSION 3.31)
project(HotSpotter)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory("${PROJECT_SOURCE_DIR}/client" "${PROJECT_SOURCE_DIR}/client/out")
add_subdirectory("${PROJECT_SOURCE_DIR}/injector" "${PROJECT_SOURCE_DIR}/injector/out")
