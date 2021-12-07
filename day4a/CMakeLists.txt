cmake_minimum_required(VERSION 3.0.0)

project(AoC2021 VERSION 0.1.0)

set(ALL_SOURCES
	src/day4a.cpp
)
add_executable(day4a ${ALL_SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
