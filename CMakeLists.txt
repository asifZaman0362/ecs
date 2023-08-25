cmake_minimum_required(VERSION 3.15)
project(ecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 17)

if (APPLE)
    find_package(SFML 2.5 COMPONENTS window system graphics audio network REQUIRED)
endif()

add_executable(ecs main.cc ecs.cpp position.cpp velocity.cpp colorsystem.cpp render.cpp)
target_link_libraries(ecs sfml-window sfml-graphics sfml-system)
