###########################
# LRender project CMakeList
# include 
###########################
cmake_minimum_required(VERSION 3.0)
project(LRender)


aux_source_directory(. DIR_TOOT_SRCS)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(LRender 
    src/main.cpp 
    src/image.cpp
    src/lrMath.cpp
    src/lrGraphic.cpp
    include/display.h
    include/image.h
    include/macro.h
    include/lrMath.h
    include/lrGraphic.h 
)

if(WIN32)
    set(SOURCES ${SOURCES} src/displayWin.cpp)
    message(STATUS "compile in WIN")
elseif(APPLE)
    set(SOURCES ${SOURCES} src/displayMac.m)
    #set(SOURCES ${SOURCES} src/displayTest.cpp)
    message(STATUS "compile in MAC")
else()
    message(STATUS "compile in other")
endif()


 