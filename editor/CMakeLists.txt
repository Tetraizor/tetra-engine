# Gather all .cpp files in src/
file(GLOB_RECURSE ALL_EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(Editor
    ${ALL_EDITOR_SOURCES}
)

target_include_directories(
    Editor 
    PRIVATE src
)

target_link_libraries(
    Editor
    PRIVATE TetraEngine
)

if (NOT BUILD_ENGINE_ONLY)
    if (MSVC)
        target_link_options(Editor 
            PRIVATE 
            "/WHOLEARCHIVE:TetraEngine"
        )
    elseif(APPLE)
        target_link_libraries(Editor
            PRIVATE
            "-Wl,-force_load,$<TARGET_FILE:TetraEngine>"
        )
    else()
        target_link_options(Editor 
            PRIVATE  
            "-Wl,--whole-archive"
            $<TARGET_FILE:TetraEngine>
            "-Wl,--no-whole-archive"
        )
    endif()
else()
    target_link_libraries(Editor PRIVATE TetraEngine)
endif()

# Platform-specific resources
if (WIN32)
    target_sources(Editor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../platform/windows/editor.rc"
    )
endif()

set_target_output_dirs(Editor)
