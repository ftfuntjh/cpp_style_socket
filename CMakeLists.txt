cmake_minimum_required(VERSION 3.14)
project(network_utils)

set(CMAKE_CXX_STANDARD 11)

option(ENABLE_TEST "enable build test code" OFF)

add_library(network_utils INTERFACE)
set("${CMAKE_PROJECT_NAME}_INCLUDE_DIR" "${CMAKE_SOURCE_DIR}/include")
target_include_directories(network_utils INTERFACE include)
if (ENABLE_TEST)
    add_subdirectory(test)
endif ()
