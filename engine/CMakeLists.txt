# Gather all .cpp files in src/
file(GLOB_RECURSE ALL_ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# === external/ Dependencies ===

# glad1
add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# Build TetraEngine as shared or static
if (BUILD_ENGINE_ONLY)
    message(STATUS "Building engine as shared DLL")
    add_library(TetraEngine SHARED ${ALL_ENGINE_SOURCES})
    target_compile_definitions(TetraEngine PRIVATE TETRA_ENGINE_EXPORTS)
else()
    message(STATUS "Building engine as static library")
    add_library(TetraEngine STATIC ${ALL_ENGINE_SOURCES})
endif()

# Include paths
target_include_directories(TetraEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

find_package(OpenGL REQUIRED)

# Link dependencies
target_link_libraries(TetraEngine
    PUBLIC SDL3::SDL3
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC glad
    PUBLIC OpenGL::GL
)

set_target_output_dirs(TetraEngine)
