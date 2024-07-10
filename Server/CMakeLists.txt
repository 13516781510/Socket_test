project(Server)
include_directories(..)
add_executable(Server Server.cpp ../TCP.cpp ../TCP.hpp)
