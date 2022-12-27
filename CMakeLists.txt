cmake_minimum_required(VERSION 3.15)
project(helloshape)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(shapelib
    shapelib/dbfopen.c
    shapelib/safileio.c
    shapelib/sbnsearch.c
    shapelib/shpopen.c
    shapelib/shptree.c
)

include_directories( ${PROJECT_SOURCE_DIR})
add_executable(print_information ${shapelib} print_information.cc)

