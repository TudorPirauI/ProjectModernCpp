cmake_minimum_required(VERSION 3.30)

include(CheckIPOSupported)
include(FetchContent)
include(ProcessorCount)
include(CheckCXXCompilerFlag)

project(Eter)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT WIN32)
    check_cxx_compiler_flag("-flto=auto" COMPILER_SUPPORTS_FLTO_AUTO)
    if (COMPILER_SUPPORTS_FLTO_AUTO)
        message(STATUS "Using Auto LTO")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto=auto")
    elseif (check_cxx_compiler_flag ("-flto" COMPILER_SUPPORTS_FLTO))
        message(STATUS "Using LTO")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native -flto")
    else ()
        message(STATUS "Not using LTO")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
    endif ()
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
include_directories("lib")

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
    set(CMAKE_PREFIX_PATH "C:\\Qt\\6.8.1\\mingw_64")
    set(Qt6_DIR "C:\\Qt\\6.8.1\\mingw_64\\lib\\cmake\\Qt6")
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# This is mostly useless, but it helps on certain setups
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
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

target_precompile_headers(${PROJECT_NAME} PRIVATE lib/pch.h)

target_link_libraries(Eter PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

#add_subdirectory(tests)