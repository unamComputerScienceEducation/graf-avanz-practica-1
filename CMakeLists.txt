cmake_minimum_required(VERSION 3.24)

project(practica_1 LANGUAGES CXX)

# Set to C++11
set ( CMAKE_CXX_STANDARD          20 )
set ( CMAKE_CXX_STANDARD_REQUIRED ON )
set ( CMAKE_CXX_EXTENSIONS        OFF )

include_directories(${PROJECT_SOURCE_DIR}/include)

set( SOURCE_ONE_WEEKEND
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCE_ONE_WEEKEND})


