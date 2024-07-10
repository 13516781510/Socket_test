cmake_minimum_required(VERSION 3.28)
project(TCPtest)

set(CMAKE_CXX_STANDARD 17)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/exe)
set( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
add_subdirectory(Client)
add_subdirectory(Server)

