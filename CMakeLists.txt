cmake_minimum_required(VERSION 3.29)

project(Eter)

set(CMAKE_CXX_STANDARD 23)

add_executable(Eter src/main.cpp
        src/Player.h
        src/Card.h
        src/Player.cpp
        src/Card.cpp
        src/Game/Game.h
        src/Game/Game.cpp
        src/Wizard.cpp
        src/Wizard.h
        src/Game/Antrenament.cpp
        src/Game/Antrenament.h
        src/Game/Antrenament.h
        src/Game/Antrenament.cpp
        src/Board/GameBoard.cpp
        src/Board/GameBoard.h
        src/Board/CardStack.cpp
        src/Board/CardStack.h)

