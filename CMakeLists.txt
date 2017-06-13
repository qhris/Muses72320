cmake_minimum_required(VERSION 2.6)
project(Tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/tests/framework)
include_directories(${CMAKE_SOURCE_DIR}/tests/mock)
aux_source_directory(${CMAKE_SOURCE_DIR}/tests TEST_SOURCES)

add_executable(Tests ${TEST_SOURCES} src/Muses72320.cpp)
add_custom_target(check COMMAND Tests DEPENDS Tests)
