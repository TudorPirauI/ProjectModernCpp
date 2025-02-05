cmake_minimum_required(VERSION 3.31)

project(JsonUtilsLibrary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the library type based on the platform
if (WIN32)
    set(LIBRARY_TYPE SHARED)
else ()
    set(LIBRARY_TYPE SHARED)
endif ()

add_library(JsonUtils ${LIBRARY_TYPE} JsonUtils.cpp)
target_include_directories(JsonUtils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia REQUIRED)
target_link_libraries(JsonUtils PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia)

# Define JSONUTILS_EXPORTS for Windows to handle DLL export/import
if (WIN32)
    target_compile_definitions(JsonUtils PRIVATE JSONUTILS_EXPORTS)
endif ()