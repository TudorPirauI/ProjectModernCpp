find_package(Catch2 3 REQUIRED)

add_executable(Tests
        ../src/Card/Card.cpp
        ../src/GameBoard/Board.cpp
        BoardTests.cpp
        CardTests.cpp
)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

include(Catch)