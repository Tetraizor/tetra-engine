cmake_minimum_required(VERSION 3.16)
project(TetraEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_ENGINE "Build engine as a shared DLL" OFF)

include(FetchContent)

# Output folder for final binaries
set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME})

function(set_target_output_dirs target)
    foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
            LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
            ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
        )
    endforeach()
endfunction()

# --- EXTERNAL LIBRARIES --- 

# nlohmann/json
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# SDL
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared?" FORCE)
set(SDL_STATIC ON  CACHE BOOL "Build SDL static?"  FORCE)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/external/SDL/CMakeLists.txt")
  message(FATAL_ERROR "SDL submodule not found in external/SDL. Run: git submodule update --init --recursive")
endif()

FetchContent_Declare(
  SDL3
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/SDL
)
FetchContent_MakeAvailable(SDL3)

# Add your subprojects
add_subdirectory(engine)

if(NOT BUILD_SHARED_ENGINE)
  add_subdirectory(editor)
endif()
