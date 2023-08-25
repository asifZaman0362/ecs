cmake_minimum_required(VERSION 3.15)
project(ecs)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 17)

add_executable(ecs main.cc ecs.cpp position.cpp velocity.cpp colorsystem.cpp)
