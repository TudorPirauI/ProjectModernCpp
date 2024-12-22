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
    message(STATUS "Using Auto LTO")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native -flto=auto")
elseif (check_cxx_compiler_flag ("-flto" COMPILER_SUPPORTS_FLTO))
    message(STATUS "Using LTO")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native -flto")
else ()
    message(STATUS "Not using LTO")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -march=native")
endif ()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif ()

ProcessorCount(N)
if (NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
    message(STATUS "Setting parallel build level to ${N}")
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
endif ()

file(GLOB SOURCES src/**/*.cpp src/*.cpp)
file(GLOB HEADERS include/**/*.h include/*.h)

include_directories(include)

if (NOT SOURCES)
    message(FATAL_ERROR "No source files found. Please check the 'src' directory.")
endif ()

if (NOT HEADERS)
    message(FATAL_ERROR "No header files found. Please check the 'include' directory.")
endif ()

list(LENGTH SOURCES NUM_SOURCES)
list(LENGTH HEADERS NUM_HEADERS)

message(STATUS "Number of source files: ${NUM_SOURCES}")
message(STATUS "Number of header files: ${NUM_HEADERS}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_precompile_headers(${PROJECT_NAME} PRIVATE lib/pch.h)

target_link_libraries(Eter PRIVATE Qt6::Widgets)

add_subdirectory(tests)