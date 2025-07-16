file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(TetraEngine STATIC ${ENGINE_SRC})

target_include_directories(TetraEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(TetraEngine PRIVATE 
  SDL3::SDL3
  nlohmann_json::nlohmann_json  
)

foreach(CONFIG_TYPE IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
  set_target_properties(TetraEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${DIST_DIR} 
    LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${DIST_DIR} 
    ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} ${DIST_DIR} 
    PDB_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER}     ${DIST_DIR} 
  )
endforeach()
