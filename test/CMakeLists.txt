project(network_utils-test)
add_executable(network_utils-test ./main.cpp)
add_dependencies(network_utils-test network_utils)
message(STATUS "network_utils include directory ${network_utils_INCLUDE_DIR}")
target_include_directories(network_utils-test PRIVATE ${network_utils_INCLUDE_DIR})