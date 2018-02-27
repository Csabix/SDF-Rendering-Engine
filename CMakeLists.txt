cmake_minimum_required(VERSION 3.0.0)

project(interactive-sdf-renderer)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})
#include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/imgui/auto")

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
                  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp"
                  "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.h")

add_definitions(-std=c++17)
add_executable(interactive-sdf-renderer ${SOURCES})
target_link_libraries(interactive-sdf-renderer ${OPENGL_LIBRARIES} ${SDL2_LIBRARY} ${GLEW_LIBRARIES})
