# Project settings
cmake_minimum_required(VERSION 3.16)

project(TetraEngine VERSION 0.1 LANGUAGES CXX)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM_NAME "windows")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM_NAME "macos")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM_NAME "linux")
else()
    set(PLATFORM_NAME "unknown")
endif()

set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist/${PLATFORM_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DIST_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DIST_DIR}/lib)

file(MAKE_DIRECTORY ${DIST_DIR}/bin)
file(MAKE_DIRECTORY ${DIST_DIR}/lib)

add_subdirectory(engine)
add_subdirectory(editor)