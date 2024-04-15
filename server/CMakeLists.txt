cmake_minimum_required(VERSION 3.19.2)
project(Pluto)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS system thread)
add_subdirectory(HttpServer)
add_subdirectory(RPCServer)