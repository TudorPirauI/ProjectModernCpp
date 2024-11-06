cmake_minimum_required(VERSION 3.29)

project(Eter)

set(CMAKE_CXX_STANDARD 23)

add_executable(Eter main.cpp
        Player.h
        Card.h
        Player.cpp
        Card.cpp
        Game.h
        Game.cpp
        Wizard.cpp
        Wizard.h
        Antrenament.cpp
        Antrenament.h)

