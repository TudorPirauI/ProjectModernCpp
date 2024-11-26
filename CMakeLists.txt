cmake_minimum_required(VERSION 3.29)

project(Eter)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG v5.0.0
)


FetchContent_Declare(ftxui-grid-container
        GIT_REPOSITORY https://github.com/mingsheng13/grid-container-ftxui
        GIT_TAG master
)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(ftxui-grid-container)
if (NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()


add_executable(Eter src/main.cpp
        src/Player/Player.h
        src/Player/Player.cpp
        src/GameBoard/Board.cpp
        src/Card/Card.cpp
        src/Game/Antrenament.cpp
        src/Game/Antrenament.h
        src/MagicPower/MagicPower.cpp
        src/MagicPower/MagicPower.h
        src/MagicPower/Wizard.cpp
        src/MagicPower/Wizard.h
        src/MagicPower/ElementPower.cpp
        src/MagicPower/ElementPower.h
        src/utils/Logger.cpp
        src/utils/Logger.h
)

target_link_libraries(Eter PRIVATE ftxui::screen ftxui::dom ftxui::component ftxui-grid-container)

add_subdirectory(tests)