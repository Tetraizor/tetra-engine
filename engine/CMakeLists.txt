# Gather all .cpp files in src/
file(GLOB_RECURSE ALL_ENGINE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

# Build TetraEngine as shared or static
if (BUILD_SHARED_ENGINE)
    message(STATUS "Building engine as shared DLL")
    add_library(TetraEngine SHARED ${ALL_ENGINE_SOURCES})
    target_compile_definitions(TetraEngine PRIVATE TETRA_ENGINE_EXPORTS)
else()
    message(STATUS "Building engine as static library")
    add_library(TetraEngine STATIC ${ALL_ENGINE_SOURCES})
endif()

# Include paths
target_include_directories(TetraEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)

# Link dependencies
target_link_libraries(TetraEngine
    PUBLIC SDL3::SDL3
    PUBLIC nlohmann_json::nlohmann_json
)

set_target_output_dirs(TetraEngine)
