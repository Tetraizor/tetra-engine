cmake_minimum_required(VERSION 3.16)
project(TetraEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_ENGINE_ONLY "Build engine as a shared DLL" OFF)
option(BUILD_TESTS "Build unit tests" OFF)

include(FetchContent)

# Output folder for final binaries
set(OUTPUT_BASE_DIR ${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME})

function(set_target_output_dirs target)
    if(CMAKE_CONFIGURATION_TYPES) # multi-config (Visual Studio, Xcode, etc.)
        foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
            set_target_properties(${target} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
                LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
                ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${OUTPUT_CONFIG}"
            )
        endforeach()
    else() # single-config (Unix Makefiles, Ninja, etc.)
        set(CONFIG "${CMAKE_BUILD_TYPE}")
        if(NOT CONFIG)
            set(CONFIG "Debug")
        endif()
        set_target_properties(${target} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${CONFIG}"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${CONFIG}"
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/${CONFIG}"
        )
    endif()
endfunction()

# --- EXTERNAL LIBRARIES --- 

message("Starting to fetch external libraries")

# nlohmann/json
message("Fetching nlohmann/json...")
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# libsdl-org/SDL
message("Fetching SDL3...")
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared?" FORCE)
set(SDL_STATIC ON  CACHE BOOL "Build SDL static?"  FORCE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.18
)
FetchContent_MakeAvailable(SDL3)

message("Completed fetching external libraries")

# --- SUBPROJECTS ---
add_subdirectory(engine)

if(NOT BUILD_ENGINE_ONLY)
  add_subdirectory(editor)
endif()

if(BUILD_TESTS) 
    add_subdirectory(tests)
endif()
