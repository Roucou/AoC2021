cmake_minimum_required(VERSION 3.0.0)

project(AoC2021 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

set(ALL_SOURCES
	src/day13b.cpp
)
add_executable(day13b ${ALL_SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
