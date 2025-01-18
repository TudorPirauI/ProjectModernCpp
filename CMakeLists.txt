cmake_minimum_required(VERSION 3.28) # Visual Studio comes bundled with 3.28-microsoft-version-something-something

project(Eter)

include(CheckIPOSupported)
include(FetchContent)
include(ProcessorCount)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Building in Release mode with optimisations")
    add_compile_options(-Wall -Wextra -Wpedantic -Ofast)

    check_cxx_compiler_flag("-flto" COMPILER_SUPPORTS_FLTO)
    if (COMPILER_SUPPORTS_FLTO)
        message(STATUS "Using LTO")
        add_compile_options(-flto=auto)
        add_link_options(-flto=auto)
    endif ()
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Building in Debug mode")

    add_compile_options(-Og -g)
else ()
    message(WARNING "Unknown build type (${CMAKE_BUILD_TYPE}). Not applying any extra optimisations.")
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
include_directories("./lib")

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

if (WIN32)
    set(QT_VERSIONS "6.8.0" "6.8.1")
    set(QT_FOUND FALSE)
    foreach (QT_VERSION ${QT_VERSIONS})
        set(CMAKE_PREFIX_PATH "C:\\Qt\\${QT_VERSION}\\mingw_64")
        set(Qt6_DIR "C:\\Qt\\${QT_VERSION}\\mingw_64\\lib\\cmake\\Qt6")
        if (EXISTS "${CMAKE_PREFIX_PATH}")
            set(QT_FOUND TRUE)
            break()
        endif ()
    endforeach ()
    if (NOT QT_FOUND)
        message(FATAL_ERROR "Qt 6.8.0 or 6.8.1 not found.")
    endif ()
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        CONFIG
        REQUIRED
)

#find_package(nlohmann_json QUIET)
#
#if (nlohmann_json_FOUND)
#    message(STATUS "Found nlohmann_json")
#else ()
#    message(STATUS "Fetching nlohmann_json")
#
#    FetchContent_Declare(
#            nlohmann_json
#            GIT_REPOSITORY https://github.com/nlohmann/json.git
#            GIT_TAG v3.11.3
#    )
#
#    FetchContent_MakeAvailable(nlohmann_json)
#
#endif ()

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

qt_add_resources(RESOURCES ./QResource.qrc)

target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES})

# This is mostly useless, but it helps on certain setups (obviously windows is to blame)
if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets Multimedia)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

target_precompile_headers(${PROJECT_NAME} PRIVATE ./lib/pch.h)

target_link_libraries(Eter PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Multimedia
        #        nlohmann_json::nlohmann_json
)