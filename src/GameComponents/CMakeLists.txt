cmake_minimum_required(VERSION 3.30)

project(JsonUtilsLibrary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBRARY_TYPE SHARED)

add_library(JsonUtils ${LIBRARY_TYPE}
        JsonUtils.cpp
        Card.cpp
        Player.cpp
        Arena.cpp
        Board.cpp
        ../GameModes/Game.cpp
        ../GameModes/Antrenament.cpp
        ../GameModes/DuelulCombinat.cpp
        ../GameModes/DuelulElementelor.cpp
        ../GameModes/DuelulVrajitorilor.cpp
        ../Powers/ElementPower.cpp
        ../Powers/Explosion.cpp
        ../Powers/Wizard.cpp
)

target_include_directories(JsonUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_precompile_headers(JsonUtils PRIVATE ../../lib/pch.h)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)
target_link_libraries(JsonUtils PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia)

if (WIN32)
    target_compile_definitions(JsonUtils PRIVATE JSONUTILS_EXPORTS)
endif ()

add_custom_command(TARGET JsonUtils POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:JsonUtils>
        ${CMAKE_BINARY_DIR}/$<TARGET_FILE_NAME:JsonUtils>
)