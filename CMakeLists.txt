cmake_minimum_required(VERSION 3.30)

include(CheckIPOSupported)
include(FetchContent)
include(ProcessorCount)
include(CheckCXXCompilerFlag)

project(Eter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

check_cxx_compiler_flag("-flto=auto" COMPILER_SUPPORTS_FLTO_AUTO)
if (COMPILER_SUPPORTS_FLTO_AUTO)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native -flto=auto")
else ()
    check_cxx_compiler_flag("-flto" COMPILER_SUPPORTS_FLTO)
    if (COMPILER_SUPPORTS_FLTO)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native -flto")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native")
    endif ()
endif ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif ()

ProcessorCount(N)
if (NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif ()

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

include_directories(include)

if (NOT SOURCES)
    message(FATAL_ERROR "No source files found. Please check the 'src' directory.")
endif ()

if (NOT HEADERS)
    message(FATAL_ERROR "No header files found. Please check the 'include' directory.")
endif ()

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

# Stuff for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_precompile_headers(${PROJECT_NAME} PRIVATE lib/pch.h)

target_link_libraries(Eter PRIVATE Qt6::Widgets)

#add_subdirectory(tests) # todo fix this for windows