find_package(Catch2 3 REQUIRED)

set(SOURCE_FILES
        ../src/Card/Card.cpp
        ../src/GameBoard/Board.cpp
        BoardTests.cpp
)

add_executable(Catch_tests_run ${SOURCE_FILES})
target_link_libraries(Catch_tests_run PRIVATE Catch2::Catch2WithMain)
include(Catch)