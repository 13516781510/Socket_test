project(Client)
include_directories(..)
add_executable(Client Client.cpp ../TCP.cpp ../TCP.hpp)
