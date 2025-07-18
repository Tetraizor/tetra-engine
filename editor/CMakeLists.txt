add_executable(Editor
    src/main.cpp
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
        target_link_options(Editor PRIVATE "/WHOLEARCHIVE:TetraEngine")
    else()
        target_link_options(Editor PRIVATE
            "-Wl,--whole-archive" TetraEngine "-Wl,--no-whole-archive"
        )
    endif()
endif()

# Platform-specific resources
if (WIN32)
    target_sources(Editor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../platform/windows/editor.rc"
    )
endif()

set_target_output_dirs(Editor)
