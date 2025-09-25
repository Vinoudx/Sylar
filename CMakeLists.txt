cmake_minimum_required(VERSION 3.22)
project(Sylar)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20")

add_subdirectory(${PROJECT_SOURCE_DIR}/tests)