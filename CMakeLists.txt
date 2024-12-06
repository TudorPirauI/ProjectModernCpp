cmake_minimum_required(VERSION 3.30)

project(Eter)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3") # -Wall -Wextra
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)

include(ProcessorCount)
ProcessorCount(N)
if (NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif ()

include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)

FetchContent_MakeAvailable(catch2)

if (NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(Eter src/main.cpp
        src/Player/Player.cpp
        src/GameBoard/Board.cpp
        src/Card/Card.cpp
        src/Game/Antrenament.cpp
        src/MagicPower/MagicPower.cpp
        src/MagicPower/Wizard.cpp
        src/MagicPower/ElementPower.cpp
        src/Game/Game.cpp
        src/Game/DuelulVrajitorilor.cpp
        src/MagicPower/Explosion.cpp
        src/Interface/MainWindow.cpp
        src/Interface/MainWindow.h
)

target_link_libraries(Eter PRIVATE Qt6::Widgets Qt6::Multimedia)

#add_subdirectory(tests) # todo fix this for windows