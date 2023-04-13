cmake_minimum_required(VERSION 3.0)

project(shared_locking)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O0 -std=c11")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
