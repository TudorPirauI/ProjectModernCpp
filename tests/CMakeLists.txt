include(FetchContent)

FetchContent_Declare(catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
)

FetchContent_MakeAvailable(catch2)

if (NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

find_package(Catch2 3 REQUIRED)

add_executable(Tests
        ../src/Card.cpp
        ../src/Board.cpp
        BoardTests.cpp
        CardTests.cpp
)

target_link_libraries(Tests PRIVATE Catch2::Catch2WithMain)

include(Catch)