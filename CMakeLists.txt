cmake_minimum_required(VERSION 3.14)
project(socket)

set(CMAKE_CXX_STANDARD 14)

add_library(network_utils INTERFACE)
target_include_directories(network_utils INTERFACE include)
