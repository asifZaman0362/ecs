cmake_minimum_required(VERSION 3.15)
project(simplecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    find_package(SFML 2.5 COMPONENTS window system graphics audio network REQUIRED)
endif()

add_executable(simplecs main.cc ecs.cpp)
target_link_libraries(simplecs sfml-window sfml-graphics sfml-system)
