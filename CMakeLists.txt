project(Monocle)
cmake_minimum_required(VERSION 2.4)

include_directories(AFTER SYSTEM ${PROJECT_BINARY_DIR} Code)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BUILD_TESTS OFF CACHE BOOL "Build tests")

add_subdirectory(Code)

