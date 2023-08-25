cmake_minimum_required(VERSION 3.15)
project(ecs)

add_executable(ecs main.cc ecs.cpp position.cpp velocity.cpp colorsystem.cpp)
