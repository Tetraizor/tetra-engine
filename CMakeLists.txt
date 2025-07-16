cmake_minimum_required(VERSION 3.16)
project(TetraEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Output folder for final binaries
set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist/${CMAKE_SYSTEM_NAME}/$<CONFIG>)

# Platform-specific settings
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
add_subdirectory(editor)
