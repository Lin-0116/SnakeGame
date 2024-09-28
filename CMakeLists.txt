cmake_minimum_required(VERSION 3.24)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(SnakeGame
        Snake.cpp
        Snake.h test.cpp)
