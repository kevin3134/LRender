###########################
# tinyrender project CMakeList
# include 
###########################
cmake_minimum_required(VERSION 3.0)
project(LRender)


aux_source_directory(. DIR_TOOT_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(LRender 
    src/main.cpp 
    src/display.cpp
    src/image.cpp
    include/display.h
    include/image.h
    include/macro.h
)

 