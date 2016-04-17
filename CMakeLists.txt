cmake_minimum_required(VERSION 3.3)
project(render)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

include_directories("/usr/include")
find_library(GLUT "glut")
find_library(GL "GL")
find_library(GLU "GLU")
find_library(M "m")

set(SOURCE_FILES main.c utilities.c utilities.h objParser.c objParser.h)
add_executable(render ${SOURCE_FILES})
target_link_libraries(render ${GLUT} ${GL} ${GLU} ${M})
