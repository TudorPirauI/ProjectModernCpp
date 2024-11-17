cmake_minimum_required(VERSION 3.29)

project(Eter)

set(CMAKE_CXX_STANDARD 23)

add_executable(Eter src/main.cpp
        src/Player/Player.h
        src/Player/Player.cpp
        src/GameBoard/Board.cpp
        src/Card/Card.cpp)

