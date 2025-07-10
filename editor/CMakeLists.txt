add_executable(Editor src/main.cpp)

target_compile_definitions(Editor PRIVATE SDL_MAIN_HANDLED)

target_link_libraries(Editor
  PRIVATE
    TetraEngine
    SDL3::SDL3
)

# Platform specific build configurations
if(WIN32)
    target_sources(Editor PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../platform/windows/editor.rc"
    )
endif()

foreach(CONFIG_TYPE IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
  set_target_properties(Editor PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${DIST_DIR} 
    PDB_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER}     ${DIST_DIR}
  )
endforeach()
