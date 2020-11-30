cmake_minimum_required(VERSION 3.17)
project(pong_sdl)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
add_executable(pong_sdl main.cpp Game.cpp Game.h Vector2.h)
target_link_directories(pong_sdl PRIVATE ${SDL2_LIBRARY_DIRS})
target_link_libraries(pong_sdl ${SDL2_LIBRARIES})
